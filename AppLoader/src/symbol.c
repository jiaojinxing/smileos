/********************************************************************************************************* 
** 													
**                                    中国软件开源组织 							
**														
**                                   嵌入式实时操作系统							
**														
**                                       SylixOS(TM)							
**														
**                               Copyright  All Rights Reserved						
**														
**--------------文件信息--------------------------------------------------------------------------------	
**														
** 文   件   名: symbol.c											
**														
** 创   建   人: makesymbol 工具										
**														
** 文件创建日期: 2012 年 07 月 18 日						
**														
** 描        述: 系统 sylixos 符号表. (此文件有 makesymbol 工具自动生成, 请勿修改)				
*********************************************************************************************************/	
#include  "symboltools.h"											
														
#define SYMBOL_TABLE_BEGIN LW_STATIC_SYMBOL   _G_symLibSylixOS[] = { 					
  														
#define SYMBOL_TABLE_END };										
														
#define SYMBOL_ITEM_FUNC(pcName)                \								
    {   {(void *)0, (void *)0},                 \								
        #pcName, (char *)pcName,                \								
        LW_SYMBOL_TEXT                          \								
    },													
														
#define SYMBOL_ITEM_OBJ(pcName)                 \								
    {   {(void *)0, (void *)0},                 \								
        #pcName, (char *)&pcName,               \								
        LW_SYMBOL_DATA                          \								
    },													
														
/*********************************************************************************************************	
  全局对象声明												
*********************************************************************************************************/	
extern int  _Balloc(); 
extern int  _Bfree(); 
extern int  _Unwind_Backtrace(); 
extern int  _Unwind_Complete(); 
extern int  _Unwind_DeleteException(); 
extern int  _Unwind_ForcedUnwind(); 
extern int  _Unwind_GetCFA(); 
extern int  _Unwind_GetDataRelBase(); 
extern int  _Unwind_GetLanguageSpecificData(); 
extern int  _Unwind_GetRegionStart(); 
extern int  _Unwind_GetTextRelBase(); 
extern int  _Unwind_RaiseException(); 
extern int  _Unwind_Resume(); 
extern int  _Unwind_Resume_or_Rethrow(); 
extern int  _Unwind_VRS_Get(); 
extern int  _Unwind_VRS_Pop(); 
extern int  _Unwind_VRS_Set(); 
extern int  _ZdaPv(); 
extern int  _ZdlPv(); 
extern int  _Znaj(); 
extern int  _Znwj(); 
extern int  ___Unwind_Backtrace(); 
extern int  ___Unwind_ForcedUnwind(); 
extern int  ___Unwind_RaiseException(); 
extern int  ___Unwind_Resume(); 
extern int  ___Unwind_Resume_or_Rethrow(); 
extern int  __adddf3(); 
extern int  __aeabi_cdcmpeq(); 
extern int  __aeabi_cdcmple(); 
extern int  __aeabi_cdrcmple(); 
extern int  __aeabi_d2f(); 
extern int  __aeabi_d2iz(); 
extern int  __aeabi_dadd(); 
extern int  __aeabi_dcmpeq(); 
extern int  __aeabi_dcmpge(); 
extern int  __aeabi_dcmpgt(); 
extern int  __aeabi_dcmple(); 
extern int  __aeabi_dcmplt(); 
extern int  __aeabi_ddiv(); 
extern int  __aeabi_dmul(); 
extern int  __aeabi_drsub(); 
extern int  __aeabi_dsub(); 
extern int  __aeabi_f2d(); 
extern int  __aeabi_i2d(); 
extern int  __aeabi_idiv(); 
extern int  __aeabi_idivmod(); 
extern int  __aeabi_l2d(); 
extern int  __aeabi_ui2d(); 
extern int  __aeabi_uidiv(); 
extern int  __aeabi_uidivmod(); 
extern int  __aeabi_ul2d(); 
extern int  __aeabi_uldivmod(); 
extern int  __aeabi_unwind_cpp_pr0(); 
extern int  __any_on(); 
extern int  __ascii_wctomb(); 
extern int  __b2d(); 
extern int  __call_exitprocs(); 
extern int  __clzsi2(); 
extern int  __cmpdf2(); 
extern int  __copybits(); 
extern int  __cppRtDoCtors(); 
extern int  __cppRtDoDtors(); 
extern int  __cppRtDummy(); 
extern int  __d2b(); 
extern int  __divdf3(); 
extern int  __divdi3(); 
extern int  __divsi3(); 
extern int  __eqdf2(); 
extern int  __errno(); 
extern int  __extendsfdf2(); 
extern int  __fixdfsi(); 
extern int  __floatdidf(); 
extern int  __floatsidf(); 
extern int  __floatundidf(); 
extern int  __floatunsidf(); 
extern int  __fp_lock_all(); 
extern int  __fp_unlock_all(); 
extern int  __fpclassifyd(); 
extern int  __gedf2(); 
extern int  __gnu_Unwind_Backtrace(); 
extern int  __gnu_Unwind_ForcedUnwind(); 
extern int  __gnu_Unwind_RaiseException(); 
extern int  __gnu_Unwind_Restore_VFP(); 
extern int  __gnu_Unwind_Restore_VFP_D(); 
extern int  __gnu_Unwind_Restore_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Restore_WMMXC(); 
extern int  __gnu_Unwind_Restore_WMMXD(); 
extern int  __gnu_Unwind_Resume(); 
extern int  __gnu_Unwind_Resume_or_Rethrow(); 
extern int  __gnu_Unwind_Save_VFP(); 
extern int  __gnu_Unwind_Save_VFP_D(); 
extern int  __gnu_Unwind_Save_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Save_WMMXC(); 
extern int  __gnu_Unwind_Save_WMMXD(); 
extern int  __gnu_ldivmod_helper(); 
extern int  __gnu_uldivmod_helper(); 
extern int  __gnu_unwind_execute(); 
extern int  __gnu_unwind_frame(); 
extern int  __gtdf2(); 
extern int  __hi0bits(); 
extern int  __i2b(); 
extern int  __ledf2(); 
extern int  __lo0bits(); 
extern int  __locale_charset(); 
extern int  __locale_cjk_lang(); 
extern int  __locale_mb_cur_max(); 
extern int  __locale_msgcharset(); 
extern int  __lshift(); 
extern int  __ltdf2(); 
extern int  __mcmp(); 
extern int  __mdiff(); 
extern int  __mprec_bigtens(); 
extern int  __mprec_tens(); 
extern int  __mprec_tinytens(); 
extern int  __muldf3(); 
extern int  __multadd(); 
extern int  __multiply(); 
extern int  __nedf2(); 
extern int  __pow5mult(); 
extern int  __pthread_accept(); 
extern int  __pthread_connect(); 
extern int  __pthread_detach(); 
extern int  __pthread_nanosleep(); 
extern int  __pthread_poll(); 
extern int  __pthread_pread(); 
extern int  __pthread_pwrite(); 
extern int  __pthread_read(); 
extern int  __pthread_readv(); 
extern int  __pthread_recv(); 
extern int  __pthread_recvfrom(); 
extern int  __pthread_select(); 
extern int  __pthread_send(); 
extern int  __pthread_sendto(); 
extern int  __pthread_sleep(); 
extern int  __pthread_usleep(); 
extern int  __pthread_write(); 
extern int  __pthread_writev(); 
extern int  __ratio(); 
extern int  __register_exitproc(); 
extern int  __restore_core_regs(); 
extern int  __s2b(); 
extern int  __sclose(); 
extern int  __seofread(); 
extern int  __sflags(); 
extern int  __sfmoreglue(); 
extern int  __sfp(); 
extern int  __sfp_lock_acquire(); 
extern int  __sfp_lock_release(); 
extern int  __sfvwrite_r(); 
extern int  __sigtramp(); 
extern int  __sigtramp_r(); 
extern int  __sinit(); 
extern int  __sinit_lock_acquire(); 
extern int  __sinit_lock_release(); 
extern int  __smakebuf_r(); 
extern int  __sprint_r(); 
extern int  __sread(); 
extern int  __sseek(); 
extern int  __ssprint_r(); 
extern int  __subdf3(); 
extern int  __swbuf(); 
extern int  __swbuf_r(); 
extern int  __swrite(); 
extern int  __swsetup_r(); 
extern int  __truncdfsf2(); 
extern int  __udivdi3(); 
extern int  __udivsi3(); 
extern int  __ulp(); 
extern int  _calloc_r(); 
extern int  _cleanup(); 
extern int  _cleanup_r(); 
extern int  _close_r(); 
extern int  _cppRtInit(); 
extern int  _ctype_(); 
extern int  _dtoa_r(); 
extern int  _execve_r(); 
extern int  _exit(); 
extern int  _fclose_r(); 
extern int  _fdopen_r(); 
extern int  _fflush_r(); 
extern int  _fini(); 
extern int  _fork_r(); 
extern int  _fprintf_r(); 
extern int  _fputs_r(); 
extern int  _fputwc_r(); 
extern int  _free_r(); 
extern int  _fstat_r(); 
extern int  _fwalk(); 
extern int  _fwalk_reent(); 
extern int  _fwrite_r(); 
extern int  _getpid_r(); 
extern int  _gettimeofday_r(); 
extern int  _global_impure_ptr(); 
extern int  _init_signal(); 
extern int  _init_signal_r(); 
extern int  _isatty_r(); 
extern int  _kill_r(); 
extern int  _link_r(); 
extern int  _localeconv_r(); 
extern int  _lseek_r(); 
extern int  _malloc_r(); 
extern int  _mkdir_r(); 
extern int  _mprec_log10(); 
extern int  _open_r(); 
extern int  _printf_r(); 
extern int  _puts_r(); 
extern int  _raise_r(); 
extern int  _read_r(); 
extern int  _realloc_r(); 
extern int  _rename_r(); 
extern int  _rmdir_r(); 
extern int  _sbrk_r(); 
extern int  _setlocale_r(); 
extern int  _signal_r(); 
extern int  _sprintf_r(); 
extern int  _start(); 
extern int  _stat_r(); 
extern int  _strdup_r(); 
extern int  _strndup_r(); 
extern int  _svfiprintf_r(); 
extern int  _svfprintf_r(); 
extern int  _times_r(); 
extern int  _unlink_r(); 
extern int  _vfiprintf_r(); 
extern int  _vfprintf_r(); 
extern int  _vsnprintf_r(); 
extern int  _wait_r(); 
extern int  _wcrtomb_r(); 
extern int  _wctomb_r(); 
extern int  _write_r(); 
extern int  abort(); 
extern int  accept(); 
extern int  access(); 
extern int  atexit(); 
extern int  bind(); 
extern int  calloc(); 
extern int  chdir(); 
extern int  close(); 
extern int  closedir(); 
extern int  connect(); 
extern int  creat(); 
extern int  dup(); 
extern int  dup2(); 
extern int  exit(); 
extern int  fclose(); 
extern int  fcntl(); 
extern int  fdopen(); 
extern int  fflush(); 
extern int  fprintf(); 
extern int  fputs(); 
extern int  fputwc(); 
extern int  free(); 
extern int  fstat(); 
extern int  fwrite(); 
extern int  getcwd(); 
extern int  getpid(); 
extern int  getsockopt(); 
extern int  gettimeofday(); 
extern int  heap_alloc(); 
extern int  heap_free(); 
extern int  heap_init(); 
extern int  heap_show(); 
extern int  ioctl(); 
extern int  ip4_addr_isbroadcast(); 
extern int  ip4_addr_netmask_valid(); 
extern int  ip_addr_any(); 
extern int  ip_addr_broadcast(); 
extern int  ipaddr_addr(); 
extern int  ipaddr_aton(); 
extern int  ipaddr_ntoa(); 
extern int  ipaddr_ntoa_r(); 
extern int  isatty(); 
extern int  libc_init(); 
extern int  listen(); 
extern int  load_elf(); 
extern int  localeconv(); 
extern int  lseek(); 
extern int  lwip_htonl(); 
extern int  lwip_htons(); 
extern int  lwip_ntohl(); 
extern int  lwip_ntohs(); 
extern int  main(); 
extern int  malloc(); 
extern int  memchr(); 
extern int  memcpy(); 
extern int  memmove(); 
extern int  memrchr(); 
extern int  memset(); 
extern int  mkdir(); 
extern int  open(); 
extern int  opendir(); 
extern int  poll(); 
extern int  printf(); 
extern int  printk(); 
extern int  probe_elf(); 
extern int  pth_abort(); 
extern int  pth_accept(); 
extern int  pth_accept_ev(); 
extern int  pth_asprintf(); 
extern int  pth_attr_ctrl(); 
extern int  pth_attr_destroy(); 
extern int  pth_attr_get(); 
extern int  pth_attr_init(); 
extern int  pth_attr_new(); 
extern int  pth_attr_of(); 
extern int  pth_attr_set(); 
extern int  pth_barrier_init(); 
extern int  pth_barrier_reach(); 
extern int  pth_cancel(); 
extern int  pth_cancel_point(); 
extern int  pth_cancel_state(); 
extern int  pth_cleanup_pop(); 
extern int  pth_cleanup_popall(); 
extern int  pth_cleanup_push(); 
extern int  pth_cond_await(); 
extern int  pth_cond_init(); 
extern int  pth_cond_notify(); 
extern int  pth_connect(); 
extern int  pth_connect_ev(); 
extern int  pth_ctrl(); 
extern int  pth_debug(); 
extern int  pth_dumpqueue(); 
extern int  pth_dumpstate(); 
extern int  pth_event(); 
extern int  pth_event_concat(); 
extern int  pth_event_extract(); 
extern int  pth_event_free(); 
extern int  pth_event_isolate(); 
extern int  pth_event_status(); 
extern int  pth_event_typeof(); 
extern int  pth_event_walk(); 
extern int  pth_exit(); 
extern int  pth_fdmode(); 
extern int  pth_implicit_init(); 
extern int  pth_init(); 
extern int  pth_join(); 
extern int  pth_key_create(); 
extern int  pth_key_delete(); 
extern int  pth_key_destroydata(); 
extern int  pth_key_getdata(); 
extern int  pth_key_setdata(); 
extern int  pth_kill(); 
extern int  pth_mctx_set(); 
extern int  pth_mctx_switch(); 
extern int  pth_msgport_create(); 
extern int  pth_msgport_destroy(); 
extern int  pth_msgport_find(); 
extern int  pth_msgport_get(); 
extern int  pth_msgport_pending(); 
extern int  pth_msgport_put(); 
extern int  pth_msgport_reply(); 
extern int  pth_mutex_acquire(); 
extern int  pth_mutex_init(); 
extern int  pth_mutex_release(); 
extern int  pth_mutex_releaseall(); 
extern int  pth_nanosleep(); 
extern int  pth_nap(); 
extern int  pth_once(); 
extern int  pth_poll(); 
extern int  pth_poll_ev(); 
extern int  pth_pqueue_contains(); 
extern int  pth_pqueue_delete(); 
extern int  pth_pqueue_delmax(); 
extern int  pth_pqueue_favorite(); 
extern int  pth_pqueue_increase(); 
extern int  pth_pqueue_init(); 
extern int  pth_pqueue_insert(); 
extern int  pth_pqueue_tail(); 
extern int  pth_pqueue_walk(); 
extern int  pth_pread(); 
extern int  pth_pwrite(); 
extern int  pth_read(); 
extern int  pth_read_ev(); 
extern int  pth_readv(); 
extern int  pth_readv_ev(); 
extern int  pth_readv_faked(); 
extern int  pth_recv(); 
extern int  pth_recv_ev(); 
extern int  pth_recvfrom(); 
extern int  pth_recvfrom_ev(); 
extern int  pth_resume(); 
extern int  pth_ring_append(); 
extern int  pth_ring_contains(); 
extern int  pth_ring_delete(); 
extern int  pth_ring_dequeue(); 
extern int  pth_ring_favorite(); 
extern int  pth_ring_init(); 
extern int  pth_ring_insert_after(); 
extern int  pth_ring_insert_before(); 
extern int  pth_ring_pop(); 
extern int  pth_ring_prepend(); 
extern int  pth_rwlock_acquire(); 
extern int  pth_rwlock_init(); 
extern int  pth_rwlock_release(); 
extern int  pth_sched_eventmanager(); 
extern int  pth_scheduler(); 
extern int  pth_scheduler_drop(); 
extern int  pth_scheduler_init(); 
extern int  pth_scheduler_kill(); 
extern int  pth_select(); 
extern int  pth_select_ev(); 
extern int  pth_self(); 
extern int  pth_send(); 
extern int  pth_send_ev(); 
extern int  pth_sendto(); 
extern int  pth_sendto_ev(); 
extern int  pth_sfiodisc(); 
extern int  pth_sleep(); 
extern int  pth_snprintf(); 
extern int  pth_spawn(); 
extern int  pth_suspend(); 
extern int  pth_syscall_init(); 
extern int  pth_syscall_kill(); 
extern int  pth_tcb_alloc(); 
extern int  pth_tcb_free(); 
extern int  pth_thread_cleanup(); 
extern int  pth_thread_exists(); 
extern int  pth_time(); 
extern int  pth_time_cmp(); 
extern int  pth_time_div(); 
extern int  pth_time_mul(); 
extern int  pth_time_pos(); 
extern int  pth_time_t2d(); 
extern int  pth_time_t2i(); 
extern int  pth_time_usleep(); 
extern int  pth_timeout(); 
extern int  pth_usleep(); 
extern int  pth_util_cpystrn(); 
extern int  pth_util_fd_valid(); 
extern int  pth_util_fds_merge(); 
extern int  pth_util_fds_select(); 
extern int  pth_util_fds_test(); 
extern int  pth_vasprintf(); 
extern int  pth_version(); 
extern int  pth_vsnprintf(); 
extern int  pth_wait(); 
extern int  pth_write(); 
extern int  pth_write_ev(); 
extern int  pth_writev(); 
extern int  pth_writev_ev(); 
extern int  pth_writev_faked(); 
extern int  pth_writev_iov_advance(); 
extern int  pth_writev_iov_bytes(); 
extern int  pth_yield(); 
extern int  pthread_abort(); 
extern int  pthread_attr_destroy(); 
extern int  pthread_attr_getdetachstate(); 
extern int  pthread_attr_getguardsize(); 
extern int  pthread_attr_getinheritsched(); 
extern int  pthread_attr_getname_np(); 
extern int  pthread_attr_getprio_np(); 
extern int  pthread_attr_getschedparam(); 
extern int  pthread_attr_getschedpolicy(); 
extern int  pthread_attr_getscope(); 
extern int  pthread_attr_getstackaddr(); 
extern int  pthread_attr_getstacksize(); 
extern int  pthread_attr_init(); 
extern int  pthread_attr_setdetachstate(); 
extern int  pthread_attr_setguardsize(); 
extern int  pthread_attr_setinheritsched(); 
extern int  pthread_attr_setname_np(); 
extern int  pthread_attr_setprio_np(); 
extern int  pthread_attr_setschedparam(); 
extern int  pthread_attr_setschedpolicy(); 
extern int  pthread_attr_setscope(); 
extern int  pthread_attr_setstackaddr(); 
extern int  pthread_attr_setstacksize(); 
extern int  pthread_cancel(); 
extern int  pthread_cleanup_pop(); 
extern int  pthread_cleanup_push(); 
extern int  pthread_cond_broadcast(); 
extern int  pthread_cond_destroy(); 
extern int  pthread_cond_init(); 
extern int  pthread_cond_signal(); 
extern int  pthread_cond_timedwait(); 
extern int  pthread_cond_wait(); 
extern int  pthread_condattr_destroy(); 
extern int  pthread_condattr_getpshared(); 
extern int  pthread_condattr_init(); 
extern int  pthread_condattr_setpshared(); 
extern int  pthread_create(); 
extern int  pthread_equal(); 
extern int  pthread_exit(); 
extern int  pthread_getconcurrency(); 
extern int  pthread_getschedparam(); 
extern int  pthread_getspecific(); 
extern int  pthread_join(); 
extern int  pthread_key_create(); 
extern int  pthread_key_delete(); 
extern int  pthread_mutex_destroy(); 
extern int  pthread_mutex_getprioceiling(); 
extern int  pthread_mutex_init(); 
extern int  pthread_mutex_lock(); 
extern int  pthread_mutex_setprioceiling(); 
extern int  pthread_mutex_trylock(); 
extern int  pthread_mutex_unlock(); 
extern int  pthread_mutexattr_destroy(); 
extern int  pthread_mutexattr_getprioceiling(); 
extern int  pthread_mutexattr_getprotocol(); 
extern int  pthread_mutexattr_getpshared(); 
extern int  pthread_mutexattr_gettype(); 
extern int  pthread_mutexattr_init(); 
extern int  pthread_mutexattr_setprioceiling(); 
extern int  pthread_mutexattr_setprotocol(); 
extern int  pthread_mutexattr_setpshared(); 
extern int  pthread_mutexattr_settype(); 
extern int  pthread_once(); 
extern int  pthread_rwlock_destroy(); 
extern int  pthread_rwlock_init(); 
extern int  pthread_rwlock_rdlock(); 
extern int  pthread_rwlock_tryrdlock(); 
extern int  pthread_rwlock_trywrlock(); 
extern int  pthread_rwlock_unlock(); 
extern int  pthread_rwlock_wrlock(); 
extern int  pthread_rwlockattr_destroy(); 
extern int  pthread_rwlockattr_getpshared(); 
extern int  pthread_rwlockattr_init(); 
extern int  pthread_rwlockattr_setpshared(); 
extern int  pthread_self(); 
extern int  pthread_setcancelstate(); 
extern int  pthread_setcanceltype(); 
extern int  pthread_setconcurrency(); 
extern int  pthread_setschedparam(); 
extern int  pthread_setspecific(); 
extern int  pthread_testcancel(); 
extern int  pthread_yield_np(); 
extern int  puts(); 
extern int  raise(); 
extern int  read(); 
extern int  readdir(); 
extern int  recv(); 
extern int  recvfrom(); 
extern int  reloc_elf(); 
extern int  restore_core_regs(); 
extern int  rewinddir(); 
extern int  rmdir(); 
extern int  seekdir(); 
extern int  select(); 
extern int  send(); 
extern int  sendto(); 
extern int  setlocale(); 
extern int  setreent(); 
extern int  setsockopt(); 
extern int  shutdown(); 
extern int  signal(); 
extern int  sleep(); 
extern int  socket(); 
extern int  sprintf(); 
extern int  stat(); 
extern int  strchrnul(); 
extern int  strcmp(); 
extern int  strdup(); 
extern int  strlen(); 
extern int  strndup(); 
extern int  syscall_accept(); 
extern int  syscall_connect(); 
extern int  syscall_read(); 
extern int  syscall_recv(); 
extern int  syscall_recvfrom(); 
extern int  syscall_select(); 
extern int  syscall_send(); 
extern int  syscall_sendto(); 
extern int  syscall_sleep(); 
extern int  syscall_template(); 
extern int  syscall_usleep(); 
extern int  syscall_write(); 
extern int  telldir(); 
extern int  time(); 
extern int  uheap_create(); 
extern int  usleep(); 
extern int  vfiprintf(); 
extern int  vfprintf(); 
extern int  vsnprintf(); 
extern int  wcrtomb(); 
extern int  write(); 
extern int  xmalloc(); 
extern int  xstrdup(); 
extern int  xstrndup(); 
extern int  xzalloc(); 
extern int  yield(); 
extern int  __ctype_ptr__; 
extern int  __mb_cur_max; 
extern int  __wctomb; 
extern int  _data; 
extern int  _impure_ptr; 
extern int  pth_internal_version; 
extern int  pth_state_names; 
extern int  _PathLocale; 
extern int  __bss_start; 
extern int  __bss_start__; 
extern int  __mlocale_changed; 
extern int  __nlocale_changed; 
extern int  __pth_compat_unit; 
extern int  pth_DQ; 
extern int  pth_NQ; 
extern int  pth_RQ; 
extern int  pth_SQ; 
extern int  pth_WQ; 
extern int  pth_current; 
extern int  pth_errno_flag; 
extern int  pth_errno_storage; 
extern int  pth_favournew; 
extern int  pth_initialized; 
extern int  pth_loadval; 
extern int  pth_main; 
extern int  pth_sched; 
extern int  pth_time_zero; 
extern int  __aeabi_idiv0; 
extern int  __aeabi_ldiv0; 
extern int  __aeabi_unwind_cpp_pr1; 
extern int  __aeabi_unwind_cpp_pr2; 
extern int  __dso_handle; 
/*********************************************************************************************************	
  系统静态符号表												
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN												
    SYMBOL_ITEM_FUNC(_Balloc) 
    SYMBOL_ITEM_FUNC(_Bfree) 
    SYMBOL_ITEM_FUNC(_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(_Unwind_Complete) 
    SYMBOL_ITEM_FUNC(_Unwind_DeleteException) 
    SYMBOL_ITEM_FUNC(_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(_Unwind_GetCFA) 
    SYMBOL_ITEM_FUNC(_Unwind_GetDataRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_GetLanguageSpecificData) 
    SYMBOL_ITEM_FUNC(_Unwind_GetRegionStart) 
    SYMBOL_ITEM_FUNC(_Unwind_GetTextRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Get) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Pop) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Set) 
    SYMBOL_ITEM_FUNC(_ZdaPv) 
    SYMBOL_ITEM_FUNC(_ZdlPv) 
    SYMBOL_ITEM_FUNC(_Znaj) 
    SYMBOL_ITEM_FUNC(_Znwj) 
    SYMBOL_ITEM_FUNC(___Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(___Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(___Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__adddf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cdrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_d2f) 
    SYMBOL_ITEM_FUNC(__aeabi_d2iz) 
    SYMBOL_ITEM_FUNC(__aeabi_dadd) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmplt) 
    SYMBOL_ITEM_FUNC(__aeabi_ddiv) 
    SYMBOL_ITEM_FUNC(__aeabi_dmul) 
    SYMBOL_ITEM_FUNC(__aeabi_drsub) 
    SYMBOL_ITEM_FUNC(__aeabi_dsub) 
    SYMBOL_ITEM_FUNC(__aeabi_f2d) 
    SYMBOL_ITEM_FUNC(__aeabi_i2d) 
    SYMBOL_ITEM_FUNC(__aeabi_idiv) 
    SYMBOL_ITEM_FUNC(__aeabi_idivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_l2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2d) 
    SYMBOL_ITEM_FUNC(__aeabi_uidiv) 
    SYMBOL_ITEM_FUNC(__aeabi_uidivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2d) 
    SYMBOL_ITEM_FUNC(__aeabi_uldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_unwind_cpp_pr0) 
    SYMBOL_ITEM_FUNC(__any_on) 
    SYMBOL_ITEM_FUNC(__ascii_wctomb) 
    SYMBOL_ITEM_FUNC(__b2d) 
    SYMBOL_ITEM_FUNC(__call_exitprocs) 
    SYMBOL_ITEM_FUNC(__clzsi2) 
    SYMBOL_ITEM_FUNC(__cmpdf2) 
    SYMBOL_ITEM_FUNC(__copybits) 
    SYMBOL_ITEM_FUNC(__cppRtDoCtors) 
    SYMBOL_ITEM_FUNC(__cppRtDoDtors) 
    SYMBOL_ITEM_FUNC(__cppRtDummy) 
    SYMBOL_ITEM_FUNC(__d2b) 
    SYMBOL_ITEM_FUNC(__divdf3) 
    SYMBOL_ITEM_FUNC(__divdi3) 
    SYMBOL_ITEM_FUNC(__divsi3) 
    SYMBOL_ITEM_FUNC(__eqdf2) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(__extendsfdf2) 
    SYMBOL_ITEM_FUNC(__fixdfsi) 
    SYMBOL_ITEM_FUNC(__floatdidf) 
    SYMBOL_ITEM_FUNC(__floatsidf) 
    SYMBOL_ITEM_FUNC(__floatundidf) 
    SYMBOL_ITEM_FUNC(__floatunsidf) 
    SYMBOL_ITEM_FUNC(__fp_lock_all) 
    SYMBOL_ITEM_FUNC(__fp_unlock_all) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(__gedf2) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_ldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_uldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_execute) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_frame) 
    SYMBOL_ITEM_FUNC(__gtdf2) 
    SYMBOL_ITEM_FUNC(__hi0bits) 
    SYMBOL_ITEM_FUNC(__i2b) 
    SYMBOL_ITEM_FUNC(__ledf2) 
    SYMBOL_ITEM_FUNC(__lo0bits) 
    SYMBOL_ITEM_FUNC(__locale_charset) 
    SYMBOL_ITEM_FUNC(__locale_cjk_lang) 
    SYMBOL_ITEM_FUNC(__locale_mb_cur_max) 
    SYMBOL_ITEM_FUNC(__locale_msgcharset) 
    SYMBOL_ITEM_FUNC(__lshift) 
    SYMBOL_ITEM_FUNC(__ltdf2) 
    SYMBOL_ITEM_FUNC(__mcmp) 
    SYMBOL_ITEM_FUNC(__mdiff) 
    SYMBOL_ITEM_FUNC(__mprec_bigtens) 
    SYMBOL_ITEM_FUNC(__mprec_tens) 
    SYMBOL_ITEM_FUNC(__mprec_tinytens) 
    SYMBOL_ITEM_FUNC(__muldf3) 
    SYMBOL_ITEM_FUNC(__multadd) 
    SYMBOL_ITEM_FUNC(__multiply) 
    SYMBOL_ITEM_FUNC(__nedf2) 
    SYMBOL_ITEM_FUNC(__pow5mult) 
    SYMBOL_ITEM_FUNC(__pthread_accept) 
    SYMBOL_ITEM_FUNC(__pthread_connect) 
    SYMBOL_ITEM_FUNC(__pthread_detach) 
    SYMBOL_ITEM_FUNC(__pthread_nanosleep) 
    SYMBOL_ITEM_FUNC(__pthread_poll) 
    SYMBOL_ITEM_FUNC(__pthread_pread) 
    SYMBOL_ITEM_FUNC(__pthread_pwrite) 
    SYMBOL_ITEM_FUNC(__pthread_read) 
    SYMBOL_ITEM_FUNC(__pthread_readv) 
    SYMBOL_ITEM_FUNC(__pthread_recv) 
    SYMBOL_ITEM_FUNC(__pthread_recvfrom) 
    SYMBOL_ITEM_FUNC(__pthread_select) 
    SYMBOL_ITEM_FUNC(__pthread_send) 
    SYMBOL_ITEM_FUNC(__pthread_sendto) 
    SYMBOL_ITEM_FUNC(__pthread_sleep) 
    SYMBOL_ITEM_FUNC(__pthread_usleep) 
    SYMBOL_ITEM_FUNC(__pthread_write) 
    SYMBOL_ITEM_FUNC(__pthread_writev) 
    SYMBOL_ITEM_FUNC(__ratio) 
    SYMBOL_ITEM_FUNC(__register_exitproc) 
    SYMBOL_ITEM_FUNC(__restore_core_regs) 
    SYMBOL_ITEM_FUNC(__s2b) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__seofread) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(__sfmoreglue) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sfp_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sfp_lock_release) 
    SYMBOL_ITEM_FUNC(__sfvwrite_r) 
    SYMBOL_ITEM_FUNC(__sigtramp) 
    SYMBOL_ITEM_FUNC(__sigtramp_r) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(__sinit_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sinit_lock_release) 
    SYMBOL_ITEM_FUNC(__smakebuf_r) 
    SYMBOL_ITEM_FUNC(__sprint_r) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__ssprint_r) 
    SYMBOL_ITEM_FUNC(__subdf3) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swbuf_r) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(__swsetup_r) 
    SYMBOL_ITEM_FUNC(__truncdfsf2) 
    SYMBOL_ITEM_FUNC(__udivdi3) 
    SYMBOL_ITEM_FUNC(__udivsi3) 
    SYMBOL_ITEM_FUNC(__ulp) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(_cleanup_r) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(_cppRtInit) 
    SYMBOL_ITEM_FUNC(_ctype_) 
    SYMBOL_ITEM_FUNC(_dtoa_r) 
    SYMBOL_ITEM_FUNC(_execve_r) 
    SYMBOL_ITEM_FUNC(_exit) 
    SYMBOL_ITEM_FUNC(_fclose_r) 
    SYMBOL_ITEM_FUNC(_fdopen_r) 
    SYMBOL_ITEM_FUNC(_fflush_r) 
    SYMBOL_ITEM_FUNC(_fini) 
    SYMBOL_ITEM_FUNC(_fork_r) 
    SYMBOL_ITEM_FUNC(_fprintf_r) 
    SYMBOL_ITEM_FUNC(_fputs_r) 
    SYMBOL_ITEM_FUNC(_fputwc_r) 
    SYMBOL_ITEM_FUNC(_free_r) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(_fwalk_reent) 
    SYMBOL_ITEM_FUNC(_fwrite_r) 
    SYMBOL_ITEM_FUNC(_getpid_r) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(_global_impure_ptr) 
    SYMBOL_ITEM_FUNC(_init_signal) 
    SYMBOL_ITEM_FUNC(_init_signal_r) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(_kill_r) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(_localeconv_r) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(_malloc_r) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_mprec_log10) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_printf_r) 
    SYMBOL_ITEM_FUNC(_puts_r) 
    SYMBOL_ITEM_FUNC(_raise_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rmdir_r) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_setlocale_r) 
    SYMBOL_ITEM_FUNC(_signal_r) 
    SYMBOL_ITEM_FUNC(_sprintf_r) 
    SYMBOL_ITEM_FUNC(_start) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(_strdup_r) 
    SYMBOL_ITEM_FUNC(_strndup_r) 
    SYMBOL_ITEM_FUNC(_svfiprintf_r) 
    SYMBOL_ITEM_FUNC(_svfprintf_r) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(_vfiprintf_r) 
    SYMBOL_ITEM_FUNC(_vfprintf_r) 
    SYMBOL_ITEM_FUNC(_vsnprintf_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(_wcrtomb_r) 
    SYMBOL_ITEM_FUNC(_wctomb_r) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(accept) 
    SYMBOL_ITEM_FUNC(access) 
    SYMBOL_ITEM_FUNC(atexit) 
    SYMBOL_ITEM_FUNC(bind) 
    SYMBOL_ITEM_FUNC(calloc) 
    SYMBOL_ITEM_FUNC(chdir) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(closedir) 
    SYMBOL_ITEM_FUNC(connect) 
    SYMBOL_ITEM_FUNC(creat) 
    SYMBOL_ITEM_FUNC(dup) 
    SYMBOL_ITEM_FUNC(dup2) 
    SYMBOL_ITEM_FUNC(exit) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(fputs) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(fwrite) 
    SYMBOL_ITEM_FUNC(getcwd) 
    SYMBOL_ITEM_FUNC(getpid) 
    SYMBOL_ITEM_FUNC(getsockopt) 
    SYMBOL_ITEM_FUNC(gettimeofday) 
    SYMBOL_ITEM_FUNC(heap_alloc) 
    SYMBOL_ITEM_FUNC(heap_free) 
    SYMBOL_ITEM_FUNC(heap_init) 
    SYMBOL_ITEM_FUNC(heap_show) 
    SYMBOL_ITEM_FUNC(ioctl) 
    SYMBOL_ITEM_FUNC(ip4_addr_isbroadcast) 
    SYMBOL_ITEM_FUNC(ip4_addr_netmask_valid) 
    SYMBOL_ITEM_FUNC(ip_addr_any) 
    SYMBOL_ITEM_FUNC(ip_addr_broadcast) 
    SYMBOL_ITEM_FUNC(ipaddr_addr) 
    SYMBOL_ITEM_FUNC(ipaddr_aton) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa_r) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(libc_init) 
    SYMBOL_ITEM_FUNC(listen) 
    SYMBOL_ITEM_FUNC(load_elf) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(lseek) 
    SYMBOL_ITEM_FUNC(lwip_htonl) 
    SYMBOL_ITEM_FUNC(lwip_htons) 
    SYMBOL_ITEM_FUNC(lwip_ntohl) 
    SYMBOL_ITEM_FUNC(lwip_ntohs) 
    SYMBOL_ITEM_FUNC(main) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(memrchr) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(mkdir) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(opendir) 
    SYMBOL_ITEM_FUNC(poll) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(printk) 
    SYMBOL_ITEM_FUNC(probe_elf) 
    SYMBOL_ITEM_FUNC(pth_abort) 
    SYMBOL_ITEM_FUNC(pth_accept) 
    SYMBOL_ITEM_FUNC(pth_accept_ev) 
    SYMBOL_ITEM_FUNC(pth_asprintf) 
    SYMBOL_ITEM_FUNC(pth_attr_ctrl) 
    SYMBOL_ITEM_FUNC(pth_attr_destroy) 
    SYMBOL_ITEM_FUNC(pth_attr_get) 
    SYMBOL_ITEM_FUNC(pth_attr_init) 
    SYMBOL_ITEM_FUNC(pth_attr_new) 
    SYMBOL_ITEM_FUNC(pth_attr_of) 
    SYMBOL_ITEM_FUNC(pth_attr_set) 
    SYMBOL_ITEM_FUNC(pth_barrier_init) 
    SYMBOL_ITEM_FUNC(pth_barrier_reach) 
    SYMBOL_ITEM_FUNC(pth_cancel) 
    SYMBOL_ITEM_FUNC(pth_cancel_point) 
    SYMBOL_ITEM_FUNC(pth_cancel_state) 
    SYMBOL_ITEM_FUNC(pth_cleanup_pop) 
    SYMBOL_ITEM_FUNC(pth_cleanup_popall) 
    SYMBOL_ITEM_FUNC(pth_cleanup_push) 
    SYMBOL_ITEM_FUNC(pth_cond_await) 
    SYMBOL_ITEM_FUNC(pth_cond_init) 
    SYMBOL_ITEM_FUNC(pth_cond_notify) 
    SYMBOL_ITEM_FUNC(pth_connect) 
    SYMBOL_ITEM_FUNC(pth_connect_ev) 
    SYMBOL_ITEM_FUNC(pth_ctrl) 
    SYMBOL_ITEM_FUNC(pth_debug) 
    SYMBOL_ITEM_FUNC(pth_dumpqueue) 
    SYMBOL_ITEM_FUNC(pth_dumpstate) 
    SYMBOL_ITEM_FUNC(pth_event) 
    SYMBOL_ITEM_FUNC(pth_event_concat) 
    SYMBOL_ITEM_FUNC(pth_event_extract) 
    SYMBOL_ITEM_FUNC(pth_event_free) 
    SYMBOL_ITEM_FUNC(pth_event_isolate) 
    SYMBOL_ITEM_FUNC(pth_event_status) 
    SYMBOL_ITEM_FUNC(pth_event_typeof) 
    SYMBOL_ITEM_FUNC(pth_event_walk) 
    SYMBOL_ITEM_FUNC(pth_exit) 
    SYMBOL_ITEM_FUNC(pth_fdmode) 
    SYMBOL_ITEM_FUNC(pth_implicit_init) 
    SYMBOL_ITEM_FUNC(pth_init) 
    SYMBOL_ITEM_FUNC(pth_join) 
    SYMBOL_ITEM_FUNC(pth_key_create) 
    SYMBOL_ITEM_FUNC(pth_key_delete) 
    SYMBOL_ITEM_FUNC(pth_key_destroydata) 
    SYMBOL_ITEM_FUNC(pth_key_getdata) 
    SYMBOL_ITEM_FUNC(pth_key_setdata) 
    SYMBOL_ITEM_FUNC(pth_kill) 
    SYMBOL_ITEM_FUNC(pth_mctx_set) 
    SYMBOL_ITEM_FUNC(pth_mctx_switch) 
    SYMBOL_ITEM_FUNC(pth_msgport_create) 
    SYMBOL_ITEM_FUNC(pth_msgport_destroy) 
    SYMBOL_ITEM_FUNC(pth_msgport_find) 
    SYMBOL_ITEM_FUNC(pth_msgport_get) 
    SYMBOL_ITEM_FUNC(pth_msgport_pending) 
    SYMBOL_ITEM_FUNC(pth_msgport_put) 
    SYMBOL_ITEM_FUNC(pth_msgport_reply) 
    SYMBOL_ITEM_FUNC(pth_mutex_acquire) 
    SYMBOL_ITEM_FUNC(pth_mutex_init) 
    SYMBOL_ITEM_FUNC(pth_mutex_release) 
    SYMBOL_ITEM_FUNC(pth_mutex_releaseall) 
    SYMBOL_ITEM_FUNC(pth_nanosleep) 
    SYMBOL_ITEM_FUNC(pth_nap) 
    SYMBOL_ITEM_FUNC(pth_once) 
    SYMBOL_ITEM_FUNC(pth_poll) 
    SYMBOL_ITEM_FUNC(pth_poll_ev) 
    SYMBOL_ITEM_FUNC(pth_pqueue_contains) 
    SYMBOL_ITEM_FUNC(pth_pqueue_delete) 
    SYMBOL_ITEM_FUNC(pth_pqueue_delmax) 
    SYMBOL_ITEM_FUNC(pth_pqueue_favorite) 
    SYMBOL_ITEM_FUNC(pth_pqueue_increase) 
    SYMBOL_ITEM_FUNC(pth_pqueue_init) 
    SYMBOL_ITEM_FUNC(pth_pqueue_insert) 
    SYMBOL_ITEM_FUNC(pth_pqueue_tail) 
    SYMBOL_ITEM_FUNC(pth_pqueue_walk) 
    SYMBOL_ITEM_FUNC(pth_pread) 
    SYMBOL_ITEM_FUNC(pth_pwrite) 
    SYMBOL_ITEM_FUNC(pth_read) 
    SYMBOL_ITEM_FUNC(pth_read_ev) 
    SYMBOL_ITEM_FUNC(pth_readv) 
    SYMBOL_ITEM_FUNC(pth_readv_ev) 
    SYMBOL_ITEM_FUNC(pth_readv_faked) 
    SYMBOL_ITEM_FUNC(pth_recv) 
    SYMBOL_ITEM_FUNC(pth_recv_ev) 
    SYMBOL_ITEM_FUNC(pth_recvfrom) 
    SYMBOL_ITEM_FUNC(pth_recvfrom_ev) 
    SYMBOL_ITEM_FUNC(pth_resume) 
    SYMBOL_ITEM_FUNC(pth_ring_append) 
    SYMBOL_ITEM_FUNC(pth_ring_contains) 
    SYMBOL_ITEM_FUNC(pth_ring_delete) 
    SYMBOL_ITEM_FUNC(pth_ring_dequeue) 
    SYMBOL_ITEM_FUNC(pth_ring_favorite) 
    SYMBOL_ITEM_FUNC(pth_ring_init) 
    SYMBOL_ITEM_FUNC(pth_ring_insert_after) 
    SYMBOL_ITEM_FUNC(pth_ring_insert_before) 
    SYMBOL_ITEM_FUNC(pth_ring_pop) 
    SYMBOL_ITEM_FUNC(pth_ring_prepend) 
    SYMBOL_ITEM_FUNC(pth_rwlock_acquire) 
    SYMBOL_ITEM_FUNC(pth_rwlock_init) 
    SYMBOL_ITEM_FUNC(pth_rwlock_release) 
    SYMBOL_ITEM_FUNC(pth_sched_eventmanager) 
    SYMBOL_ITEM_FUNC(pth_scheduler) 
    SYMBOL_ITEM_FUNC(pth_scheduler_drop) 
    SYMBOL_ITEM_FUNC(pth_scheduler_init) 
    SYMBOL_ITEM_FUNC(pth_scheduler_kill) 
    SYMBOL_ITEM_FUNC(pth_select) 
    SYMBOL_ITEM_FUNC(pth_select_ev) 
    SYMBOL_ITEM_FUNC(pth_self) 
    SYMBOL_ITEM_FUNC(pth_send) 
    SYMBOL_ITEM_FUNC(pth_send_ev) 
    SYMBOL_ITEM_FUNC(pth_sendto) 
    SYMBOL_ITEM_FUNC(pth_sendto_ev) 
    SYMBOL_ITEM_FUNC(pth_sfiodisc) 
    SYMBOL_ITEM_FUNC(pth_sleep) 
    SYMBOL_ITEM_FUNC(pth_snprintf) 
    SYMBOL_ITEM_FUNC(pth_spawn) 
    SYMBOL_ITEM_FUNC(pth_suspend) 
    SYMBOL_ITEM_FUNC(pth_syscall_init) 
    SYMBOL_ITEM_FUNC(pth_syscall_kill) 
    SYMBOL_ITEM_FUNC(pth_tcb_alloc) 
    SYMBOL_ITEM_FUNC(pth_tcb_free) 
    SYMBOL_ITEM_FUNC(pth_thread_cleanup) 
    SYMBOL_ITEM_FUNC(pth_thread_exists) 
    SYMBOL_ITEM_FUNC(pth_time) 
    SYMBOL_ITEM_FUNC(pth_time_cmp) 
    SYMBOL_ITEM_FUNC(pth_time_div) 
    SYMBOL_ITEM_FUNC(pth_time_mul) 
    SYMBOL_ITEM_FUNC(pth_time_pos) 
    SYMBOL_ITEM_FUNC(pth_time_t2d) 
    SYMBOL_ITEM_FUNC(pth_time_t2i) 
    SYMBOL_ITEM_FUNC(pth_time_usleep) 
    SYMBOL_ITEM_FUNC(pth_timeout) 
    SYMBOL_ITEM_FUNC(pth_usleep) 
    SYMBOL_ITEM_FUNC(pth_util_cpystrn) 
    SYMBOL_ITEM_FUNC(pth_util_fd_valid) 
    SYMBOL_ITEM_FUNC(pth_util_fds_merge) 
    SYMBOL_ITEM_FUNC(pth_util_fds_select) 
    SYMBOL_ITEM_FUNC(pth_util_fds_test) 
    SYMBOL_ITEM_FUNC(pth_vasprintf) 
    SYMBOL_ITEM_FUNC(pth_version) 
    SYMBOL_ITEM_FUNC(pth_vsnprintf) 
    SYMBOL_ITEM_FUNC(pth_wait) 
    SYMBOL_ITEM_FUNC(pth_write) 
    SYMBOL_ITEM_FUNC(pth_write_ev) 
    SYMBOL_ITEM_FUNC(pth_writev) 
    SYMBOL_ITEM_FUNC(pth_writev_ev) 
    SYMBOL_ITEM_FUNC(pth_writev_faked) 
    SYMBOL_ITEM_FUNC(pth_writev_iov_advance) 
    SYMBOL_ITEM_FUNC(pth_writev_iov_bytes) 
    SYMBOL_ITEM_FUNC(pth_yield) 
    SYMBOL_ITEM_FUNC(pthread_abort) 
    SYMBOL_ITEM_FUNC(pthread_attr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_attr_getdetachstate) 
    SYMBOL_ITEM_FUNC(pthread_attr_getguardsize) 
    SYMBOL_ITEM_FUNC(pthread_attr_getinheritsched) 
    SYMBOL_ITEM_FUNC(pthread_attr_getname_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_getprio_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_getschedparam) 
    SYMBOL_ITEM_FUNC(pthread_attr_getschedpolicy) 
    SYMBOL_ITEM_FUNC(pthread_attr_getscope) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstackaddr) 
    SYMBOL_ITEM_FUNC(pthread_attr_getstacksize) 
    SYMBOL_ITEM_FUNC(pthread_attr_init) 
    SYMBOL_ITEM_FUNC(pthread_attr_setdetachstate) 
    SYMBOL_ITEM_FUNC(pthread_attr_setguardsize) 
    SYMBOL_ITEM_FUNC(pthread_attr_setinheritsched) 
    SYMBOL_ITEM_FUNC(pthread_attr_setname_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_setprio_np) 
    SYMBOL_ITEM_FUNC(pthread_attr_setschedparam) 
    SYMBOL_ITEM_FUNC(pthread_attr_setschedpolicy) 
    SYMBOL_ITEM_FUNC(pthread_attr_setscope) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstackaddr) 
    SYMBOL_ITEM_FUNC(pthread_attr_setstacksize) 
    SYMBOL_ITEM_FUNC(pthread_cancel) 
    SYMBOL_ITEM_FUNC(pthread_cleanup_pop) 
    SYMBOL_ITEM_FUNC(pthread_cleanup_push) 
    SYMBOL_ITEM_FUNC(pthread_cond_broadcast) 
    SYMBOL_ITEM_FUNC(pthread_cond_destroy) 
    SYMBOL_ITEM_FUNC(pthread_cond_init) 
    SYMBOL_ITEM_FUNC(pthread_cond_signal) 
    SYMBOL_ITEM_FUNC(pthread_cond_timedwait) 
    SYMBOL_ITEM_FUNC(pthread_cond_wait) 
    SYMBOL_ITEM_FUNC(pthread_condattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_condattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_condattr_init) 
    SYMBOL_ITEM_FUNC(pthread_condattr_setpshared) 
    SYMBOL_ITEM_FUNC(pthread_create) 
    SYMBOL_ITEM_FUNC(pthread_equal) 
    SYMBOL_ITEM_FUNC(pthread_exit) 
    SYMBOL_ITEM_FUNC(pthread_getconcurrency) 
    SYMBOL_ITEM_FUNC(pthread_getschedparam) 
    SYMBOL_ITEM_FUNC(pthread_getspecific) 
    SYMBOL_ITEM_FUNC(pthread_join) 
    SYMBOL_ITEM_FUNC(pthread_key_create) 
    SYMBOL_ITEM_FUNC(pthread_key_delete) 
    SYMBOL_ITEM_FUNC(pthread_mutex_destroy) 
    SYMBOL_ITEM_FUNC(pthread_mutex_getprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutex_init) 
    SYMBOL_ITEM_FUNC(pthread_mutex_lock) 
    SYMBOL_ITEM_FUNC(pthread_mutex_setprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutex_trylock) 
    SYMBOL_ITEM_FUNC(pthread_mutex_unlock) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getprotocol) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_gettype) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_init) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setprioceiling) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setprotocol) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_setpshared) 
    SYMBOL_ITEM_FUNC(pthread_mutexattr_settype) 
    SYMBOL_ITEM_FUNC(pthread_once) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_destroy) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_init) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_rdlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_tryrdlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_trywrlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_unlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlock_wrlock) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_destroy) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_getpshared) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_init) 
    SYMBOL_ITEM_FUNC(pthread_rwlockattr_setpshared) 
    SYMBOL_ITEM_FUNC(pthread_self) 
    SYMBOL_ITEM_FUNC(pthread_setcancelstate) 
    SYMBOL_ITEM_FUNC(pthread_setcanceltype) 
    SYMBOL_ITEM_FUNC(pthread_setconcurrency) 
    SYMBOL_ITEM_FUNC(pthread_setschedparam) 
    SYMBOL_ITEM_FUNC(pthread_setspecific) 
    SYMBOL_ITEM_FUNC(pthread_testcancel) 
    SYMBOL_ITEM_FUNC(pthread_yield_np) 
    SYMBOL_ITEM_FUNC(puts) 
    SYMBOL_ITEM_FUNC(raise) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(readdir) 
    SYMBOL_ITEM_FUNC(recv) 
    SYMBOL_ITEM_FUNC(recvfrom) 
    SYMBOL_ITEM_FUNC(reloc_elf) 
    SYMBOL_ITEM_FUNC(restore_core_regs) 
    SYMBOL_ITEM_FUNC(rewinddir) 
    SYMBOL_ITEM_FUNC(rmdir) 
    SYMBOL_ITEM_FUNC(seekdir) 
    SYMBOL_ITEM_FUNC(select) 
    SYMBOL_ITEM_FUNC(send) 
    SYMBOL_ITEM_FUNC(sendto) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(setreent) 
    SYMBOL_ITEM_FUNC(setsockopt) 
    SYMBOL_ITEM_FUNC(shutdown) 
    SYMBOL_ITEM_FUNC(signal) 
    SYMBOL_ITEM_FUNC(sleep) 
    SYMBOL_ITEM_FUNC(socket) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(strchrnul) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(syscall_accept) 
    SYMBOL_ITEM_FUNC(syscall_connect) 
    SYMBOL_ITEM_FUNC(syscall_read) 
    SYMBOL_ITEM_FUNC(syscall_recv) 
    SYMBOL_ITEM_FUNC(syscall_recvfrom) 
    SYMBOL_ITEM_FUNC(syscall_select) 
    SYMBOL_ITEM_FUNC(syscall_send) 
    SYMBOL_ITEM_FUNC(syscall_sendto) 
    SYMBOL_ITEM_FUNC(syscall_sleep) 
    SYMBOL_ITEM_FUNC(syscall_template) 
    SYMBOL_ITEM_FUNC(syscall_usleep) 
    SYMBOL_ITEM_FUNC(syscall_write) 
    SYMBOL_ITEM_FUNC(telldir) 
    SYMBOL_ITEM_FUNC(time) 
    SYMBOL_ITEM_FUNC(uheap_create) 
    SYMBOL_ITEM_FUNC(usleep) 
    SYMBOL_ITEM_FUNC(vfiprintf) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(write) 
    SYMBOL_ITEM_FUNC(xmalloc) 
    SYMBOL_ITEM_FUNC(xstrdup) 
    SYMBOL_ITEM_FUNC(xstrndup) 
    SYMBOL_ITEM_FUNC(xzalloc) 
    SYMBOL_ITEM_FUNC(yield) 
    SYMBOL_ITEM_OBJ(__ctype_ptr__) 
    SYMBOL_ITEM_OBJ(__mb_cur_max) 
    SYMBOL_ITEM_OBJ(__wctomb) 
    SYMBOL_ITEM_OBJ(_data) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
    SYMBOL_ITEM_OBJ(pth_internal_version) 
    SYMBOL_ITEM_OBJ(pth_state_names) 
    SYMBOL_ITEM_OBJ(_PathLocale) 
    SYMBOL_ITEM_OBJ(__bss_start) 
    SYMBOL_ITEM_OBJ(__bss_start__) 
    SYMBOL_ITEM_OBJ(__mlocale_changed) 
    SYMBOL_ITEM_OBJ(__nlocale_changed) 
    SYMBOL_ITEM_OBJ(__pth_compat_unit) 
    SYMBOL_ITEM_OBJ(pth_DQ) 
    SYMBOL_ITEM_OBJ(pth_NQ) 
    SYMBOL_ITEM_OBJ(pth_RQ) 
    SYMBOL_ITEM_OBJ(pth_SQ) 
    SYMBOL_ITEM_OBJ(pth_WQ) 
    SYMBOL_ITEM_OBJ(pth_current) 
    SYMBOL_ITEM_OBJ(pth_errno_flag) 
    SYMBOL_ITEM_OBJ(pth_errno_storage) 
    SYMBOL_ITEM_OBJ(pth_favournew) 
    SYMBOL_ITEM_OBJ(pth_initialized) 
    SYMBOL_ITEM_OBJ(pth_loadval) 
    SYMBOL_ITEM_OBJ(pth_main) 
    SYMBOL_ITEM_OBJ(pth_sched) 
    SYMBOL_ITEM_OBJ(pth_time_zero) 
    SYMBOL_ITEM_OBJ(__aeabi_idiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_ldiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr1) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr2) 
    SYMBOL_ITEM_OBJ(__dso_handle) 
SYMBOL_TABLE_END												
/*********************************************************************************************************	
  END													
*********************************************************************************************************/	
