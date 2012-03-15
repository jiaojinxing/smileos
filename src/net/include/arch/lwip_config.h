/*********************************************************************************************************
**
** Copyright (c) 2011 - 2012  Jiao JinXing <JiaoJinXing1987@gmail.com>
**
** Licensed under the Academic Free License version 2.1
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**
**--------------------------------------------------------------------------------------------------------
** File name:               lwip_config.h
** Last modified Date:      2012-2-22
** Last Version:            1.0.0
** Descriptions:            LwIP ����ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created by:              JiaoJinXing
** Created date:            2012-2-22
** Version:                 1.0.0
** Descriptions:            �����ļ�
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef LWIP_CONFIG_H_
#define LWIP_CONFIG_H_

#include "lwip_fix.h"

#define __KB            KB
#define __MB            MB
/*********************************************************************************************************
*                                       lwip ����
*
*  ע  ��  : �����������Ϊ������ lwip ѡ��, ��ϸ������Ҫ�� lwip ��������ļ������.
*            ���ʹ����������, ����������� PBUF_POOL_BUFSIZE ������, ����: wifi �������ڵײ�Э����Ҫ�ϴ�
*            ��ͷ��, ������Ҫ PBUF_POOL_BUFSIZE �Ӵ�. (ʹ�������� POOL �ڴ���������������Ч��, ��ֱ��
*            ʹ�� DMA �������ݰ�)
*
*            LW_CFG_LWIP_MSG_SIZE
*            LW_CFG_LWIP_NUM_NETBUF
*            LW_CFG_LWIP_NUM_POOLS
*            LW_CFG_LWIP_MEM_SIZE       �����ö�������Э��ջ��������ϢϢ���.
*
*********************************************************************************************************/

/*********************************************************************************************************
                     lwip �������� (�������þ��� RAM ʹ������ lwip Ч��)
*********************************************************************************************************/

#define LW_CFG_LWIP_MEM_SIZE            (512 * __KB)                    /*  lwip �ڴ��С               */
#define LW_CFG_LWIP_MSG_SIZE            512                             /*  lwip �ڲ���Ϣ���л��峤��   */
#define LW_CFG_LWIP_POOL_SIZE           1600                            /*  lwip POOL �ڴ���С        */
                                                                        /*  ע��: �������ֶ����        */

#define LW_CFG_LWIP_NUM_PBUFS           256                             /*  ϵͳ�� pbuf ����            */
#define LW_CFG_LWIP_NUM_NETBUF          256                             /*  ����������� netbuf ����    */
#define LW_CFG_LWIP_NUM_POOLS           512                             /*  �ȳ�����������(���ٻ���)    */
                                                                        /*  ��������ʹ��                */

/*********************************************************************************************************
                                  lwip У������� (����ʹ���������У��Ͳ����)
*********************************************************************************************************/

#define LW_CFG_LWIP_GEN_CHECKSUM        1                               /*  �Ƿ�ͨ��Э��ջ�Լ�����У���*/
                                                                        /*  ����������Զ����㷢��У���*/
                                                                        /*  �����ÿ�Ϊ 0                */
#define LW_CFG_LWIP_CHECK_CHECKSUM      1                               /*  �Ƿ�������������ݰ�У���*/
                                                                        /*  ����������Զ�������У���*/
                                                                        /*  �����ÿ�Ϊ 0                */

/*********************************************************************************************************
                                  lwip �������
*********************************************************************************************************/

#define LW_CFG_LWIP_DEBUG               1                               /*  lwip �����ڵ���ģʽ         */
#define LW_CFG_LWIP_DEBUG_LEVEL         1                               /*  lwip ���Եȼ�               */

#define LW_CFG_LWIP_PPP                 0                               /*  lwip ppp ֧��               */
#define LW_CFG_LWIP_PPPOE               0                               /*  lwip pppoe ֧��             */
#define LW_CFG_LWIP_NUM_PPP             2                               /*  lwip ppp ���Ự��         */

#define LW_CFG_LWIP_PRIORITY            20                              /*  lwip thread default priority*/
#define LW_CFG_LWIP_STK_SIZE            16 * KB                         /*  lwip thread default stksize */

#define LW_CFG_LWIP_TCP_PCB             60                              /*  ����ͬʱ�� TCP ������       */
#define LW_CFG_LWIP_UDP_PCB             50                              /*  ����ͬʱ�� UDP ����         */
#define LW_CFG_LWIP_RAW_PCB             15                              /*  ����ͬʱ�� RAW ����         */

#define LW_CFG_LWIP_DHCP                0                               /*  �Ƿ����� DHCP Э��          */
#define LW_CFG_LWIP_AUTOIP              0                               /*  �Զ�������ȡ IP (һ�㲻��Ҫ)*/

/*********************************************************************************************************
*                                       lwip SNMP ����
*********************************************************************************************************/

#define LW_CFG_LWIP_SNMP                0                               /*  SNMP ����                   */

/*********************************************************************************************************
*                                       lwip IGMP ����
*********************************************************************************************************/

#define LW_CFG_LWIP_IGMP                0                               /*  �Ƿ�ʹ�� IGMP               */
#define LW_CFG_LWIP_IGMP_GROUP          20                              /*  �ɼ���� IGMP ���������    */

/*********************************************************************************************************
*                                       lwip ��̫������
*********************************************************************************************************/

#define LW_CFG_LWIP_ARP_TABLE_SIZE      30                              /*  ��̫���ӿ� ARP ���С       */
#define LW_CFG_LWIP_ARP_TRUST_IP_MAC    0                               /*  ���� IP ���Ƿ���� ARP      */
                                                                        /*  windows ������, ����Ϊ 0    */
#define LW_CFG_LWIP_ETH_PAD_SIZE        0                               /*  ��̫��ͷ����������        */

/*********************************************************************************************************
*                                       �������������
*********************************************************************************************************/

#define LW_CFG_STRUCT_PACK_FIELD(x)     x
#define LW_CFG_STRUCT_PACK_STRUCT
#define LW_CFG_STRUCT_PACK_BEGIN        __packed                        /*  ���ֽ����Žṹ��            */
#define LW_CFG_STRUCT_PACK_END                                          /*  �������ֽ����Žṹ��        */

/*********************************************************************************************************
  Platform specific locking
*********************************************************************************************************/

#define SYS_LIGHTWEIGHT_PROT            1

/*********************************************************************************************************
  Memory options
*********************************************************************************************************/

#define MEM_SIZE                        LW_CFG_LWIP_MEM_SIZE            /*  malloc �Ѵ�С               */
#define MEMP_NUM_PBUF                   LW_CFG_LWIP_NUM_PBUFS           /*  npbufs                      */
#define PBUF_POOL_SIZE                  LW_CFG_LWIP_NUM_POOLS           /*  pool num                    */
#define PBUF_POOL_BUFSIZE               LW_CFG_LWIP_POOL_SIZE           /*  pool block size             */

#if MEM_SIZE >= (32 * __MB)
#define MEMP_NUM_REASSDATA              150                             /*  ͬʱ��������� IP ���ݰ�    */
#elif MEM_SIZE >= (1 * __MB)
#define MEMP_NUM_REASSDATA              100
#elif MEM_SIZE >= (512 * __KB)
#define MEMP_NUM_REASSDATA              80
#elif MEM_SIZE >= (256 * __KB)
#define MEMP_NUM_REASSDATA              40
#elif MEM_SIZE >= (128 * __KB)
#define MEMP_NUM_REASSDATA              20
#else
#define MEMP_NUM_REASSDATA              5
#endif                                                                  /*  MEM_SIZE >= ...             */

/*********************************************************************************************************
  ...PCB
*********************************************************************************************************/

#define MEMP_NUM_RAW_PCB                LW_CFG_LWIP_RAW_PCB
#define MEMP_NUM_UDP_PCB                LW_CFG_LWIP_UDP_PCB
#define MEMP_NUM_TCP_PCB                LW_CFG_LWIP_TCP_PCB
#define MEMP_NUM_TCP_PCB_LISTEN         LW_CFG_LWIP_TCP_PCB

#define MEMP_NUM_NETCONN                (LW_CFG_LWIP_RAW_PCB + LW_CFG_LWIP_UDP_PCB + \
                                         LW_CFG_LWIP_TCP_PCB)

#define MEMP_NUM_TCPIP_MSG_API          (LW_CFG_LWIP_TCP_PCB + LW_CFG_LWIP_UDP_PCB + LW_CFG_LWIP_RAW_PCB)
#define MEMP_NUM_TCPIP_MSG_INPKT        LW_CFG_LWIP_MSG_SIZE            /*  tcp input msgqueue use      */

/*********************************************************************************************************
  check sum
*********************************************************************************************************/

#if LW_CFG_LWIP_GEN_CHECKSUM == 0
#define CHECKSUM_GEN_IP                 0
#define CHECKSUM_GEN_UDP                0
#define CHECKSUM_GEN_TCP                0
#endif                                                                  /*  LW_CFG_LWIP_GEN_CHECKSUM    */

#if LW_CFG_LWIP_CHECK_CHECKSUM == 0
#define CHECKSUM_CHECK_IP               0
#define CHECKSUM_CHECK_UDP              0
#define CHECKSUM_CHECK_TCP              0
#endif                                                                  /*  LW_CFG_LWIP_CHECK_CHECKSUM  */

#define LWIP_CHECKSUM_ON_COPY           1                               /*  �������ݰ�ͬʱ���� chksum   */

/*********************************************************************************************************
  SmileOS do not use MEMP_NUM_NETCONN, because sylixos use another socket interface.
*********************************************************************************************************/

#define IP_FORWARD                      1                               /*  ���� IP ת��                */
#define IP_REASSEMBLY                   1
#define IP_FRAG                         1
#define IP_REASS_MAX_PBUFS              (MEMP_NUM_PBUF / 2)

#define IP_SOF_BROADCAST                1                               /*  Use the SOF_BROADCAST       */
#define IP_SOF_BROADCAST_RECV           1

#define IP_FORWARD_ALLOW_TX_ON_RX_NETIF 1

/*********************************************************************************************************
  IPv6
*********************************************************************************************************/

#define LWIP_IPV6                       1
#define LWIP_IPV6_MLD                   1
#define LWIP_IPV6_FORWARD               1
#define LWIP_ICMP6                      1
#define LWIP_IPV6_FRAG                  1
#define LWIP_IPV6_REASS                 1

#define MEMP_NUM_MLD6_GROUP             16
#define LWIP_ND6_NUM_NEIGHBORS          LW_CFG_LWIP_ARP_TABLE_SIZE
#define LWIP_ND6_NUM_DESTINATIONS       LW_CFG_LWIP_ARP_TABLE_SIZE
#define LWIP_IPV6_DHCP6                 LW_CFG_LWIP_DHCP

/*********************************************************************************************************
  dhcp & autoip
*********************************************************************************************************/

#define LWIP_DHCP                       LW_CFG_LWIP_DHCP                /*  DHCP                        */
#define LWIP_DHCP_BOOTP_FILE            0                               /*  not include bootp file now  */
#define LWIP_AUTOIP                     LW_CFG_LWIP_AUTOIP

#if (LWIP_DHCP > 0) && (LWIP_AUTOIP > 0)
#define LWIP_DHCP_AUTOIP_COOP           1
#endif                                                                  /*  (LWIP_DHCP > 0)             */
                                                                        /*  (LWIP_AUTOIP > 0)           */
#define MEMP_NUM_SYS_TIMEOUT            (LW_CFG_LWIP_RAW_PCB + LW_CFG_LWIP_UDP_PCB + \
                                         LW_CFG_LWIP_TCP_PCB + 20)

#define MEMP_NUM_NETBUF                 LW_CFG_LWIP_NUM_NETBUF

/*********************************************************************************************************
  ICMP
*********************************************************************************************************/

#define LWIP_ICMP                       1

/*********************************************************************************************************
  RAW
*********************************************************************************************************/

#define LWIP_RAW                        1

/*********************************************************************************************************
  SNMP
*********************************************************************************************************/

#define LWIP_SNMP                       LW_CFG_LWIP_SNMP
#define SNMP_PRIVATE_MIB                1                               /*  support now!                */

#define SNMP_GET_SYSUPTIME(sysuptime)   {                                       \
                                            __netSnmpGetTimestamp(&sysuptime);  \
                                        }
#define SNMP_PRIVATE_MIB_INIT()         __netSnmpPriMibInit()

/*********************************************************************************************************
  IGMP
*********************************************************************************************************/

#define LWIP_IGMP                       LW_CFG_LWIP_IGMP
#define MEMP_NUM_IGMP_GROUP             LW_CFG_LWIP_IGMP_GROUP

/*********************************************************************************************************
  DNS
*********************************************************************************************************/

#define LWIP_DNS                        0

#ifndef MEMP_NUM_NETDB
#define MEMP_NUM_NETDB                  10
#endif                                                                  /*  MEMP_NUM_NETDB              */

#define DNS_MAX_NAME_LENGTH             PATH_MAX

#define DNS_LOCAL_HOSTLIST              1
#define DNS_LOCAL_HOSTLIST_INIT         {{"none-host", 0UL}}

                                                                        /*  ��Χ IPv4 �����ֽ����ַ    */
#define DNS_LOCAL_HOSTLIST_IS_DYNAMIC   1
#define DNS_LOOKUP_LOCAL_EXTERN(x)      __inetHostTableGetItem(x)

/*********************************************************************************************************
  transmit layer (ע��: ��� TCP_WND �����������ջ���, ���������������ʱ, ����оƬ�������.
                        ��������������, ���������������� TCP_WND ��С. ��ȷ������ 2 * TCP_MSS)
*********************************************************************************************************/

#define LWIP_UDP                        1
#define LWIP_UDPLITE                    1
#define LWIP_NETBUF_RECVINFO            1

#define LWIP_TCP                        1
#define TCP_LISTEN_BACKLOG              1
#define LWIP_TCP_TIMESTAMPS             1

#define TCP_QUEUE_OOSEQ                 1

#ifndef TCP_MSS
#define TCP_MSS                         1460                            /*  usually                     */
#endif                                                                  /*  TCP_MSS                     */

#define TCP_CALCULATE_EFF_SEND_MSS      1                               /*  use effective send MSS      */

#if MEM_SIZE >= (8 * __MB)
#define TCP_WND                         ((64 * __KB) - 1)               /*  MAX WINDOW                  */
#elif MEM_SIZE >= (4 * __MB)
#define TCP_WND                         (32  * __KB)
#elif MEM_SIZE >= (1 * __MB)
#define TCP_WND                         (16  * __KB)
#elif MEM_SIZE >= (512 * __KB)
#define TCP_WND                         ( 8  * __KB)
/*********************************************************************************************************
  MEM_SIZE < 128 KB  SMALL TCP_MSS XXX
*********************************************************************************************************/
#elif MEM_SIZE >= (64 * __KB)
#define TCP_WND                         ( 4  * TCP_MSS)
#else
#undef  TCP_MSS
#define TCP_MSS                         536                             /*  small mss                   */
#define TCP_WND                         ( 4  * TCP_MSS)
#endif                                                                  /*  MEM_SIZE >= ...             */

#if TCP_WND < (4  * TCP_MSS)
#undef  TCP_WND
#define TCP_WND                         ( 4  * TCP_MSS)                 /*  must bigger than 4 * TCP_MSS*/
#endif                                                                  /*  TCP_WND < (4  * TCP_MSS)    */

#define TCP_SND_BUF                     ((64 * __KB) - 1)               /*  tcp snd buf size (max size) */
#define MEMP_NUM_TCP_SEG                (8 * TCP_SND_QUEUELEN)

#define LWIP_TCP_KEEPALIVE              1
#define LWIP_SO_SNDTIMEO                1
#define LWIP_SO_RCVTIMEO                1
#define LWIP_SO_RCVBUF                  1

#define SO_REUSE                        1                               /*  enable SO_REUSEADDR         */
#define SO_REUSE_RXTOALL                1

#define LWIP_FIONREAD_LINUXMODE         1                               /*  linux FIONREAD compatibility*/

/*********************************************************************************************************
  Network Interfaces options
*********************************************************************************************************/

#define LWIP_NETIF_HOSTNAME             1                               /*  netif have nostname member  */
#define LWIP_NETIF_API                  1                               /*  support netif api           */
#define LWIP_NETIF_STATUS_CALLBACK      1                               /*  interface status change     */
#define LWIP_NETIF_REMOVE_CALLBACK      1                               /*  interface remove hook       */
#define LWIP_NETIF_LINK_CALLBACK        1                               /*  link status change          */
#define LWIP_NETIF_HWADDRHINT           1                               /*  XXX                         */
#define LWIP_NETIF_LOOPBACK             1

/*********************************************************************************************************
  eth net options
*********************************************************************************************************/

#define LWIP_ARP                        1
#define ARP_QUEUEING                    1
#define ARP_TABLE_SIZE                  LW_CFG_LWIP_ARP_TABLE_SIZE
#define ETHARP_TRUST_IP_MAC             LW_CFG_LWIP_ARP_TRUST_IP_MAC
#define ETHARP_SUPPORT_VLAN             1                               /*  IEEE 802.1q VLAN            */
#define ETH_PAD_SIZE                    LW_CFG_LWIP_ETH_PAD_SIZE
#define ETHARP_SUPPORT_STATIC_ENTRIES   1

/*********************************************************************************************************
  loop interface
*********************************************************************************************************/

#define LWIP_HAVE_LOOPIF                1                               /*  127.0.0.1                   */

/*********************************************************************************************************
  slip interface
*********************************************************************************************************/

#define LWIP_HAVE_SLIPIF                0

/*********************************************************************************************************
  inet thread
*********************************************************************************************************/

#define TCPIP_THREAD_NAME               "lwip"
#define TCPIP_THREAD_STACKSIZE          LW_CFG_LWIP_STK_SIZE
#define TCPIP_THREAD_PRIO               LW_CFG_LWIP_PRIORITY

#define SLIPIF_THREAD_NAME              "slip"
#define SLIPIF_THREAD_STACKSIZE         LW_CFG_LWIP_STK_SIZE
#define SLIPIF_THREAD_PRIO              LW_CFG_LWIP_PRIORITY

#define PPP_THREAD_NAME                 "ppp"
#define PPP_THREAD_STACKSIZE            LW_CFG_LWIP_STK_SIZE
#define PPP_THREAD_PRIO                 LW_CFG_LWIP_PRIORITY

#define DEFAULT_THREAD_NAME             "netdef"
#define DEFAULT_THREAD_STACKSIZE        LW_CFG_LWIP_STK_SIZE
#define DEFAULT_THREAD_PRIO             LW_CFG_LWIP_PRIORITY

/*********************************************************************************************************
  Socket options
*********************************************************************************************************/

#define ERRNO                                                           /*  include errno               */
#define LWIP_SOCKET                     1
#define LWIP_TIMEVAL_PRIVATE            0                               /*  SmileOS has already defined */
#define LWIP_COMPAT_SOCKETS             1                               /*  some function conflict      */

#define LWIP_POSIX_SOCKETS_IO_NAMES     0                               /*  do not have this!!!         */

#define LWIP_DNS_API_HOSTENT_STORAGE    1                               /*  have bsd DNS                */
#define LWIP_DNS_API_DECLARE_STRUCTS    1                               /*  use lwip DNS struct         */

/*********************************************************************************************************
  Statistics options
*********************************************************************************************************/

#define LWIP_STATS                      1
#define LWIP_STATS_DISPLAY              1
#define LWIP_STATS_LARGE                1

/*********************************************************************************************************
  PPP options
*********************************************************************************************************/

#define PPP_SUPPORT                     LW_CFG_LWIP_PPP
#define PPPOE_SUPPORT                   LW_CFG_LWIP_PPPOE

#if PPP_SUPPORT > 0 || PPPOE_SUPPORT > 0
#define MEMP_NUM_PPPOE_INTERFACES       LW_CFG_LWIP_NUM_PPP
#define NUM_PPP                         LW_CFG_LWIP_NUM_PPP

#define PAP_SUPPORT                     1
#define CHAP_SUPPORT                    1
#define MD5_SUPPORT                     1
#define VJ_SUPPORT                      1

#define LCP_ECHOINTERVAL                10                              /*  check lcp link per 10seconds*/
#endif                                                                  /*  PPP_SUPPORT > 0 ||          */
                                                                        /*  PPPOE_SUPPORT > 0           */

/*********************************************************************************************************
  SmileOS ��Ҫʹ�� lwip core lock ģʽ, ��֧��ȫ˫�� socket
*********************************************************************************************************/

#define LWIP_TCPIP_CORE_LOCKING         1

/*********************************************************************************************************
  Debugging options (TCP UDP IP debug & only can print message)
*********************************************************************************************************/

#define LWIP_DBG_TYPES_ON               LWIP_DBG_ON

#if LW_CFG_LWIP_DEBUG > 0
#define LWIP_DEBUG
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_OFF              /*  ����ȫ��������ӡ��Ϣ        */
#define TCP_DEBUG                       (LWIP_DBG_ON | 0)               /*  ������ TCP UDP IP debug     */
#define UDP_DEBUG                       (LWIP_DBG_OFF | 0)
#define IP_DEBUG                        (LWIP_DBG_ON | 0)
#define SOCKETS_DEBUG                   (LWIP_DBG_ON | 0)
#else
#define LWIP_DBG_MIN_LEVEL              LWIP_DBG_LEVEL_SEVERE           /*  ������������ӡ��Ϣ          */
#endif                                                                  /*  LW_CFG_LWIP_DEBUG > 0       */

#endif                                                                  /*  LWIP_CONFIG_H_              */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
