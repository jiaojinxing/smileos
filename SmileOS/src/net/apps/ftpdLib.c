
/********************************************************************************************************/
#include "kern/kern.h"
#include "kern/ipc.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

#include "ftpdLibFix.h"
#include "list.h"

#define FTPD_TASK_PRIO          20
#define FTPD_WORK_TASK_PRIO     30
#define FTPD_TASK_STACK_SIZE    (16 * KB)

#define LOCAL                   static
#define BOOL                    bool_t

#define OK                      0
#define ERROR                   -1
#define EOS                     '\0'

#define FAST                    register
#define FOREVER                 for (;;)

#define MAX_FILENAME_LENGTH     PATH_MAX
#define MAX_LOGIN_NAME_LEN      NAME_MAX
#define BUFSIZE                 LINE_MAX

#define SEM_ID                  sem_t
#define WAIT_FOREVER            0
#define semTake(a, b)           sem_wait(&a, b)
#define semGive(a)              sem_signal(&a)
#define semDelete(a)            sem_free(&a)
#define semBCreate(a)           sem_new(a, 0)
#define semMCreate(a)           sem_new(a, 1)

#define inet_ntoa_b(a, b)       inet_ntoa_r(a, b, sizeof(b))

#define sysClkRateGet()         HZ
#define taskDelay(a)            msleep(a * HZ / 1000)

#define VX_SUPERVISOR_MODE      0
#define VX_UNBREAKABLE          0

#define INET_ADDR_LEN           16

#define vxWorksVersion          5

#define fioRead                 read

IMPORT int taskSpawn(const char *name, int priority, int options, int stack_size,
        FUNCPTR func, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6,
        int arg7, int arg8, int arg9, int arg10);

IMPORT void logMsg(char *format, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6);

IMPORT void pathCat(const char *, const char *, char *);

IMPORT void pathCondense(char *);

IMPORT int fdprintf(int fd, const char *format, ...);
/********************************************************************************************************/

#define FTPD_BINARY_TYPE	0x1
#define FTPD_ASCII_TYPE		0x2
#define FTPD_EBCDIC_TYPE	0x4
#define FTPD_LOCAL_BYTE_TYPE	0x8


#define FTPD_STREAM_MODE	0x10
#define FTPD_BLOCK_MODE		0x20
#define FTPD_COMPRESSED_MODE	0x40


#define FTPD_NO_RECORD_STRU	0x100
#define FTPD_RECORD_STRU	0x200
#define FTPD_PAGE_STRU		0x400


#define FTPD_USER_OK		0x1000
#define	FTPD_ANONYMOUS		0x2000

#define FTPD_PASSIVE		0x10000


#define FTPD_REPRESENTATION(slot)	( (slot)->status	& 0xff)

#ifdef	_unused_
#define FTPD_TRANS_MODE(slot)		(((slot)->status >> 8)	& 0xff)
#define FTPD_FILE_STRUCTURE(slot)	(((slot)->status >> 16)	& 0xff)
#define FTPD_STATUS(slot)		(((slot)->status >> 24) & 0xff)
#endif


#define FTP_DATA_PORT		20
#define FTP_DAEMON_PORT		21


#define FTPD_SOCK_FREE		-1


#define FTPD_WORK_TASK_NAME_LEN	40


#define FTPD_WAIT_MAX		90
#define FTPD_WAIT_INTERVAL	5
#define	FTPD_CONN_TIMEOUT	200


#define FTPD_UC(ch)		(((int) (ch)) & 0xff)


#define	FTPD_MULTI_LINE		0x10000000

#define FTPD_WINDOW_SIZE	10240

#define	FTPD_ANONYMOUS_USER_1	"anonymous"
#define	FTPD_ANONYMOUS_USER_2	"ftp"
#define	FTPD_ANONYMOUS_USER_3	"guest"

/* globals */

int ftpdDebug			= FALSE;	/* TRUE: debugging messages */

int ftpdTaskPriority		= FTPD_TASK_PRIO;
int ftpdTaskOptions 		= VX_SUPERVISOR_MODE | VX_UNBREAKABLE;
int ftpdWorkTaskPriority	= FTPD_WORK_TASK_PRIO;
int ftpdWorkTaskOptions		= VX_SUPERVISOR_MODE | VX_UNBREAKABLE;
int ftpdWorkTaskStackSize	= FTPD_TASK_STACK_SIZE;
int ftpdWindowSize              = FTPD_WINDOW_SIZE;
int ftpsMaxClients = 4; 	/* Default max. for simultaneous connections */
int ftpsCurrentClients;

/* locals */

LOCAL BOOL ftpsActive = FALSE; 	/* Server started? */
LOCAL BOOL ftpsShutdownFlag; 	/* Server halt requested? */


typedef struct
    {
    NODE		node;		
    int			status;		
    int			byteCount;	
    int			cmdSock;	
    STATUS		cmdSockError;   
    int			dataSock;	
    struct sockaddr_in	peerAddr;	
    struct sockaddr_in 	dataAddr; 	
    caddr_t		buf;		
    int			bufSize;	
    time_t		timeUsed;	
    char 		curDirName 	
				[MAX_FILENAME_LENGTH+1];
    char                curUserName 	/* current user */
				[MAX_LOGIN_NAME_LEN+1];
    } FTPD_SESSION_DATA;

LOCAL int ftpdTaskId 		= -1;
LOCAL int ftpdServerSock 	= FTPD_SOCK_FREE;

LOCAL LIST		ftpsSessionList;
LOCAL SEM_ID		ftpsMutexSem;
LOCAL SEM_ID 		ftpsSignalSem;

LOCAL char		ftpdWorkTaskName [FTPD_WORK_TASK_NAME_LEN];
LOCAL int		ftpdNumTasks;
LOCAL FUNCPTR		pLoginVrfyFunc = NULL ;
LOCAL char		defaultHomeDir	/* default root directory */
				[MAX_FILENAME_LENGTH+1] = {0};
LOCAL char		guestHomeDir	/* anonymous root directory */
				[MAX_FILENAME_LENGTH+1] = {0};
LOCAL char		writeDirName	/* anonymous upload directory */
				[MAX_FILENAME_LENGTH+1] = {0};

/* Various messages to be told to the clients */

LOCAL const char *messages [] =
    {
    "Can't open passive connection",
    "Parameter not accepted",
    "Data connection error",
    "Directory non existent or syntax error",
    "Local resource failure: %s",
    "VxWorks FTP server (VxWorks %s) ready.",
    "Password required",
    "User logged in",
    "Bye...see you later",
    "USER and PASS required",
    "No files found or invalid directory or permission problem",
    "Transfer complete",
    "File \"%s\" not found or permission problem",
    "Cannot create file \"%s\" or permission problem",
    "Changed directory to \"%s\"",
    "Type set to I, binary mode",
    "Type set to A, ASCII mode",
    "Port set okay",
    "Current directory is \"%s\"",
    "File structure set to NO RECORD",
    "Stream mode okay",
    "Allocate and Account not required",
    "Entering Passive Mode (%d,%d,%d,%d,%d,%d)",
    "NOOP -- did nothing as requested...hah!",
    "Command not recognized",
    "Error in input file",
    "Unimplemented TYPE %d",
    "You could at least say goodbye.",
    "The following commands are recognized:",
    "End of command list.",
    "File deleted successfully.",
    "Login failed.",
    "File exists, ready for destination name.",
    "File renamed successfuly.",
    "\"%s\" directory created",
    "Directory %s removed",
    "%s: not a plain file",
    "UNIX Type: L8 Version: VxWorks",
    "\"%s\" directory could not be created",
    "Timeout (%d seconds): closing connection.",
    "Guest login ok, send your complete e-mail address as password.",
    "Guest login ok, access restrictions apply.",
    "Guest login ok, upload directory is %s.",
    "Guest access denied.",
    };


#define MSG_PASSIVE_ERROR	0
#define MSG_PARAM_BAD		1
#define MSG_DATA_CONN_ERROR	2
#define MSG_DIR_NOT_PRESENT	3
#define MSG_LOCAL_RESOURCE_FAIL	4
#define MSG_SERVER_READY	5
#define MSG_PASSWORD_REQUIRED	6
#define MSG_USER_LOGGED_IN	7
#define MSG_SEE_YOU_LATER	8
#define MSG_USER_PASS_REQ	9
#define MSG_DIR_ERROR		10
#define MSG_TRANS_COMPLETE	11
#define MSG_FILE_ERROR		12
#define MSG_CREATE_ERROR	13
#define MSG_CHANGED_DIR		14
#define MSG_TYPE_BINARY		15
#define MSG_TYPE_ASCII		16
#define MSG_PORT_SET		17
#define MSG_CUR_DIR		18
#define MSG_FILE_STRU		19
#define MSG_STREAM_MODE		20
#define MSG_ALLOC_ACCOUNT	21
#define MSG_PASSIVE_MODE	22
#define MSG_NOOP_OKAY		23
#define MSG_BAD_COMMAND		24
#define MSG_INPUT_FILE_ERROR	25
#define MSG_TYPE_ERROR		26
#define MSG_NO_GOOD_BYE		27
#define	MSG_COMMAND_LIST_BEGIN	28
#define	MSG_COMMAND_LIST_END	29
#define MSG_DELE_OK             30
#define MSG_USER_LOGIN_FAILED   31
#define	MSG_RNFR_OK		32
#define	MSG_RNTO_OK		33
#define	MSG_MKD_OK		34
#define	MSG_RMD_OK		35
#define	MSG_FILE_NOTREG		36
#define	MSG_SYST_REPLY		37
#define	MSG_MKD_ERROR		38
#define	MSG_CONN_TIMEOUT	39
#define	MSG_GUEST_PASS		40
#define	MSG_GUEST_OK		41
#define	MSG_GUEST_UPLOAD_OK	42
#define	MSG_GUEST_ACCESS	43

LOCAL char *ftpdCommandList =
"HELP	USER	PASS	QUIT	LIST	NLST\n\
RETR	STOR	CWD	TYPE	PORT	PWD\n\
STRU	MODE	ALLO	ACCT	PASV	NOOP\n\
DELE	RNFR	RNTO	MKD	RMD	MDTM\n\
SIZE	SYST	XCWD	XPWD	XMKD	XRMD\n";


/* forward declarations */

LOCAL FTPD_SESSION_DATA *ftpdSessionAdd (void);
LOCAL void ftpdSessionDelete (FTPD_SESSION_DATA *);
LOCAL STATUS ftpdWorkTask (FTPD_SESSION_DATA *);
LOCAL STATUS ftpdCmdSend (FTPD_SESSION_DATA *, int, int, const char *,
                          int, int, int, int, int, int);
LOCAL STATUS ftpdDataConnGet (FTPD_SESSION_DATA *);
LOCAL void ftpdDataStreamSend (FTPD_SESSION_DATA *, char *);
LOCAL STATUS ftpdDataStreamReceive (FTPD_SESSION_DATA *, FILE *outStream);
LOCAL void ftpdSockFree (int *);
LOCAL STATUS ftpdDirListGet (int, char *, BOOL);
LOCAL void ftpdDebugMsg (char *, int, int, int, int);
LOCAL void unImplementedType (FTPD_SESSION_DATA *pSlot);
LOCAL void dataError (FTPD_SESSION_DATA *pSlot);
LOCAL void fileError (FTPD_SESSION_DATA *pSlot);
LOCAL void transferOkay (FTPD_SESSION_DATA *pSlot);
STATUS ftpLoginUserVerify (char *name, char *passwd);
void ftpdLoginInstall(FUNCPTR pLoginFunc);

#if 0
#define	rmdir(dName)	remove((dName))
#define	mkdir(dName)	ftpMkdir((dName))


LOCAL STATUS ftpMkdir
    (
    char *dirName		/* directory name */
    )
    {
    int fd;

    if ((fd = open (dirName, O_RDWR | O_CREAT, FSTAT_DIR | DEFAULT_DIR_PERM))
	 == ERROR)
	{
	return (ERROR);
	}

    return (close (fd));
    }
#endif


LOCAL void ftpdTask (void)
    {
    int		newSock;
    FAST FTPD_SESSION_DATA *pSlot;
    int		on = 1;
    int		addrLen;
    struct sockaddr_in	addr;
    char	a_ip_addr [INET_ADDR_LEN];  

    ftpdNumTasks = 0;


    FOREVER
        {

        addrLen = sizeof (struct sockaddr);

        ftpdDebugMsg ("waiting for a new client connection...\n",0,0,0,0);

        newSock = accept (ftpdServerSock, (struct sockaddr *) &addr, &addrLen);
	if (newSock < 0)
            {
            ftpdDebugMsg ("cannot accept a new connection\n",0,0,0,0);
            break;
            }

        semTake (ftpsMutexSem, WAIT_FOREVER);

        setsockopt (newSock, SOL_SOCKET, SO_KEEPALIVE, (char *) &on,
                    sizeof (on));

        inet_ntoa_b (addr.sin_addr, a_ip_addr);
        ftpdDebugMsg ("accepted a new client connection from %s\n",
                      (int) a_ip_addr, 0, 0, 0);


	pSlot = (FTPD_SESSION_DATA *)lstFirst (&ftpsSessionList);
	while (pSlot != NULL)
	    {
	    FTPD_SESSION_DATA * pNext ;
	    time_t now = time(NULL);
	    pNext = (FTPD_SESSION_DATA *)lstNext (&pSlot->node);
	    if( (now - pSlot->timeUsed) > FTPD_CONN_TIMEOUT )
		{
		ftpdDebugMsg("session %x idle byteCount %d\n",
			(int) pSlot, pSlot->byteCount, 0, 0);
		if ( pSlot->byteCount == 0 )
		    {
		    ftpdCmdSend (pSlot, pSlot->cmdSock, 421,
			messages [MSG_CONN_TIMEOUT], 
			now - pSlot->timeUsed, 0, 0, 0, 0, 0);
		    ftpdDebugMsg("deleting session %x\n", (int)pSlot, 0, 0, 0);
		    /* XXX ftpdSessionDelete( pSlot ); */
		    ftpdSockFree (&pSlot->cmdSock);
		    taskDelay( sysClkRateGet() / 10 );
		    }
		else
		    pSlot->byteCount = 0 ;
		}
	    pSlot = pNext ;
	    }


        pSlot = ftpdSessionAdd ();
        if (pSlot == NULL) 
            {

            ftpdCmdSend (pSlot, newSock, 421, 
                         "Session limit reached, closing control connection", 
                         0, 0, 0, 0, 0, 0);
            ftpdDebugMsg ("cannot get a new connection slot\n",0,0,0,0);
            semGive (ftpsMutexSem);
            continue;
            }

	pSlot->cmdSock	= newSock;


        bcopy ( (char *)&addr, (char *)&pSlot->peerAddr, 
               sizeof (struct sockaddr_in));
        bcopy ( (char *)&addr, (char *)&pSlot->dataAddr, 
               sizeof (struct sockaddr_in));
        pSlot->dataAddr.sin_port = htons (FTP_DATA_PORT);


        sprintf (ftpdWorkTaskName, "tFtpdServ%d", ftpdNumTasks);


	ftpdDebugMsg ("creating a new server task %s...\n", 
		      (int) ftpdWorkTaskName, 0, 0, 0);

	if (taskSpawn (ftpdWorkTaskName, ftpdWorkTaskPriority,
		       ftpdWorkTaskOptions, ftpdWorkTaskStackSize,
		       (FUNCPTR)ftpdWorkTask, (int) pSlot,
		       0, 0, 0, 0, 0, 0, 0, 0, 0) == ERROR)
	    {

            ftpdCmdSend (pSlot, newSock, 421, 
                         "Service not available, closing control connection",
                         0, 0, 0, 0, 0, 0);
	    ftpdSessionDelete (pSlot);
	    ftpdDebugMsg ("cannot create a new work task\n",0,0,0,0);
            semGive (ftpsMutexSem);
            continue;
	    }

	ftpdDebugMsg ("done.\n",0,0,0,0);


        semGive (ftpsMutexSem);
	}


    ftpsActive = FALSE;

    return;
    }

/*******************************************************************************
* RETURNS:
* OK if server started, or ERROR otherwise.
*
* ERRNO: N/A
*/

STATUS ftpdInit
    (
    int		port,		
    int 	stackSize 	
    )
    {
    int 		on = 1;
    struct sockaddr_in 	ctrlAddr;

    if (ftpsActive)
	return (OK);

    ftpsShutdownFlag = FALSE;
    ftpsCurrentClients = 0;


    ftpdServerSock = socket (AF_INET, SOCK_STREAM, 0);
    if (ftpdServerSock < 0)
        return (ERROR);


    lstInit (&ftpsSessionList);
#ifndef SMILEOS
    ftpsMutexSem = semMCreate (SEM_Q_FIFO | SEM_DELETE_SAFE);
#else
    semMCreate(&ftpsMutexSem);
#endif
    if (ftpsMutexSem == NULL)
        {
        close (ftpdServerSock);
        return (ERROR);
        }

#ifndef SMILEOS
    ftpsSignalSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
#else
    semBCreate(&ftpsSignalSem);
#endif
    if (ftpsSignalSem == NULL)
        {
        close (ftpdServerSock);
        semDelete (ftpsMutexSem);
        return (ERROR);
        }

    if ( port == 0 )
	port = FTP_DAEMON_PORT ;

    ctrlAddr.sin_family = AF_INET;
    ctrlAddr.sin_addr.s_addr = INADDR_ANY;
    ctrlAddr.sin_port = htons ( port );

    if (setsockopt (ftpdServerSock, SOL_SOCKET, SO_REUSEADDR,
                    (char *) &on, sizeof (on)) < 0)
        {
        close (ftpdServerSock);
        semDelete (ftpsMutexSem);
        semDelete (ftpsSignalSem);
        return (ERROR);
        }

    if (bind (ftpdServerSock, (struct sockaddr *) &ctrlAddr,
              sizeof (ctrlAddr)) < 0)
        {
        close (ftpdServerSock);
        semDelete (ftpsMutexSem);
        semDelete (ftpsSignalSem);
        return (ERROR);
        }

    if (listen (ftpdServerSock, 5) < 0)
        {
        close (ftpdServerSock);
        semDelete (ftpsMutexSem);
        semDelete (ftpsSignalSem);
        return (ERROR);
        }


    ftpdTaskId = taskSpawn ("tFtpdTask", ftpdTaskPriority - 1, ftpdTaskOptions,
                            stackSize == 0 ? ftpdWorkTaskStackSize : stackSize,
                            (FUNCPTR) ftpdTask, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (ftpdTaskId == ERROR)
        {
        ftpdDebugMsg ("ERROR: ftpdTask cannot be created\n",0,0,0,0);
        close (ftpdServerSock);
        semDelete (ftpsMutexSem);
        semDelete (ftpsSignalSem);
        return (ERROR);			
        }
        

    ftpsActive = TRUE;
    
    ftpdLoginInstall(ftpLoginUserVerify);
    
    ftpdDebugMsg ("ftpdTask created\n",0,0,0,0);
    return (OK);
    }

/*******************************************************************************
*/
#ifndef SMILEOS
STATUS ftpdDelete (void)
    {
    BOOL serverActive = FALSE;
    FTPD_SESSION_DATA * 	pData;

    if (! ftpsActive)  
        return (OK);

    semTake (ftpsMutexSem, WAIT_FOREVER);

    taskDelete (ftpdTaskId);
    ftpdTaskId = -1;

    if (ftpsCurrentClients != 0)
        serverActive = TRUE;


    ftpsShutdownFlag = TRUE;


    pData = (FTPD_SESSION_DATA *)lstFirst (&ftpsSessionList);
    while (pData != NULL)
        {
        ftpdSockFree (&pData->cmdSock);
        pData = (FTPD_SESSION_DATA *)lstNext (&pData->node);
        }

    semGive (ftpsMutexSem);   


    if (serverActive)
        {
 
        semTake (ftpsSignalSem, WAIT_FOREVER);
        }


    ftpdSockFree (&ftpdServerSock);


    lstFree (&ftpsSessionList);   

    semDelete (ftpsMutexSem);

    semDelete (ftpsSignalSem);

    ftpsActive = FALSE;

    return (OK);
    }
#endif

/*******************************************************************************
*/

LOCAL FTPD_SESSION_DATA *ftpdSessionAdd (void)
    {
    FAST FTPD_SESSION_DATA 	*pSlot;

    if (ftpsCurrentClients >= ftpsMaxClients)
        return (NULL);


    pSlot = (FTPD_SESSION_DATA *) calloc (sizeof (FTPD_SESSION_DATA), 1);
    if (pSlot == NULL)
	{
	return (NULL);
	}

    if (ftpdWindowSize < 0 || ftpdWindowSize > 65536)
	    ftpdWindowSize = FTPD_WINDOW_SIZE;
    
    pSlot->bufSize = ftpdWindowSize ;

    ftpdDebugMsg ("allocating new session buffer %d bytes\n",
	pSlot->bufSize,0,0,0);

    pSlot->buf = malloc( pSlot->bufSize );

    if( pSlot->buf == NULL )
	{
	free(pSlot);
	return (NULL);
	}


    pSlot->dataSock = FTPD_SOCK_FREE;
    pSlot->cmdSock = FTPD_SOCK_FREE;
    pSlot->cmdSockError = OK;
    pSlot->status = FTPD_STREAM_MODE | FTPD_BINARY_TYPE | FTPD_NO_RECORD_STRU;
    pSlot->byteCount = 0;

	strcpy( pSlot->curDirName,"/tffs0/" );
	
    semTake (ftpsMutexSem, WAIT_FOREVER);

    lstAdd (&ftpsSessionList, &pSlot->node);
    ftpdNumTasks++;
    ftpsCurrentClients++;
    semGive (ftpsMutexSem);

    return (pSlot);
    }

/*******************************************************************************
*/

LOCAL void ftpdSessionDelete
    (
    FAST FTPD_SESSION_DATA *pSlot      
    )
    {
    if (pSlot == NULL)			/* null slot? don't do anything */
        return;

    semTake (ftpsMutexSem, WAIT_FOREVER);

    --ftpdNumTasks;
    --ftpsCurrentClients;
    lstDelete (&ftpsSessionList, &pSlot->node);

    ftpdSockFree (&pSlot->cmdSock);	/* release data and command sockets */
    ftpdSockFree (&pSlot->dataSock);

    free (pSlot->buf);
    free (pSlot);


    if (ftpsShutdownFlag)
        {
        if (ftpsCurrentClients == 0)
            semGive (ftpsSignalSem);
        }
    semGive (ftpsMutexSem);

    return;
    }

/*******************************************************************************
*
* ftpPathAccessVerify - verify client access to a path
*
*/
LOCAL STATUS ftpPathAccessVerify
    (
    FTPD_SESSION_DATA *pSlot,
    char * path,
    int mode
    )
    {
    char * where ;
    int len;

    /* allways allow access if not anonymous user */
    if( (pSlot->status & FTPD_ANONYMOUS) == 0 )
	return (OK);

    if( mode == O_RDONLY )
	{
	where = guestHomeDir ;
	}
    else
	{
	where = writeDirName ;
	}
        len = strlen(where);
    
    if (*where==EOS||*path==EOS) goto deny;
    
    /* perform case-insensitive comparison a la strncmp() */
	{
	FAST char *s1, *s2;
	FAST int i;
	for( s1 = where, s2 = path, i = 0; 
		(*s1!=EOS && *s2!=EOS) && i<len;
		s1++, s2++, i-- )
	    {
	    if(toupper(*s1) == toupper(*s2))
		continue ;
	    else
		goto deny;
	    }
	}

    ftpdDebugMsg ("access mode %d allowed for path %s\n", mode, (int)path,0,0);
    return OK ;
deny:
    ftpdDebugMsg ("access mode %d denied for path %s\n", mode, (int)path,0,0);
    return ERROR;
    }


/*******************************************************************************
*
* ftpdPathForPrint - prepare a path to be printed to client
*
*/
LOCAL char * ftpdPathForPrint( FTPD_SESSION_DATA *pSlot, char * path )
    {
    if( pSlot->status & FTPD_ANONYMOUS )
	{
	int len = strlen( guestHomeDir );
	if( strncmp( guestHomeDir, path, len) != 0)
	    return NULL ;
	strcpy( path, path+len );	
	}
    return(path);
    }

/*******************************************************************************
*
*
*
*/
LOCAL void ftpPathNormalize
    (
    FTPD_SESSION_DATA   *pSlot,
    char *		path,
    char *		buffer,
    char **		pResult
    )
    {
    if ( (strcmp(path,".") == 0) || (path[0] == EOS) )
	{
	*pResult = &pSlot->curDirName [0];
	return ;
	}

    (void) pathCat (pSlot->curDirName, path, buffer);

    pathCondense (buffer);		/* condense ".." shit */

    *pResult = buffer;
    }

/*******************************************************************************
*/

LOCAL STATUS ftpdWorkTask
    (
    FTPD_SESSION_DATA   *pSlot  /* pointer to the active slot to be handled */
    )
    {
    FAST int		sock;		
    FAST char		*pBuf, *pBufEnd;
    struct sockaddr_in	passiveAddr;	
    char		*dirName;	
    FAST int		numRead;
    int			addrLen = sizeof (passiveAddr);	
    int			portNum [6];	
    u_long 		value;
    char 		renFile [MAX_FILENAME_LENGTH];
    char 		newPath [MAX_FILENAME_LENGTH];
    char		*pFileName;
    FILE 		*outStream;
    char                *upperCommand;   


    pBuf = &pSlot->buf [0];	
    pBufEnd = pSlot->bufSize-1 + pBuf ;
    sock = pSlot->cmdSock;

    if (ftpsShutdownFlag)
        {

        ftpdCmdSend (pSlot, sock, 421, 
                     "Service not available, closing control connection",
                     0, 0, 0, 0, 0, 0);
        ftpdSessionDelete (pSlot);
        return (OK);
        }


    if (ftpdCmdSend (pSlot, sock, 220, messages [MSG_SERVER_READY],
		(int)vxWorksVersion, 0, 0, 0, 0, 0) == ERROR)
	goto connectionLost;

    FOREVER
	{
	taskDelay (0);		/* time share among same priority tasks */


	if (pSlot->cmdSockError == ERROR)
	    goto connectionLost;

	pSlot->byteCount = 0 ;
	(void) time( &pSlot->timeUsed );	


        if (ftpsShutdownFlag)
            break;

	bzero( pBuf, 8 );


	FOREVER
	    {
	    taskDelay (0);	

	    if ((numRead = read (sock, pBuf, 1)) <= 0)
		{

                if (ftpsShutdownFlag)
                    {
                    *pBuf = EOS;
                    break;
                    }


		if (numRead == 0)
		    ftpdCmdSend (pSlot, sock, 221, messages [MSG_NO_GOOD_BYE],
		    0, 0, 0, 0, 0, 0);

		ftpdSessionDelete (pSlot);
		return ERROR;
		}
	    
	    if ( *pBuf == '\r' )
		continue;
	    if (( *pBuf == '\n' ) || (pBuf == pBufEnd))
	    {
		*pBuf = EOS;
		break;
	    }

	    pBuf++;		

	    } /* FOREVER - read line */

	pBuf = &pSlot->buf [0];


	for (upperCommand = pBuf; (*upperCommand != ' ') &&
	     (*upperCommand != EOS); upperCommand++)
	    *upperCommand = toupper (*upperCommand);


	ftpdDebugMsg ("read command %s\n", (int)pBuf,0,0,0);


        if (ftpsShutdownFlag)
            {
            ftpdCmdSend (pSlot, sock, 421, 
                         "Service not available, closing control connection",
                         0, 0, 0, 0, 0, 0);
            break;
            }

	if (strncmp (pBuf, "USER", 4) == 0)
	    {
            if ( *(pBuf + 4) == '\0' )
               pSlot->curUserName[0] = '\0';    
            else
               strncpy(pSlot->curUserName, pBuf+5, MAX_LOGIN_NAME_LEN);

	    pSlot->status &= ~(FTPD_USER_OK | FTPD_ANONYMOUS);

	    if( (strcmp(pSlot->curUserName, FTPD_ANONYMOUS_USER_1) ==0) ||
		(strcmp(pSlot->curUserName, FTPD_ANONYMOUS_USER_2) ==0) ||
		(strcmp(pSlot->curUserName, FTPD_ANONYMOUS_USER_3) ==0)  )
		{
		pSlot->status |= FTPD_ANONYMOUS ;	/* tentative */
		ftpdCmdSend (pSlot, sock, 331, messages [MSG_GUEST_PASS],
			0, 0, 0, 0, 0, 0) ;
		continue;
		}

	    if (ftpdCmdSend (pSlot, sock, 331, messages [MSG_PASSWORD_REQUIRED],
			 0, 0, 0, 0, 0, 0) == ERROR)
		goto connectionLost;
	    continue;
	    }
	else if (strncmp (pBuf, "PASS", 4) == 0)
	    {
	    if( pSlot->status & FTPD_ANONYMOUS )
		{
		if( (guestHomeDir[0] == EOS) || (pBuf[5]==EOS) )
		    {
		    ftpdCmdSend (pSlot, sock, 530,
			messages [MSG_USER_LOGIN_FAILED], 0, 0, 0, 0, 0, 0);
		    continue;
		    }

		pSlot->status |= FTPD_USER_OK;
		strcpy (pSlot->curDirName, guestHomeDir);
		if( guestHomeDir [ strlen(guestHomeDir) -1 ] != '/');
		    strcat(pSlot->curDirName, "/");

		strcpy(newPath, writeDirName);

		if( writeDirName[0] == EOS )
		    ftpdCmdSend (pSlot, sock, 230,
			messages [MSG_GUEST_OK], 0, 0, 0, 0, 0, 0);
		else
		    ftpdCmdSend (pSlot, sock, 230,
			messages [MSG_GUEST_UPLOAD_OK], 
			(int) ftpdPathForPrint(pSlot, newPath),
			0, 0, 0, 0, 0);
		continue;
		}

	   if ( pLoginVrfyFunc != (FUNCPTR)NULL )
	       {
		ftpdDebugMsg ("Verifying login for user %s\n",
			(int)pSlot->curUserName,0,0,0);
	       if ( (*pLoginVrfyFunc)(pSlot->curUserName, pBuf+5) != OK )
		   {
		   if (ftpdCmdSend (pSlot, sock,
                                    530, messages [MSG_USER_LOGIN_FAILED], 
                                    0, 0, 0, 0, 0, 0) == ERROR)
		       goto connectionLost;
		   pSlot->status &= ~FTPD_USER_OK;
		   continue;
		   }
	        }
	     
	    else if( guestHomeDir[0] != EOS )
		{
	        if (ftpdCmdSend (pSlot, sock,
				530, messages [MSG_USER_LOGIN_FAILED], 
				0, 0, 0, 0, 0, 0) == ERROR)
		   goto connectionLost;
	        pSlot->status &= ~FTPD_USER_OK;
	        continue;
		}

	    pSlot->status |= FTPD_USER_OK;
	    if (ftpdCmdSend (pSlot, sock, 230, messages [MSG_USER_LOGGED_IN],
			 0, 0, 0, 0, 0, 0) == ERROR)
		goto connectionLost;
	    continue;
	    }
	else if (strncmp (pBuf, "QUIT", 4) == 0)
	    {
	    /* sayonara */

	    ftpdCmdSend (pSlot, sock, 221, messages [MSG_SEE_YOU_LATER],
			 0, 0, 0, 0, 0, 0);
	    ftpdSessionDelete (pSlot);
	    return OK;
	    }
	else if (strncmp (pBuf, "SYST", 4) == 0)
	    {
	    ftpdCmdSend (pSlot, sock, 215, messages [MSG_SYST_REPLY],
			 0, 0, 0, 0, 0, 0);
	    continue;
	    }
	else if (strncmp (pBuf, "HELP", 4) == 0)
	    {

	    if (ftpdCmdSend (pSlot, sock, FTPD_MULTI_LINE | 214,
			messages [MSG_COMMAND_LIST_BEGIN], 0, 0, 0, 0, 0, 0) 
			 == ERROR)
		goto connectionLost;

	    if (write (pSlot->cmdSock, ftpdCommandList,
				strlen (ftpdCommandList)) <= 0)
		goto connectionLost;


	    if (ftpdCmdSend (pSlot, sock, 214, messages [MSG_COMMAND_LIST_END],
			 0, 0, 0, 0, 0, 0) == ERROR)
		
		goto connectionLost;
	    continue;		/* All is well go wait for the next command */
	    }
	else if ((pSlot->status & FTPD_USER_OK) == 0)	/* validated yet? */
	    {

	    if (ftpdCmdSend (pSlot, sock, 530, messages [MSG_USER_PASS_REQ],
			 0, 0, 0, 0, 0, 0) == ERROR)
		
		goto connectionLost;

	    /* do not proceed further until he's legit */

	    continue;
	    }

	if (strncmp (pBuf, "LIST", 4) == 0 ||
		 strncmp (pBuf, "NLST", 4) == 0)
	    {
	    STATUS retVal;

	    
	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &dirName );

	    ftpdDebugMsg ("listing %s\n", (int)dirName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, dirName, O_RDONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	


	    if (ftpdDataConnGet (pSlot) == ERROR)
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 426, messages [MSG_DATA_CONN_ERROR],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}


	    retVal = ftpdDirListGet (pSlot->dataSock, dirName,
				     (strncmp (pBuf, "LIST", 4) == 0));

	    if (retVal == ERROR)
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_DIR_ERROR],
			     0, 0, 0, 0, 0, 0) == ERROR)
		    
		    goto connectionLost;
		}
	    else
		{
                if (ftpdCmdSend (pSlot, sock, 
                                 226, messages [MSG_TRANS_COMPLETE],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		}


	    ftpdSockFree (&pSlot->dataSock);
	    }
	else if ( (strncmp (pBuf, "RETR", 4) == 0) ||
		  (strncmp (pBuf, "SIZE", 4) == 0) ||
		  (strncmp (pBuf, "MDTM", 4) == 0) )
	    {
	    struct stat fileStat ;


	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );

	    ftpdDebugMsg ("accessing %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_RDONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if(stat(pFileName, &fileStat) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_FILE_ERROR],
			     (int)(&pBuf[5]), 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}

	    if( ! S_ISREG (fileStat.st_mode) )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_FILE_NOTREG],
			     (int)(&pBuf[5]), 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}
	    /* decide what to do with the file according to the actual cmd */
	    if (strncmp (pBuf, "RETR", 4) == 0)
		{
		/* ship it away */
		ftpdDataStreamSend (pSlot, pFileName);
		}
	    else if (strncmp (pBuf, "SIZE", 4) == 0) 
		{
		if (ftpdCmdSend (pSlot, sock, 213, "%lu",
			     fileStat.st_size, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		}
	    else if (strncmp (pBuf, "MDTM", 4) == 0)
		{
		struct tm		fileDate;
		localtime_r (&fileStat.st_mtime, &fileDate);

		if (ftpdCmdSend (pSlot, sock, 213, "%04d%02d%02d%02d%02d%02d",
			     fileDate.tm_year+1900,
			     fileDate.tm_mon,
			     fileDate.tm_mday,
			     fileDate.tm_hour,
			     fileDate.tm_min,
			     fileDate.tm_sec 
			     ) == ERROR)
		    goto connectionLost;
		}

	    }
	else if (strncmp (pBuf, "STOR", 4) == 0)
	    {
	    STATUS retVal ;
	    /* store a file */

	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );

	    ftpdDebugMsg ("STOR %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_WRONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if ((outStream = fopen (pFileName, "w")) == NULL)
	        {
		if (ftpdCmdSend (pSlot, sock, 553, messages[MSG_CREATE_ERROR],
			(int)(&pBuf[5]), 0, 0, 0, 0, 0) == ERROR)

		    goto connectionLost;
		continue;
		}

	    /* receive the file */

	    retVal = ftpdDataStreamReceive (pSlot, outStream);
	    (void) fclose (outStream);

	    if( retVal == ERROR )
		{
		unlink( pFileName );
		}
	    }
	else if (strncmp (pBuf, "RNFR", 4) == 0)
	    {
	    struct stat st;	/* for stat() system call */

	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );

	    ftpdDebugMsg ("RNFR %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_RDWR) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if( stat( pFileName, &st ) != OK )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_FILE_ERROR],
			     (int) pFileName, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;

		renFile[0] = EOS ;
		continue;
		}
	    strncpy( renFile, pFileName, MAX_FILENAME_LENGTH );
	    if (ftpdCmdSend (pSlot, sock, 350, messages [MSG_RNFR_OK],
			 0, 0, 0, 0, 0, 0) == ERROR)
		goto connectionLost ;
	    }
	else if (strncmp (pBuf, "RNTO", 4) == 0)
	    {
	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );

	    ftpdDebugMsg ("RNTO %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_WRONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if( rename( renFile, pFileName ) != OK )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_DIR_ERROR],
			     (int) &pBuf[5], 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		renFile[0] = EOS ;
		continue ;
		}
	    if (ftpdCmdSend (pSlot, sock, 250, messages [MSG_RNTO_OK],
			 0, 0, 0, 0, 0, 0) == ERROR)
		goto connectionLost ;
	    renFile[0] = EOS ;
	    }
	else if (strncmp (pBuf, "DELE", 4) == 0)
	    {
	    ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );
	    ftpdDebugMsg ("DELE %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_WRONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if( remove( pFileName ) != OK )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_FILE_ERROR],
			     (int) &pBuf[5], 0, 0, 0, 0, 0) == ERROR)
			    
		    goto connectionLost ;
		continue;
		}
	    else
		{
		if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_DELE_OK],
			     0, 0, 0, 0, 0, 0) == ERROR)
			
		    goto connectionLost ;
		}
	    }
	else if ((strncmp (pBuf, "MKD", 3) == 0) ||
		 (strncmp (pBuf, "XMKD", 4) == 0) )
	    {
	    if(strncmp (pBuf, "MKD", 3) == 0)
		ftpPathNormalize ( pSlot, &pBuf[4], newPath, &pFileName );
	    else
		ftpPathNormalize ( pSlot, &pBuf[5], newPath, &pFileName );
	    ftpdDebugMsg ("MKD %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_WRONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if( mkdir( pFileName, 0777 ) != OK )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_MKD_ERROR],
			     (int) &pBuf[4], 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		continue;
		}
	    else
		{
		if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_MKD_OK],
			     (int) &pBuf[4], 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		}
	    }
	else if ((strncmp (pBuf, "RMD", 3) == 0) ||
		 (strncmp (pBuf, "XRMD", 4) == 0) )
	    {
	    char *pDir ;
	    if(strncmp (pBuf, "RMD", 3) == 0)
		pDir = pBuf+4 ;
	    else
		pDir = pBuf+5 ;

	    ftpPathNormalize ( pSlot, pDir, newPath, &pFileName );
	    ftpdDebugMsg ("RMD %s\n", (int)pFileName,0,0,0);

	    if( ftpPathAccessVerify(pSlot, pFileName, O_WRONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	

	    if( rmdir( pFileName ) != OK )
		{
		if (ftpdCmdSend (pSlot, sock, 550, messages [MSG_FILE_ERROR],
			     (int) pDir, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		continue;
		}
	    else
		{
		if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_RMD_OK],
			     (int) pDir, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		}
	    }
	else if ((strncmp (pBuf, "CWD",  3) == 0)||
		 (strncmp (pBuf, "CDUP", 4) == 0)||
		 (strncmp (pBuf, "XCWD", 4) == 0)||
		 (strncmp (pBuf, "XCUP", 4) == 0) )
	    {
	    struct stat st;	

	    if(strncmp (pBuf, "CWD", 3) == 0)
		ftpPathNormalize ( pSlot, &pBuf[4], newPath, &dirName );
	    else if( strncmp (pBuf, "XCWD", 4) == 0)
		ftpPathNormalize ( pSlot, &pBuf[5], newPath, &dirName );
	    else
		ftpPathNormalize ( pSlot, "..", newPath, &dirName );

	    if( ftpPathAccessVerify(pSlot, newPath, O_RDONLY) == ERROR )
		{
		if (ftpdCmdSend (pSlot, sock, 
                                 550, messages [MSG_GUEST_ACCESS],
                                 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}	


	    if( (stat( newPath, &st ) != OK ) || !S_ISDIR (st.st_mode))
		{
		if (ftpdCmdSend (pSlot, sock, 501, 
			messages[MSG_DIR_NOT_PRESENT],
			     0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost ;
		continue;
		}

	    (void) strncpy (pSlot->curDirName, newPath, MAX_FILENAME_LENGTH);


	    if (ftpdCmdSend (pSlot, sock, 250, messages [MSG_CHANGED_DIR],
			    (int)ftpdPathForPrint(pSlot,newPath),
			    0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
	else if (strncmp (pBuf, "TYPE", 4) == 0)
	    {

	    if (pBuf [5] == 'I' || pBuf [5] == 'i' ||
		pBuf [5] == 'L' || pBuf [5] == 'l')
		{
	        pSlot->status |= FTPD_BINARY_TYPE;
		pSlot->status &= ~FTPD_ASCII_TYPE;
		if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_TYPE_BINARY],
			     0, 0, 0, 0, 0, 0) == ERROR)
			
		    goto connectionLost;
		}
	    else if (pBuf [5] == 'A' || pBuf [5] == 'a')
		{
	        pSlot->status |= FTPD_ASCII_TYPE;
		pSlot->status &= ~FTPD_BINARY_TYPE;
		if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_TYPE_ASCII],
			     0, 0, 0, 0, 0, 0) == ERROR)

		    goto connectionLost;
		}
	    else
		{
		if (ftpdCmdSend (pSlot, sock, 504, messages [MSG_PARAM_BAD],
			     0, 0, 0, 0, 0, 0) == ERROR)
		    
		    goto connectionLost;
		}
	    }
	else if (strncmp (pBuf, "PORT", 4) == 0)
	    {

	    (void) sscanf (&pBuf [5], "%d,%d,%d,%d,%d,%d",
			   &portNum [0], &portNum [1], &portNum [2],
			   &portNum [3], &portNum [4], &portNum [5]);

	    pSlot->dataAddr.sin_port = portNum [4] * 256 + portNum [5];


	    pSlot->dataAddr.sin_port = htons (pSlot->dataAddr.sin_port);


            value = (portNum [0] << 24) | (portNum [1] << 16) |
                    (portNum [2] << 8) | portNum [3];
            pSlot->dataAddr.sin_addr.s_addr = htonl (value);

	    if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_PORT_SET],
			 0, 0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
	else if ((strncmp (pBuf, "PWD", 3) == 0) ||
		(strncmp (pBuf, "XPWD", 4) == 0))
	    {

	    (void) strcpy (pBuf, pSlot->curDirName);
	    if (ftpdCmdSend (pSlot, sock, 257, messages [MSG_CUR_DIR],
			 (int)ftpdPathForPrint(pSlot,pBuf),
			 0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
	else if (strncmp (pBuf, "STRU", 4) == 0)
	    {

	    if (pBuf [5] == 'F' || pBuf [5] == 'f')
		{
	        if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_FILE_STRU],
			     0, 0, 0, 0, 0, 0) == ERROR)
		    
		    goto connectionLost;
		}
	    else
		{
	        if (ftpdCmdSend (pSlot, sock, 504, messages [MSG_PARAM_BAD],
			     0, 0, 0, 0, 0, 0) == ERROR)

		    goto connectionLost;
		}
	    }
	else if (strncmp (pBuf, "MODE", 4) == 0)
	    {
	    /* specify transfer mode */

	    /* we only support stream mode -- no block or compressed mode */

	    if (pBuf [5] == 'S' || pBuf [5] == 's')
		{
	        if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_STREAM_MODE],
			     0, 0, 0, 0, 0, 0) == ERROR)

		    goto connectionLost;
		}
	    else
		{
	        if (ftpdCmdSend (pSlot, sock, 504, messages [MSG_PARAM_BAD],
			     0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		}
	    }
	else if (strncmp (pBuf, "ALLO", 4) == 0 ||
		 strncmp (pBuf, "ACCT", 4) == 0)
	    {
	    /* allocate and account commands are not need */

	    if (ftpdCmdSend (pSlot, sock, 202, messages [MSG_ALLOC_ACCOUNT],
			 0, 0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
	else if (strncmp (pBuf, "PASV", 4) == 0)
	    {
	    int outval1;
	    int outval2;
	    int outval3;
	    int outval4;
	    int outval5;
	    int outval6;


	    ftpdSockFree (&pSlot->dataSock);


	    if ((pSlot->dataSock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		{
		if (ftpdCmdSend (pSlot, sock, 425, messages [MSG_PASSIVE_ERROR],
			     0, 0, 0, 0, 0, 0) == ERROR)

		    goto connectionLost;
		continue ;
		}
	    

	    addrLen = sizeof (pSlot->dataAddr);


	    if (getsockname (pSlot->cmdSock,
			     (struct sockaddr *) &pSlot->dataAddr,
			     &addrLen) < 0)
		{

		if (ftpdCmdSend (pSlot, sock,
				 425, messages [MSG_PASSIVE_ERROR],
				 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		}


	    pSlot->dataAddr.sin_port = htons (0);
	    addrLen = sizeof (struct sockaddr_in);

	    if (bind (pSlot->dataSock, (struct sockaddr *)&pSlot->dataAddr,
		      sizeof (struct sockaddr_in)) < 0 ||
		getsockname (pSlot->dataSock,
			     (struct sockaddr *) &pSlot->dataAddr,
			     &addrLen) < 0 ||
		listen (pSlot->dataSock, 1) < 0)
		{
		ftpdSockFree (&pSlot->dataSock);
		if (ftpdCmdSend (pSlot, sock, 
				 425, messages [MSG_PASSIVE_ERROR],
				 0, 0, 0, 0, 0, 0) == ERROR)
		    goto connectionLost;
		continue;
		}


	    pSlot->status |= FTPD_PASSIVE;

	    value = pSlot->dataAddr.sin_addr.s_addr;
	    outval1 = ( (u_char *)&value)[0];
	    outval2 = ( (u_char *)&value)[1];
	    outval3 = ( (u_char *)&value)[2];
	    outval4 = ( (u_char *)&value)[3];


	    outval5 = ( (u_char *)&pSlot->dataAddr.sin_port)[0];
	    outval6 = ( (u_char *)&pSlot->dataAddr.sin_port)[1];


	    if (ftpdCmdSend (pSlot, pSlot->cmdSock, 
			     227, messages [MSG_PASSIVE_MODE],
			     outval1, outval2, outval3, outval4,
			     outval5, outval6) == ERROR)

		goto connectionLost;
	    }
	else if (strncmp (pBuf, "NOOP", 4) == 0)
	    {
	    /* don't do anything */

	    if (ftpdCmdSend (pSlot, sock, 200, messages [MSG_NOOP_OKAY],
			 0, 0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
       else
           {

	    if (ftpdCmdSend (pSlot, sock, 500, messages [MSG_BAD_COMMAND],
			 0, 0, 0, 0, 0, 0) == ERROR)

		goto connectionLost;
	    }
	}


    ftpdSessionDelete (pSlot);
    return (OK);
connectionLost:
    ftpdDebugMsg ("FTP Control connection error, closed.\n", 0,0,0,0);
    ftpdSessionDelete (pSlot);
    return (ERROR);
    }

/*******************************************************************************
*/

LOCAL STATUS ftpdDataConnGet
    (
    FTPD_SESSION_DATA   *pSlot        
    )
    {
    FAST int		newSock;	
    int			addrLen;	
    struct sockaddr_in	addr;	
    int			on = 1;		
    int			retry = 0;	



    if (pSlot->cmdSock == FTPD_SOCK_FREE)
        return (ERROR);

    pSlot->byteCount = 0;
    (void) time( &pSlot->timeUsed );

    if (pSlot->dataSock != FTPD_SOCK_FREE)
        {

	if (pSlot->status & FTPD_PASSIVE)
	    {
	    /* we're being passive.  wait for our client to connect to us. */

	    addrLen = sizeof (struct sockaddr);

	    ftpdDebugMsg ("doing accept for passive fd=%d\n",
		(int)pSlot->dataSock,0,0,0);

	    if ((newSock = accept (pSlot->dataSock, (struct sockaddr *) &addr,
				   &addrLen)) < 0)
		{
                ftpdCmdSend (pSlot, pSlot->cmdSock,
                             425, "Can't open data connection",
                             0, 0, 0, 0, 0, 0);

                ftpdSockFree (&pSlot->dataSock);

		/* we can't be passive no more */

		pSlot->status &= ~FTPD_PASSIVE;

		return (ERROR);
		}


            if (setsockopt (newSock, SOL_SOCKET, SO_KEEPALIVE, (char *) &on,
                            sizeof (on)) != 0)
                {
                ftpdSockFree (&pSlot->dataSock);
                return (ERROR);
                }


	    if (ftpdWindowSize < 0 || ftpdWindowSize > 65536)
		ftpdWindowSize = FTPD_WINDOW_SIZE;

	    /* set the window size  */

	    if (setsockopt(newSock, SOL_SOCKET, SO_SNDBUF,
		(char *)&ftpdWindowSize, sizeof (ftpdWindowSize)))
		    printf("Couldn't set the Send Window to 10k\n");

	    if (setsockopt(newSock, SOL_SOCKET, SO_RCVBUF,
		(char *)&ftpdWindowSize, sizeof (ftpdWindowSize)))
		    printf("Couldn't set the Send Window to 10k\n");

	    /* replace the dataSock with our new connection */

	    (void) close (pSlot->dataSock);
	    pSlot->dataSock = newSock;

	    /* N.B.: we stay passive */

            if (ftpdCmdSend (pSlot, pSlot->cmdSock,
                             150, "Opening %s mode data connection",
                             pSlot->status & FTPD_ASCII_TYPE ? (int) "ASCII"
                             : (int) "BINARY", 0, 0, 0, 0, 0) == ERROR)
		{
		(void) close (pSlot->dataSock);
		return (ERROR);
		}

	    return (OK);
	    }
	else
	    {
	    /* reuse the old connection -- it's still useful */

            if (ftpdCmdSend (pSlot, pSlot->cmdSock, 
                             125, "Using existing data connection",
                             0, 0, 0, 0, 0, 0) == ERROR)
		{
	    	ftpdSockFree (&pSlot->dataSock);
		return (ERROR);
		}
	    return (OK);
	    }
	}
    else
        {
        /* Determine address for local end of connection. */

        addrLen = sizeof (struct sockaddr);

        if (getsockname (pSlot->cmdSock, (struct sockaddr *) &addr, &addrLen)
                < 0)
            {
            return (ERROR);
            }

        /* Replace control port with default data port. */

        addr.sin_port = htons (FTP_DATA_PORT);

	/* open a new data socket */

	if ((pSlot->dataSock = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	    { 
	    return (ERROR);
	    }

	if (setsockopt (pSlot->dataSock, SOL_SOCKET,
			SO_REUSEADDR, (char *) &on, sizeof (on)) < 0 ||
	    bind (pSlot->dataSock, (struct sockaddr *) &addr,
		  sizeof (addr)) < 0)
	    {
	    ftpdSockFree (&pSlot->dataSock);
	    return (ERROR);
	    }


        /* Set socket address to PORT command values or default. */

        bcopy ( (char *)&pSlot->dataAddr, (char *)&addr, 
               sizeof (struct sockaddr_in));

	/* try until we get a connection to the client's port */

	while (connect (pSlot->dataSock,
			(struct sockaddr *) &addr, sizeof (addr)) < 0)
	    {
	    if ((errno & 0xffff) == EADDRINUSE && retry < FTPD_WAIT_MAX)
	        {
		taskDelay (FTPD_WAIT_INTERVAL * sysClkRateGet ());
		retry += FTPD_WAIT_INTERVAL;
		continue;
		}

	    /* timeout -- we give up */
            ftpdCmdSend (pSlot, pSlot->cmdSock,
                         425, "Can't build data connection",
                         0, 0, 0, 0, 0, 0);
	    ftpdSockFree (&pSlot->dataSock);
	    return (ERROR);
	    }

            if (setsockopt (pSlot->dataSock, SOL_SOCKET, SO_KEEPALIVE, 
                            (char *) &on, sizeof (on)) != 0)
                {
                ftpdSockFree (&pSlot->dataSock);
                return (ERROR);
                }

	    /* Check for window size validity */

	    if (ftpdWindowSize < 0 || ftpdWindowSize > 65536)
		ftpdWindowSize = FTPD_WINDOW_SIZE;

	    /* set the window size  */

	    if (setsockopt(pSlot->dataSock, SOL_SOCKET, SO_SNDBUF,
		(char *)&ftpdWindowSize, sizeof (ftpdWindowSize)))
		    printf("Couldn't set the Send Window to 10k\n");

	    if (setsockopt(pSlot->dataSock, SOL_SOCKET, SO_RCVBUF,
		(char *)&ftpdWindowSize, sizeof (ftpdWindowSize)))
		    printf("Couldn't set the Send Window to 10k\n");

            if (ftpdCmdSend (pSlot, pSlot->cmdSock, 
                             150, "Opening %s mode data connection",
                             pSlot->status & FTPD_ASCII_TYPE ? (int) "ASCII" :
                             (int) "BINARY", 0, 0, 0, 0, 0) == ERROR)
		{
		ftpdSockFree (&pSlot->dataSock);
		return (ERROR);
		}
	}

    return (OK);
    }

/*******************************************************************************
* SEE ALSO:
* ftpdDataStreamReceive  which is symmetric to this function.
*/

LOCAL void ftpdDataStreamSend
    (
    FTPD_SESSION_DATA   *pSlot,         /* pointer to our session slot */
    char                *fileName       /* pointer to the input file stream */
    )
    {
    FAST char	*pBuf;			
    FAST int	netFd;		
    FAST int	fileFd;			
    FAST int	cnt;		
    int		retval = 0;

    /* get a fresh connection or reuse the old one */

    if (ftpdDataConnGet (pSlot) == ERROR)
	{
	dataError (pSlot);
	return;
	}

    if ((fileFd = open (fileName, O_RDONLY, 0)) == ERROR)
	{
	fileError (pSlot);
	return;
	}

    pBuf = &pSlot->buf [0];

    if (pSlot->status & FTPD_ASCII_TYPE)	/* ASCII representation */
	{
	FAST FILE * inStream = NULL ;	/* buffered input file stream */
	FAST FILE * outStream = NULL ;	/* buffered output socket stream */
	FAST int	ch;		/* character holder */

	if ((inStream = fdopen (fileFd, "r")) == NULL)
	    {
	    fileError (pSlot);
	    return;
	    }

	if ((outStream = fdopen (pSlot->dataSock, "w")) == NULL)
	    {
	    goto data_err ;
	    }


	while ((ch = getc (inStream)) !=  EOF)
	    {
	    pSlot->byteCount++;

	    /* if '\n' is encountered, we prepend a '\r' */

	    if (ch == '\n')
		{
		if (ferror (outStream))
		    goto data_err ;

		if (putc ('\r', outStream) == EOF)
		    goto data_err ;

		taskDelay(0);
		}

	    if (putc (ch, outStream) == EOF)
		goto data_err ;

            /* Abort the file transfer if a shutdown is in progress. */

            if (ch == '\n' && ftpsShutdownFlag)
		goto data_err ;
	    }

	/* flush it out */

	(void) fflush (outStream);

	if (ferror (inStream))
	    {
	    /* error in reading the file */
	    goto file_err ;
	    }

	if (ferror (outStream))
	    {
	    /* error in sending the file */
	    goto data_err ;
	    }

	fclose (outStream);
	fclose (inStream);

	/* everything is okay */
	transferOkay (pSlot);
	return ;

data_err:
	dataError (pSlot);
	goto _err;

file_err:
	fileError (pSlot);
_err:
	if( outStream != NULL )
	    fclose (outStream);
	if( inStream != NULL )
	    fclose (inStream);
	return;
	}
    else if (pSlot->status & FTPD_BINARY_TYPE)	/* BINARY representation */
	{

	netFd = pSlot->dataSock;

	/* unbuffered block I/O between file and network */

	while ((cnt = read (fileFd, pBuf, pSlot->bufSize)) > 0 &&
	       (retval = write (netFd, pBuf, cnt)) == cnt)
            {
	    pSlot->byteCount += cnt;

            if (ftpsShutdownFlag)
                {
                /* Abort the file transfer if a shutdown is in progress. */

                cnt = 1;
                break;
                }
	    taskDelay(0);
            }

	/* cnt should be zero if the transfer ended normally */

	if (cnt != 0)
	    {
	    if (cnt < 0)
		{
		fileError (pSlot);
		close( fileFd );
		return;
		}

	    ftpdDebugMsg ("data xfer failed: read %d bytes, wrote %d bytes\n",
		cnt, retval,0,0);

	    dataError (pSlot);
	    close( fileFd );
	    return;
	    }

	transferOkay (pSlot);
	close( fileFd );
	}
    else
	unImplementedType (pSlot);	/* invalide representation type */
    }

/*******************************************************************************
* SEE ALSO:
* ftpdDataStreamSend which is symmetric to this function.
*/

LOCAL STATUS ftpdDataStreamReceive
    (
    FTPD_SESSION_DATA   *pSlot,
    FILE                *outStream
    )
    {
    FAST char	*pBuf;		
    FAST int 	ch;		
    FAST FILE	*inStream;	
    FAST int	fileFd;		
    FAST int	netFd;	
    FAST BOOL	dontPutc;	
    FAST int	cnt;		

    /* get a fresh data connection or reuse the old one */

    if (ftpdDataConnGet (pSlot) == ERROR)
	goto data_err;

    pBuf = &pSlot->buf [0];

    if (pSlot->status & FTPD_ASCII_TYPE)
	{
	/* ASCII representation */


	if ((inStream = fdopen (pSlot->dataSock, "r")) == NULL)
	    goto data_err;


	while ((ch = getc (inStream)) != EOF)
	    {
	    dontPutc = FALSE;

	    pSlot->byteCount++;


	    while (ch == '\r')
		{
		if (ferror (outStream))
		    {
		    fclose (inStream);
		    goto data_err ;
		    }


		if ((ch = getc (inStream)) != '\n')
		    {
		    (void) putc ('\r', outStream);

		    if (ch == '\0' || ch ==  EOF)
			{
			dontPutc = TRUE;
			break;
			}
		    }
		taskDelay(0);
		}

	    if (dontPutc == FALSE)
		(void) putc (ch, outStream);


            if (ch == '\n' && ftpsShutdownFlag)
	        {
	        fclose (inStream);
	        goto data_err ;
                }
            } 


	(void) fflush (outStream);

	if (ferror (inStream))
	    {

	    fclose (inStream);
	    goto data_err ;
	    }

	if (ferror (outStream))
	    {

	    fclose (inStream);
	    goto file_err ;
	    }


	fclose (inStream);
	transferOkay (pSlot);
	return OK ;
	}
    else if (pSlot->status & FTPD_BINARY_TYPE)
	{

	fileFd = fileno (outStream);

	netFd  = pSlot->dataSock;


	while ((cnt = fioRead (netFd, pBuf, pSlot->bufSize)) > 0)
	    {
	    if (write (fileFd, pBuf, cnt) != cnt)
		goto file_err ;

	    pSlot->byteCount += cnt;


            if (ftpsShutdownFlag)
                {
                cnt = -1;
                break;
                }
	    taskDelay(0);
	    }

	if (cnt < 0)
	    goto data_err ;


	transferOkay (pSlot);
	return OK ;
	}
    else
	{
	unImplementedType (pSlot);	
	return ERROR;
	}

file_err:
    fileError (pSlot);
    return ERROR;

data_err:
    dataError (pSlot);
    return ERROR;
    }

/*******************************************************************************
*
*/

LOCAL void ftpdSockFree
    (
    int *pSock
    )
    {
    if (*pSock != FTPD_SOCK_FREE)
        {
	(void) close (*pSock);
	*pSock = FTPD_SOCK_FREE;
	}
    }

/*******************************************************************************
*
* NOTE: we're using EOS as integral part of the pattern and name.
*/
LOCAL BOOL ftpDirListPattern ( char * pat, char * name )
    {
    FAST char * pPat, *pNext ;
    const char anyOne = '?' ;
    const char anyMany = '*' ;

    pPat = pat ;
    for(pNext = name ; * pNext != EOS ; pNext ++ )
	{
	if( *pPat == anyOne )
	    {
	    pPat ++ ;
	    }
	else if( *pPat == anyMany ) 
	    {
	    if ( pNext[0] == '.' )	/* '*' dont match . .. and .xxx */
		return FALSE ;
	    if ( toupper(pPat[1]) == toupper(pNext[1]) )
		pPat ++ ;
	    else if (toupper(pPat[1]) == toupper(pNext[0]) )
		pPat += 2 ;
	    else 
		continue ;
	    }
	else if( toupper(*pPat) != toupper(*pNext) )
	    {
	    return FALSE ;
	    }
	else
	    {
	    pPat ++ ;
	    }
	}
    /* loop is done, let's see if there is anything left */
    if( (*pPat == EOS) || (pPat[0] == anyMany && pPat[1] == EOS))
	return TRUE ;
    else
	return FALSE ;
    }

#ifdef	__unitest__
int ftpPatternTest(void)
    {
    int i, err = 0 ;
    const struct 
	{
	char * pat;
	char * name;
	BOOL result;
	} testData[] = {
	{ "*",		"abcd.e_f-j",			1},
	{ "*",		"1",				1},
	{ "*",		"",				1},
	{ "*abcde.f",	"abcde.f",			1},
	{ "*abcde.f",	"0abcde.f",			1},
	{ "*ab.f",	"0abcde.f",			0},
	{ "*de.f",	"0abcde.f",			1},
	{ "*ab?.c",	"zabcde.f",			0},
	{ "*ab??.c",	"zabcde.f",			0},
	{ "*ab???.c",	"zabcde.f",			0},
	{ "*ab???.?",	"zabcde.f",			1},
	{ "*ab???.?",	"aBcde.c",			1},
	{ "*",		"a",				1},
	{ "*",		"a.1",				1},
	{ "*",		"a.b.c.$%(",			1},
	{ "*.*",	"a.b",				1},
	{ "*.*",	"1.2.3.4",			1},
	{ "*.*",	".a.b",				1},
	{ "?*",		".x",				1},
	{ "?*",		"x.y.t.r.#@%",			1},
	{ "?*",		"x",				1},
	{ "?*",		".",				1},
	{ "?a*",	"aa",				1},
	{ "?a*",	"xa",				1},
	{ "?a*",	".a",				1},
	{ "?a*",	".ab",				1},
	{ "?a*",	"xaq1.&%#@___",			1},
	{ "?a*",	"_a-b.cde",			1},
	{ "?A?B?C",	"AaBbCc",			1},
	{ "?A?B?C",	"aabbcc",			1},
	{ "?A?B?C",	"AABBCC",			1},
	{ "?A?B?C",	".a.b.c",			1},
	{ "?A?B?C",	"xa-b_c",			1},
	{ "?*.dat",	"p.dat",			1},
	{ "?*.dat",	"README.Dat",			1},
	{ "?*.dat",	"Copy of some.dat",		1},
	{ "README?*",	"readMe.",			1},
	{ "README?*",	"Readme1",			1},
	{ "README?*",	"readmeFile",			1},
	{ "README?*",	"ReAdMe, please",		1},
	{ "??? ???",	"fll .,&",			1},
	{ "??? ???",	"asd ___",			1},
	{ "??? ???",	"123 321",			1},
	{ "*.*",	"a",				0},
	{ "*.*",	"1-2-3-4",			0},
	{ "?*.*",	".a",				0},
	{ "?a*",	"a",				0},
	{ "?a*",	"ax",				0},
	{ "?A?B?C",	"aBbCc",			0},
	{ "?A?B?C",	"aabcc",			0},
	{ "?A?B?C",	"AABBC",			0},
	{ "?A?B?C",	"a.b.c",			0},
	{ "?A?B?C",	"aab.c",			0},
	{ "?*.dat",	"p.d",				0},
	{ "?*.dat",	".dat",				0},
	{ "?*.dat",	"README_Dat",			0},
	{ "?*.dat",	"Copy of some dat",		0},
	{ "README?*",	"readMe",			0},
	{ "README?*",	"README",			0},
	{ "README?*",	"1README_DAT",			0},
	{ "README?*",	"_ReAdMe, please",		0},
	{ "??? ???",	"fll_.,&",			0},
	{ "??? ???",	"asd.___",			0},
	{ "??? ???",	"23 321",			0},
	{ "??? ???",	"123 21",			0},
	{ "",		"a",				0}
	};

    for(i=0; i <NELEMENTS(testData); i++ )
	{
	if( ftpDirListPattern( testData[i].pat, testData[i].name ) !=
		testData[i].result )
	    {
	    printf("Test failed, pattern \"%s\", name \"%s\", expected %d\n",
		testData[i].pat, testData[i].name, testData[i].result );
	    err ++ ;
	    }
	}
    return (err);
    }
#endif	/*__unitest__*/

/*******************************************************************************
*
* ftpdDirListEnt - list one file or directory
*
* INTERNAL
* we dont check results of each fdprintf here, just the last one, which
* should be enough in case the connection broke.
*/
LOCAL STATUS ftpdDirListEnt
    (
    int		fd,		
    char *	fileName,	
    struct stat * fileStat,	
    char *	prefix,		
    BOOL	doLong		
    )
    {
    time_t		now;		/* current clock */
    struct tm		nowDate;	/* current date & time */
    struct tm		fileDate;	/* file date/time      (long listing) */
    const  char 	*monthNames[] = {"???", "Jan", "Feb", "Mar", "Apr",
				    	 "May", "Jun", "Jul", "Aug", "Sep",
				    	 "Oct", "Nov", "Dec"};
    char fType ;

    if(doLong)
	{
	/* Break down file modified time */
	time( &now );
	localtime_r (&now, &nowDate);
	localtime_r (&fileStat->st_mtime, &fileDate);

#ifndef SMILEOS
	if( fileStat->st_attrib & DOS_ATTR_RDONLY )
	    fileStat->st_mode &= ~(S_IWUSR|S_IWGRP|S_IWOTH);
#endif
	if( S_ISDIR (fileStat->st_mode) )
	    fType = 'd' ;
	else if( S_ISREG (fileStat->st_mode) )
	    fType = '-' ;
	else 
	    fType = '?' ;

	/* print file mode */
	fdprintf(fd, "%c%c%c%c%c%c%c%c%c%c",
	    fType,
	    (fileStat->st_mode & S_IRUSR)? 'r':'-',
	    (fileStat->st_mode & S_IWUSR)? 'w':'-',
	    (fileStat->st_mode & S_IXUSR)? 'x':'-',
#ifndef SMILEOS
	    '-', '-', '-', 	/* XXX- figure out what to do with last flags */
	    (fileStat->st_attrib & DOS_ATTR_ARCHIVE)? 'A':'-',
	    (fileStat->st_attrib & DOS_ATTR_SYSTEM)?  'S':'-',
	    (fileStat->st_attrib & DOS_ATTR_HIDDEN)?  'H':'-'
#else
	    '-', '-', '-'  /* XXX- figure out what to do with last flags */
#endif
	    );

	/* fake links, user and group fields */
	fdprintf(fd, " %2d %-7s %-7s ", fileStat->st_nlink, "user", "group" );

	/* print file size - XXX: add 64 bit file size support */
	fdprintf(fd, " %9lu ", fileStat->st_size );

	/* print date */
	if( fileDate.tm_year == nowDate.tm_year )
	    {
	    fdprintf(fd, "%s %2d %02d:%02d ",
		    monthNames [fileDate.tm_mon + 1],/* month */
		    fileDate.tm_mday,               /* day of month */
		    fileDate.tm_hour,		/* hour */
		    fileDate.tm_min		/* minute */
		    );
	    }
	else
	    {
	    fdprintf(fd, "%s %2d  %04d ",
		    monthNames [fileDate.tm_mon + 1],/* month */
		    fileDate.tm_mday,		/* day of month */
		    fileDate.tm_year+1900	/* year */
		    );
	    }

	} /* if doLong */
    else
	{ /* short listing */
	if( prefix != NULL)
	    fdprintf(fd, prefix);
	}

    /* last, print file name */
    if( fdprintf(fd, "%s\n", fileName ) == ERROR )
	return ERROR;

    return OK ;
    }
/*******************************************************************************
* INTERNAL
* Borrowed from ls() in usrLib.c.
*
* RETURNS:  OK or ERROR.
*
* SEE ALSO: ls(), stat()
*/

LOCAL STATUS ftpdDirListGet
    (
    int         sd,             
    char        *dirName,       
    BOOL        doLong         
    )
    {
    FAST STATUS		status;		
    FAST DIR		*pDir;		
    FAST struct dirent	*pDirEnt;	
    struct stat		fileStat;	
#ifndef SMILEOS
    BOOL		firstFile;	
#endif
    char		*pPattern;	
    char 		fileName [MAX_FILENAME_LENGTH];
					


    if (dirName == NULL)
	dirName = ".";

    pDir = NULL ; pPattern = NULL ;

    /* Open dir */

    pDir = opendir (dirName) ;

    if( pDir == NULL && (pPattern = rindex(dirName, '/')) != NULL )
	{
	*pPattern++ = EOS ;
	pDir = opendir (dirName) ;
	}
 
     if( pDir == NULL )
	goto _nodir ;


    /* List files */
    ftpdDebugMsg ("listing dir %s pattern %s\n", (int)dirName,(int) pPattern,
	0,0);

    status = OK;
#ifndef SMILEOS
    firstFile = TRUE;
#endif

    do
	{
	errno = OK;

    	pDirEnt = readdir (pDir);

	if (pDirEnt != NULL)
	    {
	    if (pPattern != NULL &&
		ftpDirListPattern( pPattern, pDirEnt->d_name) == FALSE )
		continue ;

	    if (doLong)				/* if doing long listing */
		{

		strncpy( fileName, dirName, MAX_FILENAME_LENGTH );
		if( fileName [ strlen(fileName) -1 ] != '/');
		    strcat(fileName, "/");
		strncat( fileName,  pDirEnt->d_name, 
			MAX_FILENAME_LENGTH - strlen(dirName) );

		if (stat (fileName, &fileStat) != OK)
		    {
		    bzero( (caddr_t) &fileStat, sizeof(fileStat));
		    }
		}

	    if( ftpdDirListEnt(sd, pDirEnt->d_name,
			&fileStat, NULL, doLong ) == ERROR )
		return ( ERROR | closedir (pDir) );
	    }
	else					
	    {
	    if (errno != OK)			
		{
		if (fdprintf (sd, "error reading entry: %x\n", errno) == ERROR)
		    return ( ERROR | closedir (pDir));
		status = ERROR;
		}
	    }

	} while (pDirEnt != NULL);		/* until end of dir */


    /* fdprintf (sd, "\n"); XXX*/

    /* Close dir */
    status |= closedir (pDir);

    return (status);

_nodir:
    fdprintf (sd, "Can't open \"%s\".\n", dirName);
    return (ERROR);
    }

/*******************************************************************************
*
* unImplementedType - send FTP invalid representation type error reply
*/

LOCAL void unImplementedType
    (
    FTPD_SESSION_DATA   *pSlot
    )
    {
    ftpdCmdSend (pSlot, pSlot->cmdSock, 550, messages [MSG_TYPE_ERROR],
		 FTPD_REPRESENTATION (pSlot), 0, 0, 0, 0, 0);
    ftpdSockFree (&pSlot->dataSock);
    }

/*******************************************************************************
*
* dataError - send FTP data connection error reply
*
* Send the final error message about connection error and delete the session.
*/

LOCAL void dataError
    (
    FTPD_SESSION_DATA   *pSlot
    )
    {
    ftpdCmdSend (pSlot, pSlot->cmdSock, 426, messages [MSG_DATA_CONN_ERROR], 
                 0, 0, 0, 0, 0, 0);
    ftpdSockFree (&pSlot->dataSock);
    }

/*******************************************************************************
*
* fileError - send FTP file I/O error reply
*
* Send the final error message about file error and delete the session.
*/

LOCAL void fileError
    (
    FTPD_SESSION_DATA  *pSlot
    )
    {
    ftpdCmdSend (pSlot, pSlot->cmdSock, 551, messages [MSG_INPUT_FILE_ERROR], 
                 0, 0, 0, 0, 0, 0);
    ftpdSockFree (&pSlot->dataSock);
    }

/*******************************************************************************
*
* transferOkay - send FTP file transfer completion reply
*/

LOCAL void transferOkay
    (
    FTPD_SESSION_DATA   *pSlot
    )
    {
    ftpdSockFree (&pSlot->dataSock);
    ftpdCmdSend (pSlot, pSlot->cmdSock, 226, messages [MSG_TRANS_COMPLETE], 
                 0, 0, 0, 0, 0, 0);
    }

/*******************************************************************************
*
* ftpdCmdSend - send a FTP command reply
*
* In response to a request, we send a reply containing completion
* status, error status, and other information over a command connection.
*/

LOCAL STATUS ftpdCmdSend
    (
    FTPD_SESSION_DATA  *pSlot,         /* pointer to the session slot */
    int 		controlSock, 	/* control socket for reply */
    int                 code,           /* FTP status code */
    const char         *format,        /* printf style format string */
    int                 arg1,
    int                 arg2,
    int                 arg3,
    int                 arg4,
    int                 arg5,
    int                 arg6
    )
    {
    int                 buflen;
    char		buf [BUFSIZE];		/* local buffer */
    FAST char 		*pBuf = &buf [0];	/* pointer to buffer */
    BOOL 		lineContinue =
				(code & FTPD_MULTI_LINE) == FTPD_MULTI_LINE;

    /*
     * If this routine is called before a session is established, the
     * pointer to session-specific data is NULL. Otherwise, exit with
     * an error if an earlier attempt to send a control message failed.
     */

    if ( (pSlot != NULL) && (pSlot->cmdSockError == ERROR))
        return (ERROR);

    code &= ~FTPD_MULTI_LINE;	/* strip multi-line bit from reply code */

    /* embed the code first */

    (void) sprintf (pBuf, "%d%c", code, lineContinue ? '-' : ' ');
    pBuf += strlen (pBuf);

    (void) sprintf (pBuf, format, arg1, arg2, arg3, arg4, arg5, arg6);
    pBuf += strlen (pBuf);

    /* telnet style command terminator */

    (void) sprintf (pBuf, "\r\n");

    /* send it over to our client */

    buflen = strlen (buf);

    if ( write (controlSock, buf, buflen) != buflen )
        {
        if (pSlot != NULL)
            pSlot->cmdSockError = ERROR;
    	ftpdDebugMsg ("sent %s Failed on write\n", (int)buf,0,0,0);
        return (ERROR);    /* Write Error */
        }

    ftpdDebugMsg ("sent %s\n", (int)buf,0,0,0);
    return (OK);			/* Command Sent Successfully */
    }

/*******************************************************************************
*
* ftpdDebugMsg - print out FTP command request and replies for debugging.
*/

LOCAL void ftpdDebugMsg
    (
    char        *format,
    int         arg1,
    int         arg2,
    int         arg3,
    int         arg4
    )
    {
    if (ftpdDebug == TRUE)
	logMsg (format, arg1, arg2, arg3, arg4, 0, 0);
    }

STATUS ftpLoginUserVerify(char  *name, char  *passwd )
    {
        if ( (strcmp(name, "hualong") == 0) && (strcmp(passwd, "hualong") == 0) )
        {
            return (OK);
        }
        else
        {
            return (ERROR);
        }
    }


/*******************************************************************************
* RETURNS: N/A
*
* SEE ALSO: loginLib
*/

void ftpdLoginInstall
    (
    FUNCPTR pLoginFunc
    )
    {
    pLoginVrfyFunc = pLoginFunc;
    }

/*******************************************************************************
* RETURNS: OK, or ERROR
*/
STATUS ftpdAnonymousAllow
    (
    const char *	rootDir,	/* path for guest root directory */
    const char *	uploadDir	/* path for guest uploads */
    )
    {
    if( rootDir == NULL )
	return ERROR;

    strncpy( guestHomeDir, rootDir, MAX_FILENAME_LENGTH );

    if( uploadDir != NULL )
	{
	strcpy( writeDirName, guestHomeDir );
	strcat( writeDirName, "/" );
	strncat( writeDirName, uploadDir, MAX_FILENAME_LENGTH );
	pathCondense( writeDirName );
	}
    else
	{
	writeDirName[0] = EOS ;	/* clear */
	}

    return OK ;
    }

/*******************************************************************************
*
* RETURNS: OK
*/
STATUS ftpdHomeDirSet( const char * homeDir )
    {
    if( homeDir == NULL )
	return ERROR;

    strncpy( defaultHomeDir, homeDir, MAX_FILENAME_LENGTH );

    return( OK);
    }

