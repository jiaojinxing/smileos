/********************************************************************************************************* 
** 													
**                                    中国软件开源组织 							
**														
**                                   嵌入式实时操作系统							
**														
**                                       SmileOS(TM)							
**														
**                               Copyright  All Rights Reserved						
**														
**--------------文件信息--------------------------------------------------------------------------------	
**														
** 文   件   名: sys_symbol.c											
**														
** 创   建   人: MakeSymbol 工具										
**														
** 文件创建日期: 2013 年 02 月 22 日						
**														
** 描        述: SmileOS 操作系统符号表. (此文件由 MakeSymbol 工具自动生成, 请勿修改)			
*********************************************************************************************************/	
#include "kern/func_config.h"                                          
#if CONFIG_MODULE_EN 
#include "module/symbol.h"										
														
#define SYMBOL_TABLE_BEGIN const symbol_t sys_symbol_tbl[] = { 						
  														
#define SYMBOL_TABLE_END   {0, 0, 0} };									
														
#define SYMBOL_ITEM_FUNC(pcName)                \
    {   #pcName, (void *)pcName,                \
        SYMBOL_TEXT                             \
    },													
														
#define SYMBOL_ITEM_OBJ(pcName)                 \
    {   #pcName, (void *)&pcName,               \
        SYMBOL_DATA                             \
    },													
														
/*********************************************************************************************************	
** 全局对象声明												
*********************************************************************************************************/	
extern int  MIN(); 
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
extern int  __aeabi_d2uiz(); 
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
extern int  __ascii_mbtowc(); 
extern int  __ascii_wctomb(); 
extern int  __b2d(); 
extern int  __board_mmc_getcd(); 
extern int  __clzsi2(); 
extern int  __cmpdf2(); 
extern int  __copybits(); 
extern int  __d2b(); 
extern int  __divdf3(); 
extern int  __divdi3(); 
extern int  __divsi3(); 
extern int  __eqdf2(); 
extern int  __errno(); 
extern int  __extendsfdf2(); 
extern int  __ffssi2(); 
extern int  __fixdfsi(); 
extern int  __fixunsdfsi(); 
extern int  __floatdidf(); 
extern int  __floatsidf(); 
extern int  __floatundidf(); 
extern int  __floatunsidf(); 
extern int  __fp_lock_all(); 
extern int  __fp_unlock_all(); 
extern int  __fpclassifyd(); 
extern int  __fxprintf(); 
extern int  __gedf2(); 
extern int  __get_socket(); 
extern int  __gethex(); 
extern int  __gettzinfo(); 
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
extern int  __hexdig_init(); 
extern int  __hexnan(); 
extern int  __hi0bits(); 
extern int  __i2b(); 
extern int  __kfree(); 
extern int  __kmalloc(); 
extern int  __ledf2(); 
extern int  __libc_clntudp_bufcreate(); 
extern int  __lo0bits(); 
extern int  __locale_charset(); 
extern int  __locale_cjk_lang(); 
extern int  __locale_mb_cur_max(); 
extern int  __locale_msgcharset(); 
extern int  __lshift(); 
extern int  __ltdf2(); 
extern int  __malloc_lock(); 
extern int  __malloc_unlock(); 
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
extern int  __ratio(); 
extern int  __restore_core_regs(); 
extern int  __s2b(); 
extern int  __sccl(); 
extern int  __sclose(); 
extern int  __seofread(); 
extern int  __sflags(); 
extern int  __sfmoreglue(); 
extern int  __sfp(); 
extern int  __sfp_lock_acquire(); 
extern int  __sfp_lock_release(); 
extern int  __sfvwrite_r(); 
extern int  __sinit(); 
extern int  __sinit_lock_acquire(); 
extern int  __sinit_lock_release(); 
extern int  __smakebuf_r(); 
extern int  __sprint_r(); 
extern int  __sread(); 
extern int  __srefill_r(); 
extern int  __sseek(); 
extern int  __ssprint_r(); 
extern int  __ssrefill_r(); 
extern int  __ssvfiscanf_r(); 
extern int  __ssvfscanf_r(); 
extern int  __strtok_r(); 
extern int  __subdf3(); 
extern int  __submore(); 
extern int  __swbuf(); 
extern int  __swbuf_r(); 
extern int  __swrite(); 
extern int  __swsetup_r(); 
extern int  __truncdfsf2(); 
extern int  __tz_lock(); 
extern int  __tz_unlock(); 
extern int  __tzcalc_limits(); 
extern int  __udivdi3(); 
extern int  __udivsi3(); 
extern int  __ulp(); 
extern int  __vfs_close(); 
extern int  __vfs_closedir(); 
extern int  _atoi_r(); 
extern int  _calloc_r(); 
extern int  _cleanup(); 
extern int  _cleanup_r(); 
extern int  _close_r(); 
extern int  _create_xid(); 
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
extern int  _fputwc_r(); 
extern int  _fread_r(); 
extern int  _free_r(); 
extern int  _fseek_r(); 
extern int  _fstat_r(); 
extern int  _ftell_r(); 
extern int  _fwalk(); 
extern int  _fwalk_reent(); 
extern int  _fwrite_r(); 
extern int  _getpid_r(); 
extern int  _gettimeofday_r(); 
extern int  _global_impure_ptr(); 
extern int  _init(); 
extern int  _isatty_r(); 
extern int  _kill_r(); 
extern int  _link_r(); 
extern int  _localeconv_r(); 
extern int  _lseek_r(); 
extern int  _malloc_r(); 
extern int  _mbrtowc_r(); 
extern int  _mbtowc_r(); 
extern int  _memalign_r(); 
extern int  _mkdir_r(); 
extern int  _mktm_r(); 
extern int  _mprec_log10(); 
extern int  _open_r(); 
extern int  _printf_r(); 
extern int  _puts_r(); 
extern int  _read_r(); 
extern int  _realloc_r(); 
extern int  _remove_r(); 
extern int  _rename_r(); 
extern int  _rmdir_r(); 
extern int  _sbrk_r(); 
extern int  _seterr_reply(); 
extern int  _setlocale_r(); 
extern int  _sfread_r(); 
extern int  _snprintf_r(); 
extern int  _sprintf_r(); 
extern int  _sscanf_r(); 
extern int  _start(); 
extern int  _stat_r(); 
extern int  _strdup_r(); 
extern int  _strndup_r(); 
extern int  _strtod_r(); 
extern int  _strtol_r(); 
extern int  _strtoul_r(); 
extern int  _sungetc_r(); 
extern int  _svfiprintf_r(); 
extern int  _svfprintf_r(); 
extern int  _times_r(); 
extern int  _ungetc_r(); 
extern int  _unlink_r(); 
extern int  _vfiprintf_r(); 
extern int  _vfprintf_r(); 
extern int  _vsnprintf_r(); 
extern int  _vsprintf_r(); 
extern int  _wait_r(); 
extern int  _wcrtomb_r(); 
extern int  _wctomb_r(); 
extern int  _write_r(); 
extern int  abort(); 
extern int  accept(); 
extern int  add_mtd_device(); 
extern int  alarm(); 
extern int  arch_build_context(); 
extern int  arch_in_kernel(); 
extern int  arch_kernel_enter(); 
extern int  arch_mmu_init(); 
extern int  arch_relocate_rel(); 
extern int  arch_relocate_rela(); 
extern int  arch_switch_context(); 
extern int  arch_switch_context_to(); 
extern int  atoi(); 
extern int  atomic_add(); 
extern int  atomic_dec(); 
extern int  atomic_dec_and_test(); 
extern int  atomic_dec_return(); 
extern int  atomic_inc(); 
extern int  atomic_inc_and_test(); 
extern int  atomic_inc_return(); 
extern int  atomic_read(); 
extern int  atomic_set(); 
extern int  atomic_sub(); 
extern int  atomic_sub_and_test(); 
extern int  atomic_sub_return(); 
extern int  audio_init(); 
extern int  authnone_create(); 
extern int  bind(); 
extern int  bindresvport(); 
extern int  bkdr_hash(); 
extern int  board_devices_create(); 
extern int  board_drivers_install(); 
extern int  board_mem_map(); 
extern int  board_nand_init(); 
extern int  board_resv_space(); 
extern int  calloc(); 
extern int  chdir(); 
extern int  clnt_create(); 
extern int  clnttcp_create(); 
extern int  clntudp_bufcreate(); 
extern int  clntudp_create(); 
extern int  clock_init(); 
extern int  close(); 
extern int  closedir(); 
extern int  clust2sect(); 
extern int  connect(); 
extern int  cpu_devices_create(); 
extern int  cpu_drivers_install(); 
extern int  cpu_init(); 
extern int  cpu_interrupt_init(); 
extern int  cpu_mem_map(); 
extern int  cpu_reset_init(); 
extern int  cpu_resv_space(); 
extern int  cpu_timer_init(); 
extern int  creat(); 
extern int  dabt_c_handler(); 
extern int  default_workqueue_create(); 
extern int  del_mtd_device(); 
extern int  device_create(); 
extern int  device_get(); 
extern int  device_lookup(); 
extern int  device_manager_init(); 
extern int  device_remove(); 
extern int  disk_initialize(); 
extern int  disk_ioctl(); 
extern int  disk_read(); 
extern int  disk_status(); 
extern int  disk_write(); 
extern int  dlclose(); 
extern int  dlerror(); 
extern int  dlopen(); 
extern int  dlsym(); 
extern int  dns_gethostbyname(); 
extern int  dns_getserver(); 
extern int  dns_init(); 
extern int  dns_local_addhost(); 
extern int  dns_local_removehost(); 
extern int  dns_setserver(); 
extern int  dns_tmr(); 
extern int  do_bind(); 
extern int  do_close(); 
extern int  do_connect(); 
extern int  do_delconn(); 
extern int  do_disconnect(); 
extern int  do_getaddr(); 
extern int  do_gethostbyname(); 
extern int  do_listen(); 
extern int  do_netifapi_netif_add(); 
extern int  do_netifapi_netif_common(); 
extern int  do_netifapi_netif_set_addr(); 
extern int  do_newconn(); 
extern int  do_recv(); 
extern int  do_send(); 
extern int  do_write(); 
extern int  driver_install(); 
extern int  driver_lookup(); 
extern int  driver_manager_init(); 
extern int  driver_ref_by_name(); 
extern int  driver_remove(); 
extern int  dup(); 
extern int  dup2(); 
extern int  etharp_add_static_entry(); 
extern int  etharp_cleanup_netif(); 
extern int  etharp_find_addr(); 
extern int  etharp_output(); 
extern int  etharp_query(); 
extern int  etharp_remove_static_entry(); 
extern int  etharp_request(); 
extern int  etharp_tmr(); 
extern int  ethbroadcast(); 
extern int  ethernet_input(); 
extern int  ethernetif_init(); 
extern int  ethzero(); 
extern int  f_chmod(); 
extern int  f_close(); 
extern int  f_getfree(); 
extern int  f_lseek(); 
extern int  f_mkdir(); 
extern int  f_mkfs(); 
extern int  f_mount(); 
extern int  f_open(); 
extern int  f_opendir(); 
extern int  f_read(); 
extern int  f_readdir(); 
extern int  f_rename(); 
extern int  f_stat(); 
extern int  f_sync(); 
extern int  f_truncate(); 
extern int  f_unlink(); 
extern int  f_utime(); 
extern int  f_write(); 
extern int  fb_create(); 
extern int  fb_init(); 
extern int  fclose(); 
extern int  fcntl(); 
extern int  fdopen(); 
extern int  ff_cre_syncobj(); 
extern int  ff_del_syncobj(); 
extern int  ff_memalloc(); 
extern int  ff_memfree(); 
extern int  ff_rel_grant(); 
extern int  ff_req_grant(); 
extern int  fflush(); 
extern int  fifo_create(); 
extern int  fifo_init(); 
extern int  file_system_install(); 
extern int  file_system_lookup(); 
extern int  file_system_manager_init(); 
extern int  file_system_remove(); 
extern int  fiq_c_handler(); 
extern int  fprintf(); 
extern int  fputwc(); 
extern int  fread(); 
extern int  free(); 
extern int  freeaddrinfo(); 
extern int  fseek(); 
extern int  fstat(); 
extern int  ftell(); 
extern int  ftpd_init(); 
extern int  fwrite(); 
extern int  get_fat(); 
extern int  get_fattime(); 
extern int  get_mtd_device(); 
extern int  get_mtd_device_nm(); 
extern int  getaddrinfo(); 
extern int  getcwd(); 
extern int  gethostbyname(); 
extern int  gethostbyname_r(); 
extern int  getpeername(); 
extern int  getpid(); 
extern int  getsockname(); 
extern int  getsockopt(); 
extern int  getticks(); 
extern int  gettid(); 
extern int  gmtime(); 
extern int  gmtime_r(); 
extern int  hash_tbl_create(); 
extern int  hash_tbl_destroy(); 
extern int  hash_tbl_insert(); 
extern int  hash_tbl_lookup(); 
extern int  heap_alloc(); 
extern int  heap_check(); 
extern int  heap_free(); 
extern int  heap_init(); 
extern int  hweight32(); 
extern int  hweight8(); 
extern int  icmp_dest_unreach(); 
extern int  icmp_input(); 
extern int  icmp_time_exceeded(); 
extern int  in_interrupt(); 
extern int  inet_chksum(); 
extern int  inet_chksum_pbuf(); 
extern int  inet_chksum_pseudo(); 
extern int  inet_chksum_pseudo_partial(); 
extern int  interrupt_disable(); 
extern int  interrupt_enter(); 
extern int  interrupt_exec(); 
extern int  interrupt_exit(); 
extern int  interrupt_init(); 
extern int  interrupt_install(); 
extern int  interrupt_mask(); 
extern int  interrupt_resume(); 
extern int  interrupt_unmask(); 
extern int  ioctl(); 
extern int  ip4_addr_isbroadcast(); 
extern int  ip4_addr_netmask_valid(); 
extern int  ip_addr_any(); 
extern int  ip_addr_broadcast(); 
extern int  ip_frag(); 
extern int  ip_input(); 
extern int  ip_output(); 
extern int  ip_output_hinted(); 
extern int  ip_output_if(); 
extern int  ip_reass(); 
extern int  ip_reass_tmr(); 
extern int  ip_route(); 
extern int  ipaddr_addr(); 
extern int  ipaddr_aton(); 
extern int  ipaddr_ntoa(); 
extern int  ipaddr_ntoa_r(); 
extern int  ipc_task_cleanup(); 
extern int  ipc_task_init(); 
extern int  irq_c_handler(); 
extern int  is_network_up(); 
extern int  isatty(); 
extern int  isr_invaild(); 
extern int  iswspace(); 
extern int  ka_to_ua(); 
extern int  kernel_init(); 
extern int  kernel_start(); 
extern int  kernel_timer(); 
extern int  kheap_check(); 
extern int  kheap_create(); 
extern int  kidle_create(); 
extern int  kill(); 
extern int  klogd_create(); 
extern int  kthread_create(); 
extern int  kthread_shell(); 
extern int  lcd_init(); 
extern int  listen(); 
extern int  localeconv(); 
extern int  lwip_accept(); 
extern int  lwip_bind(); 
extern int  lwip_chksum_copy(); 
extern int  lwip_close(); 
extern int  lwip_connect(); 
extern int  lwip_fcntl(); 
extern int  lwip_freeaddrinfo(); 
extern int  lwip_getaddrinfo(); 
extern int  lwip_gethostbyname(); 
extern int  lwip_gethostbyname_r(); 
extern int  lwip_getpeername(); 
extern int  lwip_getsockname(); 
extern int  lwip_getsockopt(); 
extern int  lwip_htonl(); 
extern int  lwip_htons(); 
extern int  lwip_init(); 
extern int  lwip_ioctl(); 
extern int  lwip_listen(); 
extern int  lwip_ntohl(); 
extern int  lwip_ntohs(); 
extern int  lwip_read(); 
extern int  lwip_recv(); 
extern int  lwip_recvfrom(); 
extern int  lwip_select(); 
extern int  lwip_send(); 
extern int  lwip_sendto(); 
extern int  lwip_setsockopt(); 
extern int  lwip_shutdown(); 
extern int  lwip_socket(); 
extern int  lwip_socket_init(); 
extern int  lwip_socket_set_ctx(); 
extern int  lwip_strerr(); 
extern int  lwip_write(); 
extern int  main(); 
extern int  malloc(); 
extern int  mbrtowc(); 
extern int  mem_calloc(); 
extern int  mem_free(); 
extern int  mem_free_callback(); 
extern int  mem_init(); 
extern int  mem_malloc(); 
extern int  mem_trim(); 
extern int  memalign(); 
extern int  memchr(); 
extern int  memcmp(); 
extern int  memcpy(); 
extern int  memmove(); 
extern int  memp_free(); 
extern int  memp_init(); 
extern int  memp_malloc(); 
extern int  memrchr(); 
extern int  memset(); 
extern int  mkdir(); 
extern int  mmc_erase_blocks(); 
extern int  mmc_getcd(); 
extern int  mmc_init(); 
extern int  mmc_read_blocks(); 
extern int  mmc_set_clock(); 
extern int  mmc_switch_part(); 
extern int  mmc_write_blocks(); 
extern int  mmu_clean_dcache_index(); 
extern int  mmu_clean_invalidate_dcache_index(); 
extern int  mmu_clean_invalidate_dcache_mva(); 
extern int  mmu_disable(); 
extern int  mmu_disable_align_fault_check(); 
extern int  mmu_disable_dcache(); 
extern int  mmu_disable_icache(); 
extern int  mmu_drain_write_buffer(); 
extern int  mmu_enable(); 
extern int  mmu_enable_align_fault_check(); 
extern int  mmu_enable_dcache(); 
extern int  mmu_enable_icache(); 
extern int  mmu_get_cache_type(); 
extern int  mmu_get_cpu_id(); 
extern int  mmu_get_data_fault_status(); 
extern int  mmu_get_fault_address(); 
extern int  mmu_get_prefetch_fault_status(); 
extern int  mmu_invalidate_dcache(); 
extern int  mmu_invalidate_dtlb(); 
extern int  mmu_invalidate_dtlb_mva(); 
extern int  mmu_invalidate_icache(); 
extern int  mmu_invalidate_icache_dcache(); 
extern int  mmu_invalidate_icache_mva(); 
extern int  mmu_invalidate_itlb(); 
extern int  mmu_invalidate_itlb_dtlb(); 
extern int  mmu_invalidate_itlb_mva(); 
extern int  mmu_map_page(); 
extern int  mmu_map_region(); 
extern int  mmu_map_section(); 
extern int  mmu_set_domain(); 
extern int  mmu_set_sys_rom_protect_bit(); 
extern int  mmu_set_ttb(); 
extern int  mmu_set_vector_addr(); 
extern int  mmu_unmap_section(); 
extern int  mmu_wait_for_interrupt(); 
extern int  module_close(); 
extern int  module_init(); 
extern int  module_load(); 
extern int  module_open(); 
extern int  module_ref_by_addr(); 
extern int  module_symbol(); 
extern int  module_unload(); 
extern int  module_unref(); 
extern int  mount_point_get(); 
extern int  mount_point_install(); 
extern int  mount_point_lookup(); 
extern int  mount_point_manager_init(); 
extern int  mount_point_remove(); 
extern int  mountproc3_dump_3(); 
extern int  mountproc3_export_3(); 
extern int  mountproc3_mnt_3(); 
extern int  mountproc3_null_3(); 
extern int  mountproc3_umnt_3(); 
extern int  mountproc3_umntall_3(); 
extern int  mqueue_abort(); 
extern int  mqueue_abort_fetch(); 
extern int  mqueue_abort_post(); 
extern int  mqueue_create(); 
extern int  mqueue_destroy(); 
extern int  mqueue_fetch(); 
extern int  mqueue_flush(); 
extern int  mqueue_msg_nr(); 
extern int  mqueue_post(); 
extern int  mqueue_set_valid(); 
extern int  mqueue_tryfetch(); 
extern int  mqueue_trypost(); 
extern int  mqueue_valid(); 
extern int  msleep(); 
extern int  mtd_get_len_incl_bad(); 
extern int  mtdblock_create(); 
extern int  mtdblock_init(); 
extern int  mutex_abort(); 
extern int  mutex_create(); 
extern int  mutex_destroy(); 
extern int  mutex_lock(); 
extern int  mutex_set_valid(); 
extern int  mutex_trylock(); 
extern int  mutex_unlock(); 
extern int  mutex_valid(); 
extern int  nand_calculate_ecc(); 
extern int  nand_correct_data(); 
extern int  nand_default_bbt(); 
extern int  nand_erase_nand(); 
extern int  nand_flash_ids(); 
extern int  nand_init(); 
extern int  nand_isbad_bbt(); 
extern int  nand_manuf_ids(); 
extern int  nand_read_buf(); 
extern int  nand_read_buf16(); 
extern int  nand_read_byte(); 
extern int  nand_register(); 
extern int  nand_release(); 
extern int  nand_scan(); 
extern int  nand_scan_bbt(); 
extern int  nand_scan_ident(); 
extern int  nand_scan_tail(); 
extern int  nand_update_bbt(); 
extern int  nand_wait_ready(); 
extern int  nand_write_buf(); 
extern int  nand_write_buf16(); 
extern int  nandmtd_erase_block(); 
extern int  nanf(); 
extern int  netbuf_alloc(); 
extern int  netbuf_chain(); 
extern int  netbuf_data(); 
extern int  netbuf_delete(); 
extern int  netbuf_first(); 
extern int  netbuf_free(); 
extern int  netbuf_new(); 
extern int  netbuf_next(); 
extern int  netbuf_ref(); 
extern int  netconn_accept(); 
extern int  netconn_alloc(); 
extern int  netconn_bind(); 
extern int  netconn_close(); 
extern int  netconn_connect(); 
extern int  netconn_delete(); 
extern int  netconn_disconnect(); 
extern int  netconn_free(); 
extern int  netconn_getaddr(); 
extern int  netconn_gethostbyname(); 
extern int  netconn_listen_with_backlog(); 
extern int  netconn_new_with_proto_and_callback(); 
extern int  netconn_recv(); 
extern int  netconn_recv_tcp_pbuf(); 
extern int  netconn_recved(); 
extern int  netconn_send(); 
extern int  netconn_sendto(); 
extern int  netconn_shutdown(); 
extern int  netconn_write_partly(); 
extern int  netif_add(); 
extern int  netif_find(); 
extern int  netif_init(); 
extern int  netif_loop_output(); 
extern int  netif_poll(); 
extern int  netif_remove(); 
extern int  netif_set_addr(); 
extern int  netif_set_default(); 
extern int  netif_set_down(); 
extern int  netif_set_gw(); 
extern int  netif_set_ipaddr(); 
extern int  netif_set_link_callback(); 
extern int  netif_set_link_down(); 
extern int  netif_set_link_up(); 
extern int  netif_set_netmask(); 
extern int  netif_set_remove_callback(); 
extern int  netif_set_status_callback(); 
extern int  netif_set_up(); 
extern int  netifapi_netif_add(); 
extern int  netifapi_netif_common(); 
extern int  netifapi_netif_set_addr(); 
extern int  netio_init(); 
extern int  nfsproc3_access_3(); 
extern int  nfsproc3_commit_3(); 
extern int  nfsproc3_create_3(); 
extern int  nfsproc3_fsinfo_3(); 
extern int  nfsproc3_fsstat_3(); 
extern int  nfsproc3_getattr_3(); 
extern int  nfsproc3_link_3(); 
extern int  nfsproc3_lookup_3(); 
extern int  nfsproc3_mkdir_3(); 
extern int  nfsproc3_mknod_3(); 
extern int  nfsproc3_null_3(); 
extern int  nfsproc3_pathconf_3(); 
extern int  nfsproc3_read_3(); 
extern int  nfsproc3_readdir_3(); 
extern int  nfsproc3_readdirplus_3(); 
extern int  nfsproc3_readlink_3(); 
extern int  nfsproc3_remove_3(); 
extern int  nfsproc3_rename_3(); 
extern int  nfsproc3_rmdir_3(); 
extern int  nfsproc3_setattr_3(); 
extern int  nfsproc3_symlink_3(); 
extern int  nfsproc3_write_3(); 
extern int  null_init(); 
extern int  nval_del(); 
extern int  nval_get(); 
extern int  nval_hasvalues(); 
extern int  nval_list(); 
extern int  nval_set(); 
extern int  open(); 
extern int  opendir(); 
extern int  pabt_c_handler(); 
extern int  pause(); 
extern int  pbuf_alloc(); 
extern int  pbuf_alloced_custom(); 
extern int  pbuf_cat(); 
extern int  pbuf_chain(); 
extern int  pbuf_clen(); 
extern int  pbuf_coalesce(); 
extern int  pbuf_copy(); 
extern int  pbuf_copy_partial(); 
extern int  pbuf_dechain(); 
extern int  pbuf_fill_chksum(); 
extern int  pbuf_free(); 
extern int  pbuf_free_callback(); 
extern int  pbuf_get_at(); 
extern int  pbuf_header(); 
extern int  pbuf_memcmp(); 
extern int  pbuf_memfind(); 
extern int  pbuf_realloc(); 
extern int  pbuf_ref(); 
extern int  pbuf_strstr(); 
extern int  pbuf_take(); 
extern int  pipe(); 
extern int  pipe_create(); 
extern int  pmap_getport(); 
extern int  poll(); 
extern int  printf(); 
extern int  printk(); 
extern int  procfs_creat(); 
extern int  procfs_mkdir(); 
extern int  put_fat(); 
extern int  put_mtd_device(); 
extern int  puts(); 
extern int  raise(); 
extern int  ramdisk_create(); 
extern int  ramdisk_init(); 
extern int  raw_bind(); 
extern int  raw_connect(); 
extern int  raw_input(); 
extern int  raw_new(); 
extern int  raw_recv(); 
extern int  raw_remove(); 
extern int  raw_send(); 
extern int  raw_sendto(); 
extern int  read(); 
extern int  readdir(); 
extern int  recv(); 
extern int  recvfrom(); 
extern int  remove(); 
extern int  rename(); 
extern int  restore_core_regs(); 
extern int  rewinddir(); 
extern int  rmdir(); 
extern int  schedule(); 
extern int  seekdir(); 
extern int  select(); 
extern int  select_init(); 
extern int  select_select(); 
extern int  select_unselect(); 
extern int  sem_abort(); 
extern int  sem_create(); 
extern int  sem_destroy(); 
extern int  sem_set_valid(); 
extern int  sem_signal(); 
extern int  sem_sync(); 
extern int  sem_trywait(); 
extern int  sem_valid(); 
extern int  sem_wait(); 
extern int  send(); 
extern int  sendto(); 
extern int  serial0_init(); 
extern int  setlocale(); 
extern int  setpinfo(); 
extern int  setsockopt(); 
extern int  sharemem_create(); 
extern int  sharemem_init(); 
extern int  shutdown(); 
extern int  signal(); 
extern int  sigprocmask(); 
extern int  sigsuspend(); 
extern int  sleep(); 
extern int  smileos_socket_report(); 
extern int  snprintf(); 
extern int  socket(); 
extern int  socket_attach(); 
extern int  socket_init(); 
extern int  socket_op_end(); 
extern int  socket_priv_fd(); 
extern int  socket_stat(); 
extern int  sprintf(); 
extern int  sscanf(); 
extern int  stat(); 
extern int  stats_display(); 
extern int  stats_display_mem(); 
extern int  stats_display_memp(); 
extern int  stats_display_proto(); 
extern int  stats_display_sys(); 
extern int  stats_init(); 
extern int  strcat(); 
extern int  strchr(); 
extern int  strchrnul(); 
extern int  strcmp(); 
extern int  strcpy(); 
extern int  strdup(); 
extern int  strlcpy(); 
extern int  strlen(); 
extern int  strncmp(); 
extern int  strncpy(); 
extern int  strndup(); 
extern int  strnlen(); 
extern int  strtod(); 
extern int  strtof(); 
extern int  strtok_r(); 
extern int  strtol(); 
extern int  strtoul(); 
extern int  swi_handler(); 
extern int  symbol_name(); 
extern int  sys_arch_mbox_fetch(); 
extern int  sys_arch_mbox_tryfetch(); 
extern int  sys_arch_sem_wait(); 
extern int  sys_dns_lookup_local(); 
extern int  sys_do_enter(); 
extern int  sys_init(); 
extern int  sys_jiffies(); 
extern int  sys_mbox_free(); 
extern int  sys_mbox_new(); 
extern int  sys_mbox_post(); 
extern int  sys_mbox_set_invalid(); 
extern int  sys_mbox_trypost(); 
extern int  sys_mbox_valid(); 
extern int  sys_msleep(); 
extern int  sys_mutex_free(); 
extern int  sys_mutex_lock(); 
extern int  sys_mutex_new(); 
extern int  sys_mutex_set_invalid(); 
extern int  sys_mutex_unlock(); 
extern int  sys_mutex_valid(); 
extern int  sys_now(); 
extern int  sys_sem_free(); 
extern int  sys_sem_new(); 
extern int  sys_sem_set_invalid(); 
extern int  sys_sem_signal(); 
extern int  sys_sem_valid(); 
extern int  sys_symbol_lookup(); 
extern int  sys_symbol_tbl_init(); 
extern int  sys_thread_hostent(); 
extern int  sys_thread_new(); 
extern int  sys_timeout(); 
extern int  sys_timeouts_init(); 
extern int  sys_timeouts_mbox_fetch(); 
extern int  sys_untimeout(); 
extern int  task_cleanup(); 
extern int  task_getpid(); 
extern int  task_schedule(); 
extern int  task_sleep(); 
extern int  task_switch_hook(); 
extern int  tcp_abandon(); 
extern int  tcp_abort(); 
extern int  tcp_accept(); 
extern int  tcp_alloc(); 
extern int  tcp_arg(); 
extern int  tcp_backoff(); 
extern int  tcp_bind(); 
extern int  tcp_close(); 
extern int  tcp_connect(); 
extern int  tcp_debug_state_str(); 
extern int  tcp_eff_send_mss(); 
extern int  tcp_enqueue_flags(); 
extern int  tcp_err(); 
extern int  tcp_fasttmr(); 
extern int  tcp_init(); 
extern int  tcp_input(); 
extern int  tcp_keepalive(); 
extern int  tcp_listen_with_backlog(); 
extern int  tcp_new(); 
extern int  tcp_next_iss(); 
extern int  tcp_output(); 
extern int  tcp_pcb_lists(); 
extern int  tcp_pcb_purge(); 
extern int  tcp_pcb_remove(); 
extern int  tcp_persist_backoff(); 
extern int  tcp_poll(); 
extern int  tcp_process_refused_data(); 
extern int  tcp_recv(); 
extern int  tcp_recv_null(); 
extern int  tcp_recved(); 
extern int  tcp_rexmit(); 
extern int  tcp_rexmit_fast(); 
extern int  tcp_rexmit_rto(); 
extern int  tcp_rst(); 
extern int  tcp_seg_copy(); 
extern int  tcp_seg_free(); 
extern int  tcp_segs_free(); 
extern int  tcp_send_empty_ack(); 
extern int  tcp_send_fin(); 
extern int  tcp_sent(); 
extern int  tcp_setprio(); 
extern int  tcp_shutdown(); 
extern int  tcp_slowtmr(); 
extern int  tcp_state_str(); 
extern int  tcp_timer_needed(); 
extern int  tcp_tmr(); 
extern int  tcp_update_rcv_ann_wnd(); 
extern int  tcp_write(); 
extern int  tcp_zero_window_probe(); 
extern int  tcpip_apimsg(); 
extern int  tcpip_apimsg_lock(); 
extern int  tcpip_callback_with_block(); 
extern int  tcpip_callbackmsg_delete(); 
extern int  tcpip_callbackmsg_new(); 
extern int  tcpip_init(); 
extern int  tcpip_input(); 
extern int  tcpip_netifapi_lock(); 
extern int  tcpip_timeout(); 
extern int  tcpip_trycallback(); 
extern int  tcpip_untimeout(); 
extern int  telldir(); 
extern int  test_code(); 
extern int  test_code1(); 
extern int  test_code2(); 
extern int  test_code3(); 
extern int  test_code4(); 
extern int  test_code5(); 
extern int  test_code6(); 
extern int  time(); 
extern int  touch_init(); 
extern int  ua_to_ka(); 
extern int  udp_bind(); 
extern int  udp_connect(); 
extern int  udp_disconnect(); 
extern int  udp_init(); 
extern int  udp_input(); 
extern int  udp_new(); 
extern int  udp_recv(); 
extern int  udp_remove(); 
extern int  udp_send(); 
extern int  udp_send_chksum(); 
extern int  udp_sendto(); 
extern int  udp_sendto_chksum(); 
extern int  udp_sendto_if(); 
extern int  udp_sendto_if_chksum(); 
extern int  undf_c_handler(); 
extern int  ungetc(); 
extern int  usleep(); 
extern int  vfiprintf(); 
extern int  vfprintf(); 
extern int  vfs_access(); 
extern int  vfs_block_helper(); 
extern int  vfs_chdir(); 
extern int  vfs_close(); 
extern int  vfs_closedir(); 
extern int  vfs_dup(); 
extern int  vfs_dup2(); 
extern int  vfs_event_report(); 
extern int  vfs_fcntl(); 
extern int  vfs_fdatasync(); 
extern int  vfs_file_alloc(); 
extern int  vfs_file_free(); 
extern int  vfs_fstat(); 
extern int  vfs_fsync(); 
extern int  vfs_ftruncate(); 
extern int  vfs_get_file(); 
extern int  vfs_getcwd(); 
extern int  vfs_init(); 
extern int  vfs_ioctl(); 
extern int  vfs_isatty(); 
extern int  vfs_link(); 
extern int  vfs_lseek(); 
extern int  vfs_mkdir(); 
extern int  vfs_mkfs(); 
extern int  vfs_mount(); 
extern int  vfs_mount_point_lookup(); 
extern int  vfs_mount_point_lookup2(); 
extern int  vfs_mount_point_lookup2_ref(); 
extern int  vfs_mount_point_lookup_ref(); 
extern int  vfs_open(); 
extern int  vfs_opendir(); 
extern int  vfs_path_add_mount_point(); 
extern int  vfs_path_format(); 
extern int  vfs_path_normalization(); 
extern int  vfs_process_cleanup(); 
extern int  vfs_process_init(); 
extern int  vfs_put_file(); 
extern int  vfs_read(); 
extern int  vfs_readdir(); 
extern int  vfs_rename(); 
extern int  vfs_rewinddir(); 
extern int  vfs_rmdir(); 
extern int  vfs_scan_file(); 
extern int  vfs_seekdir(); 
extern int  vfs_select(); 
extern int  vfs_select_file(); 
extern int  vfs_stat(); 
extern int  vfs_sync(); 
extern int  vfs_telldir(); 
extern int  vfs_truncate(); 
extern int  vfs_unlink(); 
extern int  vfs_unmount(); 
extern int  vfs_unselect_file(); 
extern int  vfs_write(); 
extern int  vmm_init(); 
extern int  vsnprintf(); 
extern int  vsprintf(); 
extern int  wcrtomb(); 
extern int  workqueue_add(); 
extern int  workqueue_create(); 
extern int  write(); 
extern int  xdr_ACCESS3args(); 
extern int  xdr_ACCESS3res(); 
extern int  xdr_ACCESS3resfail(); 
extern int  xdr_ACCESS3resok(); 
extern int  xdr_COMMIT3args(); 
extern int  xdr_COMMIT3res(); 
extern int  xdr_COMMIT3resfail(); 
extern int  xdr_COMMIT3resok(); 
extern int  xdr_CREATE3args(); 
extern int  xdr_CREATE3res(); 
extern int  xdr_CREATE3resfail(); 
extern int  xdr_CREATE3resok(); 
extern int  xdr_FSINFO3args(); 
extern int  xdr_FSINFO3res(); 
extern int  xdr_FSINFO3resfail(); 
extern int  xdr_FSINFO3resok(); 
extern int  xdr_FSSTAT3args(); 
extern int  xdr_FSSTAT3res(); 
extern int  xdr_FSSTAT3resfail(); 
extern int  xdr_FSSTAT3resok(); 
extern int  xdr_GETATTR3args(); 
extern int  xdr_GETATTR3res(); 
extern int  xdr_GETATTR3resok(); 
extern int  xdr_LINK3args(); 
extern int  xdr_LINK3res(); 
extern int  xdr_LINK3resfail(); 
extern int  xdr_LINK3resok(); 
extern int  xdr_LOOKUP3args(); 
extern int  xdr_LOOKUP3res(); 
extern int  xdr_LOOKUP3resfail(); 
extern int  xdr_LOOKUP3resok(); 
extern int  xdr_MKDIR3args(); 
extern int  xdr_MKDIR3res(); 
extern int  xdr_MKDIR3resfail(); 
extern int  xdr_MKDIR3resok(); 
extern int  xdr_MKNOD3args(); 
extern int  xdr_MKNOD3res(); 
extern int  xdr_MKNOD3resfail(); 
extern int  xdr_MKNOD3resok(); 
extern int  xdr_PATHCONF3args(); 
extern int  xdr_PATHCONF3res(); 
extern int  xdr_PATHCONF3resfail(); 
extern int  xdr_PATHCONF3resok(); 
extern int  xdr_READ3args(); 
extern int  xdr_READ3res(); 
extern int  xdr_READ3resfail(); 
extern int  xdr_READ3resok(); 
extern int  xdr_READDIR3args(); 
extern int  xdr_READDIR3res(); 
extern int  xdr_READDIR3resfail(); 
extern int  xdr_READDIR3resok(); 
extern int  xdr_READDIRPLUS3args(); 
extern int  xdr_READDIRPLUS3res(); 
extern int  xdr_READDIRPLUS3resfail(); 
extern int  xdr_READDIRPLUS3resok(); 
extern int  xdr_READLINK3args(); 
extern int  xdr_READLINK3res(); 
extern int  xdr_READLINK3resfail(); 
extern int  xdr_READLINK3resok(); 
extern int  xdr_REMOVE3args(); 
extern int  xdr_REMOVE3res(); 
extern int  xdr_REMOVE3resfail(); 
extern int  xdr_REMOVE3resok(); 
extern int  xdr_RENAME3args(); 
extern int  xdr_RENAME3res(); 
extern int  xdr_RENAME3resfail(); 
extern int  xdr_RENAME3resok(); 
extern int  xdr_RMDIR3args(); 
extern int  xdr_RMDIR3res(); 
extern int  xdr_RMDIR3resfail(); 
extern int  xdr_RMDIR3resok(); 
extern int  xdr_SETATTR3args(); 
extern int  xdr_SETATTR3res(); 
extern int  xdr_SETATTR3resfail(); 
extern int  xdr_SETATTR3resok(); 
extern int  xdr_SYMLINK3args(); 
extern int  xdr_SYMLINK3res(); 
extern int  xdr_SYMLINK3resfail(); 
extern int  xdr_SYMLINK3resok(); 
extern int  xdr_WRITE3args(); 
extern int  xdr_WRITE3res(); 
extern int  xdr_WRITE3resfail(); 
extern int  xdr_WRITE3resok(); 
extern int  xdr_accepted_reply(); 
extern int  xdr_array(); 
extern int  xdr_bool(); 
extern int  xdr_bytes(); 
extern int  xdr_callhdr(); 
extern int  xdr_char(); 
extern int  xdr_cookie3(); 
extern int  xdr_cookieverf3(); 
extern int  xdr_count3(); 
extern int  xdr_createhow3(); 
extern int  xdr_createmode3(); 
extern int  xdr_createverf3(); 
extern int  xdr_des_block(); 
extern int  xdr_devicedata3(); 
extern int  xdr_dirlist3(); 
extern int  xdr_dirlistplus3(); 
extern int  xdr_diropargs3(); 
extern int  xdr_dirpath(); 
extern int  xdr_double(); 
extern int  xdr_entry3(); 
extern int  xdr_entryplus3(); 
extern int  xdr_enum(); 
extern int  xdr_exportnode(); 
extern int  xdr_exports(); 
extern int  xdr_fattr3(); 
extern int  xdr_fhandle3(); 
extern int  xdr_fileid3(); 
extern int  xdr_filename3(); 
extern int  xdr_float(); 
extern int  xdr_free(); 
extern int  xdr_ftype3(); 
extern int  xdr_gid3(); 
extern int  xdr_groupnode(); 
extern int  xdr_groups(); 
extern int  xdr_hyper(); 
extern int  xdr_int(); 
extern int  xdr_int16_t(); 
extern int  xdr_int32(); 
extern int  xdr_int32_t(); 
extern int  xdr_int64(); 
extern int  xdr_int64_t(); 
extern int  xdr_int8_t(); 
extern int  xdr_long(); 
extern int  xdr_longlong_t(); 
extern int  xdr_mknoddata3(); 
extern int  xdr_mode3(); 
extern int  xdr_mountbody(); 
extern int  xdr_mountlist(); 
extern int  xdr_mountres3(); 
extern int  xdr_mountres3_ok(); 
extern int  xdr_mountstat3(); 
extern int  xdr_name(); 
extern int  xdr_netobj(); 
extern int  xdr_nfs_fh3(); 
extern int  xdr_nfspath3(); 
extern int  xdr_nfsstat3(); 
extern int  xdr_nfstime3(); 
extern int  xdr_offset3(); 
extern int  xdr_opaque(); 
extern int  xdr_opaque_auth(); 
extern int  xdr_pmap(); 
extern int  xdr_pointer(); 
extern int  xdr_post_op_attr(); 
extern int  xdr_post_op_fh3(); 
extern int  xdr_pre_op_attr(); 
extern int  xdr_reference(); 
extern int  xdr_rejected_reply(); 
extern int  xdr_replymsg(); 
extern int  xdr_sattr3(); 
extern int  xdr_sattrguard3(); 
extern int  xdr_set_atime(); 
extern int  xdr_set_gid3(); 
extern int  xdr_set_mode3(); 
extern int  xdr_set_mtime(); 
extern int  xdr_set_size3(); 
extern int  xdr_set_uid3(); 
extern int  xdr_short(); 
extern int  xdr_size3(); 
extern int  xdr_sizeof(); 
extern int  xdr_specdata3(); 
extern int  xdr_stable_how(); 
extern int  xdr_string(); 
extern int  xdr_symlinkdata3(); 
extern int  xdr_time_how(); 
extern int  xdr_u_char(); 
extern int  xdr_u_hyper(); 
extern int  xdr_u_int(); 
extern int  xdr_u_long(); 
extern int  xdr_u_longlong_t(); 
extern int  xdr_u_short(); 
extern int  xdr_uid3(); 
extern int  xdr_uint16_t(); 
extern int  xdr_uint32(); 
extern int  xdr_uint32_t(); 
extern int  xdr_uint64(); 
extern int  xdr_uint64_t(); 
extern int  xdr_uint8_t(); 
extern int  xdr_union(); 
extern int  xdr_vector(); 
extern int  xdr_void(); 
extern int  xdr_wcc_attr(); 
extern int  xdr_wcc_data(); 
extern int  xdr_wrapstring(); 
extern int  xdr_writeverf3(); 
extern int  xdrmem_create(); 
extern int  xdrrec_create(); 
extern int  xdrrec_endofrecord(); 
extern int  xdrrec_eof(); 
extern int  xdrrec_skiprecord(); 
extern int  xdrstdio_create(); 
extern int  xmalloc(); 
extern int  xstrdup(); 
extern int  xstrndup(); 
extern int  xxx_init(); 
extern int  xzalloc(); 
extern int  yaffs1_scan(); 
extern int  yaffs2_checkpt_invalidate(); 
extern int  yaffs2_checkpt_invalidate_stream(); 
extern int  yaffs2_checkpt_open(); 
extern int  yaffs2_checkpt_rd(); 
extern int  yaffs2_checkpt_required(); 
extern int  yaffs2_checkpt_restore(); 
extern int  yaffs2_checkpt_wr(); 
extern int  yaffs2_clear_oldest_dirty_seq(); 
extern int  yaffs2_find_oldest_dirty_seq(); 
extern int  yaffs2_find_refresh_block(); 
extern int  yaffs2_get_checkpt_sum(); 
extern int  yaffs2_handle_hole(); 
extern int  yaffs2_scan_backwards(); 
extern int  yaffs2_update_oldest_dirty_seq(); 
extern int  yaffs_access(); 
extern int  yaffs_add_device(); 
extern int  yaffs_add_find_tnode_0(); 
extern int  yaffs_add_obj_to_dir(); 
extern int  yaffs_addr_to_chunk(); 
extern int  yaffs_alloc_raw_obj(); 
extern int  yaffs_alloc_raw_tnode(); 
extern int  yaffs_attribs_init(); 
extern int  yaffs_bg_gc(); 
extern int  yaffs_block_became_dirty(); 
extern int  yaffs_block_ok_for_gc(); 
extern int  yaffs_calc_checkpt_blocks_required(); 
extern int  yaffs_calc_oldest_dirty_seq(); 
extern int  yaffs_calc_tags_ecc(); 
extern int  yaffs_check_alloc_available(); 
extern int  yaffs_check_chunk_bit(); 
extern int  yaffs_check_ff(); 
extern int  yaffs_check_tags_ecc(); 
extern int  yaffs_checkpoint_save(); 
extern int  yaffs_checkpt_close(); 
extern int  yaffs_chmod(); 
extern int  yaffs_chunk_del(); 
extern int  yaffs_clear_chunk_bit(); 
extern int  yaffs_clear_chunk_bits(); 
extern int  yaffs_clone_str(); 
extern int  yaffs_close(); 
extern int  yaffs_closedir(); 
extern int  yaffs_count_chunk_bits(); 
extern int  yaffs_count_free_chunks(); 
extern int  yaffs_create_dir(); 
extern int  yaffs_create_file(); 
extern int  yaffs_create_special(); 
extern int  yaffs_create_symlink(); 
extern int  yaffs_deinit_nand(); 
extern int  yaffs_deinit_raw_tnodes_and_objs(); 
extern int  yaffs_deinitialise(); 
extern int  yaffs_del_obj(); 
extern int  yaffs_dev_rewind(); 
extern int  yaffs_do_file_wr(); 
extern int  yaffs_dump_dev(); 
extern int  yaffs_dup(); 
extern int  yaffs_ecc_calc(); 
extern int  yaffs_ecc_calc_other(); 
extern int  yaffs_ecc_correct(); 
extern int  yaffs_ecc_correct_other(); 
extern int  yaffs_erase_block(); 
extern int  yaffs_error_to_str(); 
extern int  yaffs_fchmod(); 
extern int  yaffs_fdatasync(); 
extern int  yaffs_fgetxattr(); 
extern int  yaffs_file_rd(); 
extern int  yaffs_find_by_name(); 
extern int  yaffs_find_by_number(); 
extern int  yaffs_find_or_create_by_number(); 
extern int  yaffs_find_tnode_0(); 
extern int  yaffs_flistxattr(); 
extern int  yaffs_flush(); 
extern int  yaffs_flush_file(); 
extern int  yaffs_flush_whole_cache(); 
extern int  yaffs_format(); 
extern int  yaffs_format_dev(); 
extern int  yaffs_free_raw_obj(); 
extern int  yaffs_free_raw_tnode(); 
extern int  yaffs_freespace(); 
extern int  yaffs_fremovexattr(); 
extern int  yaffs_fsetxattr(); 
extern int  yaffs_fstat(); 
extern int  yaffs_fsync(); 
extern int  yaffs_ftruncate(); 
extern int  yaffs_futime(); 
extern int  yaffs_get_attribs(); 
extern int  yaffs_get_equivalent_obj(); 
extern int  yaffs_get_error(); 
extern int  yaffs_get_group_base(); 
extern int  yaffs_get_mtd_device(); 
extern int  yaffs_get_n_free_chunks(); 
extern int  yaffs_get_obj_inode(); 
extern int  yaffs_get_obj_length(); 
extern int  yaffs_get_obj_link_count(); 
extern int  yaffs_get_obj_name(); 
extern int  yaffs_get_obj_type(); 
extern int  yaffs_get_symlink_alias(); 
extern int  yaffs_get_temp_buffer(); 
extern int  yaffs_get_tnode(); 
extern int  yaffs_get_trace(); 
extern int  yaffs_get_xattrib(); 
extern int  yaffs_getdev(); 
extern int  yaffs_getxattr(); 
extern int  yaffs_guts_initialise(); 
extern int  yaffs_handle_chunk_error(); 
extern int  yaffs_handle_defered_free(); 
extern int  yaffs_handle_shadowed_obj(); 
extern int  yaffs_init_nand(); 
extern int  yaffs_init_raw_tnodes_and_objs(); 
extern int  yaffs_inodecount(); 
extern int  yaffs_is_non_empty_dir(); 
extern int  yaffs_lgetxattr(); 
extern int  yaffs_link(); 
extern int  yaffs_link_fixup(); 
extern int  yaffs_link_obj(); 
extern int  yaffs_list_xattrib(); 
extern int  yaffs_listxattr(); 
extern int  yaffs_llistxattr(); 
extern int  yaffs_load_attribs(); 
extern int  yaffs_load_attribs_oh(); 
extern int  yaffs_load_current_time(); 
extern int  yaffs_lost_n_found(); 
extern int  yaffs_lremovexattr(); 
extern int  yaffs_lseek(); 
extern int  yaffs_lsetxattr(); 
extern int  yaffs_lstat(); 
extern int  yaffs_mark_bad(); 
extern int  yaffs_max_file_size(); 
extern int  yaffs_mkdir(); 
extern int  yaffs_mknod(); 
extern int  yaffs_mount(); 
extern int  yaffs_mount2(); 
extern int  yaffs_mount_common(); 
extern int  yaffs_mtd_drv_install(); 
extern int  yaffs_n_handles(); 
extern int  yaffs_next_dev(); 
extern int  yaffs_oh_size_load(); 
extern int  yaffs_oh_to_size(); 
extern int  yaffs_open(); 
extern int  yaffs_open_sharing(); 
extern int  yaffs_opendir(); 
extern int  yaffs_pack_tags1(); 
extern int  yaffs_pack_tags2(); 
extern int  yaffs_pack_tags2_tags_only(); 
extern int  yaffs_pread(); 
extern int  yaffs_put_chunk_in_file(); 
extern int  yaffs_put_mtd_device(); 
extern int  yaffs_pwrite(); 
extern int  yaffs_qsort(); 
extern int  yaffs_query_init_block_state(); 
extern int  yaffs_rd_chunk_tags_nand(); 
extern int  yaffs_read(); 
extern int  yaffs_readdir(); 
extern int  yaffs_readlink(); 
extern int  yaffs_release_temp_buffer(); 
extern int  yaffs_remount(); 
extern int  yaffs_remove_device(); 
extern int  yaffs_remove_xattrib(); 
extern int  yaffs_removexattr(); 
extern int  yaffs_rename(); 
extern int  yaffs_rename_obj(); 
extern int  yaffs_resize_file(); 
extern int  yaffs_resize_file_down(); 
extern int  yaffs_rewinddir(); 
extern int  yaffs_rmdir(); 
extern int  yaffs_root(); 
extern int  yaffs_set_attribs(); 
extern int  yaffs_set_chunk_bit(); 
extern int  yaffs_set_error(); 
extern int  yaffs_set_obj_name(); 
extern int  yaffs_set_obj_name_from_oh(); 
extern int  yaffs_set_trace(); 
extern int  yaffs_set_xattrib(); 
extern int  yaffs_setxattr(); 
extern int  yaffs_skip_rest_of_block(); 
extern int  yaffs_skip_verification(); 
extern int  yaffs_stat(); 
extern int  yaffs_still_some_chunks(); 
extern int  yaffs_summary_add(); 
extern int  yaffs_summary_deinit(); 
extern int  yaffs_summary_fetch(); 
extern int  yaffs_summary_gc(); 
extern int  yaffs_summary_init(); 
extern int  yaffs_summary_read(); 
extern int  yaffs_symlink(); 
extern int  yaffs_sync(); 
extern int  yaffs_tags_compat_install(); 
extern int  yaffs_tags_marshall_install(); 
extern int  yaffs_totalspace(); 
extern int  yaffs_truncate(); 
extern int  yaffs_unlink(); 
extern int  yaffs_unlinker(); 
extern int  yaffs_unmount(); 
extern int  yaffs_unmount2(); 
extern int  yaffs_unpack_tags1(); 
extern int  yaffs_unpack_tags2(); 
extern int  yaffs_unpack_tags2_tags_only(); 
extern int  yaffs_update_dirty_dirs(); 
extern int  yaffs_update_oh(); 
extern int  yaffs_utime(); 
extern int  yaffs_verify_blk(); 
extern int  yaffs_verify_blocks(); 
extern int  yaffs_verify_chunk_bit_id(); 
extern int  yaffs_verify_collected_blk(); 
extern int  yaffs_verify_dir(); 
extern int  yaffs_verify_file(); 
extern int  yaffs_verify_file_sane(); 
extern int  yaffs_verify_free_chunks(); 
extern int  yaffs_verify_link(); 
extern int  yaffs_verify_mtd(); 
extern int  yaffs_verify_obj(); 
extern int  yaffs_verify_obj_in_dir(); 
extern int  yaffs_verify_objects(); 
extern int  yaffs_verify_oh(); 
extern int  yaffs_verify_special(); 
extern int  yaffs_verify_symlink(); 
extern int  yaffs_wr_chunk_tags_nand(); 
extern int  yaffs_wr_file(); 
extern int  yaffs_write(); 
extern int  yaffsfs_CheckMemRegion(); 
extern int  yaffsfs_CurrentTime(); 
extern int  yaffsfs_FindDevice(); 
extern int  yaffsfs_GetLastError(); 
extern int  yaffsfs_Init(); 
extern int  yaffsfs_Lock(); 
extern int  yaffsfs_Mount(); 
extern int  yaffsfs_SetError(); 
extern int  yaffsfs_Unlock(); 
extern int  yaffsfs_Unmount(); 
extern int  zero_init(); 
extern int  __gnu_thumb1_case_sqi(); 
extern int  __gnu_thumb1_case_uqi(); 
extern int  __gnu_thumb1_case_shi(); 
extern int  __gnu_thumb1_case_uhi(); 
extern int  __gnu_thumb1_case_si(); 
extern int  __aeabi_uidiv(); 
extern int  __aeabi_uidivmod(); 
extern int  __udivsi3(); 
extern int  __aeabi_idiv(); 
extern int  __aeabi_idivmod(); 
extern int  __divsi3(); 
extern int  __umodsi3(); 
extern int  __modsi3(); 
extern int  _call_via_fp(); 
extern int  _call_via_ip(); 
extern int  _call_via_lr(); 
extern int  _call_via_r0(); 
extern int  _call_via_r1(); 
extern int  _call_via_r2(); 
extern int  _call_via_r3(); 
extern int  _call_via_r4(); 
extern int  _call_via_r5(); 
extern int  _call_via_r6(); 
extern int  _call_via_r7(); 
extern int  _call_via_r8(); 
extern int  _call_via_r9(); 
extern int  _call_via_sl(); 
extern int  _call_via_sp(); 
extern int  _arm_return(); 
extern int  _arm_return_r11(); 
extern int  _arm_return_r7(); 
extern int  _interwork_call_via_fp(); 
extern int  _interwork_call_via_ip(); 
extern int  _interwork_call_via_lr(); 
extern int  _interwork_call_via_r0(); 
extern int  _interwork_call_via_r1(); 
extern int  _interwork_call_via_r2(); 
extern int  _interwork_call_via_r3(); 
extern int  _interwork_call_via_r4(); 
extern int  _interwork_call_via_r5(); 
extern int  _interwork_call_via_r6(); 
extern int  _interwork_call_via_r7(); 
extern int  _interwork_call_via_r8(); 
extern int  _interwork_call_via_r9(); 
extern int  _interwork_call_via_sl(); 
extern int  _interwork_call_via_sp(); 
extern int  _interwork_r11_call_via_fp(); 
extern int  _interwork_r11_call_via_ip(); 
extern int  _interwork_r11_call_via_r0(); 
extern int  _interwork_r11_call_via_r1(); 
extern int  _interwork_r11_call_via_r2(); 
extern int  _interwork_r11_call_via_r3(); 
extern int  _interwork_r11_call_via_r4(); 
extern int  _interwork_r11_call_via_r5(); 
extern int  _interwork_r11_call_via_r6(); 
extern int  _interwork_r11_call_via_r7(); 
extern int  _interwork_r11_call_via_r8(); 
extern int  _interwork_r11_call_via_r9(); 
extern int  _interwork_r11_call_via_sl(); 
extern int  _interwork_r11_call_via_sp(); 
extern int  _interwork_r7_call_via_fp(); 
extern int  _interwork_r7_call_via_ip(); 
extern int  _interwork_r7_call_via_r0(); 
extern int  _interwork_r7_call_via_r1(); 
extern int  _interwork_r7_call_via_r2(); 
extern int  _interwork_r7_call_via_r3(); 
extern int  _interwork_r7_call_via_r4(); 
extern int  _interwork_r7_call_via_r5(); 
extern int  _interwork_r7_call_via_r6(); 
extern int  _interwork_r7_call_via_r7(); 
extern int  _interwork_r7_call_via_r8(); 
extern int  _interwork_r7_call_via_r9(); 
extern int  _interwork_r7_call_via_sl(); 
extern int  _interwork_r7_call_via_sp(); 
extern int  __aeabi_llsr(); 
extern int  __lshrdi3(); 
extern int  __aeabi_lasr(); 
extern int  __ashrdi3(); 
extern int  __aeabi_llsl(); 
extern int  __ashldi3(); 
extern int  __aeabi_dneg(); 
extern int  __negdf2(); 
extern int  __adddf3(); 
extern int  __aeabi_dadd(); 
extern int  __aeabi_drsub(); 
extern int  __aeabi_dsub(); 
extern int  __aeabi_f2d(); 
extern int  __aeabi_i2d(); 
extern int  __aeabi_l2d(); 
extern int  __aeabi_ui2d(); 
extern int  __aeabi_ul2d(); 
extern int  __extendsfdf2(); 
extern int  __floatdidf(); 
extern int  __floatsidf(); 
extern int  __floatundidf(); 
extern int  __floatunsidf(); 
extern int  __subdf3(); 
extern int  __aeabi_ddiv(); 
extern int  __aeabi_dmul(); 
extern int  __divdf3(); 
extern int  __muldf3(); 
extern int  __aeabi_cdcmpeq(); 
extern int  __aeabi_cdcmple(); 
extern int  __aeabi_cdrcmple(); 
extern int  __aeabi_dcmpeq(); 
extern int  __aeabi_dcmpge(); 
extern int  __aeabi_dcmpgt(); 
extern int  __aeabi_dcmple(); 
extern int  __aeabi_dcmplt(); 
extern int  __cmpdf2(); 
extern int  __eqdf2(); 
extern int  __gedf2(); 
extern int  __gtdf2(); 
extern int  __ledf2(); 
extern int  __ltdf2(); 
extern int  __nedf2(); 
extern int  __aeabi_dcmpun(); 
extern int  __unorddf2(); 
extern int  __aeabi_d2iz(); 
extern int  __fixdfsi(); 
extern int  __aeabi_d2uiz(); 
extern int  __fixunsdfsi(); 
extern int  __aeabi_d2f(); 
extern int  __truncdfsf2(); 
extern int  __aeabi_fneg(); 
extern int  __negsf2(); 
extern int  __addsf3(); 
extern int  __aeabi_fadd(); 
extern int  __aeabi_frsub(); 
extern int  __aeabi_fsub(); 
extern int  __aeabi_i2f(); 
extern int  __aeabi_l2f(); 
extern int  __aeabi_ui2f(); 
extern int  __aeabi_ul2f(); 
extern int  __floatdisf(); 
extern int  __floatsisf(); 
extern int  __floatundisf(); 
extern int  __floatunsisf(); 
extern int  __subsf3(); 
extern int  __aeabi_fdiv(); 
extern int  __aeabi_fmul(); 
extern int  __divsf3(); 
extern int  __mulsf3(); 
extern int  __aeabi_cfcmpeq(); 
extern int  __aeabi_cfcmple(); 
extern int  __aeabi_cfrcmple(); 
extern int  __aeabi_fcmpeq(); 
extern int  __aeabi_fcmpge(); 
extern int  __aeabi_fcmpgt(); 
extern int  __aeabi_fcmple(); 
extern int  __aeabi_fcmplt(); 
extern int  __cmpsf2(); 
extern int  __eqsf2(); 
extern int  __gesf2(); 
extern int  __gtsf2(); 
extern int  __lesf2(); 
extern int  __ltsf2(); 
extern int  __nesf2(); 
extern int  __aeabi_fcmpun(); 
extern int  __unordsf2(); 
extern int  __aeabi_f2iz(); 
extern int  __fixsfsi(); 
extern int  __aeabi_f2uiz(); 
extern int  __fixunssfsi(); 
extern int  __clzsi2(); 
extern int  __clzdi2(); 
extern int  __aeabi_lcmp(); 
extern int  __aeabi_ulcmp(); 
extern int  __aeabi_ldivmod(); 
extern int  __aeabi_uldivmod(); 
extern int  __aeabi_lmul(); 
extern int  __muldi3(); 
extern int  __negdi2(); 
extern int  __cmpdi2(); 
extern int  __ucmpdi2(); 
extern int  __clear_cache(); 
extern int  __enable_execute_stack(); 
extern int  __absvsi2(); 
extern int  __absvdi2(); 
extern int  __addvsi3(); 
extern int  __addvdi3(); 
extern int  __subvsi3(); 
extern int  __subvdi3(); 
extern int  __mulvsi3(); 
extern int  __mulvdi3(); 
extern int  __negvsi2(); 
extern int  __negvdi2(); 
extern int  __ffssi2(); 
extern int  __ffsdi2(); 
extern int  __ctzsi2(); 
extern int  __ctzdi2(); 
extern int  __popcountsi2(); 
extern int  __popcountdi2(); 
extern int  __paritysi2(); 
extern int  __paritydi2(); 
extern int  __powisf2(); 
extern int  __powidf2(); 
extern int  __mulsc3(); 
extern int  __muldc3(); 
extern int  __divsc3(); 
extern int  __divdc3(); 
extern int  __bswapsi2(); 
extern int  __bswapdi2(); 
extern int  __aeabi_f2lz(); 
extern int  __fixsfdi(); 
extern int  __aeabi_d2lz(); 
extern int  __fixdfdi(); 
extern int  __aeabi_f2ulz(); 
extern int  __fixunssfdi(); 
extern int  __aeabi_d2ulz(); 
extern int  __fixunsdfdi(); 
extern int  __eprintf(); 
extern int  __gcc_bcmp(); 
extern int  __divdi3(); 
extern int  __moddi3(); 
extern int  __udivdi3(); 
extern int  __umoddi3(); 
extern int  __udiv_w_sdiv(); 
extern int  __udivmoddi4(); 
extern int  __gnu_ldivmod_helper(); 
extern int  __gnu_uldivmod_helper(); 
extern int  __aeabi_uread4(); 
extern int  __aeabi_uread8(); 
extern int  __aeabi_uwrite4(); 
extern int  __aeabi_uwrite8(); 
extern int  __gnu_f2h_alternative(); 
extern int  __gnu_f2h_ieee(); 
extern int  __gnu_h2f_alternative(); 
extern int  __gnu_h2f_ieee(); 
extern int  __gnu_h2f_internal(); 
extern int  _Unwind_Complete(); 
extern int  _Unwind_DeleteException(); 
extern int  _Unwind_GetCFA(); 
extern int  _Unwind_VRS_Get(); 
extern int  _Unwind_VRS_Pop(); 
extern int  _Unwind_VRS_Set(); 
extern int  __aeabi_unwind_cpp_pr0(); 
extern int  __gnu_Unwind_Backtrace(); 
extern int  __gnu_Unwind_ForcedUnwind(); 
extern int  __gnu_Unwind_RaiseException(); 
extern int  __gnu_Unwind_Resume(); 
extern int  __gnu_Unwind_Resume_or_Rethrow(); 
extern int  _Unwind_Backtrace(); 
extern int  _Unwind_ForcedUnwind(); 
extern int  _Unwind_RaiseException(); 
extern int  _Unwind_Resume(); 
extern int  _Unwind_Resume_or_Rethrow(); 
extern int  ___Unwind_Backtrace(); 
extern int  ___Unwind_ForcedUnwind(); 
extern int  ___Unwind_RaiseException(); 
extern int  ___Unwind_Resume(); 
extern int  ___Unwind_Resume_or_Rethrow(); 
extern int  __gnu_Unwind_Restore_VFP(); 
extern int  __gnu_Unwind_Restore_VFP_D(); 
extern int  __gnu_Unwind_Restore_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Restore_WMMXC(); 
extern int  __gnu_Unwind_Restore_WMMXD(); 
extern int  __gnu_Unwind_Save_VFP(); 
extern int  __gnu_Unwind_Save_VFP_D(); 
extern int  __gnu_Unwind_Save_VFP_D_16_to_31(); 
extern int  __gnu_Unwind_Save_WMMXC(); 
extern int  __gnu_Unwind_Save_WMMXD(); 
extern int  __restore_core_regs(); 
extern int  restore_core_regs(); 
extern int  _Unwind_GetDataRelBase(); 
extern int  _Unwind_GetLanguageSpecificData(); 
extern int  _Unwind_GetRegionStart(); 
extern int  _Unwind_GetTextRelBase(); 
extern int  __gnu_unwind_execute(); 
extern int  __gnu_unwind_frame(); 
extern int  __gcc_personality_v0(); 
extern int  __emutls_get_address(); 
extern int  __emutls_register_common(); 
extern int  acoshl(); 
extern int  acosl(); 
extern int  asinhl(); 
extern int  asinl(); 
extern int  atan2l(); 
extern int  atanhl(); 
extern int  atanl(); 
extern int  cabs(); 
extern int  cabsf(); 
extern int  cacos(); 
extern int  cacosf(); 
extern int  cacosh(); 
extern int  cacoshf(); 
extern int  carg(); 
extern int  cargf(); 
extern int  casin(); 
extern int  casinf(); 
extern int  casinh(); 
extern int  casinhf(); 
extern int  catan(); 
extern int  catanf(); 
extern int  catanh(); 
extern int  catanhf(); 
extern int  cbrtl(); 
extern int  ccos(); 
extern int  ccosf(); 
extern int  ccosh(); 
extern int  ccoshf(); 
extern int  ceill(); 
extern int  _cchsh(); 
extern int  _ctans(); 
extern int  _redupi(); 
extern int  _cchshf(); 
extern int  _ctansf(); 
extern int  _redupif(); 
extern int  cexp(); 
extern int  cexpf(); 
extern int  cimag(); 
extern int  cimagf(); 
extern int  clog(); 
extern int  clogf(); 
extern int  conj(); 
extern int  conjf(); 
extern int  copysignl(); 
extern int  coshl(); 
extern int  cosl(); 
extern int  cpow(); 
extern int  cpowf(); 
extern int  cproj(); 
extern int  cprojf(); 
extern int  creal(); 
extern int  crealf(); 
extern int  csin(); 
extern int  csinf(); 
extern int  csinh(); 
extern int  csinhf(); 
extern int  csqrt(); 
extern int  csqrtf(); 
extern int  ctan(); 
extern int  ctanf(); 
extern int  ctanh(); 
extern int  ctanhf(); 
extern int  __ieee754_acos(); 
extern int  __ieee754_acosh(); 
extern int  __ieee754_asin(); 
extern int  __ieee754_atan2(); 
extern int  __ieee754_atanh(); 
extern int  __ieee754_cosh(); 
extern int  __ieee754_exp(); 
extern int  __ieee754_fmod(); 
extern int  __ieee754_hypot(); 
extern int  __ieee754_j0(); 
extern int  __ieee754_y0(); 
extern int  __ieee754_j1(); 
extern int  __ieee754_y1(); 
extern int  __ieee754_jn(); 
extern int  __ieee754_yn(); 
extern int  __ieee754_log(); 
extern int  __ieee754_log10(); 
extern int  __ieee754_pow(); 
extern int  __ieee754_rem_pio2(); 
extern int  __ieee754_remainder(); 
extern int  __ieee754_scalb(); 
extern int  __ieee754_sinh(); 
extern int  __ieee754_sqrt(); 
extern int  __ieee754_acosf(); 
extern int  __ieee754_acoshf(); 
extern int  __ieee754_asinf(); 
extern int  __ieee754_atan2f(); 
extern int  __ieee754_atanhf(); 
extern int  __ieee754_coshf(); 
extern int  __ieee754_expf(); 
extern int  __ieee754_fmodf(); 
extern int  __ieee754_hypotf(); 
extern int  __ieee754_j0f(); 
extern int  __ieee754_y0f(); 
extern int  __ieee754_j1f(); 
extern int  __ieee754_y1f(); 
extern int  __ieee754_jnf(); 
extern int  __ieee754_ynf(); 
extern int  __ieee754_logf(); 
extern int  __ieee754_log10f(); 
extern int  __ieee754_powf(); 
extern int  __ieee754_rem_pio2f(); 
extern int  __ieee754_remainderf(); 
extern int  __ieee754_scalbf(); 
extern int  __ieee754_sinhf(); 
extern int  __ieee754_sqrtf(); 
extern int  __ieee754_gamma_r(); 
extern int  __ieee754_lgamma_r(); 
extern int  __ieee754_gammaf_r(); 
extern int  __ieee754_lgammaf_r(); 
extern int  erfcl(); 
extern int  erfl(); 
extern int  exp2l(); 
extern int  expl(); 
extern int  expm1l(); 
extern int  fabsl(); 
extern int  fdiml(); 
extern int  floorl(); 
extern int  fmal(); 
extern int  fmaxl(); 
extern int  fminl(); 
extern int  fmodl(); 
extern int  frexpl(); 
extern int  hypotl(); 
extern int  ilogbl(); 
extern int  __kernel_cos(); 
extern int  __kernel_rem_pio2(); 
extern int  __kernel_sin(); 
extern int  __kernel_standard(); 
extern int  __kernel_tan(); 
extern int  __kernel_cosf(); 
extern int  __kernel_rem_pio2f(); 
extern int  __kernel_sinf(); 
extern int  __kernel_tanf(); 
extern int  ldexpl(); 
extern int  lgammal(); 
extern int  llrintl(); 
extern int  llroundl(); 
extern int  log10l(); 
extern int  log1pl(); 
extern int  logl(); 
extern int  lrintl(); 
extern int  lroundl(); 
extern int  modfl(); 
extern int  nanl(); 
extern int  nearbyintl(); 
extern int  nextafterl(); 
extern int  powl(); 
extern int  remainderl(); 
extern int  remquol(); 
extern int  rintl(); 
extern int  roundl(); 
extern int  asinh(); 
extern int  atan(); 
extern int  cbrt(); 
extern int  ceil(); 
extern int  copysign(); 
extern int  cos(); 
extern int  erf(); 
extern int  erfc(); 
extern int  exp10(); 
extern int  expm1(); 
extern int  fabs(); 
extern int  fdim(); 
extern int  finite(); 
extern int  floor(); 
extern int  fma(); 
extern int  fmax(); 
extern int  fmin(); 
extern int  __fpclassifyd(); 
extern int  frexp(); 
extern int  ilogb(); 
extern int  infinity(); 
extern int  isinf(); 
extern int  __isinfd(); 
extern int  isnan(); 
extern int  __isnand(); 
extern int  ldexp(); 
extern int  llrint(); 
extern int  llround(); 
extern int  log1p(); 
extern int  log2(); 
extern int  logb(); 
extern int  lrint(); 
extern int  lround(); 
extern int  matherr(); 
extern int  modf(); 
extern int  nan(); 
extern int  nearbyint(); 
extern int  nextafter(); 
extern int  pow10(); 
extern int  remquo(); 
extern int  rint(); 
extern int  round(); 
extern int  scalbln(); 
extern int  scalbn(); 
extern int  __signbitd(); 
extern int  __signbitf(); 
extern int  significand(); 
extern int  sin(); 
extern int  tan(); 
extern int  tanh(); 
extern int  trunc(); 
extern int  scalblnl(); 
extern int  scalbnl(); 
extern int  asinhf(); 
extern int  atanf(); 
extern int  cbrtf(); 
extern int  ceilf(); 
extern int  copysignf(); 
extern int  cosf(); 
extern int  erfcf(); 
extern int  erff(); 
extern int  exp10f(); 
extern int  expm1f(); 
extern int  fabsf(); 
extern int  fdimf(); 
extern int  finitef(); 
extern int  floorf(); 
extern int  fmaf(); 
extern int  fmaxf(); 
extern int  fminf(); 
extern int  __fpclassifyf(); 
extern int  frexpf(); 
extern int  ilogbf(); 
extern int  infinityf(); 
extern int  isinff(); 
extern int  __isinff(); 
extern int  isnanf(); 
extern int  __isnanf(); 
extern int  ldexpf(); 
extern int  llrintf(); 
extern int  llroundf(); 
extern int  log1pf(); 
extern int  log2f(); 
extern int  logbf(); 
extern int  lrintf(); 
extern int  lroundf(); 
extern int  modff(); 
extern int  nanf(); 
extern int  nearbyintf(); 
extern int  nextafterf(); 
extern int  pow10f(); 
extern int  remquof(); 
extern int  rintf(); 
extern int  roundf(); 
extern int  scalblnf(); 
extern int  scalbnf(); 
extern int  significandf(); 
extern int  sinf(); 
extern int  tanf(); 
extern int  tanhf(); 
extern int  truncf(); 
extern int  sinhl(); 
extern int  sinl(); 
extern int  sqrtl(); 
extern int  tanhl(); 
extern int  tanl(); 
extern int  tgammal(); 
extern int  truncl(); 
extern int  acos(); 
extern int  acosh(); 
extern int  asin(); 
extern int  atan2(); 
extern int  atanh(); 
extern int  cosh(); 
extern int  drem(); 
extern int  exp(); 
extern int  exp2(); 
extern int  fmod(); 
extern int  gamma(); 
extern int  hypot(); 
extern int  j0(); 
extern int  y0(); 
extern int  j1(); 
extern int  y1(); 
extern int  jn(); 
extern int  yn(); 
extern int  lgamma(); 
extern int  log(); 
extern int  log10(); 
extern int  pow(); 
extern int  remainder(); 
extern int  scalb(); 
extern int  sincos(); 
extern int  sinh(); 
extern int  sqrt(); 
extern int  tgamma(); 
extern int  acosf(); 
extern int  acoshf(); 
extern int  asinf(); 
extern int  atan2f(); 
extern int  atanhf(); 
extern int  coshf(); 
extern int  dremf(); 
extern int  expf(); 
extern int  exp2f(); 
extern int  fmodf(); 
extern int  gammaf(); 
extern int  hypotf(); 
extern int  j0f(); 
extern int  y0f(); 
extern int  j1f(); 
extern int  y1f(); 
extern int  jnf(); 
extern int  ynf(); 
extern int  lgammaf(); 
extern int  logf(); 
extern int  log10f(); 
extern int  powf(); 
extern int  remainderf(); 
extern int  scalbf(); 
extern int  sincosf(); 
extern int  sinhf(); 
extern int  sqrtf(); 
extern int  tgammaf(); 
extern int  gamma_r(); 
extern int  lgamma_r(); 
extern int  gammaf_r(); 
extern int  lgammaf_r(); 
extern int  __adjust(); 
extern int  __register_exitproc(); 
extern int  __call_exitprocs(); 
extern int  __dprintf(); 
extern int  __exp10(); 
extern int  __ten_mul(); 
extern int  _Exit(); 
extern int  a64l(); 
extern int  abort(); 
extern int  abs(); 
extern int  access(); 
extern int  __aeabi_atexit(); 
extern int  argz_add(); 
extern int  argz_add_sep(); 
extern int  argz_append(); 
extern int  argz_count(); 
extern int  argz_create(); 
extern int  argz_create_sep(); 
extern int  argz_delete(); 
extern int  argz_extract(); 
extern int  argz_insert(); 
extern int  argz_next(); 
extern int  argz_replace(); 
extern int  argz_stringify(); 
extern int  asctime(); 
extern int  asctime_r(); 
extern int  _asiprintf_r(); 
extern int  asiprintf(); 
extern int  _asniprintf_r(); 
extern int  asniprintf(); 
extern int  _asnprintf_r(); 
extern int  asnprintf(); 
extern int  _asprintf_r(); 
extern int  asprintf(); 
extern int  __assert(); 
extern int  __assert_func(); 
extern int  atexit(); 
extern int  atof(); 
extern int  atoff(); 
extern int  _atoi_r(); 
extern int  atoi(); 
extern int  _atol_r(); 
extern int  atol(); 
extern int  _atoll_r(); 
extern int  atoll(); 
extern int  bcmp(); 
extern int  bcopy(); 
extern int  bsearch(); 
extern int  btowc(); 
extern int  _buf_findstr(); 
extern int  bzero(); 
extern int  calloc(); 
extern int  _calloc_r(); 
extern int  cfree(); 
extern int  clearerr(); 
extern int  clock(); 
extern int  _close_r(); 
extern int  ctime(); 
extern int  ctime_r(); 
extern int  __cxa_atexit(); 
extern int  __cxa_finalize(); 
extern int  difftime(); 
extern int  _diprintf_r(); 
extern int  diprintf(); 
extern int  div(); 
extern int  _dprintf_r(); 
extern int  dprintf(); 
extern int  _drand48_r(); 
extern int  drand48(); 
extern int  _dtoa_r(); 
extern int  __dtoa(); 
extern int  _dcvt(); 
extern int  _gcvt(); 
extern int  ecvtbuf(); 
extern int  fcvtbuf(); 
extern int  ecvt(); 
extern int  ecvtf(); 
extern int  fcvt(); 
extern int  fcvtf(); 
extern int  gcvt(); 
extern int  gcvtf(); 
extern int  __env_lock(); 
extern int  __env_unlock(); 
extern int  envz_add(); 
extern int  envz_entry(); 
extern int  envz_get(); 
extern int  envz_merge(); 
extern int  envz_remove(); 
extern int  envz_strip(); 
extern int  __eprintf(); 
extern int  _erand48_r(); 
extern int  erand48(); 
extern int  __errno(); 
extern int  _execve_r(); 
extern int  _fork_r(); 
extern int  _wait_r(); 
extern int  exit(); 
extern int  _fclose_r(); 
extern int  fclose(); 
extern int  _fcloseall_r(); 
extern int  fcloseall(); 
extern int  _fdopen_r(); 
extern int  fdopen(); 
extern int  feof(); 
extern int  ferror(); 
extern int  _fflush_r(); 
extern int  fflush(); 
extern int  ffs(); 
extern int  _fgetc_r(); 
extern int  fgetc(); 
extern int  _fgetpos_r(); 
extern int  fgetpos(); 
extern int  _fgets_r(); 
extern int  fgets(); 
extern int  _fgetwc_r(); 
extern int  fgetwc(); 
extern int  _fgetws_r(); 
extern int  fgetws(); 
extern int  fileno(); 
extern int  __fp_lock_all(); 
extern int  __fp_unlock_all(); 
extern int  __sfmoreglue(); 
extern int  __sfp(); 
extern int  __sfp_lock_acquire(); 
extern int  __sfp_lock_release(); 
extern int  __sinit(); 
extern int  __sinit_lock_acquire(); 
extern int  __sinit_lock_release(); 
extern int  _cleanup(); 
extern int  _cleanup_r(); 
extern int  __libc_fini_array(); 
extern int  _fiprintf_r(); 
extern int  fiprintf(); 
extern int  _fiscanf_r(); 
extern int  fiscanf(); 
extern int  __fix_locale_grouping_str(); 
extern int  __sflags(); 
extern int  _fmemopen_r(); 
extern int  fmemopen(); 
extern int  _fopen_r(); 
extern int  fopen(); 
extern int  _fopencookie_r(); 
extern int  fopencookie(); 
extern int  _fprintf_r(); 
extern int  fprintf(); 
extern int  _fpurge_r(); 
extern int  fpurge(); 
extern int  _fputc_r(); 
extern int  fputc(); 
extern int  _fputs_r(); 
extern int  fputs(); 
extern int  _fputwc_r(); 
extern int  fputwc(); 
extern int  _fputws_r(); 
extern int  fputws(); 
extern int  _fread_r(); 
extern int  fread(); 
extern int  _freopen_r(); 
extern int  freopen(); 
extern int  _fscanf_r(); 
extern int  fscanf(); 
extern int  _fseek_r(); 
extern int  fseek(); 
extern int  _fseeko_r(); 
extern int  fseeko(); 
extern int  _fsetpos_r(); 
extern int  fsetpos(); 
extern int  _fstat_r(); 
extern int  _ftell_r(); 
extern int  ftell(); 
extern int  _ftello_r(); 
extern int  ftello(); 
extern int  _funopen_r(); 
extern int  funopen(); 
extern int  __sfvwrite_r(); 
extern int  _fwalk(); 
extern int  _fwalk_reent(); 
extern int  _fwide_r(); 
extern int  fwide(); 
extern int  _fwprintf_r(); 
extern int  fwprintf(); 
extern int  _fwrite_r(); 
extern int  fwrite(); 
extern int  _fwscanf_r(); 
extern int  fwscanf(); 
extern int  __gethex(); 
extern int  __hexdig_init(); 
extern int  __hexnan(); 
extern int  _getc_r(); 
extern int  getc(); 
extern int  _getc_unlocked_r(); 
extern int  getc_unlocked(); 
extern int  _getchar_r(); 
extern int  getchar(); 
extern int  _getchar_unlocked_r(); 
extern int  getchar_unlocked(); 
extern int  __getdelim(); 
extern int  _findenv(); 
extern int  getenv(); 
extern int  _findenv_r(); 
extern int  _getenv_r(); 
extern int  __getline(); 
extern int  __getopt_long_only_r(); 
extern int  __getopt_long_r(); 
extern int  __getopt_r(); 
extern int  getopt(); 
extern int  getopt_long(); 
extern int  getopt_long_only(); 
extern int  __getreent(); 
extern int  _gets_r(); 
extern int  gets(); 
extern int  getsubopt(); 
extern int  _gettimeofday_r(); 
extern int  __gettzinfo(); 
extern int  getw(); 
extern int  _getwc_r(); 
extern int  getwc(); 
extern int  _getwchar_r(); 
extern int  getwchar(); 
extern int  gmtime(); 
extern int  gmtime_r(); 
extern int  __call_hash(); 
extern int  __expand_table(); 
extern int  __hash_open(); 
extern int  __big_delete(); 
extern int  __big_insert(); 
extern int  __big_keydata(); 
extern int  __big_return(); 
extern int  __big_split(); 
extern int  __find_bigpair(); 
extern int  __find_last_page(); 
extern int  __buf_free(); 
extern int  __buf_init(); 
extern int  __get_buf(); 
extern int  __reclaim_buf(); 
extern int  __log2(); 
extern int  __add_ovflpage(); 
extern int  __addel(); 
extern int  __delpair(); 
extern int  __free_ovflpage(); 
extern int  __get_page(); 
extern int  __ibitmap(); 
extern int  __put_page(); 
extern int  __split_page(); 
extern int  hcreate(); 
extern int  hdestroy(); 
extern int  hsearch(); 
extern int  hcreate_r(); 
extern int  hdestroy_r(); 
extern int  hsearch_r(); 
extern int  index(); 
extern int  __libc_init_array(); 
extern int  _iprintf_r(); 
extern int  iprintf(); 
extern int  isalnum(); 
extern int  isalpha(); 
extern int  isascii(); 
extern int  _isatty_r(); 
extern int  isblank(); 
extern int  _iscanf_r(); 
extern int  iscanf(); 
extern int  iscntrl(); 
extern int  isdigit(); 
extern int  islower(); 
extern int  isgraph(); 
extern int  isprint(); 
extern int  ispunct(); 
extern int  isspace(); 
extern int  isupper(); 
extern int  iswalnum(); 
extern int  iswalpha(); 
extern int  iswblank(); 
extern int  iswcntrl(); 
extern int  iswctype(); 
extern int  iswdigit(); 
extern int  iswgraph(); 
extern int  iswlower(); 
extern int  iswprint(); 
extern int  iswpunct(); 
extern int  iswspace(); 
extern int  iswupper(); 
extern int  iswxdigit(); 
extern int  isxdigit(); 
extern int  _jrand48_r(); 
extern int  jrand48(); 
extern int  _l64a_r(); 
extern int  l64a(); 
extern int  labs(); 
extern int  localtime(); 
extern int  localtime_r(); 
extern int  _lcong48_r(); 
extern int  lcong48(); 
extern int  __ctype_load_locale(); 
extern int  __get_current_ctype_locale(); 
extern int  ldiv(); 
extern int  __part_load_locale(); 
extern int  _ldcheck(); 
extern int  _ldtoa_r(); 
extern int  _strtold(); 
extern int  _link_r(); 
extern int  llabs(); 
extern int  lldiv(); 
extern int  __get_current_messages_locale(); 
extern int  __messages_load_locale(); 
extern int  __get_current_monetary_locale(); 
extern int  __monetary_load_locale(); 
extern int  __get_current_numeric_locale(); 
extern int  __numeric_load_locale(); 
extern int  __locale_charset(); 
extern int  __locale_cjk_lang(); 
extern int  __locale_mb_cur_max(); 
extern int  __locale_msgcharset(); 
extern int  _localeconv_r(); 
extern int  _setlocale_r(); 
extern int  localeconv(); 
extern int  setlocale(); 
extern int  _lrand48_r(); 
extern int  lrand48(); 
extern int  _lseek_r(); 
extern int  __smakebuf_r(); 
extern int  memalign(); 
extern int  _memalign_r(); 
extern int  free(); 
extern int  malloc(); 
extern int  mblen(); 
extern int  _mblen_r(); 
extern int  mbrlen(); 
extern int  _mbrtowc_r(); 
extern int  mbrtowc(); 
extern int  mbsinit(); 
extern int  _mbsnrtowcs_r(); 
extern int  mbsnrtowcs(); 
extern int  _mbsrtowcs_r(); 
extern int  mbsrtowcs(); 
extern int  mbstowcs(); 
extern int  _mbstowcs_r(); 
extern int  mbtowc(); 
extern int  __ascii_mbtowc(); 
extern int  _mbtowc_r(); 
extern int  memccpy(); 
extern int  memchr(); 
extern int  memcmp(); 
extern int  memcpy(); 
extern int  memmem(); 
extern int  memmove(); 
extern int  mempcpy(); 
extern int  memset(); 
extern int  _mkdir_r(); 
extern int  _mkdtemp_r(); 
extern int  _mkostemp_r(); 
extern int  _mkostemps_r(); 
extern int  _mkstemp_r(); 
extern int  _mkstemps_r(); 
extern int  _mktemp_r(); 
extern int  mkdtemp(); 
extern int  mkostemp(); 
extern int  mkostemps(); 
extern int  mkstemp(); 
extern int  mkstemps(); 
extern int  mktemp(); 
extern int  mktime(); 
extern int  __tzcalc_limits(); 
extern int  _mktm_r(); 
extern int  __malloc_lock(); 
extern int  __malloc_unlock(); 
extern int  _Balloc(); 
extern int  _Bfree(); 
extern int  __any_on(); 
extern int  __b2d(); 
extern int  __copybits(); 
extern int  __d2b(); 
extern int  __hi0bits(); 
extern int  __i2b(); 
extern int  __lo0bits(); 
extern int  __lshift(); 
extern int  __mcmp(); 
extern int  __mdiff(); 
extern int  __multadd(); 
extern int  __multiply(); 
extern int  __pow5mult(); 
extern int  __ratio(); 
extern int  __s2b(); 
extern int  __ulp(); 
extern int  _mprec_log10(); 
extern int  _mrand48_r(); 
extern int  mrand48(); 
extern int  malloc_usable_size(); 
extern int  _malloc_usable_size_r(); 
extern int  nl_langinfo(); 
extern int  _nrand48_r(); 
extern int  nrand48(); 
extern int  on_exit(); 
extern int  _open_memstream_r(); 
extern int  _open_wmemstream_r(); 
extern int  open_memstream(); 
extern int  open_wmemstream(); 
extern int  _open_r(); 
extern int  _perror_r(); 
extern int  perror(); 
extern int  _printf_r(); 
extern int  printf(); 
extern int  _putc_r(); 
extern int  putc(); 
extern int  _putc_unlocked_r(); 
extern int  putc_unlocked(); 
extern int  _putchar_r(); 
extern int  putchar(); 
extern int  _putchar_unlocked_r(); 
extern int  putchar_unlocked(); 
extern int  putenv(); 
extern int  _putenv_r(); 
extern int  _puts_r(); 
extern int  puts(); 
extern int  putw(); 
extern int  _putwc_r(); 
extern int  putwc(); 
extern int  _putwchar_r(); 
extern int  putwchar(); 
extern int  _pvalloc_r(); 
extern int  qsort(); 
extern int  rand(); 
extern int  srand(); 
extern int  __dorand48(); 
extern int  rand_r(); 
extern int  _read_r(); 
extern int  realloc(); 
extern int  _reallocf_r(); 
extern int  reallocf(); 
extern int  _realloc_r(); 
extern int  _reclaim_reent(); 
extern int  _wrapup_reent(); 
extern int  cleanup_glue(); 
extern int  __srefill_r(); 
extern int  _remove_r(); 
extern int  remove(); 
extern int  rename(); 
extern int  _rename_r(); 
extern int  _rewind_r(); 
extern int  rewind(); 
extern int  __srget(); 
extern int  __srget_r(); 
extern int  rindex(); 
extern int  copysign(); 
extern int  finite(); 
extern int  __fpclassifyd(); 
extern int  frexp(); 
extern int  isinf(); 
extern int  __isinfd(); 
extern int  isnan(); 
extern int  __isnand(); 
extern int  ldexp(); 
extern int  modf(); 
extern int  nan(); 
extern int  scalbn(); 
extern int  _sbrk_r(); 
extern int  _scanf_r(); 
extern int  scanf(); 
extern int  __sccl(); 
extern int  _seed48_r(); 
extern int  seed48(); 
extern int  setbuf(); 
extern int  setbuffer(); 
extern int  setenv(); 
extern int  unsetenv(); 
extern int  _setenv_r(); 
extern int  _unsetenv_r(); 
extern int  longjmp(); 
extern int  setjmp(); 
extern int  setlinebuf(); 
extern int  setvbuf(); 
extern int  copysignf(); 
extern int  finitef(); 
extern int  __fpclassifyf(); 
extern int  frexpf(); 
extern int  isinff(); 
extern int  __isinff(); 
extern int  isnanf(); 
extern int  __isnanf(); 
extern int  ldexpf(); 
extern int  modff(); 
extern int  nanf(); 
extern int  scalbnf(); 
extern int  __signgam(); 
extern int  _siprintf_r(); 
extern int  siprintf(); 
extern int  _siscanf_r(); 
extern int  siscanf(); 
extern int  _sniprintf_r(); 
extern int  sniprintf(); 
extern int  _snprintf_r(); 
extern int  snprintf(); 
extern int  _sprintf_r(); 
extern int  sprintf(); 
extern int  _srand48_r(); 
extern int  srand48(); 
extern int  _sscanf_r(); 
extern int  sscanf(); 
extern int  _stat_r(); 
extern int  __sclose(); 
extern int  __seofread(); 
extern int  __sread(); 
extern int  __sseek(); 
extern int  __swrite(); 
extern int  stpcpy(); 
extern int  stpncpy(); 
extern int  strcasecmp(); 
extern int  strcasestr(); 
extern int  strcat(); 
extern int  strchr(); 
extern int  strcmp(); 
extern int  strcoll(); 
extern int  strcpy(); 
extern int  strcspn(); 
extern int  strdup(); 
extern int  _strdup_r(); 
extern int  strerror(); 
extern int  strerror_r(); 
extern int  strftime(); 
extern int  strlcat(); 
extern int  strlcpy(); 
extern int  strlen(); 
extern int  strlwr(); 
extern int  strncasecmp(); 
extern int  strncat(); 
extern int  strncmp(); 
extern int  strncpy(); 
extern int  strndup(); 
extern int  _strndup_r(); 
extern int  strnlen(); 
extern int  strpbrk(); 
extern int  strptime(); 
extern int  strrchr(); 
extern int  strsep(); 
extern int  strsignal(); 
extern int  strspn(); 
extern int  strstr(); 
extern int  _strtod_r(); 
extern int  strtod(); 
extern int  strtof(); 
extern int  strtok(); 
extern int  __strtok_r(); 
extern int  strtok_r(); 
extern int  _strtol_r(); 
extern int  strtol(); 
extern int  strtold(); 
extern int  strtoll(); 
extern int  _strtoll_r(); 
extern int  _strtoul_r(); 
extern int  strtoul(); 
extern int  strtoull(); 
extern int  _strtoull_r(); 
extern int  strupr(); 
extern int  strxfrm(); 
extern int  __ssprint_r(); 
extern int  _svfiprintf_r(); 
extern int  __ssrefill_r(); 
extern int  __ssvfiscanf_r(); 
extern int  _sfread_r(); 
extern int  _sungetc_r(); 
extern int  _svfiwprintf_r(); 
extern int  __ssvfiwscanf_r(); 
extern int  _svfprintf_r(); 
extern int  __ssvfscanf_r(); 
extern int  _svfwprintf_r(); 
extern int  __ssvfwscanf_r(); 
extern int  swab(); 
extern int  _swprintf_r(); 
extern int  swprintf(); 
extern int  _swscanf_r(); 
extern int  swscanf(); 
extern int  close(); 
extern int  execve(); 
extern int  fcntl(); 
extern int  fork(); 
extern int  fstat(); 
extern int  getpid(); 
extern int  gettimeofday(); 
extern int  isatty(); 
extern int  kill(); 
extern int  link(); 
extern int  lseek(); 
extern int  open(); 
extern int  read(); 
extern int  sbrk(); 
extern int  stat(); 
extern int  _system_r(); 
extern int  system(); 
extern int  times(); 
extern int  unlink(); 
extern int  wait(); 
extern int  write(); 
extern int  tdelete(); 
extern int  tdestroy(); 
extern int  tfind(); 
extern int  time(); 
extern int  __get_current_time_locale(); 
extern int  __time_load_locale(); 
extern int  _times_r(); 
extern int  _tmpfile_r(); 
extern int  tmpfile(); 
extern int  _tempnam_r(); 
extern int  _tmpnam_r(); 
extern int  tempnam(); 
extern int  tmpnam(); 
extern int  toascii(); 
extern int  tolower(); 
extern int  toupper(); 
extern int  _towctrans_r(); 
extern int  towctrans(); 
extern int  towlower(); 
extern int  towupper(); 
extern int  tsearch(); 
extern int  twalk(); 
extern int  __tz_lock(); 
extern int  __tz_unlock(); 
extern int  tzset(); 
extern int  _tzset_r(); 
extern int  _user_strerror(); 
extern int  __submore(); 
extern int  _ungetc_r(); 
extern int  ungetc(); 
extern int  _ungetwc_r(); 
extern int  ungetwc(); 
extern int  _unlink_r(); 
extern int  pvalloc(); 
extern int  valloc(); 
extern int  _valloc_r(); 
extern int  _vasiprintf_r(); 
extern int  vasiprintf(); 
extern int  _vasniprintf_r(); 
extern int  vasniprintf(); 
extern int  _vasnprintf_r(); 
extern int  vasnprintf(); 
extern int  _vasprintf_r(); 
extern int  vasprintf(); 
extern int  _vdiprintf_r(); 
extern int  vdiprintf(); 
extern int  _vdprintf_r(); 
extern int  vdprintf(); 
extern int  __sprint_r(); 
extern int  _vfiprintf_r(); 
extern int  vfiprintf(); 
extern int  __svfiscanf(); 
extern int  __svfiscanf_r(); 
extern int  _vfiscanf_r(); 
extern int  vfiscanf(); 
extern int  _vfiwprintf_r(); 
extern int  vfiwprintf(); 
extern int  __svfiwscanf(); 
extern int  __svfiwscanf_r(); 
extern int  _vfiwscanf_r(); 
extern int  vfiwscanf(); 
extern int  _vfprintf_r(); 
extern int  vfprintf(); 
extern int  __svfscanf(); 
extern int  __svfscanf_r(); 
extern int  _vfscanf_r(); 
extern int  vfscanf(); 
extern int  _vfwprintf_r(); 
extern int  vfwprintf(); 
extern int  __svfwscanf(); 
extern int  __svfwscanf_r(); 
extern int  _vfwscanf_r(); 
extern int  vfwscanf(); 
extern int  _viprintf_r(); 
extern int  viprintf(); 
extern int  _viscanf_r(); 
extern int  viscanf(); 
extern int  _vprintf_r(); 
extern int  vprintf(); 
extern int  _vscanf_r(); 
extern int  vscanf(); 
extern int  _vsiprintf_r(); 
extern int  vsiprintf(); 
extern int  _vsiscanf_r(); 
extern int  vsiscanf(); 
extern int  _vsniprintf_r(); 
extern int  vsniprintf(); 
extern int  _vsnprintf_r(); 
extern int  vsnprintf(); 
extern int  _vsprintf_r(); 
extern int  vsprintf(); 
extern int  _vsscanf_r(); 
extern int  vsscanf(); 
extern int  _vswprintf_r(); 
extern int  vswprintf(); 
extern int  _vswscanf_r(); 
extern int  vswscanf(); 
extern int  _vwprintf_r(); 
extern int  vwprintf(); 
extern int  _vwscanf_r(); 
extern int  vwscanf(); 
extern int  __swbuf(); 
extern int  __swbuf_r(); 
extern int  wcpcpy(); 
extern int  wcpncpy(); 
extern int  _wcrtomb_r(); 
extern int  wcrtomb(); 
extern int  wcscasecmp(); 
extern int  wcscat(); 
extern int  wcschr(); 
extern int  wcscmp(); 
extern int  wcscoll(); 
extern int  wcscpy(); 
extern int  wcscspn(); 
extern int  _wcsdup_r(); 
extern int  wcsdup(); 
extern int  __ctloc(); 
extern int  wcsftime(); 
extern int  wcslcat(); 
extern int  wcslcpy(); 
extern int  wcslen(); 
extern int  wcsncasecmp(); 
extern int  wcsncat(); 
extern int  wcsncmp(); 
extern int  wcsncpy(); 
extern int  wcsnlen(); 
extern int  _wcsnrtombs_r(); 
extern int  wcsnrtombs(); 
extern int  wcspbrk(); 
extern int  wcsrchr(); 
extern int  _wcsrtombs_r(); 
extern int  wcsrtombs(); 
extern int  wcsspn(); 
extern int  wcsstr(); 
extern int  _wcstod_r(); 
extern int  _wcstof_r(); 
extern int  wcstod(); 
extern int  wcstof(); 
extern int  wcstok(); 
extern int  _wcstol_r(); 
extern int  wcstol(); 
extern int  wcstold(); 
extern int  wcstoll(); 
extern int  _wcstoll_r(); 
extern int  wcstombs(); 
extern int  _wcstombs_r(); 
extern int  _wcstoul_r(); 
extern int  wcstoul(); 
extern int  wcstoull(); 
extern int  _wcstoull_r(); 
extern int  wcswidth(); 
extern int  wcsxfrm(); 
extern int  wctob(); 
extern int  wctomb(); 
extern int  __ascii_wctomb(); 
extern int  _wctomb_r(); 
extern int  _wctrans_r(); 
extern int  wctrans(); 
extern int  _wctype_r(); 
extern int  wctype(); 
extern int  __wcwidth(); 
extern int  wcwidth(); 
extern int  wmemchr(); 
extern int  wmemcmp(); 
extern int  wmemcpy(); 
extern int  wmemmove(); 
extern int  wmemset(); 
extern int  _wprintf_r(); 
extern int  wprintf(); 
extern int  _write_r(); 
extern int  _wscanf_r(); 
extern int  wscanf(); 
extern int  __swsetup_r(); 
extern int  __ctype_ptr__; 
extern int  __mb_cur_max; 
extern int  __mbtowc; 
extern int  __wctomb; 
extern int  _data; 
extern int  _impure_ptr; 
extern int  _tzname; 
extern int  devfs; 
extern int  fatfs; 
extern int  fb_drv; 
extern int  nand_curr_device; 
extern int  nfs; 
extern int  procfs; 
extern int  rootfs; 
extern int  socket_drv; 
extern int  sys_do_table; 
extern int  yaffs1; 
extern int  yaffs2; 
extern int  yaffs_trace_mask; 
extern int  yaffsfs_deviceList; 
extern int  _PathLocale; 
extern int  __bss_start; 
extern int  __bss_start__; 
extern int  __hexdig; 
extern int  __mlocale_changed; 
extern int  __nlocale_changed; 
extern int  _daylight; 
extern int  _null_auth; 
extern int  _timezone; 
extern int  current; 
extern int  current_header; 
extern int  current_iphdr_dest; 
extern int  current_iphdr_src; 
extern int  current_netif; 
extern int  cwd; 
extern int  device_lock; 
extern int  file_system_lock; 
extern int  h_errno; 
extern int  info_lock; 
extern int  infos; 
extern int  interrupt_nest; 
extern int  lock_tcpip_core; 
extern int  lwip_stats; 
extern int  mtd_table; 
extern int  nand_info; 
extern int  netif_default; 
extern int  netif_list; 
extern int  os_started; 
extern int  pbuf_free_ooseq_pending; 
extern int  mount_point_lock; 
extern int  ram_heap; 
extern int  reents; 
extern int  rootfs_point; 
extern int  rpc_createerr; 
extern int  svc_fdset; 
extern int  svc_max_pollfd; 
extern int  svc_pollfd; 
extern int  tasks; 
extern int  tcp_active_pcbs; 
extern int  tcp_active_pcbs_changed; 
extern int  tcp_bound_pcbs; 
extern int  tcp_input_pcb; 
extern int  tcp_listen_pcbs; 
extern int  tcp_ticks; 
extern int  tcp_tmp_pcb; 
extern int  tcp_tw_pcbs; 
extern int  udp_pcbs; 
extern int  yaffs_wr_attempts; 
extern int  __aeabi_idiv0; 
extern int  __aeabi_ldiv0; 
extern int  __aeabi_unwind_cpp_pr1; 
extern int  __aeabi_unwind_cpp_pr2; 
extern int  board_mmc_getcd; 
extern int  __CTOR_LIST__; 
extern int  __DTOR_LIST__; 
extern int  __clz_tab; 
extern int  __popcount_tab; 
extern int  __aeabi_idiv0; 
extern int  __aeabi_ldiv0; 
extern int  __aeabi_unwind_cpp_pr1; 
extern int  __aeabi_unwind_cpp_pr2; 
extern int  __fdlib_version; 
extern int  __infinity; 
extern int  __infinityf; 
extern int  __infinityld; 
extern int  __ctype_ptr__; 
extern int  environ; 
extern int  opterr; 
extern int  optopt; 
extern int  __default_hash; 
extern int  _impure_ptr; 
extern int  __mb_cur_max; 
extern int  __mbtowc; 
extern int  _tzname; 
extern int  __wctomb; 
extern int  optarg; 
extern int  optind; 
extern int  _PathLocale; 
extern int  __mlocale_changed; 
extern int  __nlocale_changed; 
extern int  _daylight; 
extern int  _timezone; 
extern int  __infinity; 
extern int  __infinityf; 
extern int  __infinityld; 
extern int  tm_year_base; 
extern int  __unctrl; 
extern int  __unctrllen; 
extern int  __hexdig; 
extern int  suboptarg; 
extern int  errno; 
/*********************************************************************************************************	
** 系统静态符号表												
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN												
    SYMBOL_ITEM_FUNC(MIN) 
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
    SYMBOL_ITEM_FUNC(__aeabi_d2uiz) 
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
    SYMBOL_ITEM_FUNC(__ascii_mbtowc) 
    SYMBOL_ITEM_FUNC(__ascii_wctomb) 
    SYMBOL_ITEM_FUNC(__b2d) 
    SYMBOL_ITEM_FUNC(__board_mmc_getcd) 
    SYMBOL_ITEM_FUNC(__clzsi2) 
    SYMBOL_ITEM_FUNC(__cmpdf2) 
    SYMBOL_ITEM_FUNC(__copybits) 
    SYMBOL_ITEM_FUNC(__d2b) 
    SYMBOL_ITEM_FUNC(__divdf3) 
    SYMBOL_ITEM_FUNC(__divdi3) 
    SYMBOL_ITEM_FUNC(__divsi3) 
    SYMBOL_ITEM_FUNC(__eqdf2) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(__extendsfdf2) 
    SYMBOL_ITEM_FUNC(__ffssi2) 
    SYMBOL_ITEM_FUNC(__fixdfsi) 
    SYMBOL_ITEM_FUNC(__fixunsdfsi) 
    SYMBOL_ITEM_FUNC(__floatdidf) 
    SYMBOL_ITEM_FUNC(__floatsidf) 
    SYMBOL_ITEM_FUNC(__floatundidf) 
    SYMBOL_ITEM_FUNC(__floatunsidf) 
    SYMBOL_ITEM_FUNC(__fp_lock_all) 
    SYMBOL_ITEM_FUNC(__fp_unlock_all) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(__fxprintf) 
    SYMBOL_ITEM_FUNC(__gedf2) 
    SYMBOL_ITEM_FUNC(__get_socket) 
    SYMBOL_ITEM_FUNC(__gethex) 
    SYMBOL_ITEM_FUNC(__gettzinfo) 
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
    SYMBOL_ITEM_FUNC(__hexdig_init) 
    SYMBOL_ITEM_FUNC(__hexnan) 
    SYMBOL_ITEM_FUNC(__hi0bits) 
    SYMBOL_ITEM_FUNC(__i2b) 
    SYMBOL_ITEM_FUNC(__kfree) 
    SYMBOL_ITEM_FUNC(__kmalloc) 
    SYMBOL_ITEM_FUNC(__ledf2) 
    SYMBOL_ITEM_FUNC(__libc_clntudp_bufcreate) 
    SYMBOL_ITEM_FUNC(__lo0bits) 
    SYMBOL_ITEM_FUNC(__locale_charset) 
    SYMBOL_ITEM_FUNC(__locale_cjk_lang) 
    SYMBOL_ITEM_FUNC(__locale_mb_cur_max) 
    SYMBOL_ITEM_FUNC(__locale_msgcharset) 
    SYMBOL_ITEM_FUNC(__lshift) 
    SYMBOL_ITEM_FUNC(__ltdf2) 
    SYMBOL_ITEM_FUNC(__malloc_lock) 
    SYMBOL_ITEM_FUNC(__malloc_unlock) 
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
    SYMBOL_ITEM_FUNC(__ratio) 
    SYMBOL_ITEM_FUNC(__restore_core_regs) 
    SYMBOL_ITEM_FUNC(__s2b) 
    SYMBOL_ITEM_FUNC(__sccl) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__seofread) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(__sfmoreglue) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sfp_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sfp_lock_release) 
    SYMBOL_ITEM_FUNC(__sfvwrite_r) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(__sinit_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sinit_lock_release) 
    SYMBOL_ITEM_FUNC(__smakebuf_r) 
    SYMBOL_ITEM_FUNC(__sprint_r) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__srefill_r) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__ssprint_r) 
    SYMBOL_ITEM_FUNC(__ssrefill_r) 
    SYMBOL_ITEM_FUNC(__ssvfiscanf_r) 
    SYMBOL_ITEM_FUNC(__ssvfscanf_r) 
    SYMBOL_ITEM_FUNC(__strtok_r) 
    SYMBOL_ITEM_FUNC(__subdf3) 
    SYMBOL_ITEM_FUNC(__submore) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swbuf_r) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(__swsetup_r) 
    SYMBOL_ITEM_FUNC(__truncdfsf2) 
    SYMBOL_ITEM_FUNC(__tz_lock) 
    SYMBOL_ITEM_FUNC(__tz_unlock) 
    SYMBOL_ITEM_FUNC(__tzcalc_limits) 
    SYMBOL_ITEM_FUNC(__udivdi3) 
    SYMBOL_ITEM_FUNC(__udivsi3) 
    SYMBOL_ITEM_FUNC(__ulp) 
    SYMBOL_ITEM_FUNC(__vfs_close) 
    SYMBOL_ITEM_FUNC(__vfs_closedir) 
    SYMBOL_ITEM_FUNC(_atoi_r) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(_cleanup_r) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(_create_xid) 
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
    SYMBOL_ITEM_FUNC(_fputwc_r) 
    SYMBOL_ITEM_FUNC(_fread_r) 
    SYMBOL_ITEM_FUNC(_free_r) 
    SYMBOL_ITEM_FUNC(_fseek_r) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_ftell_r) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(_fwalk_reent) 
    SYMBOL_ITEM_FUNC(_fwrite_r) 
    SYMBOL_ITEM_FUNC(_getpid_r) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(_global_impure_ptr) 
    SYMBOL_ITEM_FUNC(_init) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(_kill_r) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(_localeconv_r) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(_malloc_r) 
    SYMBOL_ITEM_FUNC(_mbrtowc_r) 
    SYMBOL_ITEM_FUNC(_mbtowc_r) 
    SYMBOL_ITEM_FUNC(_memalign_r) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_mktm_r) 
    SYMBOL_ITEM_FUNC(_mprec_log10) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_printf_r) 
    SYMBOL_ITEM_FUNC(_puts_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(_remove_r) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rmdir_r) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_seterr_reply) 
    SYMBOL_ITEM_FUNC(_setlocale_r) 
    SYMBOL_ITEM_FUNC(_sfread_r) 
    SYMBOL_ITEM_FUNC(_snprintf_r) 
    SYMBOL_ITEM_FUNC(_sprintf_r) 
    SYMBOL_ITEM_FUNC(_sscanf_r) 
    SYMBOL_ITEM_FUNC(_start) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(_strdup_r) 
    SYMBOL_ITEM_FUNC(_strndup_r) 
    SYMBOL_ITEM_FUNC(_strtod_r) 
    SYMBOL_ITEM_FUNC(_strtol_r) 
    SYMBOL_ITEM_FUNC(_strtoul_r) 
    SYMBOL_ITEM_FUNC(_sungetc_r) 
    SYMBOL_ITEM_FUNC(_svfiprintf_r) 
    SYMBOL_ITEM_FUNC(_svfprintf_r) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_ungetc_r) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(_vfiprintf_r) 
    SYMBOL_ITEM_FUNC(_vfprintf_r) 
    SYMBOL_ITEM_FUNC(_vsnprintf_r) 
    SYMBOL_ITEM_FUNC(_vsprintf_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(_wcrtomb_r) 
    SYMBOL_ITEM_FUNC(_wctomb_r) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(accept) 
    SYMBOL_ITEM_FUNC(add_mtd_device) 
    SYMBOL_ITEM_FUNC(alarm) 
    SYMBOL_ITEM_FUNC(arch_build_context) 
    SYMBOL_ITEM_FUNC(arch_mmu_init) 
    SYMBOL_ITEM_FUNC(arch_relocate_rel) 
    SYMBOL_ITEM_FUNC(arch_relocate_rela) 
    SYMBOL_ITEM_FUNC(arch_switch_context) 
    SYMBOL_ITEM_FUNC(arch_switch_context_to) 
    SYMBOL_ITEM_FUNC(atoi) 
    SYMBOL_ITEM_FUNC(atomic_add) 
    SYMBOL_ITEM_FUNC(atomic_dec) 
    SYMBOL_ITEM_FUNC(atomic_dec_and_test) 
    SYMBOL_ITEM_FUNC(atomic_dec_return) 
    SYMBOL_ITEM_FUNC(atomic_inc) 
    SYMBOL_ITEM_FUNC(atomic_inc_and_test) 
    SYMBOL_ITEM_FUNC(atomic_inc_return) 
    SYMBOL_ITEM_FUNC(atomic_read) 
    SYMBOL_ITEM_FUNC(atomic_set) 
    SYMBOL_ITEM_FUNC(atomic_sub) 
    SYMBOL_ITEM_FUNC(atomic_sub_and_test) 
    SYMBOL_ITEM_FUNC(atomic_sub_return) 
    SYMBOL_ITEM_FUNC(audio_init) 
    SYMBOL_ITEM_FUNC(authnone_create) 
    SYMBOL_ITEM_FUNC(bind) 
    SYMBOL_ITEM_FUNC(bindresvport) 
    SYMBOL_ITEM_FUNC(bkdr_hash) 
    SYMBOL_ITEM_FUNC(board_devices_create) 
    SYMBOL_ITEM_FUNC(board_drivers_install) 
    SYMBOL_ITEM_FUNC(board_mem_map) 
    SYMBOL_ITEM_FUNC(board_nand_init) 
    SYMBOL_ITEM_FUNC(board_resv_space) 
    SYMBOL_ITEM_FUNC(calloc) 
    SYMBOL_ITEM_FUNC(chdir) 
    SYMBOL_ITEM_FUNC(clnt_create) 
    SYMBOL_ITEM_FUNC(clnttcp_create) 
    SYMBOL_ITEM_FUNC(clntudp_bufcreate) 
    SYMBOL_ITEM_FUNC(clntudp_create) 
    SYMBOL_ITEM_FUNC(clock_init) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(closedir) 
    SYMBOL_ITEM_FUNC(clust2sect) 
    SYMBOL_ITEM_FUNC(connect) 
    SYMBOL_ITEM_FUNC(cpu_devices_create) 
    SYMBOL_ITEM_FUNC(cpu_drivers_install) 
    SYMBOL_ITEM_FUNC(cpu_init) 
    SYMBOL_ITEM_FUNC(cpu_interrupt_init) 
    SYMBOL_ITEM_FUNC(cpu_mem_map) 
    SYMBOL_ITEM_FUNC(cpu_reset_init) 
    SYMBOL_ITEM_FUNC(cpu_resv_space) 
    SYMBOL_ITEM_FUNC(cpu_timer_init) 
    SYMBOL_ITEM_FUNC(creat) 
    SYMBOL_ITEM_FUNC(dabt_c_handler) 
    SYMBOL_ITEM_FUNC(default_workqueue_create) 
    SYMBOL_ITEM_FUNC(del_mtd_device) 
    SYMBOL_ITEM_FUNC(device_create) 
    SYMBOL_ITEM_FUNC(device_get) 
    SYMBOL_ITEM_FUNC(device_lookup) 
    SYMBOL_ITEM_FUNC(device_manager_init) 
    SYMBOL_ITEM_FUNC(device_remove) 
    SYMBOL_ITEM_FUNC(disk_initialize) 
    SYMBOL_ITEM_FUNC(disk_ioctl) 
    SYMBOL_ITEM_FUNC(disk_read) 
    SYMBOL_ITEM_FUNC(disk_status) 
    SYMBOL_ITEM_FUNC(disk_write) 
    SYMBOL_ITEM_FUNC(dlclose) 
    SYMBOL_ITEM_FUNC(dlerror) 
    SYMBOL_ITEM_FUNC(dlopen) 
    SYMBOL_ITEM_FUNC(dlsym) 
    SYMBOL_ITEM_FUNC(dns_gethostbyname) 
    SYMBOL_ITEM_FUNC(dns_getserver) 
    SYMBOL_ITEM_FUNC(dns_init) 
    SYMBOL_ITEM_FUNC(dns_local_addhost) 
    SYMBOL_ITEM_FUNC(dns_local_removehost) 
    SYMBOL_ITEM_FUNC(dns_setserver) 
    SYMBOL_ITEM_FUNC(dns_tmr) 
    SYMBOL_ITEM_FUNC(do_bind) 
    SYMBOL_ITEM_FUNC(do_close) 
    SYMBOL_ITEM_FUNC(do_connect) 
    SYMBOL_ITEM_FUNC(do_delconn) 
    SYMBOL_ITEM_FUNC(do_disconnect) 
    SYMBOL_ITEM_FUNC(do_getaddr) 
    SYMBOL_ITEM_FUNC(do_gethostbyname) 
    SYMBOL_ITEM_FUNC(do_listen) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_add) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_common) 
    SYMBOL_ITEM_FUNC(do_netifapi_netif_set_addr) 
    SYMBOL_ITEM_FUNC(do_newconn) 
    SYMBOL_ITEM_FUNC(do_recv) 
    SYMBOL_ITEM_FUNC(do_send) 
    SYMBOL_ITEM_FUNC(do_write) 
    SYMBOL_ITEM_FUNC(driver_install) 
    SYMBOL_ITEM_FUNC(driver_lookup) 
    SYMBOL_ITEM_FUNC(driver_manager_init) 
    SYMBOL_ITEM_FUNC(driver_ref_by_name) 
    SYMBOL_ITEM_FUNC(driver_remove) 
    SYMBOL_ITEM_FUNC(dup) 
    SYMBOL_ITEM_FUNC(dup2) 
    SYMBOL_ITEM_FUNC(etharp_add_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_cleanup_netif) 
    SYMBOL_ITEM_FUNC(etharp_find_addr) 
    SYMBOL_ITEM_FUNC(etharp_output) 
    SYMBOL_ITEM_FUNC(etharp_query) 
    SYMBOL_ITEM_FUNC(etharp_remove_static_entry) 
    SYMBOL_ITEM_FUNC(etharp_request) 
    SYMBOL_ITEM_FUNC(etharp_tmr) 
    SYMBOL_ITEM_FUNC(ethbroadcast) 
    SYMBOL_ITEM_FUNC(ethernet_input) 
    SYMBOL_ITEM_FUNC(ethernetif_init) 
    SYMBOL_ITEM_FUNC(ethzero) 
    SYMBOL_ITEM_FUNC(f_chmod) 
    SYMBOL_ITEM_FUNC(f_close) 
    SYMBOL_ITEM_FUNC(f_getfree) 
    SYMBOL_ITEM_FUNC(f_lseek) 
    SYMBOL_ITEM_FUNC(f_mkdir) 
    SYMBOL_ITEM_FUNC(f_mkfs) 
    SYMBOL_ITEM_FUNC(f_mount) 
    SYMBOL_ITEM_FUNC(f_open) 
    SYMBOL_ITEM_FUNC(f_opendir) 
    SYMBOL_ITEM_FUNC(f_read) 
    SYMBOL_ITEM_FUNC(f_readdir) 
    SYMBOL_ITEM_FUNC(f_rename) 
    SYMBOL_ITEM_FUNC(f_stat) 
    SYMBOL_ITEM_FUNC(f_sync) 
    SYMBOL_ITEM_FUNC(f_truncate) 
    SYMBOL_ITEM_FUNC(f_unlink) 
    SYMBOL_ITEM_FUNC(f_utime) 
    SYMBOL_ITEM_FUNC(f_write) 
    SYMBOL_ITEM_FUNC(fb_create) 
    SYMBOL_ITEM_FUNC(fb_init) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(ff_cre_syncobj) 
    SYMBOL_ITEM_FUNC(ff_del_syncobj) 
    SYMBOL_ITEM_FUNC(ff_memalloc) 
    SYMBOL_ITEM_FUNC(ff_memfree) 
    SYMBOL_ITEM_FUNC(ff_rel_grant) 
    SYMBOL_ITEM_FUNC(ff_req_grant) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(fifo_create) 
    SYMBOL_ITEM_FUNC(fifo_init) 
    SYMBOL_ITEM_FUNC(file_system_install) 
    SYMBOL_ITEM_FUNC(file_system_lookup) 
    SYMBOL_ITEM_FUNC(file_system_manager_init) 
    SYMBOL_ITEM_FUNC(file_system_remove) 
    SYMBOL_ITEM_FUNC(fiq_c_handler) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(fread) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(freeaddrinfo) 
    SYMBOL_ITEM_FUNC(fseek) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(ftell) 
    SYMBOL_ITEM_FUNC(ftpd_init) 
    SYMBOL_ITEM_FUNC(fwrite) 
    SYMBOL_ITEM_FUNC(get_fat) 
    SYMBOL_ITEM_FUNC(get_fattime) 
    SYMBOL_ITEM_FUNC(get_mtd_device) 
    SYMBOL_ITEM_FUNC(get_mtd_device_nm) 
    SYMBOL_ITEM_FUNC(getaddrinfo) 
    SYMBOL_ITEM_FUNC(getcwd) 
    SYMBOL_ITEM_FUNC(gethostbyname) 
    SYMBOL_ITEM_FUNC(gethostbyname_r) 
    SYMBOL_ITEM_FUNC(getpeername) 
    SYMBOL_ITEM_FUNC(getpid) 
    SYMBOL_ITEM_FUNC(getsockname) 
    SYMBOL_ITEM_FUNC(getsockopt) 
    SYMBOL_ITEM_FUNC(getticks) 
    SYMBOL_ITEM_FUNC(gettid) 
    SYMBOL_ITEM_FUNC(gmtime) 
    SYMBOL_ITEM_FUNC(gmtime_r) 
    SYMBOL_ITEM_FUNC(hash_tbl_create) 
    SYMBOL_ITEM_FUNC(hash_tbl_destroy) 
    SYMBOL_ITEM_FUNC(hash_tbl_insert) 
    SYMBOL_ITEM_FUNC(hash_tbl_lookup) 
    SYMBOL_ITEM_FUNC(heap_alloc) 
    SYMBOL_ITEM_FUNC(heap_check) 
    SYMBOL_ITEM_FUNC(heap_free) 
    SYMBOL_ITEM_FUNC(heap_init) 
    SYMBOL_ITEM_FUNC(hweight32) 
    SYMBOL_ITEM_FUNC(hweight8) 
    SYMBOL_ITEM_FUNC(icmp_dest_unreach) 
    SYMBOL_ITEM_FUNC(icmp_input) 
    SYMBOL_ITEM_FUNC(icmp_time_exceeded) 
    SYMBOL_ITEM_FUNC(in_interrupt) 
    SYMBOL_ITEM_FUNC(inet_chksum) 
    SYMBOL_ITEM_FUNC(inet_chksum_pbuf) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo) 
    SYMBOL_ITEM_FUNC(inet_chksum_pseudo_partial) 
    SYMBOL_ITEM_FUNC(interrupt_disable) 
    SYMBOL_ITEM_FUNC(interrupt_enter) 
    SYMBOL_ITEM_FUNC(interrupt_exec) 
    SYMBOL_ITEM_FUNC(interrupt_exit) 
    SYMBOL_ITEM_FUNC(interrupt_init) 
    SYMBOL_ITEM_FUNC(interrupt_install) 
    SYMBOL_ITEM_FUNC(interrupt_mask) 
    SYMBOL_ITEM_FUNC(interrupt_resume) 
    SYMBOL_ITEM_FUNC(interrupt_unmask) 
    SYMBOL_ITEM_FUNC(ioctl) 
    SYMBOL_ITEM_FUNC(ip4_addr_isbroadcast) 
    SYMBOL_ITEM_FUNC(ip4_addr_netmask_valid) 
    SYMBOL_ITEM_FUNC(ip_addr_any) 
    SYMBOL_ITEM_FUNC(ip_addr_broadcast) 
    SYMBOL_ITEM_FUNC(ip_frag) 
    SYMBOL_ITEM_FUNC(ip_input) 
    SYMBOL_ITEM_FUNC(ip_output) 
    SYMBOL_ITEM_FUNC(ip_output_hinted) 
    SYMBOL_ITEM_FUNC(ip_output_if) 
    SYMBOL_ITEM_FUNC(ip_reass) 
    SYMBOL_ITEM_FUNC(ip_reass_tmr) 
    SYMBOL_ITEM_FUNC(ip_route) 
    SYMBOL_ITEM_FUNC(ipaddr_addr) 
    SYMBOL_ITEM_FUNC(ipaddr_aton) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa_r) 
    SYMBOL_ITEM_FUNC(ipc_task_cleanup) 
    SYMBOL_ITEM_FUNC(ipc_task_init) 
    SYMBOL_ITEM_FUNC(irq_c_handler) 
    SYMBOL_ITEM_FUNC(is_network_up) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(isr_invaild) 
    SYMBOL_ITEM_FUNC(iswspace) 
    SYMBOL_ITEM_FUNC(ka_to_ua) 
    SYMBOL_ITEM_FUNC(kernel_init) 
    SYMBOL_ITEM_FUNC(kernel_start) 
    SYMBOL_ITEM_FUNC(kernel_timer) 
    SYMBOL_ITEM_FUNC(kheap_check) 
    SYMBOL_ITEM_FUNC(kheap_create) 
    SYMBOL_ITEM_FUNC(kidle_create) 
    SYMBOL_ITEM_FUNC(kill) 
    SYMBOL_ITEM_FUNC(klogd_create) 
    SYMBOL_ITEM_FUNC(kthread_create) 
    SYMBOL_ITEM_FUNC(kthread_shell) 
    SYMBOL_ITEM_FUNC(lcd_init) 
    SYMBOL_ITEM_FUNC(listen) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(lwip_accept) 
    SYMBOL_ITEM_FUNC(lwip_bind) 
    SYMBOL_ITEM_FUNC(lwip_chksum_copy) 
    SYMBOL_ITEM_FUNC(lwip_close) 
    SYMBOL_ITEM_FUNC(lwip_connect) 
    SYMBOL_ITEM_FUNC(lwip_fcntl) 
    SYMBOL_ITEM_FUNC(lwip_freeaddrinfo) 
    SYMBOL_ITEM_FUNC(lwip_getaddrinfo) 
    SYMBOL_ITEM_FUNC(lwip_gethostbyname) 
    SYMBOL_ITEM_FUNC(lwip_gethostbyname_r) 
    SYMBOL_ITEM_FUNC(lwip_getpeername) 
    SYMBOL_ITEM_FUNC(lwip_getsockname) 
    SYMBOL_ITEM_FUNC(lwip_getsockopt) 
    SYMBOL_ITEM_FUNC(lwip_htonl) 
    SYMBOL_ITEM_FUNC(lwip_htons) 
    SYMBOL_ITEM_FUNC(lwip_init) 
    SYMBOL_ITEM_FUNC(lwip_ioctl) 
    SYMBOL_ITEM_FUNC(lwip_listen) 
    SYMBOL_ITEM_FUNC(lwip_ntohl) 
    SYMBOL_ITEM_FUNC(lwip_ntohs) 
    SYMBOL_ITEM_FUNC(lwip_read) 
    SYMBOL_ITEM_FUNC(lwip_recv) 
    SYMBOL_ITEM_FUNC(lwip_recvfrom) 
    SYMBOL_ITEM_FUNC(lwip_select) 
    SYMBOL_ITEM_FUNC(lwip_send) 
    SYMBOL_ITEM_FUNC(lwip_sendto) 
    SYMBOL_ITEM_FUNC(lwip_setsockopt) 
    SYMBOL_ITEM_FUNC(lwip_shutdown) 
    SYMBOL_ITEM_FUNC(lwip_socket) 
    SYMBOL_ITEM_FUNC(lwip_socket_init) 
    SYMBOL_ITEM_FUNC(lwip_socket_set_ctx) 
    SYMBOL_ITEM_FUNC(lwip_strerr) 
    SYMBOL_ITEM_FUNC(lwip_write) 
    SYMBOL_ITEM_FUNC(main) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(mbrtowc) 
    SYMBOL_ITEM_FUNC(mem_calloc) 
    SYMBOL_ITEM_FUNC(mem_free) 
    SYMBOL_ITEM_FUNC(mem_free_callback) 
    SYMBOL_ITEM_FUNC(mem_init) 
    SYMBOL_ITEM_FUNC(mem_malloc) 
    SYMBOL_ITEM_FUNC(mem_trim) 
    SYMBOL_ITEM_FUNC(memalign) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcmp) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(memp_free) 
    SYMBOL_ITEM_FUNC(memp_init) 
    SYMBOL_ITEM_FUNC(memp_malloc) 
    SYMBOL_ITEM_FUNC(memrchr) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(mkdir) 
    SYMBOL_ITEM_FUNC(mmc_erase_blocks) 
    SYMBOL_ITEM_FUNC(mmc_getcd) 
    SYMBOL_ITEM_FUNC(mmc_init) 
    SYMBOL_ITEM_FUNC(mmc_read_blocks) 
    SYMBOL_ITEM_FUNC(mmc_set_clock) 
    SYMBOL_ITEM_FUNC(mmc_switch_part) 
    SYMBOL_ITEM_FUNC(mmc_write_blocks) 
    SYMBOL_ITEM_FUNC(mmu_clean_dcache_index) 
    SYMBOL_ITEM_FUNC(mmu_clean_invalidate_dcache_index) 
    SYMBOL_ITEM_FUNC(mmu_clean_invalidate_dcache_mva) 
    SYMBOL_ITEM_FUNC(mmu_disable) 
    SYMBOL_ITEM_FUNC(mmu_disable_align_fault_check) 
    SYMBOL_ITEM_FUNC(mmu_disable_dcache) 
    SYMBOL_ITEM_FUNC(mmu_disable_icache) 
    SYMBOL_ITEM_FUNC(mmu_drain_write_buffer) 
    SYMBOL_ITEM_FUNC(mmu_enable) 
    SYMBOL_ITEM_FUNC(mmu_enable_align_fault_check) 
    SYMBOL_ITEM_FUNC(mmu_enable_dcache) 
    SYMBOL_ITEM_FUNC(mmu_enable_icache) 
    SYMBOL_ITEM_FUNC(mmu_get_cache_type) 
    SYMBOL_ITEM_FUNC(mmu_get_cpu_id) 
    SYMBOL_ITEM_FUNC(mmu_get_data_fault_status) 
    SYMBOL_ITEM_FUNC(mmu_get_fault_address) 
    SYMBOL_ITEM_FUNC(mmu_get_prefetch_fault_status) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dcache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dtlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_dtlb_mva) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache_dcache) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_icache_mva) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb_dtlb) 
    SYMBOL_ITEM_FUNC(mmu_invalidate_itlb_mva) 
    SYMBOL_ITEM_FUNC(mmu_map_page) 
    SYMBOL_ITEM_FUNC(mmu_map_region) 
    SYMBOL_ITEM_FUNC(mmu_map_section) 
    SYMBOL_ITEM_FUNC(mmu_set_domain) 
    SYMBOL_ITEM_FUNC(mmu_set_sys_rom_protect_bit) 
    SYMBOL_ITEM_FUNC(mmu_set_ttb) 
    SYMBOL_ITEM_FUNC(mmu_set_vector_addr) 
    SYMBOL_ITEM_FUNC(mmu_unmap_section) 
    SYMBOL_ITEM_FUNC(mmu_wait_for_interrupt) 
    SYMBOL_ITEM_FUNC(module_close) 
    SYMBOL_ITEM_FUNC(module_init) 
    SYMBOL_ITEM_FUNC(module_load) 
    SYMBOL_ITEM_FUNC(module_open) 
    SYMBOL_ITEM_FUNC(module_ref_by_addr) 
    SYMBOL_ITEM_FUNC(module_symbol) 
    SYMBOL_ITEM_FUNC(module_unload) 
    SYMBOL_ITEM_FUNC(module_unref) 
    SYMBOL_ITEM_FUNC(mount_point_get) 
    SYMBOL_ITEM_FUNC(mount_point_install) 
    SYMBOL_ITEM_FUNC(mount_point_lookup) 
    SYMBOL_ITEM_FUNC(mount_point_manager_init) 
    SYMBOL_ITEM_FUNC(mount_point_remove) 
    SYMBOL_ITEM_FUNC(mountproc3_dump_3) 
    SYMBOL_ITEM_FUNC(mountproc3_export_3) 
    SYMBOL_ITEM_FUNC(mountproc3_mnt_3) 
    SYMBOL_ITEM_FUNC(mountproc3_null_3) 
    SYMBOL_ITEM_FUNC(mountproc3_umnt_3) 
    SYMBOL_ITEM_FUNC(mountproc3_umntall_3) 
    SYMBOL_ITEM_FUNC(mqueue_abort) 
    SYMBOL_ITEM_FUNC(mqueue_abort_fetch) 
    SYMBOL_ITEM_FUNC(mqueue_abort_post) 
    SYMBOL_ITEM_FUNC(mqueue_create) 
    SYMBOL_ITEM_FUNC(mqueue_destroy) 
    SYMBOL_ITEM_FUNC(mqueue_fetch) 
    SYMBOL_ITEM_FUNC(mqueue_flush) 
    SYMBOL_ITEM_FUNC(mqueue_msg_nr) 
    SYMBOL_ITEM_FUNC(mqueue_post) 
    SYMBOL_ITEM_FUNC(mqueue_set_valid) 
    SYMBOL_ITEM_FUNC(mqueue_tryfetch) 
    SYMBOL_ITEM_FUNC(mqueue_trypost) 
    SYMBOL_ITEM_FUNC(mqueue_valid) 
    SYMBOL_ITEM_FUNC(msleep) 
    SYMBOL_ITEM_FUNC(mtd_get_len_incl_bad) 
    SYMBOL_ITEM_FUNC(mtdblock_create) 
    SYMBOL_ITEM_FUNC(mtdblock_init) 
    SYMBOL_ITEM_FUNC(mutex_abort) 
    SYMBOL_ITEM_FUNC(mutex_create) 
    SYMBOL_ITEM_FUNC(mutex_destroy) 
    SYMBOL_ITEM_FUNC(mutex_lock) 
    SYMBOL_ITEM_FUNC(mutex_set_valid) 
    SYMBOL_ITEM_FUNC(mutex_trylock) 
    SYMBOL_ITEM_FUNC(mutex_unlock) 
    SYMBOL_ITEM_FUNC(mutex_valid) 
    SYMBOL_ITEM_FUNC(nand_calculate_ecc) 
    SYMBOL_ITEM_FUNC(nand_correct_data) 
    SYMBOL_ITEM_FUNC(nand_default_bbt) 
    SYMBOL_ITEM_FUNC(nand_erase_nand) 
    SYMBOL_ITEM_FUNC(nand_flash_ids) 
    SYMBOL_ITEM_FUNC(nand_init) 
    SYMBOL_ITEM_FUNC(nand_isbad_bbt) 
    SYMBOL_ITEM_FUNC(nand_manuf_ids) 
    SYMBOL_ITEM_FUNC(nand_read_buf) 
    SYMBOL_ITEM_FUNC(nand_read_buf16) 
    SYMBOL_ITEM_FUNC(nand_read_byte) 
    SYMBOL_ITEM_FUNC(nand_register) 
    SYMBOL_ITEM_FUNC(nand_release) 
    SYMBOL_ITEM_FUNC(nand_scan) 
    SYMBOL_ITEM_FUNC(nand_scan_bbt) 
    SYMBOL_ITEM_FUNC(nand_scan_ident) 
    SYMBOL_ITEM_FUNC(nand_scan_tail) 
    SYMBOL_ITEM_FUNC(nand_update_bbt) 
    SYMBOL_ITEM_FUNC(nand_wait_ready) 
    SYMBOL_ITEM_FUNC(nand_write_buf) 
    SYMBOL_ITEM_FUNC(nand_write_buf16) 
    SYMBOL_ITEM_FUNC(nandmtd_erase_block) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(netbuf_alloc) 
    SYMBOL_ITEM_FUNC(netbuf_chain) 
    SYMBOL_ITEM_FUNC(netbuf_data) 
    SYMBOL_ITEM_FUNC(netbuf_delete) 
    SYMBOL_ITEM_FUNC(netbuf_first) 
    SYMBOL_ITEM_FUNC(netbuf_free) 
    SYMBOL_ITEM_FUNC(netbuf_new) 
    SYMBOL_ITEM_FUNC(netbuf_next) 
    SYMBOL_ITEM_FUNC(netbuf_ref) 
    SYMBOL_ITEM_FUNC(netconn_accept) 
    SYMBOL_ITEM_FUNC(netconn_alloc) 
    SYMBOL_ITEM_FUNC(netconn_bind) 
    SYMBOL_ITEM_FUNC(netconn_close) 
    SYMBOL_ITEM_FUNC(netconn_connect) 
    SYMBOL_ITEM_FUNC(netconn_delete) 
    SYMBOL_ITEM_FUNC(netconn_disconnect) 
    SYMBOL_ITEM_FUNC(netconn_free) 
    SYMBOL_ITEM_FUNC(netconn_getaddr) 
    SYMBOL_ITEM_FUNC(netconn_gethostbyname) 
    SYMBOL_ITEM_FUNC(netconn_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(netconn_new_with_proto_and_callback) 
    SYMBOL_ITEM_FUNC(netconn_recv) 
    SYMBOL_ITEM_FUNC(netconn_recv_tcp_pbuf) 
    SYMBOL_ITEM_FUNC(netconn_recved) 
    SYMBOL_ITEM_FUNC(netconn_send) 
    SYMBOL_ITEM_FUNC(netconn_sendto) 
    SYMBOL_ITEM_FUNC(netconn_shutdown) 
    SYMBOL_ITEM_FUNC(netconn_write_partly) 
    SYMBOL_ITEM_FUNC(netif_add) 
    SYMBOL_ITEM_FUNC(netif_find) 
    SYMBOL_ITEM_FUNC(netif_init) 
    SYMBOL_ITEM_FUNC(netif_loop_output) 
    SYMBOL_ITEM_FUNC(netif_poll) 
    SYMBOL_ITEM_FUNC(netif_remove) 
    SYMBOL_ITEM_FUNC(netif_set_addr) 
    SYMBOL_ITEM_FUNC(netif_set_default) 
    SYMBOL_ITEM_FUNC(netif_set_down) 
    SYMBOL_ITEM_FUNC(netif_set_gw) 
    SYMBOL_ITEM_FUNC(netif_set_ipaddr) 
    SYMBOL_ITEM_FUNC(netif_set_link_callback) 
    SYMBOL_ITEM_FUNC(netif_set_link_down) 
    SYMBOL_ITEM_FUNC(netif_set_link_up) 
    SYMBOL_ITEM_FUNC(netif_set_netmask) 
    SYMBOL_ITEM_FUNC(netif_set_remove_callback) 
    SYMBOL_ITEM_FUNC(netif_set_status_callback) 
    SYMBOL_ITEM_FUNC(netif_set_up) 
    SYMBOL_ITEM_FUNC(netifapi_netif_add) 
    SYMBOL_ITEM_FUNC(netifapi_netif_common) 
    SYMBOL_ITEM_FUNC(netifapi_netif_set_addr) 
    SYMBOL_ITEM_FUNC(netio_init) 
    SYMBOL_ITEM_FUNC(nfsproc3_access_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_commit_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_create_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_fsinfo_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_fsstat_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_getattr_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_link_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_lookup_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_mkdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_mknod_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_null_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_pathconf_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_read_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readdirplus_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_readlink_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_remove_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_rename_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_rmdir_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_setattr_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_symlink_3) 
    SYMBOL_ITEM_FUNC(nfsproc3_write_3) 
    SYMBOL_ITEM_FUNC(null_init) 
    SYMBOL_ITEM_FUNC(nval_del) 
    SYMBOL_ITEM_FUNC(nval_get) 
    SYMBOL_ITEM_FUNC(nval_hasvalues) 
    SYMBOL_ITEM_FUNC(nval_list) 
    SYMBOL_ITEM_FUNC(nval_set) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(opendir) 
    SYMBOL_ITEM_FUNC(pabt_c_handler) 
    SYMBOL_ITEM_FUNC(pause) 
    SYMBOL_ITEM_FUNC(pbuf_alloc) 
    SYMBOL_ITEM_FUNC(pbuf_alloced_custom) 
    SYMBOL_ITEM_FUNC(pbuf_cat) 
    SYMBOL_ITEM_FUNC(pbuf_chain) 
    SYMBOL_ITEM_FUNC(pbuf_clen) 
    SYMBOL_ITEM_FUNC(pbuf_coalesce) 
    SYMBOL_ITEM_FUNC(pbuf_copy) 
    SYMBOL_ITEM_FUNC(pbuf_copy_partial) 
    SYMBOL_ITEM_FUNC(pbuf_dechain) 
    SYMBOL_ITEM_FUNC(pbuf_fill_chksum) 
    SYMBOL_ITEM_FUNC(pbuf_free) 
    SYMBOL_ITEM_FUNC(pbuf_free_callback) 
    SYMBOL_ITEM_FUNC(pbuf_get_at) 
    SYMBOL_ITEM_FUNC(pbuf_header) 
    SYMBOL_ITEM_FUNC(pbuf_memcmp) 
    SYMBOL_ITEM_FUNC(pbuf_memfind) 
    SYMBOL_ITEM_FUNC(pbuf_realloc) 
    SYMBOL_ITEM_FUNC(pbuf_ref) 
    SYMBOL_ITEM_FUNC(pbuf_strstr) 
    SYMBOL_ITEM_FUNC(pbuf_take) 
    SYMBOL_ITEM_FUNC(pipe) 
    SYMBOL_ITEM_FUNC(pipe_create) 
    SYMBOL_ITEM_FUNC(pmap_getport) 
    SYMBOL_ITEM_FUNC(poll) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(printk) 
    SYMBOL_ITEM_FUNC(put_fat) 
    SYMBOL_ITEM_FUNC(put_mtd_device) 
    SYMBOL_ITEM_FUNC(puts) 
    SYMBOL_ITEM_FUNC(raise) 
    SYMBOL_ITEM_FUNC(ramdisk_create) 
    SYMBOL_ITEM_FUNC(ramdisk_init) 
    SYMBOL_ITEM_FUNC(raw_bind) 
    SYMBOL_ITEM_FUNC(raw_connect) 
    SYMBOL_ITEM_FUNC(raw_input) 
    SYMBOL_ITEM_FUNC(raw_new) 
    SYMBOL_ITEM_FUNC(raw_recv) 
    SYMBOL_ITEM_FUNC(raw_remove) 
    SYMBOL_ITEM_FUNC(raw_send) 
    SYMBOL_ITEM_FUNC(raw_sendto) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(readdir) 
    SYMBOL_ITEM_FUNC(recv) 
    SYMBOL_ITEM_FUNC(recvfrom) 
    SYMBOL_ITEM_FUNC(remove) 
    SYMBOL_ITEM_FUNC(rename) 
    SYMBOL_ITEM_FUNC(restore_core_regs) 
    SYMBOL_ITEM_FUNC(rewinddir) 
    SYMBOL_ITEM_FUNC(rmdir) 
    SYMBOL_ITEM_FUNC(schedule) 
    SYMBOL_ITEM_FUNC(seekdir) 
    SYMBOL_ITEM_FUNC(select) 
    SYMBOL_ITEM_FUNC(select_init) 
    SYMBOL_ITEM_FUNC(select_select) 
    SYMBOL_ITEM_FUNC(select_unselect) 
    SYMBOL_ITEM_FUNC(sem_abort) 
    SYMBOL_ITEM_FUNC(sem_create) 
    SYMBOL_ITEM_FUNC(sem_destroy) 
    SYMBOL_ITEM_FUNC(sem_set_valid) 
    SYMBOL_ITEM_FUNC(sem_signal) 
    SYMBOL_ITEM_FUNC(sem_sync) 
    SYMBOL_ITEM_FUNC(sem_trywait) 
    SYMBOL_ITEM_FUNC(sem_valid) 
    SYMBOL_ITEM_FUNC(sem_wait) 
    SYMBOL_ITEM_FUNC(send) 
    SYMBOL_ITEM_FUNC(sendto) 
    SYMBOL_ITEM_FUNC(serial0_init) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(setpinfo) 
    SYMBOL_ITEM_FUNC(setsockopt) 
    SYMBOL_ITEM_FUNC(sharemem_create) 
    SYMBOL_ITEM_FUNC(sharemem_init) 
    SYMBOL_ITEM_FUNC(shutdown) 
    SYMBOL_ITEM_FUNC(signal) 
    SYMBOL_ITEM_FUNC(sigprocmask) 
    SYMBOL_ITEM_FUNC(sigsuspend) 
    SYMBOL_ITEM_FUNC(sleep) 
    SYMBOL_ITEM_FUNC(smileos_socket_report) 
    SYMBOL_ITEM_FUNC(snprintf) 
    SYMBOL_ITEM_FUNC(socket) 
    SYMBOL_ITEM_FUNC(socket_attach) 
    SYMBOL_ITEM_FUNC(socket_init) 
    SYMBOL_ITEM_FUNC(socket_op_end) 
    SYMBOL_ITEM_FUNC(socket_priv_fd) 
    SYMBOL_ITEM_FUNC(socket_stat) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(sscanf) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(stats_display) 
    SYMBOL_ITEM_FUNC(stats_display_mem) 
    SYMBOL_ITEM_FUNC(stats_display_memp) 
    SYMBOL_ITEM_FUNC(stats_display_proto) 
    SYMBOL_ITEM_FUNC(stats_display_sys) 
    SYMBOL_ITEM_FUNC(stats_init) 
    SYMBOL_ITEM_FUNC(strcat) 
    SYMBOL_ITEM_FUNC(strchr) 
    SYMBOL_ITEM_FUNC(strchrnul) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strcpy) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(strlcpy) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strncmp) 
    SYMBOL_ITEM_FUNC(strncpy) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(strnlen) 
    SYMBOL_ITEM_FUNC(strtod) 
    SYMBOL_ITEM_FUNC(strtof) 
    SYMBOL_ITEM_FUNC(strtok_r) 
    SYMBOL_ITEM_FUNC(strtol) 
    SYMBOL_ITEM_FUNC(strtoul) 
    SYMBOL_ITEM_FUNC(swi_handler) 
    SYMBOL_ITEM_FUNC(symbol_name) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_fetch) 
    SYMBOL_ITEM_FUNC(sys_arch_mbox_tryfetch) 
    SYMBOL_ITEM_FUNC(sys_arch_sem_wait) 
    SYMBOL_ITEM_FUNC(sys_dns_lookup_local) 
    SYMBOL_ITEM_FUNC(sys_do_enter) 
    SYMBOL_ITEM_FUNC(sys_init) 
    SYMBOL_ITEM_FUNC(sys_jiffies) 
    SYMBOL_ITEM_FUNC(sys_mbox_free) 
    SYMBOL_ITEM_FUNC(sys_mbox_new) 
    SYMBOL_ITEM_FUNC(sys_mbox_post) 
    SYMBOL_ITEM_FUNC(sys_mbox_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mbox_trypost) 
    SYMBOL_ITEM_FUNC(sys_mbox_valid) 
    SYMBOL_ITEM_FUNC(sys_msleep) 
    SYMBOL_ITEM_FUNC(sys_mutex_free) 
    SYMBOL_ITEM_FUNC(sys_mutex_lock) 
    SYMBOL_ITEM_FUNC(sys_mutex_new) 
    SYMBOL_ITEM_FUNC(sys_mutex_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_mutex_unlock) 
    SYMBOL_ITEM_FUNC(sys_mutex_valid) 
    SYMBOL_ITEM_FUNC(sys_now) 
    SYMBOL_ITEM_FUNC(sys_sem_free) 
    SYMBOL_ITEM_FUNC(sys_sem_new) 
    SYMBOL_ITEM_FUNC(sys_sem_set_invalid) 
    SYMBOL_ITEM_FUNC(sys_sem_signal) 
    SYMBOL_ITEM_FUNC(sys_sem_valid) 
    SYMBOL_ITEM_FUNC(sys_symbol_lookup) 
    SYMBOL_ITEM_FUNC(sys_symbol_tbl) 
    SYMBOL_ITEM_FUNC(sys_symbol_tbl_init) 
    SYMBOL_ITEM_FUNC(sys_thread_hostent) 
    SYMBOL_ITEM_FUNC(sys_thread_new) 
    SYMBOL_ITEM_FUNC(sys_timeout) 
    SYMBOL_ITEM_FUNC(sys_timeouts_init) 
    SYMBOL_ITEM_FUNC(sys_timeouts_mbox_fetch) 
    SYMBOL_ITEM_FUNC(sys_untimeout) 
    SYMBOL_ITEM_FUNC(task_cleanup) 
    SYMBOL_ITEM_FUNC(task_getpid) 
    SYMBOL_ITEM_FUNC(task_schedule) 
    SYMBOL_ITEM_FUNC(task_sleep) 
    SYMBOL_ITEM_FUNC(task_switch_hook) 
    SYMBOL_ITEM_FUNC(tcp_abandon) 
    SYMBOL_ITEM_FUNC(tcp_abort) 
    SYMBOL_ITEM_FUNC(tcp_accept) 
    SYMBOL_ITEM_FUNC(tcp_alloc) 
    SYMBOL_ITEM_FUNC(tcp_arg) 
    SYMBOL_ITEM_FUNC(tcp_backoff) 
    SYMBOL_ITEM_FUNC(tcp_bind) 
    SYMBOL_ITEM_FUNC(tcp_close) 
    SYMBOL_ITEM_FUNC(tcp_connect) 
    SYMBOL_ITEM_FUNC(tcp_debug_state_str) 
    SYMBOL_ITEM_FUNC(tcp_eff_send_mss) 
    SYMBOL_ITEM_FUNC(tcp_enqueue_flags) 
    SYMBOL_ITEM_FUNC(tcp_err) 
    SYMBOL_ITEM_FUNC(tcp_fasttmr) 
    SYMBOL_ITEM_FUNC(tcp_init) 
    SYMBOL_ITEM_FUNC(tcp_input) 
    SYMBOL_ITEM_FUNC(tcp_keepalive) 
    SYMBOL_ITEM_FUNC(tcp_listen_with_backlog) 
    SYMBOL_ITEM_FUNC(tcp_new) 
    SYMBOL_ITEM_FUNC(tcp_next_iss) 
    SYMBOL_ITEM_FUNC(tcp_output) 
    SYMBOL_ITEM_FUNC(tcp_pcb_lists) 
    SYMBOL_ITEM_FUNC(tcp_pcb_purge) 
    SYMBOL_ITEM_FUNC(tcp_pcb_remove) 
    SYMBOL_ITEM_FUNC(tcp_persist_backoff) 
    SYMBOL_ITEM_FUNC(tcp_poll) 
    SYMBOL_ITEM_FUNC(tcp_process_refused_data) 
    SYMBOL_ITEM_FUNC(tcp_recv) 
    SYMBOL_ITEM_FUNC(tcp_recv_null) 
    SYMBOL_ITEM_FUNC(tcp_recved) 
    SYMBOL_ITEM_FUNC(tcp_rexmit) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_fast) 
    SYMBOL_ITEM_FUNC(tcp_rexmit_rto) 
    SYMBOL_ITEM_FUNC(tcp_rst) 
    SYMBOL_ITEM_FUNC(tcp_seg_copy) 
    SYMBOL_ITEM_FUNC(tcp_seg_free) 
    SYMBOL_ITEM_FUNC(tcp_segs_free) 
    SYMBOL_ITEM_FUNC(tcp_send_empty_ack) 
    SYMBOL_ITEM_FUNC(tcp_send_fin) 
    SYMBOL_ITEM_FUNC(tcp_sent) 
    SYMBOL_ITEM_FUNC(tcp_setprio) 
    SYMBOL_ITEM_FUNC(tcp_shutdown) 
    SYMBOL_ITEM_FUNC(tcp_slowtmr) 
    SYMBOL_ITEM_FUNC(tcp_state_str) 
    SYMBOL_ITEM_FUNC(tcp_timer_needed) 
    SYMBOL_ITEM_FUNC(tcp_tmr) 
    SYMBOL_ITEM_FUNC(tcp_update_rcv_ann_wnd) 
    SYMBOL_ITEM_FUNC(tcp_write) 
    SYMBOL_ITEM_FUNC(tcp_zero_window_probe) 
    SYMBOL_ITEM_FUNC(tcpip_apimsg) 
    SYMBOL_ITEM_FUNC(tcpip_apimsg_lock) 
    SYMBOL_ITEM_FUNC(tcpip_callback_with_block) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_delete) 
    SYMBOL_ITEM_FUNC(tcpip_callbackmsg_new) 
    SYMBOL_ITEM_FUNC(tcpip_init) 
    SYMBOL_ITEM_FUNC(tcpip_input) 
    SYMBOL_ITEM_FUNC(tcpip_netifapi_lock) 
    SYMBOL_ITEM_FUNC(tcpip_timeout) 
    SYMBOL_ITEM_FUNC(tcpip_trycallback) 
    SYMBOL_ITEM_FUNC(tcpip_untimeout) 
    SYMBOL_ITEM_FUNC(telldir) 
    SYMBOL_ITEM_FUNC(test_code) 
    SYMBOL_ITEM_FUNC(test_code1) 
    SYMBOL_ITEM_FUNC(test_code2) 
    SYMBOL_ITEM_FUNC(test_code3) 
    SYMBOL_ITEM_FUNC(test_code4) 
    SYMBOL_ITEM_FUNC(test_code5) 
    SYMBOL_ITEM_FUNC(test_code6) 
    SYMBOL_ITEM_FUNC(time) 
    SYMBOL_ITEM_FUNC(touch_init) 
    SYMBOL_ITEM_FUNC(ua_to_ka) 
    SYMBOL_ITEM_FUNC(udp_bind) 
    SYMBOL_ITEM_FUNC(udp_connect) 
    SYMBOL_ITEM_FUNC(udp_disconnect) 
    SYMBOL_ITEM_FUNC(udp_init) 
    SYMBOL_ITEM_FUNC(udp_input) 
    SYMBOL_ITEM_FUNC(udp_new) 
    SYMBOL_ITEM_FUNC(udp_recv) 
    SYMBOL_ITEM_FUNC(udp_remove) 
    SYMBOL_ITEM_FUNC(udp_send) 
    SYMBOL_ITEM_FUNC(udp_send_chksum) 
    SYMBOL_ITEM_FUNC(udp_sendto) 
    SYMBOL_ITEM_FUNC(udp_sendto_chksum) 
    SYMBOL_ITEM_FUNC(udp_sendto_if) 
    SYMBOL_ITEM_FUNC(udp_sendto_if_chksum) 
    SYMBOL_ITEM_FUNC(undf_c_handler) 
    SYMBOL_ITEM_FUNC(ungetc) 
    SYMBOL_ITEM_FUNC(usleep) 
    SYMBOL_ITEM_FUNC(vfiprintf) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(vfs_access) 
    SYMBOL_ITEM_FUNC(vfs_block_helper) 
    SYMBOL_ITEM_FUNC(vfs_chdir) 
    SYMBOL_ITEM_FUNC(vfs_close) 
    SYMBOL_ITEM_FUNC(vfs_closedir) 
    SYMBOL_ITEM_FUNC(vfs_dup) 
    SYMBOL_ITEM_FUNC(vfs_dup2) 
    SYMBOL_ITEM_FUNC(vfs_event_report) 
    SYMBOL_ITEM_FUNC(vfs_fcntl) 
    SYMBOL_ITEM_FUNC(vfs_fdatasync) 
    SYMBOL_ITEM_FUNC(vfs_file_alloc) 
    SYMBOL_ITEM_FUNC(vfs_file_free) 
    SYMBOL_ITEM_FUNC(vfs_fstat) 
    SYMBOL_ITEM_FUNC(vfs_fsync) 
    SYMBOL_ITEM_FUNC(vfs_ftruncate) 
    SYMBOL_ITEM_FUNC(vfs_get_file) 
    SYMBOL_ITEM_FUNC(vfs_getcwd) 
    SYMBOL_ITEM_FUNC(vfs_init) 
    SYMBOL_ITEM_FUNC(vfs_ioctl) 
    SYMBOL_ITEM_FUNC(vfs_isatty) 
    SYMBOL_ITEM_FUNC(vfs_link) 
    SYMBOL_ITEM_FUNC(vfs_lseek) 
    SYMBOL_ITEM_FUNC(vfs_mkdir) 
    SYMBOL_ITEM_FUNC(vfs_mkfs) 
    SYMBOL_ITEM_FUNC(vfs_mount) 
    SYMBOL_ITEM_FUNC(vfs_mount_point_lookup) 
    SYMBOL_ITEM_FUNC(vfs_mount_point_lookup2) 
    SYMBOL_ITEM_FUNC(vfs_mount_point_lookup2_ref) 
    SYMBOL_ITEM_FUNC(vfs_mount_point_lookup_ref) 
    SYMBOL_ITEM_FUNC(vfs_open) 
    SYMBOL_ITEM_FUNC(vfs_opendir) 
    SYMBOL_ITEM_FUNC(vfs_path_add_mount_point) 
    SYMBOL_ITEM_FUNC(vfs_path_format) 
    SYMBOL_ITEM_FUNC(vfs_path_normalization) 
    SYMBOL_ITEM_FUNC(vfs_process_cleanup) 
    SYMBOL_ITEM_FUNC(vfs_process_init) 
    SYMBOL_ITEM_FUNC(vfs_put_file) 
    SYMBOL_ITEM_FUNC(vfs_read) 
    SYMBOL_ITEM_FUNC(vfs_readdir) 
    SYMBOL_ITEM_FUNC(vfs_rename) 
    SYMBOL_ITEM_FUNC(vfs_rewinddir) 
    SYMBOL_ITEM_FUNC(vfs_rmdir) 
    SYMBOL_ITEM_FUNC(vfs_scan_file) 
    SYMBOL_ITEM_FUNC(vfs_seekdir) 
    SYMBOL_ITEM_FUNC(vfs_select) 
    SYMBOL_ITEM_FUNC(vfs_select_file) 
    SYMBOL_ITEM_FUNC(vfs_stat) 
    SYMBOL_ITEM_FUNC(vfs_sync) 
    SYMBOL_ITEM_FUNC(vfs_telldir) 
    SYMBOL_ITEM_FUNC(vfs_truncate) 
    SYMBOL_ITEM_FUNC(vfs_unlink) 
    SYMBOL_ITEM_FUNC(vfs_unmount) 
    SYMBOL_ITEM_FUNC(vfs_unselect_file) 
    SYMBOL_ITEM_FUNC(vfs_write) 
    SYMBOL_ITEM_FUNC(vmm_init) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(vsprintf) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(workqueue_add) 
    SYMBOL_ITEM_FUNC(workqueue_create) 
    SYMBOL_ITEM_FUNC(write) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3args) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3res) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3resfail) 
    SYMBOL_ITEM_FUNC(xdr_ACCESS3resok) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3args) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3res) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3resfail) 
    SYMBOL_ITEM_FUNC(xdr_COMMIT3resok) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3args) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3res) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_CREATE3resok) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3args) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3res) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3resfail) 
    SYMBOL_ITEM_FUNC(xdr_FSINFO3resok) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3args) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3res) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3resfail) 
    SYMBOL_ITEM_FUNC(xdr_FSSTAT3resok) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3args) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3res) 
    SYMBOL_ITEM_FUNC(xdr_GETATTR3resok) 
    SYMBOL_ITEM_FUNC(xdr_LINK3args) 
    SYMBOL_ITEM_FUNC(xdr_LINK3res) 
    SYMBOL_ITEM_FUNC(xdr_LINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_LINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3args) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3res) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3resfail) 
    SYMBOL_ITEM_FUNC(xdr_LOOKUP3resok) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_MKDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3args) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3res) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3resfail) 
    SYMBOL_ITEM_FUNC(xdr_MKNOD3resok) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3args) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3res) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3resfail) 
    SYMBOL_ITEM_FUNC(xdr_PATHCONF3resok) 
    SYMBOL_ITEM_FUNC(xdr_READ3args) 
    SYMBOL_ITEM_FUNC(xdr_READ3res) 
    SYMBOL_ITEM_FUNC(xdr_READ3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READ3resok) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3args) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3res) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READDIRPLUS3resok) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3args) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3res) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_READLINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3args) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3res) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_REMOVE3resok) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3args) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3res) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3resfail) 
    SYMBOL_ITEM_FUNC(xdr_RENAME3resok) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3args) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3res) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_RMDIR3resok) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3args) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3res) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3resfail) 
    SYMBOL_ITEM_FUNC(xdr_SETATTR3resok) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3args) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3res) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3resfail) 
    SYMBOL_ITEM_FUNC(xdr_SYMLINK3resok) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3args) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3res) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3resfail) 
    SYMBOL_ITEM_FUNC(xdr_WRITE3resok) 
    SYMBOL_ITEM_FUNC(xdr_accepted_reply) 
    SYMBOL_ITEM_FUNC(xdr_array) 
    SYMBOL_ITEM_FUNC(xdr_bool) 
    SYMBOL_ITEM_FUNC(xdr_bytes) 
    SYMBOL_ITEM_FUNC(xdr_callhdr) 
    SYMBOL_ITEM_FUNC(xdr_char) 
    SYMBOL_ITEM_FUNC(xdr_cookie3) 
    SYMBOL_ITEM_FUNC(xdr_cookieverf3) 
    SYMBOL_ITEM_FUNC(xdr_count3) 
    SYMBOL_ITEM_FUNC(xdr_createhow3) 
    SYMBOL_ITEM_FUNC(xdr_createmode3) 
    SYMBOL_ITEM_FUNC(xdr_createverf3) 
    SYMBOL_ITEM_FUNC(xdr_des_block) 
    SYMBOL_ITEM_FUNC(xdr_devicedata3) 
    SYMBOL_ITEM_FUNC(xdr_dirlist3) 
    SYMBOL_ITEM_FUNC(xdr_dirlistplus3) 
    SYMBOL_ITEM_FUNC(xdr_diropargs3) 
    SYMBOL_ITEM_FUNC(xdr_dirpath) 
    SYMBOL_ITEM_FUNC(xdr_double) 
    SYMBOL_ITEM_FUNC(xdr_entry3) 
    SYMBOL_ITEM_FUNC(xdr_entryplus3) 
    SYMBOL_ITEM_FUNC(xdr_enum) 
    SYMBOL_ITEM_FUNC(xdr_exportnode) 
    SYMBOL_ITEM_FUNC(xdr_exports) 
    SYMBOL_ITEM_FUNC(xdr_fattr3) 
    SYMBOL_ITEM_FUNC(xdr_fhandle3) 
    SYMBOL_ITEM_FUNC(xdr_fileid3) 
    SYMBOL_ITEM_FUNC(xdr_filename3) 
    SYMBOL_ITEM_FUNC(xdr_float) 
    SYMBOL_ITEM_FUNC(xdr_free) 
    SYMBOL_ITEM_FUNC(xdr_ftype3) 
    SYMBOL_ITEM_FUNC(xdr_gid3) 
    SYMBOL_ITEM_FUNC(xdr_groupnode) 
    SYMBOL_ITEM_FUNC(xdr_groups) 
    SYMBOL_ITEM_FUNC(xdr_hyper) 
    SYMBOL_ITEM_FUNC(xdr_int) 
    SYMBOL_ITEM_FUNC(xdr_int16_t) 
    SYMBOL_ITEM_FUNC(xdr_int32) 
    SYMBOL_ITEM_FUNC(xdr_int32_t) 
    SYMBOL_ITEM_FUNC(xdr_int64) 
    SYMBOL_ITEM_FUNC(xdr_int64_t) 
    SYMBOL_ITEM_FUNC(xdr_int8_t) 
    SYMBOL_ITEM_FUNC(xdr_long) 
    SYMBOL_ITEM_FUNC(xdr_longlong_t) 
    SYMBOL_ITEM_FUNC(xdr_mknoddata3) 
    SYMBOL_ITEM_FUNC(xdr_mode3) 
    SYMBOL_ITEM_FUNC(xdr_mountbody) 
    SYMBOL_ITEM_FUNC(xdr_mountlist) 
    SYMBOL_ITEM_FUNC(xdr_mountres3) 
    SYMBOL_ITEM_FUNC(xdr_mountres3_ok) 
    SYMBOL_ITEM_FUNC(xdr_mountstat3) 
    SYMBOL_ITEM_FUNC(xdr_name) 
    SYMBOL_ITEM_FUNC(xdr_netobj) 
    SYMBOL_ITEM_FUNC(xdr_nfs_fh3) 
    SYMBOL_ITEM_FUNC(xdr_nfspath3) 
    SYMBOL_ITEM_FUNC(xdr_nfsstat3) 
    SYMBOL_ITEM_FUNC(xdr_nfstime3) 
    SYMBOL_ITEM_FUNC(xdr_offset3) 
    SYMBOL_ITEM_FUNC(xdr_opaque) 
    SYMBOL_ITEM_FUNC(xdr_opaque_auth) 
    SYMBOL_ITEM_FUNC(xdr_pmap) 
    SYMBOL_ITEM_FUNC(xdr_pointer) 
    SYMBOL_ITEM_FUNC(xdr_post_op_attr) 
    SYMBOL_ITEM_FUNC(xdr_post_op_fh3) 
    SYMBOL_ITEM_FUNC(xdr_pre_op_attr) 
    SYMBOL_ITEM_FUNC(xdr_reference) 
    SYMBOL_ITEM_FUNC(xdr_rejected_reply) 
    SYMBOL_ITEM_FUNC(xdr_replymsg) 
    SYMBOL_ITEM_FUNC(xdr_sattr3) 
    SYMBOL_ITEM_FUNC(xdr_sattrguard3) 
    SYMBOL_ITEM_FUNC(xdr_set_atime) 
    SYMBOL_ITEM_FUNC(xdr_set_gid3) 
    SYMBOL_ITEM_FUNC(xdr_set_mode3) 
    SYMBOL_ITEM_FUNC(xdr_set_mtime) 
    SYMBOL_ITEM_FUNC(xdr_set_size3) 
    SYMBOL_ITEM_FUNC(xdr_set_uid3) 
    SYMBOL_ITEM_FUNC(xdr_short) 
    SYMBOL_ITEM_FUNC(xdr_size3) 
    SYMBOL_ITEM_FUNC(xdr_sizeof) 
    SYMBOL_ITEM_FUNC(xdr_specdata3) 
    SYMBOL_ITEM_FUNC(xdr_stable_how) 
    SYMBOL_ITEM_FUNC(xdr_string) 
    SYMBOL_ITEM_FUNC(xdr_symlinkdata3) 
    SYMBOL_ITEM_FUNC(xdr_time_how) 
    SYMBOL_ITEM_FUNC(xdr_u_char) 
    SYMBOL_ITEM_FUNC(xdr_u_hyper) 
    SYMBOL_ITEM_FUNC(xdr_u_int) 
    SYMBOL_ITEM_FUNC(xdr_u_long) 
    SYMBOL_ITEM_FUNC(xdr_u_longlong_t) 
    SYMBOL_ITEM_FUNC(xdr_u_short) 
    SYMBOL_ITEM_FUNC(xdr_uid3) 
    SYMBOL_ITEM_FUNC(xdr_uint16_t) 
    SYMBOL_ITEM_FUNC(xdr_uint32) 
    SYMBOL_ITEM_FUNC(xdr_uint32_t) 
    SYMBOL_ITEM_FUNC(xdr_uint64) 
    SYMBOL_ITEM_FUNC(xdr_uint64_t) 
    SYMBOL_ITEM_FUNC(xdr_uint8_t) 
    SYMBOL_ITEM_FUNC(xdr_union) 
    SYMBOL_ITEM_FUNC(xdr_vector) 
    SYMBOL_ITEM_FUNC(xdr_void) 
    SYMBOL_ITEM_FUNC(xdr_wcc_attr) 
    SYMBOL_ITEM_FUNC(xdr_wcc_data) 
    SYMBOL_ITEM_FUNC(xdr_wrapstring) 
    SYMBOL_ITEM_FUNC(xdr_writeverf3) 
    SYMBOL_ITEM_FUNC(xdrmem_create) 
    SYMBOL_ITEM_FUNC(xdrrec_create) 
    SYMBOL_ITEM_FUNC(xdrrec_endofrecord) 
    SYMBOL_ITEM_FUNC(xdrrec_eof) 
    SYMBOL_ITEM_FUNC(xdrrec_skiprecord) 
    SYMBOL_ITEM_FUNC(xdrstdio_create) 
    SYMBOL_ITEM_FUNC(xmalloc) 
    SYMBOL_ITEM_FUNC(xstrdup) 
    SYMBOL_ITEM_FUNC(xstrndup) 
    SYMBOL_ITEM_FUNC(xxx_init) 
    SYMBOL_ITEM_FUNC(xzalloc) 
    SYMBOL_ITEM_FUNC(yaffs1_scan) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_invalidate) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_invalidate_stream) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_open) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_rd) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_required) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_restore) 
    SYMBOL_ITEM_FUNC(yaffs2_checkpt_wr) 
    SYMBOL_ITEM_FUNC(yaffs2_clear_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs2_find_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs2_find_refresh_block) 
    SYMBOL_ITEM_FUNC(yaffs2_get_checkpt_sum) 
    SYMBOL_ITEM_FUNC(yaffs2_handle_hole) 
    SYMBOL_ITEM_FUNC(yaffs2_scan_backwards) 
    SYMBOL_ITEM_FUNC(yaffs2_update_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs_access) 
    SYMBOL_ITEM_FUNC(yaffs_add_device) 
    SYMBOL_ITEM_FUNC(yaffs_add_find_tnode_0) 
    SYMBOL_ITEM_FUNC(yaffs_add_obj_to_dir) 
    SYMBOL_ITEM_FUNC(yaffs_addr_to_chunk) 
    SYMBOL_ITEM_FUNC(yaffs_alloc_raw_obj) 
    SYMBOL_ITEM_FUNC(yaffs_alloc_raw_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_attribs_init) 
    SYMBOL_ITEM_FUNC(yaffs_bg_gc) 
    SYMBOL_ITEM_FUNC(yaffs_block_became_dirty) 
    SYMBOL_ITEM_FUNC(yaffs_block_ok_for_gc) 
    SYMBOL_ITEM_FUNC(yaffs_calc_checkpt_blocks_required) 
    SYMBOL_ITEM_FUNC(yaffs_calc_oldest_dirty_seq) 
    SYMBOL_ITEM_FUNC(yaffs_calc_tags_ecc) 
    SYMBOL_ITEM_FUNC(yaffs_check_alloc_available) 
    SYMBOL_ITEM_FUNC(yaffs_check_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_check_ff) 
    SYMBOL_ITEM_FUNC(yaffs_check_tags_ecc) 
    SYMBOL_ITEM_FUNC(yaffs_checkpoint_save) 
    SYMBOL_ITEM_FUNC(yaffs_checkpt_close) 
    SYMBOL_ITEM_FUNC(yaffs_chmod) 
    SYMBOL_ITEM_FUNC(yaffs_chunk_del) 
    SYMBOL_ITEM_FUNC(yaffs_clear_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_clear_chunk_bits) 
    SYMBOL_ITEM_FUNC(yaffs_clone_str) 
    SYMBOL_ITEM_FUNC(yaffs_close) 
    SYMBOL_ITEM_FUNC(yaffs_closedir) 
    SYMBOL_ITEM_FUNC(yaffs_count_chunk_bits) 
    SYMBOL_ITEM_FUNC(yaffs_count_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_create_dir) 
    SYMBOL_ITEM_FUNC(yaffs_create_file) 
    SYMBOL_ITEM_FUNC(yaffs_create_special) 
    SYMBOL_ITEM_FUNC(yaffs_create_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_deinit_nand) 
    SYMBOL_ITEM_FUNC(yaffs_deinit_raw_tnodes_and_objs) 
    SYMBOL_ITEM_FUNC(yaffs_deinitialise) 
    SYMBOL_ITEM_FUNC(yaffs_del_obj) 
    SYMBOL_ITEM_FUNC(yaffs_dev_rewind) 
    SYMBOL_ITEM_FUNC(yaffs_do_file_wr) 
    SYMBOL_ITEM_FUNC(yaffs_dump_dev) 
    SYMBOL_ITEM_FUNC(yaffs_dup) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_calc) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_calc_other) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_correct) 
    SYMBOL_ITEM_FUNC(yaffs_ecc_correct_other) 
    SYMBOL_ITEM_FUNC(yaffs_erase_block) 
    SYMBOL_ITEM_FUNC(yaffs_error_to_str) 
    SYMBOL_ITEM_FUNC(yaffs_fchmod) 
    SYMBOL_ITEM_FUNC(yaffs_fdatasync) 
    SYMBOL_ITEM_FUNC(yaffs_fgetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_file_rd) 
    SYMBOL_ITEM_FUNC(yaffs_find_by_name) 
    SYMBOL_ITEM_FUNC(yaffs_find_by_number) 
    SYMBOL_ITEM_FUNC(yaffs_find_or_create_by_number) 
    SYMBOL_ITEM_FUNC(yaffs_find_tnode_0) 
    SYMBOL_ITEM_FUNC(yaffs_flistxattr) 
    SYMBOL_ITEM_FUNC(yaffs_flush) 
    SYMBOL_ITEM_FUNC(yaffs_flush_file) 
    SYMBOL_ITEM_FUNC(yaffs_flush_whole_cache) 
    SYMBOL_ITEM_FUNC(yaffs_format) 
    SYMBOL_ITEM_FUNC(yaffs_format_dev) 
    SYMBOL_ITEM_FUNC(yaffs_free_raw_obj) 
    SYMBOL_ITEM_FUNC(yaffs_free_raw_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_freespace) 
    SYMBOL_ITEM_FUNC(yaffs_fremovexattr) 
    SYMBOL_ITEM_FUNC(yaffs_fsetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_fstat) 
    SYMBOL_ITEM_FUNC(yaffs_fsync) 
    SYMBOL_ITEM_FUNC(yaffs_ftruncate) 
    SYMBOL_ITEM_FUNC(yaffs_futime) 
    SYMBOL_ITEM_FUNC(yaffs_get_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_get_equivalent_obj) 
    SYMBOL_ITEM_FUNC(yaffs_get_error) 
    SYMBOL_ITEM_FUNC(yaffs_get_group_base) 
    SYMBOL_ITEM_FUNC(yaffs_get_mtd_device) 
    SYMBOL_ITEM_FUNC(yaffs_get_n_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_inode) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_length) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_link_count) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_name) 
    SYMBOL_ITEM_FUNC(yaffs_get_obj_type) 
    SYMBOL_ITEM_FUNC(yaffs_get_symlink_alias) 
    SYMBOL_ITEM_FUNC(yaffs_get_temp_buffer) 
    SYMBOL_ITEM_FUNC(yaffs_get_tnode) 
    SYMBOL_ITEM_FUNC(yaffs_get_trace) 
    SYMBOL_ITEM_FUNC(yaffs_get_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_getdev) 
    SYMBOL_ITEM_FUNC(yaffs_getxattr) 
    SYMBOL_ITEM_FUNC(yaffs_guts_initialise) 
    SYMBOL_ITEM_FUNC(yaffs_handle_chunk_error) 
    SYMBOL_ITEM_FUNC(yaffs_handle_defered_free) 
    SYMBOL_ITEM_FUNC(yaffs_handle_shadowed_obj) 
    SYMBOL_ITEM_FUNC(yaffs_init_nand) 
    SYMBOL_ITEM_FUNC(yaffs_init_raw_tnodes_and_objs) 
    SYMBOL_ITEM_FUNC(yaffs_inodecount) 
    SYMBOL_ITEM_FUNC(yaffs_is_non_empty_dir) 
    SYMBOL_ITEM_FUNC(yaffs_lgetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_link) 
    SYMBOL_ITEM_FUNC(yaffs_link_fixup) 
    SYMBOL_ITEM_FUNC(yaffs_link_obj) 
    SYMBOL_ITEM_FUNC(yaffs_list_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_listxattr) 
    SYMBOL_ITEM_FUNC(yaffs_llistxattr) 
    SYMBOL_ITEM_FUNC(yaffs_load_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_load_attribs_oh) 
    SYMBOL_ITEM_FUNC(yaffs_load_current_time) 
    SYMBOL_ITEM_FUNC(yaffs_lost_n_found) 
    SYMBOL_ITEM_FUNC(yaffs_lremovexattr) 
    SYMBOL_ITEM_FUNC(yaffs_lseek) 
    SYMBOL_ITEM_FUNC(yaffs_lsetxattr) 
    SYMBOL_ITEM_FUNC(yaffs_lstat) 
    SYMBOL_ITEM_FUNC(yaffs_mark_bad) 
    SYMBOL_ITEM_FUNC(yaffs_max_file_size) 
    SYMBOL_ITEM_FUNC(yaffs_mkdir) 
    SYMBOL_ITEM_FUNC(yaffs_mknod) 
    SYMBOL_ITEM_FUNC(yaffs_mount) 
    SYMBOL_ITEM_FUNC(yaffs_mount2) 
    SYMBOL_ITEM_FUNC(yaffs_mount_common) 
    SYMBOL_ITEM_FUNC(yaffs_mtd_drv_install) 
    SYMBOL_ITEM_FUNC(yaffs_n_handles) 
    SYMBOL_ITEM_FUNC(yaffs_next_dev) 
    SYMBOL_ITEM_FUNC(yaffs_oh_size_load) 
    SYMBOL_ITEM_FUNC(yaffs_oh_to_size) 
    SYMBOL_ITEM_FUNC(yaffs_open) 
    SYMBOL_ITEM_FUNC(yaffs_open_sharing) 
    SYMBOL_ITEM_FUNC(yaffs_opendir) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags1) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags2) 
    SYMBOL_ITEM_FUNC(yaffs_pack_tags2_tags_only) 
    SYMBOL_ITEM_FUNC(yaffs_pread) 
    SYMBOL_ITEM_FUNC(yaffs_put_chunk_in_file) 
    SYMBOL_ITEM_FUNC(yaffs_put_mtd_device) 
    SYMBOL_ITEM_FUNC(yaffs_pwrite) 
    SYMBOL_ITEM_FUNC(yaffs_qsort) 
    SYMBOL_ITEM_FUNC(yaffs_query_init_block_state) 
    SYMBOL_ITEM_FUNC(yaffs_rd_chunk_tags_nand) 
    SYMBOL_ITEM_FUNC(yaffs_read) 
    SYMBOL_ITEM_FUNC(yaffs_readdir) 
    SYMBOL_ITEM_FUNC(yaffs_readlink) 
    SYMBOL_ITEM_FUNC(yaffs_release_temp_buffer) 
    SYMBOL_ITEM_FUNC(yaffs_remount) 
    SYMBOL_ITEM_FUNC(yaffs_remove_device) 
    SYMBOL_ITEM_FUNC(yaffs_remove_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_removexattr) 
    SYMBOL_ITEM_FUNC(yaffs_rename) 
    SYMBOL_ITEM_FUNC(yaffs_rename_obj) 
    SYMBOL_ITEM_FUNC(yaffs_resize_file) 
    SYMBOL_ITEM_FUNC(yaffs_resize_file_down) 
    SYMBOL_ITEM_FUNC(yaffs_rewinddir) 
    SYMBOL_ITEM_FUNC(yaffs_rmdir) 
    SYMBOL_ITEM_FUNC(yaffs_root) 
    SYMBOL_ITEM_FUNC(yaffs_set_attribs) 
    SYMBOL_ITEM_FUNC(yaffs_set_chunk_bit) 
    SYMBOL_ITEM_FUNC(yaffs_set_error) 
    SYMBOL_ITEM_FUNC(yaffs_set_obj_name) 
    SYMBOL_ITEM_FUNC(yaffs_set_obj_name_from_oh) 
    SYMBOL_ITEM_FUNC(yaffs_set_trace) 
    SYMBOL_ITEM_FUNC(yaffs_set_xattrib) 
    SYMBOL_ITEM_FUNC(yaffs_setxattr) 
    SYMBOL_ITEM_FUNC(yaffs_skip_rest_of_block) 
    SYMBOL_ITEM_FUNC(yaffs_skip_verification) 
    SYMBOL_ITEM_FUNC(yaffs_stat) 
    SYMBOL_ITEM_FUNC(yaffs_still_some_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_summary_add) 
    SYMBOL_ITEM_FUNC(yaffs_summary_deinit) 
    SYMBOL_ITEM_FUNC(yaffs_summary_fetch) 
    SYMBOL_ITEM_FUNC(yaffs_summary_gc) 
    SYMBOL_ITEM_FUNC(yaffs_summary_init) 
    SYMBOL_ITEM_FUNC(yaffs_summary_read) 
    SYMBOL_ITEM_FUNC(yaffs_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_sync) 
    SYMBOL_ITEM_FUNC(yaffs_tags_compat_install) 
    SYMBOL_ITEM_FUNC(yaffs_tags_marshall_install) 
    SYMBOL_ITEM_FUNC(yaffs_totalspace) 
    SYMBOL_ITEM_FUNC(yaffs_truncate) 
    SYMBOL_ITEM_FUNC(yaffs_unlink) 
    SYMBOL_ITEM_FUNC(yaffs_unlinker) 
    SYMBOL_ITEM_FUNC(yaffs_unmount) 
    SYMBOL_ITEM_FUNC(yaffs_unmount2) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags1) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags2) 
    SYMBOL_ITEM_FUNC(yaffs_unpack_tags2_tags_only) 
    SYMBOL_ITEM_FUNC(yaffs_update_dirty_dirs) 
    SYMBOL_ITEM_FUNC(yaffs_update_oh) 
    SYMBOL_ITEM_FUNC(yaffs_utime) 
    SYMBOL_ITEM_FUNC(yaffs_verify_blk) 
    SYMBOL_ITEM_FUNC(yaffs_verify_blocks) 
    SYMBOL_ITEM_FUNC(yaffs_verify_chunk_bit_id) 
    SYMBOL_ITEM_FUNC(yaffs_verify_collected_blk) 
    SYMBOL_ITEM_FUNC(yaffs_verify_dir) 
    SYMBOL_ITEM_FUNC(yaffs_verify_file) 
    SYMBOL_ITEM_FUNC(yaffs_verify_file_sane) 
    SYMBOL_ITEM_FUNC(yaffs_verify_free_chunks) 
    SYMBOL_ITEM_FUNC(yaffs_verify_link) 
    SYMBOL_ITEM_FUNC(yaffs_verify_mtd) 
    SYMBOL_ITEM_FUNC(yaffs_verify_obj) 
    SYMBOL_ITEM_FUNC(yaffs_verify_obj_in_dir) 
    SYMBOL_ITEM_FUNC(yaffs_verify_objects) 
    SYMBOL_ITEM_FUNC(yaffs_verify_oh) 
    SYMBOL_ITEM_FUNC(yaffs_verify_special) 
    SYMBOL_ITEM_FUNC(yaffs_verify_symlink) 
    SYMBOL_ITEM_FUNC(yaffs_wr_chunk_tags_nand) 
    SYMBOL_ITEM_FUNC(yaffs_wr_file) 
    SYMBOL_ITEM_FUNC(yaffs_write) 
    SYMBOL_ITEM_FUNC(yaffsfs_CheckMemRegion) 
    SYMBOL_ITEM_FUNC(yaffsfs_CurrentTime) 
    SYMBOL_ITEM_FUNC(yaffsfs_FindDevice) 
    SYMBOL_ITEM_FUNC(yaffsfs_GetLastError) 
    SYMBOL_ITEM_FUNC(yaffsfs_Init) 
    SYMBOL_ITEM_FUNC(yaffsfs_Lock) 
    SYMBOL_ITEM_FUNC(yaffsfs_Mount) 
    SYMBOL_ITEM_FUNC(yaffsfs_SetError) 
    SYMBOL_ITEM_FUNC(yaffsfs_Unlock) 
    SYMBOL_ITEM_FUNC(yaffsfs_Unmount) 
    SYMBOL_ITEM_FUNC(zero_init) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_sqi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_uqi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_shi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_uhi) 
    SYMBOL_ITEM_FUNC(__gnu_thumb1_case_si) 
    SYMBOL_ITEM_FUNC(__aeabi_uidiv) 
    SYMBOL_ITEM_FUNC(__aeabi_uidivmod) 
    SYMBOL_ITEM_FUNC(__udivsi3) 
    SYMBOL_ITEM_FUNC(__aeabi_idiv) 
    SYMBOL_ITEM_FUNC(__aeabi_idivmod) 
    SYMBOL_ITEM_FUNC(__divsi3) 
    SYMBOL_ITEM_FUNC(__umodsi3) 
    SYMBOL_ITEM_FUNC(__modsi3) 
    SYMBOL_ITEM_FUNC(_call_via_fp) 
    SYMBOL_ITEM_FUNC(_call_via_ip) 
    SYMBOL_ITEM_FUNC(_call_via_lr) 
    SYMBOL_ITEM_FUNC(_call_via_r0) 
    SYMBOL_ITEM_FUNC(_call_via_r1) 
    SYMBOL_ITEM_FUNC(_call_via_r2) 
    SYMBOL_ITEM_FUNC(_call_via_r3) 
    SYMBOL_ITEM_FUNC(_call_via_r4) 
    SYMBOL_ITEM_FUNC(_call_via_r5) 
    SYMBOL_ITEM_FUNC(_call_via_r6) 
    SYMBOL_ITEM_FUNC(_call_via_r7) 
    SYMBOL_ITEM_FUNC(_call_via_r8) 
    SYMBOL_ITEM_FUNC(_call_via_r9) 
    SYMBOL_ITEM_FUNC(_call_via_sl) 
    SYMBOL_ITEM_FUNC(_call_via_sp) 
    SYMBOL_ITEM_FUNC(_arm_return) 
    SYMBOL_ITEM_FUNC(_arm_return_r11) 
    SYMBOL_ITEM_FUNC(_arm_return_r7) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_lr) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_call_via_sp) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_r11_call_via_sp) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_fp) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_ip) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r0) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r1) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r2) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r3) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r4) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r5) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r6) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r7) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r8) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_r9) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_sl) 
    SYMBOL_ITEM_FUNC(_interwork_r7_call_via_sp) 
    SYMBOL_ITEM_FUNC(__aeabi_llsr) 
    SYMBOL_ITEM_FUNC(__lshrdi3) 
    SYMBOL_ITEM_FUNC(__aeabi_lasr) 
    SYMBOL_ITEM_FUNC(__ashrdi3) 
    SYMBOL_ITEM_FUNC(__aeabi_llsl) 
    SYMBOL_ITEM_FUNC(__ashldi3) 
    SYMBOL_ITEM_FUNC(__aeabi_dneg) 
    SYMBOL_ITEM_FUNC(__negdf2) 
    SYMBOL_ITEM_FUNC(__adddf3) 
    SYMBOL_ITEM_FUNC(__aeabi_dadd) 
    SYMBOL_ITEM_FUNC(__aeabi_drsub) 
    SYMBOL_ITEM_FUNC(__aeabi_dsub) 
    SYMBOL_ITEM_FUNC(__aeabi_f2d) 
    SYMBOL_ITEM_FUNC(__aeabi_i2d) 
    SYMBOL_ITEM_FUNC(__aeabi_l2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2d) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2d) 
    SYMBOL_ITEM_FUNC(__extendsfdf2) 
    SYMBOL_ITEM_FUNC(__floatdidf) 
    SYMBOL_ITEM_FUNC(__floatsidf) 
    SYMBOL_ITEM_FUNC(__floatundidf) 
    SYMBOL_ITEM_FUNC(__floatunsidf) 
    SYMBOL_ITEM_FUNC(__subdf3) 
    SYMBOL_ITEM_FUNC(__aeabi_ddiv) 
    SYMBOL_ITEM_FUNC(__aeabi_dmul) 
    SYMBOL_ITEM_FUNC(__divdf3) 
    SYMBOL_ITEM_FUNC(__muldf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cdcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cdrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmplt) 
    SYMBOL_ITEM_FUNC(__cmpdf2) 
    SYMBOL_ITEM_FUNC(__eqdf2) 
    SYMBOL_ITEM_FUNC(__gedf2) 
    SYMBOL_ITEM_FUNC(__gtdf2) 
    SYMBOL_ITEM_FUNC(__ledf2) 
    SYMBOL_ITEM_FUNC(__ltdf2) 
    SYMBOL_ITEM_FUNC(__nedf2) 
    SYMBOL_ITEM_FUNC(__aeabi_dcmpun) 
    SYMBOL_ITEM_FUNC(__unorddf2) 
    SYMBOL_ITEM_FUNC(__aeabi_d2iz) 
    SYMBOL_ITEM_FUNC(__fixdfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2uiz) 
    SYMBOL_ITEM_FUNC(__fixunsdfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2f) 
    SYMBOL_ITEM_FUNC(__truncdfsf2) 
    SYMBOL_ITEM_FUNC(__aeabi_fneg) 
    SYMBOL_ITEM_FUNC(__negsf2) 
    SYMBOL_ITEM_FUNC(__addsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_fadd) 
    SYMBOL_ITEM_FUNC(__aeabi_frsub) 
    SYMBOL_ITEM_FUNC(__aeabi_fsub) 
    SYMBOL_ITEM_FUNC(__aeabi_i2f) 
    SYMBOL_ITEM_FUNC(__aeabi_l2f) 
    SYMBOL_ITEM_FUNC(__aeabi_ui2f) 
    SYMBOL_ITEM_FUNC(__aeabi_ul2f) 
    SYMBOL_ITEM_FUNC(__floatdisf) 
    SYMBOL_ITEM_FUNC(__floatsisf) 
    SYMBOL_ITEM_FUNC(__floatundisf) 
    SYMBOL_ITEM_FUNC(__floatunsisf) 
    SYMBOL_ITEM_FUNC(__subsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_fdiv) 
    SYMBOL_ITEM_FUNC(__aeabi_fmul) 
    SYMBOL_ITEM_FUNC(__divsf3) 
    SYMBOL_ITEM_FUNC(__mulsf3) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_cfcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_cfrcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpeq) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpge) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpgt) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmple) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmplt) 
    SYMBOL_ITEM_FUNC(__cmpsf2) 
    SYMBOL_ITEM_FUNC(__eqsf2) 
    SYMBOL_ITEM_FUNC(__gesf2) 
    SYMBOL_ITEM_FUNC(__gtsf2) 
    SYMBOL_ITEM_FUNC(__lesf2) 
    SYMBOL_ITEM_FUNC(__ltsf2) 
    SYMBOL_ITEM_FUNC(__nesf2) 
    SYMBOL_ITEM_FUNC(__aeabi_fcmpun) 
    SYMBOL_ITEM_FUNC(__unordsf2) 
    SYMBOL_ITEM_FUNC(__aeabi_f2iz) 
    SYMBOL_ITEM_FUNC(__fixsfsi) 
    SYMBOL_ITEM_FUNC(__aeabi_f2uiz) 
    SYMBOL_ITEM_FUNC(__fixunssfsi) 
    SYMBOL_ITEM_FUNC(__clzsi2) 
    SYMBOL_ITEM_FUNC(__clzdi2) 
    SYMBOL_ITEM_FUNC(__aeabi_lcmp) 
    SYMBOL_ITEM_FUNC(__aeabi_ulcmp) 
    SYMBOL_ITEM_FUNC(__aeabi_ldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_uldivmod) 
    SYMBOL_ITEM_FUNC(__aeabi_lmul) 
    SYMBOL_ITEM_FUNC(__muldi3) 
    SYMBOL_ITEM_FUNC(__negdi2) 
    SYMBOL_ITEM_FUNC(__cmpdi2) 
    SYMBOL_ITEM_FUNC(__ucmpdi2) 
    SYMBOL_ITEM_FUNC(__clear_cache) 
    SYMBOL_ITEM_FUNC(__enable_execute_stack) 
    SYMBOL_ITEM_FUNC(__absvsi2) 
    SYMBOL_ITEM_FUNC(__absvdi2) 
    SYMBOL_ITEM_FUNC(__addvsi3) 
    SYMBOL_ITEM_FUNC(__addvdi3) 
    SYMBOL_ITEM_FUNC(__subvsi3) 
    SYMBOL_ITEM_FUNC(__subvdi3) 
    SYMBOL_ITEM_FUNC(__mulvsi3) 
    SYMBOL_ITEM_FUNC(__mulvdi3) 
    SYMBOL_ITEM_FUNC(__negvsi2) 
    SYMBOL_ITEM_FUNC(__negvdi2) 
    SYMBOL_ITEM_FUNC(__ffssi2) 
    SYMBOL_ITEM_FUNC(__ffsdi2) 
    SYMBOL_ITEM_FUNC(__ctzsi2) 
    SYMBOL_ITEM_FUNC(__ctzdi2) 
    SYMBOL_ITEM_FUNC(__popcountsi2) 
    SYMBOL_ITEM_FUNC(__popcountdi2) 
    SYMBOL_ITEM_FUNC(__paritysi2) 
    SYMBOL_ITEM_FUNC(__paritydi2) 
    SYMBOL_ITEM_FUNC(__powisf2) 
    SYMBOL_ITEM_FUNC(__powidf2) 
    SYMBOL_ITEM_FUNC(__mulsc3) 
    SYMBOL_ITEM_FUNC(__muldc3) 
    SYMBOL_ITEM_FUNC(__divsc3) 
    SYMBOL_ITEM_FUNC(__divdc3) 
    SYMBOL_ITEM_FUNC(__bswapsi2) 
    SYMBOL_ITEM_FUNC(__bswapdi2) 
    SYMBOL_ITEM_FUNC(__aeabi_f2lz) 
    SYMBOL_ITEM_FUNC(__fixsfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2lz) 
    SYMBOL_ITEM_FUNC(__fixdfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_f2ulz) 
    SYMBOL_ITEM_FUNC(__fixunssfdi) 
    SYMBOL_ITEM_FUNC(__aeabi_d2ulz) 
    SYMBOL_ITEM_FUNC(__fixunsdfdi) 
    SYMBOL_ITEM_FUNC(__eprintf) 
    SYMBOL_ITEM_FUNC(__gcc_bcmp) 
    SYMBOL_ITEM_FUNC(__divdi3) 
    SYMBOL_ITEM_FUNC(__moddi3) 
    SYMBOL_ITEM_FUNC(__udivdi3) 
    SYMBOL_ITEM_FUNC(__umoddi3) 
    SYMBOL_ITEM_FUNC(__udiv_w_sdiv) 
    SYMBOL_ITEM_FUNC(__udivmoddi4) 
    SYMBOL_ITEM_FUNC(__gnu_ldivmod_helper) 
    SYMBOL_ITEM_FUNC(__gnu_uldivmod_helper) 
    SYMBOL_ITEM_FUNC(__aeabi_uread4) 
    SYMBOL_ITEM_FUNC(__aeabi_uread8) 
    SYMBOL_ITEM_FUNC(__aeabi_uwrite4) 
    SYMBOL_ITEM_FUNC(__aeabi_uwrite8) 
    SYMBOL_ITEM_FUNC(__gnu_f2h_alternative) 
    SYMBOL_ITEM_FUNC(__gnu_f2h_ieee) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_alternative) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_ieee) 
    SYMBOL_ITEM_FUNC(__gnu_h2f_internal) 
    SYMBOL_ITEM_FUNC(_Unwind_Complete) 
    SYMBOL_ITEM_FUNC(_Unwind_DeleteException) 
    SYMBOL_ITEM_FUNC(_Unwind_GetCFA) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Get) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Pop) 
    SYMBOL_ITEM_FUNC(_Unwind_VRS_Set) 
    SYMBOL_ITEM_FUNC(__aeabi_unwind_cpp_pr0) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(_Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(_Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(_Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume) 
    SYMBOL_ITEM_FUNC(_Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(___Unwind_Backtrace) 
    SYMBOL_ITEM_FUNC(___Unwind_ForcedUnwind) 
    SYMBOL_ITEM_FUNC(___Unwind_RaiseException) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume) 
    SYMBOL_ITEM_FUNC(___Unwind_Resume_or_Rethrow) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Restore_WMMXD) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_VFP_D_16_to_31) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXC) 
    SYMBOL_ITEM_FUNC(__gnu_Unwind_Save_WMMXD) 
    SYMBOL_ITEM_FUNC(__restore_core_regs) 
    SYMBOL_ITEM_FUNC(restore_core_regs) 
    SYMBOL_ITEM_FUNC(_Unwind_GetDataRelBase) 
    SYMBOL_ITEM_FUNC(_Unwind_GetLanguageSpecificData) 
    SYMBOL_ITEM_FUNC(_Unwind_GetRegionStart) 
    SYMBOL_ITEM_FUNC(_Unwind_GetTextRelBase) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_execute) 
    SYMBOL_ITEM_FUNC(__gnu_unwind_frame) 
    SYMBOL_ITEM_FUNC(__gcc_personality_v0) 
    SYMBOL_ITEM_FUNC(__emutls_get_address) 
    SYMBOL_ITEM_FUNC(__emutls_register_common) 
    SYMBOL_ITEM_FUNC(acoshl) 
    SYMBOL_ITEM_FUNC(acosl) 
    SYMBOL_ITEM_FUNC(asinhl) 
    SYMBOL_ITEM_FUNC(asinl) 
    SYMBOL_ITEM_FUNC(atan2l) 
    SYMBOL_ITEM_FUNC(atanhl) 
    SYMBOL_ITEM_FUNC(atanl) 
    SYMBOL_ITEM_FUNC(cabs) 
    SYMBOL_ITEM_FUNC(cabsf) 
    SYMBOL_ITEM_FUNC(cacos) 
    SYMBOL_ITEM_FUNC(cacosf) 
    SYMBOL_ITEM_FUNC(cacosh) 
    SYMBOL_ITEM_FUNC(cacoshf) 
    SYMBOL_ITEM_FUNC(carg) 
    SYMBOL_ITEM_FUNC(cargf) 
    SYMBOL_ITEM_FUNC(casin) 
    SYMBOL_ITEM_FUNC(casinf) 
    SYMBOL_ITEM_FUNC(casinh) 
    SYMBOL_ITEM_FUNC(casinhf) 
    SYMBOL_ITEM_FUNC(catan) 
    SYMBOL_ITEM_FUNC(catanf) 
    SYMBOL_ITEM_FUNC(catanh) 
    SYMBOL_ITEM_FUNC(catanhf) 
    SYMBOL_ITEM_FUNC(cbrtl) 
    SYMBOL_ITEM_FUNC(ccos) 
    SYMBOL_ITEM_FUNC(ccosf) 
    SYMBOL_ITEM_FUNC(ccosh) 
    SYMBOL_ITEM_FUNC(ccoshf) 
    SYMBOL_ITEM_FUNC(ceill) 
    SYMBOL_ITEM_FUNC(_cchsh) 
    SYMBOL_ITEM_FUNC(_ctans) 
    SYMBOL_ITEM_FUNC(_redupi) 
    SYMBOL_ITEM_FUNC(_cchshf) 
    SYMBOL_ITEM_FUNC(_ctansf) 
    SYMBOL_ITEM_FUNC(_redupif) 
    SYMBOL_ITEM_FUNC(cexp) 
    SYMBOL_ITEM_FUNC(cexpf) 
    SYMBOL_ITEM_FUNC(cimag) 
    SYMBOL_ITEM_FUNC(cimagf) 
    SYMBOL_ITEM_FUNC(clog) 
    SYMBOL_ITEM_FUNC(clogf) 
    SYMBOL_ITEM_FUNC(conj) 
    SYMBOL_ITEM_FUNC(conjf) 
    SYMBOL_ITEM_FUNC(copysignl) 
    SYMBOL_ITEM_FUNC(coshl) 
    SYMBOL_ITEM_FUNC(cosl) 
    SYMBOL_ITEM_FUNC(cpow) 
    SYMBOL_ITEM_FUNC(cpowf) 
    SYMBOL_ITEM_FUNC(cproj) 
    SYMBOL_ITEM_FUNC(cprojf) 
    SYMBOL_ITEM_FUNC(creal) 
    SYMBOL_ITEM_FUNC(crealf) 
    SYMBOL_ITEM_FUNC(csin) 
    SYMBOL_ITEM_FUNC(csinf) 
    SYMBOL_ITEM_FUNC(csinh) 
    SYMBOL_ITEM_FUNC(csinhf) 
    SYMBOL_ITEM_FUNC(csqrt) 
    SYMBOL_ITEM_FUNC(csqrtf) 
    SYMBOL_ITEM_FUNC(ctan) 
    SYMBOL_ITEM_FUNC(ctanf) 
    SYMBOL_ITEM_FUNC(ctanh) 
    SYMBOL_ITEM_FUNC(ctanhf) 
    SYMBOL_ITEM_FUNC(__ieee754_acos) 
    SYMBOL_ITEM_FUNC(__ieee754_acosh) 
    SYMBOL_ITEM_FUNC(__ieee754_asin) 
    SYMBOL_ITEM_FUNC(__ieee754_atan2) 
    SYMBOL_ITEM_FUNC(__ieee754_atanh) 
    SYMBOL_ITEM_FUNC(__ieee754_cosh) 
    SYMBOL_ITEM_FUNC(__ieee754_exp) 
    SYMBOL_ITEM_FUNC(__ieee754_fmod) 
    SYMBOL_ITEM_FUNC(__ieee754_hypot) 
    SYMBOL_ITEM_FUNC(__ieee754_j0) 
    SYMBOL_ITEM_FUNC(__ieee754_y0) 
    SYMBOL_ITEM_FUNC(__ieee754_j1) 
    SYMBOL_ITEM_FUNC(__ieee754_y1) 
    SYMBOL_ITEM_FUNC(__ieee754_jn) 
    SYMBOL_ITEM_FUNC(__ieee754_yn) 
    SYMBOL_ITEM_FUNC(__ieee754_log) 
    SYMBOL_ITEM_FUNC(__ieee754_log10) 
    SYMBOL_ITEM_FUNC(__ieee754_pow) 
    SYMBOL_ITEM_FUNC(__ieee754_rem_pio2) 
    SYMBOL_ITEM_FUNC(__ieee754_remainder) 
    SYMBOL_ITEM_FUNC(__ieee754_scalb) 
    SYMBOL_ITEM_FUNC(__ieee754_sinh) 
    SYMBOL_ITEM_FUNC(__ieee754_sqrt) 
    SYMBOL_ITEM_FUNC(__ieee754_acosf) 
    SYMBOL_ITEM_FUNC(__ieee754_acoshf) 
    SYMBOL_ITEM_FUNC(__ieee754_asinf) 
    SYMBOL_ITEM_FUNC(__ieee754_atan2f) 
    SYMBOL_ITEM_FUNC(__ieee754_atanhf) 
    SYMBOL_ITEM_FUNC(__ieee754_coshf) 
    SYMBOL_ITEM_FUNC(__ieee754_expf) 
    SYMBOL_ITEM_FUNC(__ieee754_fmodf) 
    SYMBOL_ITEM_FUNC(__ieee754_hypotf) 
    SYMBOL_ITEM_FUNC(__ieee754_j0f) 
    SYMBOL_ITEM_FUNC(__ieee754_y0f) 
    SYMBOL_ITEM_FUNC(__ieee754_j1f) 
    SYMBOL_ITEM_FUNC(__ieee754_y1f) 
    SYMBOL_ITEM_FUNC(__ieee754_jnf) 
    SYMBOL_ITEM_FUNC(__ieee754_ynf) 
    SYMBOL_ITEM_FUNC(__ieee754_logf) 
    SYMBOL_ITEM_FUNC(__ieee754_log10f) 
    SYMBOL_ITEM_FUNC(__ieee754_powf) 
    SYMBOL_ITEM_FUNC(__ieee754_rem_pio2f) 
    SYMBOL_ITEM_FUNC(__ieee754_remainderf) 
    SYMBOL_ITEM_FUNC(__ieee754_scalbf) 
    SYMBOL_ITEM_FUNC(__ieee754_sinhf) 
    SYMBOL_ITEM_FUNC(__ieee754_sqrtf) 
    SYMBOL_ITEM_FUNC(__ieee754_gamma_r) 
    SYMBOL_ITEM_FUNC(__ieee754_lgamma_r) 
    SYMBOL_ITEM_FUNC(__ieee754_gammaf_r) 
    SYMBOL_ITEM_FUNC(__ieee754_lgammaf_r) 
    SYMBOL_ITEM_FUNC(erfcl) 
    SYMBOL_ITEM_FUNC(erfl) 
    SYMBOL_ITEM_FUNC(exp2l) 
    SYMBOL_ITEM_FUNC(expl) 
    SYMBOL_ITEM_FUNC(expm1l) 
    SYMBOL_ITEM_FUNC(fabsl) 
    SYMBOL_ITEM_FUNC(fdiml) 
    SYMBOL_ITEM_FUNC(floorl) 
    SYMBOL_ITEM_FUNC(fmal) 
    SYMBOL_ITEM_FUNC(fmaxl) 
    SYMBOL_ITEM_FUNC(fminl) 
    SYMBOL_ITEM_FUNC(fmodl) 
    SYMBOL_ITEM_FUNC(frexpl) 
    SYMBOL_ITEM_FUNC(hypotl) 
    SYMBOL_ITEM_FUNC(ilogbl) 
    SYMBOL_ITEM_FUNC(__kernel_cos) 
    SYMBOL_ITEM_FUNC(__kernel_rem_pio2) 
    SYMBOL_ITEM_FUNC(__kernel_sin) 
    SYMBOL_ITEM_FUNC(__kernel_standard) 
    SYMBOL_ITEM_FUNC(__kernel_tan) 
    SYMBOL_ITEM_FUNC(__kernel_cosf) 
    SYMBOL_ITEM_FUNC(__kernel_rem_pio2f) 
    SYMBOL_ITEM_FUNC(__kernel_sinf) 
    SYMBOL_ITEM_FUNC(__kernel_tanf) 
    SYMBOL_ITEM_FUNC(ldexpl) 
    SYMBOL_ITEM_FUNC(lgammal) 
    SYMBOL_ITEM_FUNC(llrintl) 
    SYMBOL_ITEM_FUNC(llroundl) 
    SYMBOL_ITEM_FUNC(log10l) 
    SYMBOL_ITEM_FUNC(log1pl) 
    SYMBOL_ITEM_FUNC(logl) 
    SYMBOL_ITEM_FUNC(lrintl) 
    SYMBOL_ITEM_FUNC(lroundl) 
    SYMBOL_ITEM_FUNC(modfl) 
    SYMBOL_ITEM_FUNC(nanl) 
    SYMBOL_ITEM_FUNC(nearbyintl) 
    SYMBOL_ITEM_FUNC(nextafterl) 
    SYMBOL_ITEM_FUNC(powl) 
    SYMBOL_ITEM_FUNC(remainderl) 
    SYMBOL_ITEM_FUNC(remquol) 
    SYMBOL_ITEM_FUNC(rintl) 
    SYMBOL_ITEM_FUNC(roundl) 
    SYMBOL_ITEM_FUNC(asinh) 
    SYMBOL_ITEM_FUNC(atan) 
    SYMBOL_ITEM_FUNC(cbrt) 
    SYMBOL_ITEM_FUNC(ceil) 
    SYMBOL_ITEM_FUNC(copysign) 
    SYMBOL_ITEM_FUNC(cos) 
    SYMBOL_ITEM_FUNC(erf) 
    SYMBOL_ITEM_FUNC(erfc) 
    SYMBOL_ITEM_FUNC(exp10) 
    SYMBOL_ITEM_FUNC(expm1) 
    SYMBOL_ITEM_FUNC(fabs) 
    SYMBOL_ITEM_FUNC(fdim) 
    SYMBOL_ITEM_FUNC(finite) 
    SYMBOL_ITEM_FUNC(floor) 
    SYMBOL_ITEM_FUNC(fma) 
    SYMBOL_ITEM_FUNC(fmax) 
    SYMBOL_ITEM_FUNC(fmin) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(frexp) 
    SYMBOL_ITEM_FUNC(ilogb) 
    SYMBOL_ITEM_FUNC(infinity) 
    SYMBOL_ITEM_FUNC(isinf) 
    SYMBOL_ITEM_FUNC(__isinfd) 
    SYMBOL_ITEM_FUNC(isnan) 
    SYMBOL_ITEM_FUNC(__isnand) 
    SYMBOL_ITEM_FUNC(ldexp) 
    SYMBOL_ITEM_FUNC(llrint) 
    SYMBOL_ITEM_FUNC(llround) 
    SYMBOL_ITEM_FUNC(log1p) 
    SYMBOL_ITEM_FUNC(log2) 
    SYMBOL_ITEM_FUNC(logb) 
    SYMBOL_ITEM_FUNC(lrint) 
    SYMBOL_ITEM_FUNC(lround) 
    SYMBOL_ITEM_FUNC(matherr) 
    SYMBOL_ITEM_FUNC(modf) 
    SYMBOL_ITEM_FUNC(nan) 
    SYMBOL_ITEM_FUNC(nearbyint) 
    SYMBOL_ITEM_FUNC(nextafter) 
    SYMBOL_ITEM_FUNC(pow10) 
    SYMBOL_ITEM_FUNC(remquo) 
    SYMBOL_ITEM_FUNC(rint) 
    SYMBOL_ITEM_FUNC(round) 
    SYMBOL_ITEM_FUNC(scalbln) 
    SYMBOL_ITEM_FUNC(scalbn) 
    SYMBOL_ITEM_FUNC(__signbitd) 
    SYMBOL_ITEM_FUNC(__signbitf) 
    SYMBOL_ITEM_FUNC(significand) 
    SYMBOL_ITEM_FUNC(sin) 
    SYMBOL_ITEM_FUNC(tan) 
    SYMBOL_ITEM_FUNC(tanh) 
    SYMBOL_ITEM_FUNC(trunc) 
    SYMBOL_ITEM_FUNC(scalblnl) 
    SYMBOL_ITEM_FUNC(scalbnl) 
    SYMBOL_ITEM_FUNC(asinhf) 
    SYMBOL_ITEM_FUNC(atanf) 
    SYMBOL_ITEM_FUNC(cbrtf) 
    SYMBOL_ITEM_FUNC(ceilf) 
    SYMBOL_ITEM_FUNC(copysignf) 
    SYMBOL_ITEM_FUNC(cosf) 
    SYMBOL_ITEM_FUNC(erfcf) 
    SYMBOL_ITEM_FUNC(erff) 
    SYMBOL_ITEM_FUNC(exp10f) 
    SYMBOL_ITEM_FUNC(expm1f) 
    SYMBOL_ITEM_FUNC(fabsf) 
    SYMBOL_ITEM_FUNC(fdimf) 
    SYMBOL_ITEM_FUNC(finitef) 
    SYMBOL_ITEM_FUNC(floorf) 
    SYMBOL_ITEM_FUNC(fmaf) 
    SYMBOL_ITEM_FUNC(fmaxf) 
    SYMBOL_ITEM_FUNC(fminf) 
    SYMBOL_ITEM_FUNC(__fpclassifyf) 
    SYMBOL_ITEM_FUNC(frexpf) 
    SYMBOL_ITEM_FUNC(ilogbf) 
    SYMBOL_ITEM_FUNC(infinityf) 
    SYMBOL_ITEM_FUNC(isinff) 
    SYMBOL_ITEM_FUNC(__isinff) 
    SYMBOL_ITEM_FUNC(isnanf) 
    SYMBOL_ITEM_FUNC(__isnanf) 
    SYMBOL_ITEM_FUNC(ldexpf) 
    SYMBOL_ITEM_FUNC(llrintf) 
    SYMBOL_ITEM_FUNC(llroundf) 
    SYMBOL_ITEM_FUNC(log1pf) 
    SYMBOL_ITEM_FUNC(log2f) 
    SYMBOL_ITEM_FUNC(logbf) 
    SYMBOL_ITEM_FUNC(lrintf) 
    SYMBOL_ITEM_FUNC(lroundf) 
    SYMBOL_ITEM_FUNC(modff) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(nearbyintf) 
    SYMBOL_ITEM_FUNC(nextafterf) 
    SYMBOL_ITEM_FUNC(pow10f) 
    SYMBOL_ITEM_FUNC(remquof) 
    SYMBOL_ITEM_FUNC(rintf) 
    SYMBOL_ITEM_FUNC(roundf) 
    SYMBOL_ITEM_FUNC(scalblnf) 
    SYMBOL_ITEM_FUNC(scalbnf) 
    SYMBOL_ITEM_FUNC(significandf) 
    SYMBOL_ITEM_FUNC(sinf) 
    SYMBOL_ITEM_FUNC(tanf) 
    SYMBOL_ITEM_FUNC(tanhf) 
    SYMBOL_ITEM_FUNC(truncf) 
    SYMBOL_ITEM_FUNC(sinhl) 
    SYMBOL_ITEM_FUNC(sinl) 
    SYMBOL_ITEM_FUNC(sqrtl) 
    SYMBOL_ITEM_FUNC(tanhl) 
    SYMBOL_ITEM_FUNC(tanl) 
    SYMBOL_ITEM_FUNC(tgammal) 
    SYMBOL_ITEM_FUNC(truncl) 
    SYMBOL_ITEM_FUNC(acos) 
    SYMBOL_ITEM_FUNC(acosh) 
    SYMBOL_ITEM_FUNC(asin) 
    SYMBOL_ITEM_FUNC(atan2) 
    SYMBOL_ITEM_FUNC(atanh) 
    SYMBOL_ITEM_FUNC(cosh) 
    SYMBOL_ITEM_FUNC(drem) 
    SYMBOL_ITEM_FUNC(exp) 
    SYMBOL_ITEM_FUNC(exp2) 
    SYMBOL_ITEM_FUNC(fmod) 
    SYMBOL_ITEM_FUNC(gamma) 
    SYMBOL_ITEM_FUNC(hypot) 
    SYMBOL_ITEM_FUNC(j0) 
    SYMBOL_ITEM_FUNC(y0) 
    SYMBOL_ITEM_FUNC(j1) 
    SYMBOL_ITEM_FUNC(y1) 
    SYMBOL_ITEM_FUNC(jn) 
    SYMBOL_ITEM_FUNC(yn) 
    SYMBOL_ITEM_FUNC(lgamma) 
    SYMBOL_ITEM_FUNC(log) 
    SYMBOL_ITEM_FUNC(log10) 
    SYMBOL_ITEM_FUNC(pow) 
    SYMBOL_ITEM_FUNC(remainder) 
    SYMBOL_ITEM_FUNC(scalb) 
    SYMBOL_ITEM_FUNC(sincos) 
    SYMBOL_ITEM_FUNC(sinh) 
    SYMBOL_ITEM_FUNC(sqrt) 
    SYMBOL_ITEM_FUNC(tgamma) 
    SYMBOL_ITEM_FUNC(acosf) 
    SYMBOL_ITEM_FUNC(acoshf) 
    SYMBOL_ITEM_FUNC(asinf) 
    SYMBOL_ITEM_FUNC(atan2f) 
    SYMBOL_ITEM_FUNC(atanhf) 
    SYMBOL_ITEM_FUNC(coshf) 
    SYMBOL_ITEM_FUNC(dremf) 
    SYMBOL_ITEM_FUNC(expf) 
    SYMBOL_ITEM_FUNC(exp2f) 
    SYMBOL_ITEM_FUNC(fmodf) 
    SYMBOL_ITEM_FUNC(gammaf) 
    SYMBOL_ITEM_FUNC(hypotf) 
    SYMBOL_ITEM_FUNC(j0f) 
    SYMBOL_ITEM_FUNC(y0f) 
    SYMBOL_ITEM_FUNC(j1f) 
    SYMBOL_ITEM_FUNC(y1f) 
    SYMBOL_ITEM_FUNC(jnf) 
    SYMBOL_ITEM_FUNC(ynf) 
    SYMBOL_ITEM_FUNC(lgammaf) 
    SYMBOL_ITEM_FUNC(logf) 
    SYMBOL_ITEM_FUNC(log10f) 
    SYMBOL_ITEM_FUNC(powf) 
    SYMBOL_ITEM_FUNC(remainderf) 
    SYMBOL_ITEM_FUNC(scalbf) 
    SYMBOL_ITEM_FUNC(sincosf) 
    SYMBOL_ITEM_FUNC(sinhf) 
    SYMBOL_ITEM_FUNC(sqrtf) 
    SYMBOL_ITEM_FUNC(tgammaf) 
    SYMBOL_ITEM_FUNC(gamma_r) 
    SYMBOL_ITEM_FUNC(lgamma_r) 
    SYMBOL_ITEM_FUNC(gammaf_r) 
    SYMBOL_ITEM_FUNC(lgammaf_r) 
    SYMBOL_ITEM_FUNC(__adjust) 
    SYMBOL_ITEM_FUNC(__register_exitproc) 
    SYMBOL_ITEM_FUNC(__call_exitprocs) 
    SYMBOL_ITEM_FUNC(__dprintf) 
    SYMBOL_ITEM_FUNC(__exp10) 
    SYMBOL_ITEM_FUNC(__ten_mul) 
    SYMBOL_ITEM_FUNC(_Exit) 
    SYMBOL_ITEM_FUNC(a64l) 
    SYMBOL_ITEM_FUNC(abort) 
    SYMBOL_ITEM_FUNC(abs) 
    SYMBOL_ITEM_FUNC(access) 
    SYMBOL_ITEM_FUNC(__aeabi_atexit) 
    SYMBOL_ITEM_FUNC(argz_add) 
    SYMBOL_ITEM_FUNC(argz_add_sep) 
    SYMBOL_ITEM_FUNC(argz_append) 
    SYMBOL_ITEM_FUNC(argz_count) 
    SYMBOL_ITEM_FUNC(argz_create) 
    SYMBOL_ITEM_FUNC(argz_create_sep) 
    SYMBOL_ITEM_FUNC(argz_delete) 
    SYMBOL_ITEM_FUNC(argz_extract) 
    SYMBOL_ITEM_FUNC(argz_insert) 
    SYMBOL_ITEM_FUNC(argz_next) 
    SYMBOL_ITEM_FUNC(argz_replace) 
    SYMBOL_ITEM_FUNC(argz_stringify) 
    SYMBOL_ITEM_FUNC(asctime) 
    SYMBOL_ITEM_FUNC(asctime_r) 
    SYMBOL_ITEM_FUNC(_asiprintf_r) 
    SYMBOL_ITEM_FUNC(asiprintf) 
    SYMBOL_ITEM_FUNC(_asniprintf_r) 
    SYMBOL_ITEM_FUNC(asniprintf) 
    SYMBOL_ITEM_FUNC(_asnprintf_r) 
    SYMBOL_ITEM_FUNC(asnprintf) 
    SYMBOL_ITEM_FUNC(_asprintf_r) 
    SYMBOL_ITEM_FUNC(asprintf) 
    SYMBOL_ITEM_FUNC(__assert) 
    SYMBOL_ITEM_FUNC(__assert_func) 
    SYMBOL_ITEM_FUNC(atexit) 
    SYMBOL_ITEM_FUNC(atof) 
    SYMBOL_ITEM_FUNC(atoff) 
    SYMBOL_ITEM_FUNC(_atoi_r) 
    SYMBOL_ITEM_FUNC(atoi) 
    SYMBOL_ITEM_FUNC(_atol_r) 
    SYMBOL_ITEM_FUNC(atol) 
    SYMBOL_ITEM_FUNC(_atoll_r) 
    SYMBOL_ITEM_FUNC(atoll) 
    SYMBOL_ITEM_FUNC(bcmp) 
    SYMBOL_ITEM_FUNC(bcopy) 
    SYMBOL_ITEM_FUNC(bsearch) 
    SYMBOL_ITEM_FUNC(btowc) 
    SYMBOL_ITEM_FUNC(_buf_findstr) 
    SYMBOL_ITEM_FUNC(bzero) 
    SYMBOL_ITEM_FUNC(calloc) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(cfree) 
    SYMBOL_ITEM_FUNC(clearerr) 
    SYMBOL_ITEM_FUNC(clock) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(ctime) 
    SYMBOL_ITEM_FUNC(ctime_r) 
    SYMBOL_ITEM_FUNC(__cxa_atexit) 
    SYMBOL_ITEM_FUNC(__cxa_finalize) 
    SYMBOL_ITEM_FUNC(difftime) 
    SYMBOL_ITEM_FUNC(_diprintf_r) 
    SYMBOL_ITEM_FUNC(diprintf) 
    SYMBOL_ITEM_FUNC(div) 
    SYMBOL_ITEM_FUNC(_dprintf_r) 
    SYMBOL_ITEM_FUNC(dprintf) 
    SYMBOL_ITEM_FUNC(_drand48_r) 
    SYMBOL_ITEM_FUNC(drand48) 
    SYMBOL_ITEM_FUNC(_dtoa_r) 
    SYMBOL_ITEM_FUNC(__dtoa) 
    SYMBOL_ITEM_FUNC(_dcvt) 
    SYMBOL_ITEM_FUNC(_gcvt) 
    SYMBOL_ITEM_FUNC(ecvtbuf) 
    SYMBOL_ITEM_FUNC(fcvtbuf) 
    SYMBOL_ITEM_FUNC(ecvt) 
    SYMBOL_ITEM_FUNC(ecvtf) 
    SYMBOL_ITEM_FUNC(fcvt) 
    SYMBOL_ITEM_FUNC(fcvtf) 
    SYMBOL_ITEM_FUNC(gcvt) 
    SYMBOL_ITEM_FUNC(gcvtf) 
    SYMBOL_ITEM_FUNC(__env_lock) 
    SYMBOL_ITEM_FUNC(__env_unlock) 
    SYMBOL_ITEM_FUNC(envz_add) 
    SYMBOL_ITEM_FUNC(envz_entry) 
    SYMBOL_ITEM_FUNC(envz_get) 
    SYMBOL_ITEM_FUNC(envz_merge) 
    SYMBOL_ITEM_FUNC(envz_remove) 
    SYMBOL_ITEM_FUNC(envz_strip) 
    SYMBOL_ITEM_FUNC(__eprintf) 
    SYMBOL_ITEM_FUNC(_erand48_r) 
    SYMBOL_ITEM_FUNC(erand48) 
    SYMBOL_ITEM_FUNC(__errno) 
    SYMBOL_ITEM_FUNC(_execve_r) 
    SYMBOL_ITEM_FUNC(_fork_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(exit) 
    SYMBOL_ITEM_FUNC(_fclose_r) 
    SYMBOL_ITEM_FUNC(fclose) 
    SYMBOL_ITEM_FUNC(_fcloseall_r) 
    SYMBOL_ITEM_FUNC(fcloseall) 
    SYMBOL_ITEM_FUNC(_fdopen_r) 
    SYMBOL_ITEM_FUNC(fdopen) 
    SYMBOL_ITEM_FUNC(feof) 
    SYMBOL_ITEM_FUNC(ferror) 
    SYMBOL_ITEM_FUNC(_fflush_r) 
    SYMBOL_ITEM_FUNC(fflush) 
    SYMBOL_ITEM_FUNC(ffs) 
    SYMBOL_ITEM_FUNC(_fgetc_r) 
    SYMBOL_ITEM_FUNC(fgetc) 
    SYMBOL_ITEM_FUNC(_fgetpos_r) 
    SYMBOL_ITEM_FUNC(fgetpos) 
    SYMBOL_ITEM_FUNC(_fgets_r) 
    SYMBOL_ITEM_FUNC(fgets) 
    SYMBOL_ITEM_FUNC(_fgetwc_r) 
    SYMBOL_ITEM_FUNC(fgetwc) 
    SYMBOL_ITEM_FUNC(_fgetws_r) 
    SYMBOL_ITEM_FUNC(fgetws) 
    SYMBOL_ITEM_FUNC(fileno) 
    SYMBOL_ITEM_FUNC(__fp_lock_all) 
    SYMBOL_ITEM_FUNC(__fp_unlock_all) 
    SYMBOL_ITEM_FUNC(__sfmoreglue) 
    SYMBOL_ITEM_FUNC(__sfp) 
    SYMBOL_ITEM_FUNC(__sfp_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sfp_lock_release) 
    SYMBOL_ITEM_FUNC(__sinit) 
    SYMBOL_ITEM_FUNC(__sinit_lock_acquire) 
    SYMBOL_ITEM_FUNC(__sinit_lock_release) 
    SYMBOL_ITEM_FUNC(_cleanup) 
    SYMBOL_ITEM_FUNC(_cleanup_r) 
    SYMBOL_ITEM_FUNC(__libc_fini_array) 
    SYMBOL_ITEM_FUNC(_fiprintf_r) 
    SYMBOL_ITEM_FUNC(fiprintf) 
    SYMBOL_ITEM_FUNC(_fiscanf_r) 
    SYMBOL_ITEM_FUNC(fiscanf) 
    SYMBOL_ITEM_FUNC(__fix_locale_grouping_str) 
    SYMBOL_ITEM_FUNC(__sflags) 
    SYMBOL_ITEM_FUNC(_fmemopen_r) 
    SYMBOL_ITEM_FUNC(fmemopen) 
    SYMBOL_ITEM_FUNC(_fopen_r) 
    SYMBOL_ITEM_FUNC(fopen) 
    SYMBOL_ITEM_FUNC(_fopencookie_r) 
    SYMBOL_ITEM_FUNC(fopencookie) 
    SYMBOL_ITEM_FUNC(_fprintf_r) 
    SYMBOL_ITEM_FUNC(fprintf) 
    SYMBOL_ITEM_FUNC(_fpurge_r) 
    SYMBOL_ITEM_FUNC(fpurge) 
    SYMBOL_ITEM_FUNC(_fputc_r) 
    SYMBOL_ITEM_FUNC(fputc) 
    SYMBOL_ITEM_FUNC(_fputs_r) 
    SYMBOL_ITEM_FUNC(fputs) 
    SYMBOL_ITEM_FUNC(_fputwc_r) 
    SYMBOL_ITEM_FUNC(fputwc) 
    SYMBOL_ITEM_FUNC(_fputws_r) 
    SYMBOL_ITEM_FUNC(fputws) 
    SYMBOL_ITEM_FUNC(_fread_r) 
    SYMBOL_ITEM_FUNC(fread) 
    SYMBOL_ITEM_FUNC(_freopen_r) 
    SYMBOL_ITEM_FUNC(freopen) 
    SYMBOL_ITEM_FUNC(_fscanf_r) 
    SYMBOL_ITEM_FUNC(fscanf) 
    SYMBOL_ITEM_FUNC(_fseek_r) 
    SYMBOL_ITEM_FUNC(fseek) 
    SYMBOL_ITEM_FUNC(_fseeko_r) 
    SYMBOL_ITEM_FUNC(fseeko) 
    SYMBOL_ITEM_FUNC(_fsetpos_r) 
    SYMBOL_ITEM_FUNC(fsetpos) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_ftell_r) 
    SYMBOL_ITEM_FUNC(ftell) 
    SYMBOL_ITEM_FUNC(_ftello_r) 
    SYMBOL_ITEM_FUNC(ftello) 
    SYMBOL_ITEM_FUNC(_funopen_r) 
    SYMBOL_ITEM_FUNC(funopen) 
    SYMBOL_ITEM_FUNC(__sfvwrite_r) 
    SYMBOL_ITEM_FUNC(_fwalk) 
    SYMBOL_ITEM_FUNC(_fwalk_reent) 
    SYMBOL_ITEM_FUNC(_fwide_r) 
    SYMBOL_ITEM_FUNC(fwide) 
    SYMBOL_ITEM_FUNC(_fwprintf_r) 
    SYMBOL_ITEM_FUNC(fwprintf) 
    SYMBOL_ITEM_FUNC(_fwrite_r) 
    SYMBOL_ITEM_FUNC(fwrite) 
    SYMBOL_ITEM_FUNC(_fwscanf_r) 
    SYMBOL_ITEM_FUNC(fwscanf) 
    SYMBOL_ITEM_FUNC(__gethex) 
    SYMBOL_ITEM_FUNC(__hexdig_init) 
    SYMBOL_ITEM_FUNC(__hexnan) 
    SYMBOL_ITEM_FUNC(_getc_r) 
    SYMBOL_ITEM_FUNC(getc) 
    SYMBOL_ITEM_FUNC(_getc_unlocked_r) 
    SYMBOL_ITEM_FUNC(getc_unlocked) 
    SYMBOL_ITEM_FUNC(_getchar_r) 
    SYMBOL_ITEM_FUNC(getchar) 
    SYMBOL_ITEM_FUNC(_getchar_unlocked_r) 
    SYMBOL_ITEM_FUNC(getchar_unlocked) 
    SYMBOL_ITEM_FUNC(__getdelim) 
    SYMBOL_ITEM_FUNC(_findenv) 
    SYMBOL_ITEM_FUNC(getenv) 
    SYMBOL_ITEM_FUNC(_findenv_r) 
    SYMBOL_ITEM_FUNC(_getenv_r) 
    SYMBOL_ITEM_FUNC(__getline) 
    SYMBOL_ITEM_FUNC(__getopt_long_only_r) 
    SYMBOL_ITEM_FUNC(__getopt_long_r) 
    SYMBOL_ITEM_FUNC(__getopt_r) 
    SYMBOL_ITEM_FUNC(getopt) 
    SYMBOL_ITEM_FUNC(getopt_long) 
    SYMBOL_ITEM_FUNC(getopt_long_only) 
    SYMBOL_ITEM_FUNC(__getreent) 
    SYMBOL_ITEM_FUNC(_gets_r) 
    SYMBOL_ITEM_FUNC(gets) 
    SYMBOL_ITEM_FUNC(getsubopt) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(__gettzinfo) 
    SYMBOL_ITEM_FUNC(getw) 
    SYMBOL_ITEM_FUNC(_getwc_r) 
    SYMBOL_ITEM_FUNC(getwc) 
    SYMBOL_ITEM_FUNC(_getwchar_r) 
    SYMBOL_ITEM_FUNC(getwchar) 
    SYMBOL_ITEM_FUNC(gmtime) 
    SYMBOL_ITEM_FUNC(gmtime_r) 
    SYMBOL_ITEM_FUNC(__call_hash) 
    SYMBOL_ITEM_FUNC(__expand_table) 
    SYMBOL_ITEM_FUNC(__hash_open) 
    SYMBOL_ITEM_FUNC(__big_delete) 
    SYMBOL_ITEM_FUNC(__big_insert) 
    SYMBOL_ITEM_FUNC(__big_keydata) 
    SYMBOL_ITEM_FUNC(__big_return) 
    SYMBOL_ITEM_FUNC(__big_split) 
    SYMBOL_ITEM_FUNC(__find_bigpair) 
    SYMBOL_ITEM_FUNC(__find_last_page) 
    SYMBOL_ITEM_FUNC(__buf_free) 
    SYMBOL_ITEM_FUNC(__buf_init) 
    SYMBOL_ITEM_FUNC(__get_buf) 
    SYMBOL_ITEM_FUNC(__reclaim_buf) 
    SYMBOL_ITEM_FUNC(__log2) 
    SYMBOL_ITEM_FUNC(__add_ovflpage) 
    SYMBOL_ITEM_FUNC(__addel) 
    SYMBOL_ITEM_FUNC(__delpair) 
    SYMBOL_ITEM_FUNC(__free_ovflpage) 
    SYMBOL_ITEM_FUNC(__get_page) 
    SYMBOL_ITEM_FUNC(__ibitmap) 
    SYMBOL_ITEM_FUNC(__put_page) 
    SYMBOL_ITEM_FUNC(__split_page) 
    SYMBOL_ITEM_FUNC(hcreate) 
    SYMBOL_ITEM_FUNC(hdestroy) 
    SYMBOL_ITEM_FUNC(hsearch) 
    SYMBOL_ITEM_FUNC(hcreate_r) 
    SYMBOL_ITEM_FUNC(hdestroy_r) 
    SYMBOL_ITEM_FUNC(hsearch_r) 
    SYMBOL_ITEM_FUNC(index) 
    SYMBOL_ITEM_FUNC(__libc_init_array) 
    SYMBOL_ITEM_FUNC(_iprintf_r) 
    SYMBOL_ITEM_FUNC(iprintf) 
    SYMBOL_ITEM_FUNC(isalnum) 
    SYMBOL_ITEM_FUNC(isalpha) 
    SYMBOL_ITEM_FUNC(isascii) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(isblank) 
    SYMBOL_ITEM_FUNC(_iscanf_r) 
    SYMBOL_ITEM_FUNC(iscanf) 
    SYMBOL_ITEM_FUNC(iscntrl) 
    SYMBOL_ITEM_FUNC(isdigit) 
    SYMBOL_ITEM_FUNC(islower) 
    SYMBOL_ITEM_FUNC(isgraph) 
    SYMBOL_ITEM_FUNC(isprint) 
    SYMBOL_ITEM_FUNC(ispunct) 
    SYMBOL_ITEM_FUNC(isspace) 
    SYMBOL_ITEM_FUNC(isupper) 
    SYMBOL_ITEM_FUNC(iswalnum) 
    SYMBOL_ITEM_FUNC(iswalpha) 
    SYMBOL_ITEM_FUNC(iswblank) 
    SYMBOL_ITEM_FUNC(iswcntrl) 
    SYMBOL_ITEM_FUNC(iswctype) 
    SYMBOL_ITEM_FUNC(iswdigit) 
    SYMBOL_ITEM_FUNC(iswgraph) 
    SYMBOL_ITEM_FUNC(iswlower) 
    SYMBOL_ITEM_FUNC(iswprint) 
    SYMBOL_ITEM_FUNC(iswpunct) 
    SYMBOL_ITEM_FUNC(iswspace) 
    SYMBOL_ITEM_FUNC(iswupper) 
    SYMBOL_ITEM_FUNC(iswxdigit) 
    SYMBOL_ITEM_FUNC(isxdigit) 
    SYMBOL_ITEM_FUNC(_jrand48_r) 
    SYMBOL_ITEM_FUNC(jrand48) 
    SYMBOL_ITEM_FUNC(_l64a_r) 
    SYMBOL_ITEM_FUNC(l64a) 
    SYMBOL_ITEM_FUNC(labs) 
    SYMBOL_ITEM_FUNC(localtime) 
    SYMBOL_ITEM_FUNC(localtime_r) 
    SYMBOL_ITEM_FUNC(_lcong48_r) 
    SYMBOL_ITEM_FUNC(lcong48) 
    SYMBOL_ITEM_FUNC(__ctype_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_ctype_locale) 
    SYMBOL_ITEM_FUNC(ldiv) 
    SYMBOL_ITEM_FUNC(__part_load_locale) 
    SYMBOL_ITEM_FUNC(_ldcheck) 
    SYMBOL_ITEM_FUNC(_ldtoa_r) 
    SYMBOL_ITEM_FUNC(_strtold) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(llabs) 
    SYMBOL_ITEM_FUNC(lldiv) 
    SYMBOL_ITEM_FUNC(__get_current_messages_locale) 
    SYMBOL_ITEM_FUNC(__messages_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_monetary_locale) 
    SYMBOL_ITEM_FUNC(__monetary_load_locale) 
    SYMBOL_ITEM_FUNC(__get_current_numeric_locale) 
    SYMBOL_ITEM_FUNC(__numeric_load_locale) 
    SYMBOL_ITEM_FUNC(__locale_charset) 
    SYMBOL_ITEM_FUNC(__locale_cjk_lang) 
    SYMBOL_ITEM_FUNC(__locale_mb_cur_max) 
    SYMBOL_ITEM_FUNC(__locale_msgcharset) 
    SYMBOL_ITEM_FUNC(_localeconv_r) 
    SYMBOL_ITEM_FUNC(_setlocale_r) 
    SYMBOL_ITEM_FUNC(localeconv) 
    SYMBOL_ITEM_FUNC(setlocale) 
    SYMBOL_ITEM_FUNC(_lrand48_r) 
    SYMBOL_ITEM_FUNC(lrand48) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(__smakebuf_r) 
    SYMBOL_ITEM_FUNC(memalign) 
    SYMBOL_ITEM_FUNC(_memalign_r) 
    SYMBOL_ITEM_FUNC(free) 
    SYMBOL_ITEM_FUNC(malloc) 
    SYMBOL_ITEM_FUNC(mblen) 
    SYMBOL_ITEM_FUNC(_mblen_r) 
    SYMBOL_ITEM_FUNC(mbrlen) 
    SYMBOL_ITEM_FUNC(_mbrtowc_r) 
    SYMBOL_ITEM_FUNC(mbrtowc) 
    SYMBOL_ITEM_FUNC(mbsinit) 
    SYMBOL_ITEM_FUNC(_mbsnrtowcs_r) 
    SYMBOL_ITEM_FUNC(mbsnrtowcs) 
    SYMBOL_ITEM_FUNC(_mbsrtowcs_r) 
    SYMBOL_ITEM_FUNC(mbsrtowcs) 
    SYMBOL_ITEM_FUNC(mbstowcs) 
    SYMBOL_ITEM_FUNC(_mbstowcs_r) 
    SYMBOL_ITEM_FUNC(mbtowc) 
    SYMBOL_ITEM_FUNC(__ascii_mbtowc) 
    SYMBOL_ITEM_FUNC(_mbtowc_r) 
    SYMBOL_ITEM_FUNC(memccpy) 
    SYMBOL_ITEM_FUNC(memchr) 
    SYMBOL_ITEM_FUNC(memcmp) 
    SYMBOL_ITEM_FUNC(memcpy) 
    SYMBOL_ITEM_FUNC(memmem) 
    SYMBOL_ITEM_FUNC(memmove) 
    SYMBOL_ITEM_FUNC(mempcpy) 
    SYMBOL_ITEM_FUNC(memset) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_mkdtemp_r) 
    SYMBOL_ITEM_FUNC(_mkostemp_r) 
    SYMBOL_ITEM_FUNC(_mkostemps_r) 
    SYMBOL_ITEM_FUNC(_mkstemp_r) 
    SYMBOL_ITEM_FUNC(_mkstemps_r) 
    SYMBOL_ITEM_FUNC(_mktemp_r) 
    SYMBOL_ITEM_FUNC(mkdtemp) 
    SYMBOL_ITEM_FUNC(mkostemp) 
    SYMBOL_ITEM_FUNC(mkostemps) 
    SYMBOL_ITEM_FUNC(mkstemp) 
    SYMBOL_ITEM_FUNC(mkstemps) 
    SYMBOL_ITEM_FUNC(mktemp) 
    SYMBOL_ITEM_FUNC(mktime) 
    SYMBOL_ITEM_FUNC(__tzcalc_limits) 
    SYMBOL_ITEM_FUNC(_mktm_r) 
    SYMBOL_ITEM_FUNC(__malloc_lock) 
    SYMBOL_ITEM_FUNC(__malloc_unlock) 
    SYMBOL_ITEM_FUNC(_Balloc) 
    SYMBOL_ITEM_FUNC(_Bfree) 
    SYMBOL_ITEM_FUNC(__any_on) 
    SYMBOL_ITEM_FUNC(__b2d) 
    SYMBOL_ITEM_FUNC(__copybits) 
    SYMBOL_ITEM_FUNC(__d2b) 
    SYMBOL_ITEM_FUNC(__hi0bits) 
    SYMBOL_ITEM_FUNC(__i2b) 
    SYMBOL_ITEM_FUNC(__lo0bits) 
    SYMBOL_ITEM_FUNC(__lshift) 
    SYMBOL_ITEM_FUNC(__mcmp) 
    SYMBOL_ITEM_FUNC(__mdiff) 
    SYMBOL_ITEM_FUNC(__multadd) 
    SYMBOL_ITEM_FUNC(__multiply) 
    SYMBOL_ITEM_FUNC(__pow5mult) 
    SYMBOL_ITEM_FUNC(__ratio) 
    SYMBOL_ITEM_FUNC(__s2b) 
    SYMBOL_ITEM_FUNC(__ulp) 
    SYMBOL_ITEM_FUNC(_mprec_log10) 
    SYMBOL_ITEM_FUNC(_mrand48_r) 
    SYMBOL_ITEM_FUNC(mrand48) 
    SYMBOL_ITEM_FUNC(malloc_usable_size) 
    SYMBOL_ITEM_FUNC(_malloc_usable_size_r) 
    SYMBOL_ITEM_FUNC(nl_langinfo) 
    SYMBOL_ITEM_FUNC(_nrand48_r) 
    SYMBOL_ITEM_FUNC(nrand48) 
    SYMBOL_ITEM_FUNC(on_exit) 
    SYMBOL_ITEM_FUNC(_open_memstream_r) 
    SYMBOL_ITEM_FUNC(_open_wmemstream_r) 
    SYMBOL_ITEM_FUNC(open_memstream) 
    SYMBOL_ITEM_FUNC(open_wmemstream) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_perror_r) 
    SYMBOL_ITEM_FUNC(perror) 
    SYMBOL_ITEM_FUNC(_printf_r) 
    SYMBOL_ITEM_FUNC(printf) 
    SYMBOL_ITEM_FUNC(_putc_r) 
    SYMBOL_ITEM_FUNC(putc) 
    SYMBOL_ITEM_FUNC(_putc_unlocked_r) 
    SYMBOL_ITEM_FUNC(putc_unlocked) 
    SYMBOL_ITEM_FUNC(_putchar_r) 
    SYMBOL_ITEM_FUNC(putchar) 
    SYMBOL_ITEM_FUNC(_putchar_unlocked_r) 
    SYMBOL_ITEM_FUNC(putchar_unlocked) 
    SYMBOL_ITEM_FUNC(putenv) 
    SYMBOL_ITEM_FUNC(_putenv_r) 
    SYMBOL_ITEM_FUNC(_puts_r) 
    SYMBOL_ITEM_FUNC(puts) 
    SYMBOL_ITEM_FUNC(putw) 
    SYMBOL_ITEM_FUNC(_putwc_r) 
    SYMBOL_ITEM_FUNC(putwc) 
    SYMBOL_ITEM_FUNC(_putwchar_r) 
    SYMBOL_ITEM_FUNC(putwchar) 
    SYMBOL_ITEM_FUNC(_pvalloc_r) 
    SYMBOL_ITEM_FUNC(qsort) 
    SYMBOL_ITEM_FUNC(rand) 
    SYMBOL_ITEM_FUNC(srand) 
    SYMBOL_ITEM_FUNC(__dorand48) 
    SYMBOL_ITEM_FUNC(rand_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(realloc) 
    SYMBOL_ITEM_FUNC(_reallocf_r) 
    SYMBOL_ITEM_FUNC(reallocf) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(_reclaim_reent) 
    SYMBOL_ITEM_FUNC(_wrapup_reent) 
    SYMBOL_ITEM_FUNC(cleanup_glue) 
    SYMBOL_ITEM_FUNC(__srefill_r) 
    SYMBOL_ITEM_FUNC(_remove_r) 
    SYMBOL_ITEM_FUNC(remove) 
    SYMBOL_ITEM_FUNC(rename) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rewind_r) 
    SYMBOL_ITEM_FUNC(rewind) 
    SYMBOL_ITEM_FUNC(__srget) 
    SYMBOL_ITEM_FUNC(__srget_r) 
    SYMBOL_ITEM_FUNC(rindex) 
    SYMBOL_ITEM_FUNC(copysign) 
    SYMBOL_ITEM_FUNC(finite) 
    SYMBOL_ITEM_FUNC(__fpclassifyd) 
    SYMBOL_ITEM_FUNC(frexp) 
    SYMBOL_ITEM_FUNC(isinf) 
    SYMBOL_ITEM_FUNC(__isinfd) 
    SYMBOL_ITEM_FUNC(isnan) 
    SYMBOL_ITEM_FUNC(__isnand) 
    SYMBOL_ITEM_FUNC(ldexp) 
    SYMBOL_ITEM_FUNC(modf) 
    SYMBOL_ITEM_FUNC(nan) 
    SYMBOL_ITEM_FUNC(scalbn) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_scanf_r) 
    SYMBOL_ITEM_FUNC(scanf) 
    SYMBOL_ITEM_FUNC(__sccl) 
    SYMBOL_ITEM_FUNC(_seed48_r) 
    SYMBOL_ITEM_FUNC(seed48) 
    SYMBOL_ITEM_FUNC(setbuf) 
    SYMBOL_ITEM_FUNC(setbuffer) 
    SYMBOL_ITEM_FUNC(setenv) 
    SYMBOL_ITEM_FUNC(unsetenv) 
    SYMBOL_ITEM_FUNC(_setenv_r) 
    SYMBOL_ITEM_FUNC(_unsetenv_r) 
    SYMBOL_ITEM_FUNC(longjmp) 
    SYMBOL_ITEM_FUNC(setjmp) 
    SYMBOL_ITEM_FUNC(setlinebuf) 
    SYMBOL_ITEM_FUNC(setvbuf) 
    SYMBOL_ITEM_FUNC(copysignf) 
    SYMBOL_ITEM_FUNC(finitef) 
    SYMBOL_ITEM_FUNC(__fpclassifyf) 
    SYMBOL_ITEM_FUNC(frexpf) 
    SYMBOL_ITEM_FUNC(isinff) 
    SYMBOL_ITEM_FUNC(__isinff) 
    SYMBOL_ITEM_FUNC(isnanf) 
    SYMBOL_ITEM_FUNC(__isnanf) 
    SYMBOL_ITEM_FUNC(ldexpf) 
    SYMBOL_ITEM_FUNC(modff) 
    SYMBOL_ITEM_FUNC(nanf) 
    SYMBOL_ITEM_FUNC(scalbnf) 
    SYMBOL_ITEM_FUNC(__signgam) 
    SYMBOL_ITEM_FUNC(_siprintf_r) 
    SYMBOL_ITEM_FUNC(siprintf) 
    SYMBOL_ITEM_FUNC(_siscanf_r) 
    SYMBOL_ITEM_FUNC(siscanf) 
    SYMBOL_ITEM_FUNC(_sniprintf_r) 
    SYMBOL_ITEM_FUNC(sniprintf) 
    SYMBOL_ITEM_FUNC(_snprintf_r) 
    SYMBOL_ITEM_FUNC(snprintf) 
    SYMBOL_ITEM_FUNC(_sprintf_r) 
    SYMBOL_ITEM_FUNC(sprintf) 
    SYMBOL_ITEM_FUNC(_srand48_r) 
    SYMBOL_ITEM_FUNC(srand48) 
    SYMBOL_ITEM_FUNC(_sscanf_r) 
    SYMBOL_ITEM_FUNC(sscanf) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(__sclose) 
    SYMBOL_ITEM_FUNC(__seofread) 
    SYMBOL_ITEM_FUNC(__sread) 
    SYMBOL_ITEM_FUNC(__sseek) 
    SYMBOL_ITEM_FUNC(__swrite) 
    SYMBOL_ITEM_FUNC(stpcpy) 
    SYMBOL_ITEM_FUNC(stpncpy) 
    SYMBOL_ITEM_FUNC(strcasecmp) 
    SYMBOL_ITEM_FUNC(strcasestr) 
    SYMBOL_ITEM_FUNC(strcat) 
    SYMBOL_ITEM_FUNC(strchr) 
    SYMBOL_ITEM_FUNC(strcmp) 
    SYMBOL_ITEM_FUNC(strcoll) 
    SYMBOL_ITEM_FUNC(strcpy) 
    SYMBOL_ITEM_FUNC(strcspn) 
    SYMBOL_ITEM_FUNC(strdup) 
    SYMBOL_ITEM_FUNC(_strdup_r) 
    SYMBOL_ITEM_FUNC(strerror) 
    SYMBOL_ITEM_FUNC(strerror_r) 
    SYMBOL_ITEM_FUNC(strftime) 
    SYMBOL_ITEM_FUNC(strlcat) 
    SYMBOL_ITEM_FUNC(strlcpy) 
    SYMBOL_ITEM_FUNC(strlen) 
    SYMBOL_ITEM_FUNC(strlwr) 
    SYMBOL_ITEM_FUNC(strncasecmp) 
    SYMBOL_ITEM_FUNC(strncat) 
    SYMBOL_ITEM_FUNC(strncmp) 
    SYMBOL_ITEM_FUNC(strncpy) 
    SYMBOL_ITEM_FUNC(strndup) 
    SYMBOL_ITEM_FUNC(_strndup_r) 
    SYMBOL_ITEM_FUNC(strnlen) 
    SYMBOL_ITEM_FUNC(strpbrk) 
    SYMBOL_ITEM_FUNC(strptime) 
    SYMBOL_ITEM_FUNC(strrchr) 
    SYMBOL_ITEM_FUNC(strsep) 
    SYMBOL_ITEM_FUNC(strsignal) 
    SYMBOL_ITEM_FUNC(strspn) 
    SYMBOL_ITEM_FUNC(strstr) 
    SYMBOL_ITEM_FUNC(_strtod_r) 
    SYMBOL_ITEM_FUNC(strtod) 
    SYMBOL_ITEM_FUNC(strtof) 
    SYMBOL_ITEM_FUNC(strtok) 
    SYMBOL_ITEM_FUNC(__strtok_r) 
    SYMBOL_ITEM_FUNC(strtok_r) 
    SYMBOL_ITEM_FUNC(_strtol_r) 
    SYMBOL_ITEM_FUNC(strtol) 
    SYMBOL_ITEM_FUNC(strtold) 
    SYMBOL_ITEM_FUNC(strtoll) 
    SYMBOL_ITEM_FUNC(_strtoll_r) 
    SYMBOL_ITEM_FUNC(_strtoul_r) 
    SYMBOL_ITEM_FUNC(strtoul) 
    SYMBOL_ITEM_FUNC(strtoull) 
    SYMBOL_ITEM_FUNC(_strtoull_r) 
    SYMBOL_ITEM_FUNC(strupr) 
    SYMBOL_ITEM_FUNC(strxfrm) 
    SYMBOL_ITEM_FUNC(__ssprint_r) 
    SYMBOL_ITEM_FUNC(_svfiprintf_r) 
    SYMBOL_ITEM_FUNC(__ssrefill_r) 
    SYMBOL_ITEM_FUNC(__ssvfiscanf_r) 
    SYMBOL_ITEM_FUNC(_sfread_r) 
    SYMBOL_ITEM_FUNC(_sungetc_r) 
    SYMBOL_ITEM_FUNC(_svfiwprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfiwscanf_r) 
    SYMBOL_ITEM_FUNC(_svfprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfscanf_r) 
    SYMBOL_ITEM_FUNC(_svfwprintf_r) 
    SYMBOL_ITEM_FUNC(__ssvfwscanf_r) 
    SYMBOL_ITEM_FUNC(swab) 
    SYMBOL_ITEM_FUNC(_swprintf_r) 
    SYMBOL_ITEM_FUNC(swprintf) 
    SYMBOL_ITEM_FUNC(_swscanf_r) 
    SYMBOL_ITEM_FUNC(swscanf) 
    SYMBOL_ITEM_FUNC(close) 
    SYMBOL_ITEM_FUNC(execve) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(fork) 
    SYMBOL_ITEM_FUNC(fstat) 
    SYMBOL_ITEM_FUNC(getpid) 
    SYMBOL_ITEM_FUNC(gettimeofday) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(kill) 
    SYMBOL_ITEM_FUNC(link) 
    SYMBOL_ITEM_FUNC(lseek) 
    SYMBOL_ITEM_FUNC(open) 
    SYMBOL_ITEM_FUNC(read) 
    SYMBOL_ITEM_FUNC(sbrk) 
    SYMBOL_ITEM_FUNC(stat) 
    SYMBOL_ITEM_FUNC(_system_r) 
    SYMBOL_ITEM_FUNC(system) 
    SYMBOL_ITEM_FUNC(times) 
    SYMBOL_ITEM_FUNC(unlink) 
    SYMBOL_ITEM_FUNC(wait) 
    SYMBOL_ITEM_FUNC(write) 
    SYMBOL_ITEM_FUNC(tdelete) 
    SYMBOL_ITEM_FUNC(tdestroy) 
    SYMBOL_ITEM_FUNC(tfind) 
    SYMBOL_ITEM_FUNC(time) 
    SYMBOL_ITEM_FUNC(__get_current_time_locale) 
    SYMBOL_ITEM_FUNC(__time_load_locale) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_tmpfile_r) 
    SYMBOL_ITEM_FUNC(tmpfile) 
    SYMBOL_ITEM_FUNC(_tempnam_r) 
    SYMBOL_ITEM_FUNC(_tmpnam_r) 
    SYMBOL_ITEM_FUNC(tempnam) 
    SYMBOL_ITEM_FUNC(tmpnam) 
    SYMBOL_ITEM_FUNC(toascii) 
    SYMBOL_ITEM_FUNC(tolower) 
    SYMBOL_ITEM_FUNC(toupper) 
    SYMBOL_ITEM_FUNC(_towctrans_r) 
    SYMBOL_ITEM_FUNC(towctrans) 
    SYMBOL_ITEM_FUNC(towlower) 
    SYMBOL_ITEM_FUNC(towupper) 
    SYMBOL_ITEM_FUNC(tsearch) 
    SYMBOL_ITEM_FUNC(twalk) 
    SYMBOL_ITEM_FUNC(__tz_lock) 
    SYMBOL_ITEM_FUNC(__tz_unlock) 
    SYMBOL_ITEM_FUNC(tzset) 
    SYMBOL_ITEM_FUNC(_tzset_r) 
    SYMBOL_ITEM_FUNC(_user_strerror) 
    SYMBOL_ITEM_FUNC(__submore) 
    SYMBOL_ITEM_FUNC(_ungetc_r) 
    SYMBOL_ITEM_FUNC(ungetc) 
    SYMBOL_ITEM_FUNC(_ungetwc_r) 
    SYMBOL_ITEM_FUNC(ungetwc) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(pvalloc) 
    SYMBOL_ITEM_FUNC(valloc) 
    SYMBOL_ITEM_FUNC(_valloc_r) 
    SYMBOL_ITEM_FUNC(_vasiprintf_r) 
    SYMBOL_ITEM_FUNC(vasiprintf) 
    SYMBOL_ITEM_FUNC(_vasniprintf_r) 
    SYMBOL_ITEM_FUNC(vasniprintf) 
    SYMBOL_ITEM_FUNC(_vasnprintf_r) 
    SYMBOL_ITEM_FUNC(vasnprintf) 
    SYMBOL_ITEM_FUNC(_vasprintf_r) 
    SYMBOL_ITEM_FUNC(vasprintf) 
    SYMBOL_ITEM_FUNC(_vdiprintf_r) 
    SYMBOL_ITEM_FUNC(vdiprintf) 
    SYMBOL_ITEM_FUNC(_vdprintf_r) 
    SYMBOL_ITEM_FUNC(vdprintf) 
    SYMBOL_ITEM_FUNC(__sprint_r) 
    SYMBOL_ITEM_FUNC(_vfiprintf_r) 
    SYMBOL_ITEM_FUNC(vfiprintf) 
    SYMBOL_ITEM_FUNC(__svfiscanf) 
    SYMBOL_ITEM_FUNC(__svfiscanf_r) 
    SYMBOL_ITEM_FUNC(_vfiscanf_r) 
    SYMBOL_ITEM_FUNC(vfiscanf) 
    SYMBOL_ITEM_FUNC(_vfiwprintf_r) 
    SYMBOL_ITEM_FUNC(vfiwprintf) 
    SYMBOL_ITEM_FUNC(__svfiwscanf) 
    SYMBOL_ITEM_FUNC(__svfiwscanf_r) 
    SYMBOL_ITEM_FUNC(_vfiwscanf_r) 
    SYMBOL_ITEM_FUNC(vfiwscanf) 
    SYMBOL_ITEM_FUNC(_vfprintf_r) 
    SYMBOL_ITEM_FUNC(vfprintf) 
    SYMBOL_ITEM_FUNC(__svfscanf) 
    SYMBOL_ITEM_FUNC(__svfscanf_r) 
    SYMBOL_ITEM_FUNC(_vfscanf_r) 
    SYMBOL_ITEM_FUNC(vfscanf) 
    SYMBOL_ITEM_FUNC(_vfwprintf_r) 
    SYMBOL_ITEM_FUNC(vfwprintf) 
    SYMBOL_ITEM_FUNC(__svfwscanf) 
    SYMBOL_ITEM_FUNC(__svfwscanf_r) 
    SYMBOL_ITEM_FUNC(_vfwscanf_r) 
    SYMBOL_ITEM_FUNC(vfwscanf) 
    SYMBOL_ITEM_FUNC(_viprintf_r) 
    SYMBOL_ITEM_FUNC(viprintf) 
    SYMBOL_ITEM_FUNC(_viscanf_r) 
    SYMBOL_ITEM_FUNC(viscanf) 
    SYMBOL_ITEM_FUNC(_vprintf_r) 
    SYMBOL_ITEM_FUNC(vprintf) 
    SYMBOL_ITEM_FUNC(_vscanf_r) 
    SYMBOL_ITEM_FUNC(vscanf) 
    SYMBOL_ITEM_FUNC(_vsiprintf_r) 
    SYMBOL_ITEM_FUNC(vsiprintf) 
    SYMBOL_ITEM_FUNC(_vsiscanf_r) 
    SYMBOL_ITEM_FUNC(vsiscanf) 
    SYMBOL_ITEM_FUNC(_vsniprintf_r) 
    SYMBOL_ITEM_FUNC(vsniprintf) 
    SYMBOL_ITEM_FUNC(_vsnprintf_r) 
    SYMBOL_ITEM_FUNC(vsnprintf) 
    SYMBOL_ITEM_FUNC(_vsprintf_r) 
    SYMBOL_ITEM_FUNC(vsprintf) 
    SYMBOL_ITEM_FUNC(_vsscanf_r) 
    SYMBOL_ITEM_FUNC(vsscanf) 
    SYMBOL_ITEM_FUNC(_vswprintf_r) 
    SYMBOL_ITEM_FUNC(vswprintf) 
    SYMBOL_ITEM_FUNC(_vswscanf_r) 
    SYMBOL_ITEM_FUNC(vswscanf) 
    SYMBOL_ITEM_FUNC(_vwprintf_r) 
    SYMBOL_ITEM_FUNC(vwprintf) 
    SYMBOL_ITEM_FUNC(_vwscanf_r) 
    SYMBOL_ITEM_FUNC(vwscanf) 
    SYMBOL_ITEM_FUNC(__swbuf) 
    SYMBOL_ITEM_FUNC(__swbuf_r) 
    SYMBOL_ITEM_FUNC(wcpcpy) 
    SYMBOL_ITEM_FUNC(wcpncpy) 
    SYMBOL_ITEM_FUNC(_wcrtomb_r) 
    SYMBOL_ITEM_FUNC(wcrtomb) 
    SYMBOL_ITEM_FUNC(wcscasecmp) 
    SYMBOL_ITEM_FUNC(wcscat) 
    SYMBOL_ITEM_FUNC(wcschr) 
    SYMBOL_ITEM_FUNC(wcscmp) 
    SYMBOL_ITEM_FUNC(wcscoll) 
    SYMBOL_ITEM_FUNC(wcscpy) 
    SYMBOL_ITEM_FUNC(wcscspn) 
    SYMBOL_ITEM_FUNC(_wcsdup_r) 
    SYMBOL_ITEM_FUNC(wcsdup) 
    SYMBOL_ITEM_FUNC(__ctloc) 
    SYMBOL_ITEM_FUNC(wcsftime) 
    SYMBOL_ITEM_FUNC(wcslcat) 
    SYMBOL_ITEM_FUNC(wcslcpy) 
    SYMBOL_ITEM_FUNC(wcslen) 
    SYMBOL_ITEM_FUNC(wcsncasecmp) 
    SYMBOL_ITEM_FUNC(wcsncat) 
    SYMBOL_ITEM_FUNC(wcsncmp) 
    SYMBOL_ITEM_FUNC(wcsncpy) 
    SYMBOL_ITEM_FUNC(wcsnlen) 
    SYMBOL_ITEM_FUNC(_wcsnrtombs_r) 
    SYMBOL_ITEM_FUNC(wcsnrtombs) 
    SYMBOL_ITEM_FUNC(wcspbrk) 
    SYMBOL_ITEM_FUNC(wcsrchr) 
    SYMBOL_ITEM_FUNC(_wcsrtombs_r) 
    SYMBOL_ITEM_FUNC(wcsrtombs) 
    SYMBOL_ITEM_FUNC(wcsspn) 
    SYMBOL_ITEM_FUNC(wcsstr) 
    SYMBOL_ITEM_FUNC(_wcstod_r) 
    SYMBOL_ITEM_FUNC(_wcstof_r) 
    SYMBOL_ITEM_FUNC(wcstod) 
    SYMBOL_ITEM_FUNC(wcstof) 
    SYMBOL_ITEM_FUNC(wcstok) 
    SYMBOL_ITEM_FUNC(_wcstol_r) 
    SYMBOL_ITEM_FUNC(wcstol) 
    SYMBOL_ITEM_FUNC(wcstold) 
    SYMBOL_ITEM_FUNC(wcstoll) 
    SYMBOL_ITEM_FUNC(_wcstoll_r) 
    SYMBOL_ITEM_FUNC(wcstombs) 
    SYMBOL_ITEM_FUNC(_wcstombs_r) 
    SYMBOL_ITEM_FUNC(_wcstoul_r) 
    SYMBOL_ITEM_FUNC(wcstoul) 
    SYMBOL_ITEM_FUNC(wcstoull) 
    SYMBOL_ITEM_FUNC(_wcstoull_r) 
    SYMBOL_ITEM_FUNC(wcswidth) 
    SYMBOL_ITEM_FUNC(wcsxfrm) 
    SYMBOL_ITEM_FUNC(wctob) 
    SYMBOL_ITEM_FUNC(wctomb) 
    SYMBOL_ITEM_FUNC(__ascii_wctomb) 
    SYMBOL_ITEM_FUNC(_wctomb_r) 
    SYMBOL_ITEM_FUNC(_wctrans_r) 
    SYMBOL_ITEM_FUNC(wctrans) 
    SYMBOL_ITEM_FUNC(_wctype_r) 
    SYMBOL_ITEM_FUNC(wctype) 
    SYMBOL_ITEM_FUNC(__wcwidth) 
    SYMBOL_ITEM_FUNC(wcwidth) 
    SYMBOL_ITEM_FUNC(wmemchr) 
    SYMBOL_ITEM_FUNC(wmemcmp) 
    SYMBOL_ITEM_FUNC(wmemcpy) 
    SYMBOL_ITEM_FUNC(wmemmove) 
    SYMBOL_ITEM_FUNC(wmemset) 
    SYMBOL_ITEM_FUNC(_wprintf_r) 
    SYMBOL_ITEM_FUNC(wprintf) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(_wscanf_r) 
    SYMBOL_ITEM_FUNC(wscanf) 
    SYMBOL_ITEM_FUNC(__swsetup_r) 
    SYMBOL_ITEM_OBJ(__ctype_ptr__) 
    SYMBOL_ITEM_OBJ(__mb_cur_max) 
    SYMBOL_ITEM_OBJ(__mbtowc) 
    SYMBOL_ITEM_OBJ(__wctomb) 
    SYMBOL_ITEM_OBJ(_data) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
    SYMBOL_ITEM_OBJ(_tzname) 
    SYMBOL_ITEM_OBJ(devfs) 
    SYMBOL_ITEM_OBJ(fatfs) 
    SYMBOL_ITEM_OBJ(fb_drv) 
    SYMBOL_ITEM_OBJ(nand_curr_device) 
    SYMBOL_ITEM_OBJ(nfs) 
    SYMBOL_ITEM_OBJ(rootfs) 
    SYMBOL_ITEM_OBJ(socket_drv) 
    SYMBOL_ITEM_OBJ(sys_do_table) 
    SYMBOL_ITEM_OBJ(yaffs1) 
    SYMBOL_ITEM_OBJ(yaffs2) 
    SYMBOL_ITEM_OBJ(yaffs_trace_mask) 
    SYMBOL_ITEM_OBJ(yaffsfs_deviceList) 
    SYMBOL_ITEM_OBJ(_PathLocale) 
    SYMBOL_ITEM_OBJ(__bss_start) 
    SYMBOL_ITEM_OBJ(__bss_start__) 
    SYMBOL_ITEM_OBJ(__hexdig) 
    SYMBOL_ITEM_OBJ(__mlocale_changed) 
    SYMBOL_ITEM_OBJ(__nlocale_changed) 
    SYMBOL_ITEM_OBJ(_daylight) 
    SYMBOL_ITEM_OBJ(_null_auth) 
    SYMBOL_ITEM_OBJ(_timezone) 
    SYMBOL_ITEM_OBJ(current) 
    SYMBOL_ITEM_OBJ(current_header) 
    SYMBOL_ITEM_OBJ(current_iphdr_dest) 
    SYMBOL_ITEM_OBJ(current_iphdr_src) 
    SYMBOL_ITEM_OBJ(current_netif) 
    SYMBOL_ITEM_OBJ(cwd) 
    SYMBOL_ITEM_OBJ(device_lock) 
    SYMBOL_ITEM_OBJ(file_system_lock) 
    SYMBOL_ITEM_OBJ(h_errno) 
    SYMBOL_ITEM_OBJ(info_lock) 
    SYMBOL_ITEM_OBJ(infos) 
    SYMBOL_ITEM_OBJ(interrupt_nest) 
    SYMBOL_ITEM_OBJ(lock_tcpip_core) 
    SYMBOL_ITEM_OBJ(lwip_stats) 
    SYMBOL_ITEM_OBJ(mtd_table) 
    SYMBOL_ITEM_OBJ(nand_info) 
    SYMBOL_ITEM_OBJ(netif_default) 
    SYMBOL_ITEM_OBJ(netif_list) 
    SYMBOL_ITEM_OBJ(os_started) 
    SYMBOL_ITEM_OBJ(pbuf_free_ooseq_pending) 
    SYMBOL_ITEM_OBJ(mount_point_lock) 
    SYMBOL_ITEM_OBJ(ram_heap) 
    SYMBOL_ITEM_OBJ(reents) 
    SYMBOL_ITEM_OBJ(rootfs_point) 
    SYMBOL_ITEM_OBJ(rpc_createerr) 
    SYMBOL_ITEM_OBJ(svc_fdset) 
    SYMBOL_ITEM_OBJ(svc_max_pollfd) 
    SYMBOL_ITEM_OBJ(svc_pollfd) 
    SYMBOL_ITEM_OBJ(tasks) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_active_pcbs_changed) 
    SYMBOL_ITEM_OBJ(tcp_bound_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_input_pcb) 
    SYMBOL_ITEM_OBJ(tcp_listen_pcbs) 
    SYMBOL_ITEM_OBJ(tcp_ticks) 
    SYMBOL_ITEM_OBJ(tcp_tmp_pcb) 
    SYMBOL_ITEM_OBJ(tcp_tw_pcbs) 
    SYMBOL_ITEM_OBJ(udp_pcbs) 
    SYMBOL_ITEM_OBJ(yaffs_wr_attempts) 
    SYMBOL_ITEM_OBJ(__aeabi_idiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_ldiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr1) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr2) 
    SYMBOL_ITEM_OBJ(board_mmc_getcd) 
    SYMBOL_ITEM_OBJ(__CTOR_LIST__) 
    SYMBOL_ITEM_OBJ(__DTOR_LIST__) 
    SYMBOL_ITEM_OBJ(__clz_tab) 
    SYMBOL_ITEM_OBJ(__popcount_tab) 
    SYMBOL_ITEM_OBJ(__aeabi_idiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_ldiv0) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr1) 
    SYMBOL_ITEM_OBJ(__aeabi_unwind_cpp_pr2) 
    SYMBOL_ITEM_OBJ(__fdlib_version) 
    SYMBOL_ITEM_OBJ(__infinity) 
    SYMBOL_ITEM_OBJ(__infinityf) 
    SYMBOL_ITEM_OBJ(__infinityld) 
    SYMBOL_ITEM_OBJ(__ctype_ptr__) 
    SYMBOL_ITEM_OBJ(environ) 
    SYMBOL_ITEM_OBJ(opterr) 
    SYMBOL_ITEM_OBJ(optopt) 
    SYMBOL_ITEM_OBJ(__default_hash) 
    SYMBOL_ITEM_OBJ(_impure_ptr) 
    SYMBOL_ITEM_OBJ(__mb_cur_max) 
    SYMBOL_ITEM_OBJ(__mbtowc) 
    SYMBOL_ITEM_OBJ(_tzname) 
    SYMBOL_ITEM_OBJ(__wctomb) 
    SYMBOL_ITEM_OBJ(optarg) 
    SYMBOL_ITEM_OBJ(optind) 
    SYMBOL_ITEM_OBJ(_PathLocale) 
    SYMBOL_ITEM_OBJ(__mlocale_changed) 
    SYMBOL_ITEM_OBJ(__nlocale_changed) 
    SYMBOL_ITEM_OBJ(_daylight) 
    SYMBOL_ITEM_OBJ(_timezone) 
    SYMBOL_ITEM_OBJ(_ctype_) 
    SYMBOL_ITEM_OBJ(_global_impure_ptr) 
    SYMBOL_ITEM_OBJ(__mprec_bigtens) 
    SYMBOL_ITEM_OBJ(__mprec_tens) 
    SYMBOL_ITEM_OBJ(__mprec_tinytens) 
    SYMBOL_ITEM_OBJ(__infinity) 
    SYMBOL_ITEM_OBJ(__infinityf) 
    SYMBOL_ITEM_OBJ(__infinityld) 
    SYMBOL_ITEM_OBJ(tm_year_base) 
    SYMBOL_ITEM_OBJ(__unctrl) 
    SYMBOL_ITEM_OBJ(__unctrllen) 
    SYMBOL_ITEM_OBJ(__hexdig) 
    SYMBOL_ITEM_OBJ(suboptarg) 
    SYMBOL_ITEM_OBJ(errno) 
SYMBOL_TABLE_END												
#endif                                               
/*********************************************************************************************************	
** END FILE													
*********************************************************************************************************/	
