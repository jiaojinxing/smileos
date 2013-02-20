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
** 文   件   名: symbol.c											
**														
** 创   建   人: MakeSymbol 工具										
**														
** 文件创建日期: 2013 年 02 月 20 日						
**														
** 描        述: SmileOS 操作系统符号表. (此文件由 MakeSymbol 工具自动生成, 请勿修改)			
*********************************************************************************************************/	
#include "kern/func_config.h"                                          
#if CONFIG_MODULE_EN 
#include "module/symbol_tool.h"										
														
#define SYMBOL_TABLE_BEGIN const symbol_t symbol_tbl[] = { 						
  														
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
extern int  .Lchange_fp(); 
extern int  .Lchange_ip(); 
extern int  .Lchange_lr(); 
extern int  .Lchange_r0(); 
extern int  .Lchange_r1(); 
extern int  .Lchange_r2(); 
extern int  .Lchange_r3(); 
extern int  .Lchange_r4(); 
extern int  .Lchange_r5(); 
extern int  .Lchange_r6(); 
extern int  .Lchange_r7(); 
extern int  .Lchange_r8(); 
extern int  .Lchange_r9(); 
extern int  .Lchange_sl(); 
extern int  .Lchange_sp(); 
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
extern int  _start(); 
extern int  arch_kernel_enter(); 
extern int  heap_alloc(); 
extern int  heap_free(); 
extern int  heap_init(); 
extern int  mem_size(); 
extern int  MIN(); 
extern int  _calloc_r(); 
extern int  _fini(); 
extern int  _free_r(); 
extern int  _init(); 
extern int  _malloc_r(); 
extern int  _realloc_r(); 
extern int  libc_init(); 
extern int  memrchr(); 
extern int  strchrnul(); 
extern int  xmalloc(); 
extern int  xstrdup(); 
extern int  xstrndup(); 
extern int  xzalloc(); 
extern int  _close_r(); 
extern int  _execve_r(); 
extern int  _exit(); 
extern int  _fork_r(); 
extern int  _fstat_r(); 
extern int  _getpid_r(); 
extern int  _gettimeofday_r(); 
extern int  _isatty_r(); 
extern int  _kill_r(); 
extern int  _link_r(); 
extern int  _lseek_r(); 
extern int  _mkdir_r(); 
extern int  _open_r(); 
extern int  _read_r(); 
extern int  _rename_r(); 
extern int  _rmdir_r(); 
extern int  _sbrk_r(); 
extern int  _stat_r(); 
extern int  _times_r(); 
extern int  _unlink_r(); 
extern int  _wait_r(); 
extern int  _write_r(); 
extern int  accept(); 
extern int  alarm(); 
extern int  bind(); 
extern int  chdir(); 
extern int  closedir(); 
extern int  connect(); 
extern int  dup(); 
extern int  dup2(); 
extern int  fcntl(); 
extern int  getcwd(); 
extern int  getsockopt(); 
extern int  ioctl(); 
extern int  listen(); 
extern int  msleep(); 
extern int  opendir(); 
extern int  pause(); 
extern int  raise(); 
extern int  readdir(); 
extern int  recv(); 
extern int  recvfrom(); 
extern int  rewinddir(); 
extern int  schedule(); 
extern int  seekdir(); 
extern int  select(); 
extern int  send(); 
extern int  sendto(); 
extern int  setpinfo(); 
extern int  setsockopt(); 
extern int  shutdown(); 
extern int  signal(); 
extern int  sigsuspend(); 
extern int  sleep(); 
extern int  socket(); 
extern int  syscall_accept(); 
extern int  syscall_connect(); 
extern int  syscall_read(); 
extern int  syscall_recv(); 
extern int  syscall_recvfrom(); 
extern int  syscall_select(); 
extern int  syscall_send(); 
extern int  syscall_sendto(); 
extern int  syscall_sigmask(); 
extern int  syscall_sleep(); 
extern int  syscall_usleep(); 
extern int  syscall_write(); 
extern int  telldir(); 
extern int  usleep(); 
extern int  creat(); 
extern int  isatty(); 
extern int  mkdir(); 
extern int  poll(); 
extern int  rmdir(); 
extern int  lwip_htonl(); 
extern int  lwip_htons(); 
extern int  lwip_ntohl(); 
extern int  lwip_ntohs(); 
extern int  ip4_addr_isbroadcast(); 
extern int  ip4_addr_netmask_valid(); 
extern int  ipaddr_addr(); 
extern int  ipaddr_aton(); 
extern int  ipaddr_ntoa(); 
extern int  ipaddr_ntoa_r(); 
extern int  pth_mctx_switch(); 
extern int  pth_mctx_set(); 
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
extern int  __pthread_sigwait(); 
extern int  __pthread_sleep(); 
extern int  __pthread_usleep(); 
extern int  __pthread_write(); 
extern int  __pthread_writev(); 
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
extern int  pthread_kill(); 
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
extern int  pthread_sigmask(); 
extern int  pthread_testcancel(); 
extern int  pthread_yield_np(); 
extern int  pth_attr_ctrl(); 
extern int  pth_attr_destroy(); 
extern int  pth_attr_get(); 
extern int  pth_attr_init(); 
extern int  pth_attr_new(); 
extern int  pth_attr_of(); 
extern int  pth_attr_set(); 
extern int  pth_abort(); 
extern int  pth_cancel(); 
extern int  pth_cancel_point(); 
extern int  pth_cancel_state(); 
extern int  pth_cleanup_pop(); 
extern int  pth_cleanup_popall(); 
extern int  pth_cleanup_push(); 
extern int  pth_key_create(); 
extern int  pth_key_delete(); 
extern int  pth_key_destroydata(); 
extern int  pth_key_getdata(); 
extern int  pth_key_setdata(); 
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
extern int  pth_wait(); 
extern int  pth_sfiodisc(); 
extern int  pth_accept(); 
extern int  pth_accept_ev(); 
extern int  pth_connect(); 
extern int  pth_connect_ev(); 
extern int  pth_nanosleep(); 
extern int  pth_poll(); 
extern int  pth_poll_ev(); 
extern int  pth_pread(); 
extern int  pth_pselect(); 
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
extern int  pth_select(); 
extern int  pth_select_ev(); 
extern int  pth_send(); 
extern int  pth_send_ev(); 
extern int  pth_sendto(); 
extern int  pth_sendto_ev(); 
extern int  pth_sigmask(); 
extern int  pth_sigwait(); 
extern int  pth_sigwait_ev(); 
extern int  pth_sleep(); 
extern int  pth_usleep(); 
extern int  pth_write(); 
extern int  pth_write_ev(); 
extern int  pth_writev(); 
extern int  pth_writev_ev(); 
extern int  pth_writev_faked(); 
extern int  pth_writev_iov_advance(); 
extern int  pth_writev_iov_bytes(); 
extern int  pth_ctrl(); 
extern int  pth_exit(); 
extern int  pth_fdmode(); 
extern int  pth_init(); 
extern int  pth_join(); 
extern int  pth_kill(); 
extern int  pth_nap(); 
extern int  pth_once(); 
extern int  pth_raise(); 
extern int  pth_resume(); 
extern int  pth_self(); 
extern int  pth_spawn(); 
extern int  pth_suspend(); 
extern int  pth_thread_cleanup(); 
extern int  pth_thread_exists(); 
extern int  pth_version(); 
extern int  pth_yield(); 
extern int  pth_msgport_create(); 
extern int  pth_msgport_destroy(); 
extern int  pth_msgport_find(); 
extern int  pth_msgport_get(); 
extern int  pth_msgport_pending(); 
extern int  pth_msgport_put(); 
extern int  pth_msgport_reply(); 
extern int  pth_pqueue_contains(); 
extern int  pth_pqueue_delete(); 
extern int  pth_pqueue_delmax(); 
extern int  pth_pqueue_favorite(); 
extern int  pth_pqueue_increase(); 
extern int  pth_pqueue_init(); 
extern int  pth_pqueue_insert(); 
extern int  pth_pqueue_tail(); 
extern int  pth_pqueue_walk(); 
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
extern int  pth_sched_eventmanager(); 
extern int  pth_sched_eventmanager_sighandler(); 
extern int  pth_scheduler(); 
extern int  pth_scheduler_drop(); 
extern int  pth_scheduler_init(); 
extern int  pth_scheduler_kill(); 
extern int  pth_asprintf(); 
extern int  pth_snprintf(); 
extern int  pth_vasprintf(); 
extern int  pth_vsnprintf(); 
extern int  pth_barrier_init(); 
extern int  pth_barrier_reach(); 
extern int  pth_cond_await(); 
extern int  pth_cond_init(); 
extern int  pth_cond_notify(); 
extern int  pth_mutex_acquire(); 
extern int  pth_mutex_init(); 
extern int  pth_mutex_release(); 
extern int  pth_mutex_releaseall(); 
extern int  pth_rwlock_acquire(); 
extern int  pth_rwlock_init(); 
extern int  pth_rwlock_release(); 
extern int  pth_implicit_init(); 
extern int  pth_syscall_init(); 
extern int  pth_syscall_kill(); 
extern int  pth_tcb_alloc(); 
extern int  pth_tcb_free(); 
extern int  pth_time(); 
extern int  pth_time_cmp(); 
extern int  pth_time_div(); 
extern int  pth_time_mul(); 
extern int  pth_time_pos(); 
extern int  pth_time_t2d(); 
extern int  pth_time_t2i(); 
extern int  pth_time_usleep(); 
extern int  pth_timeout(); 
extern int  pth_util_cpystrn(); 
extern int  pth_util_fd_valid(); 
extern int  pth_util_fds_merge(); 
extern int  pth_util_fds_select(); 
extern int  pth_util_fds_test(); 
extern int  pth_util_sigdelete(); 
extern int  _ZdaPv(); 
extern int  _ZdlPv(); 
extern int  _Znaj(); 
extern int  _Znwj(); 
extern int  __cppRtDoCtors(); 
extern int  __cppRtDoDtors(); 
extern int  __cppRtDummy(); 
extern int  _cppRtInit(); 
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
extern int  _ctype_; 
extern int  _global_impure_ptr; 
extern int  __mprec_bigtens; 
extern int  __mprec_tens; 
extern int  __mprec_tinytens; 
extern int  __infinity; 
extern int  __infinityf; 
extern int  __infinityld; 
extern int  tm_year_base; 
extern int  __unctrl; 
extern int  __unctrllen; 
extern int  __hexdig; 
extern int  suboptarg; 
extern int  errno; 
extern int  pth_state_names; 
extern int  pth_internal_version; 
extern int  __pth_compat_unit; 
extern int  pth_errno_flag; 
extern int  pth_errno_storage; 
extern int  pth_initialized; 
extern int  pth_time_zero; 
extern int  ip_addr_any; 
extern int  ip_addr_broadcast; 
extern int  pth_DQ; 
extern int  pth_NQ; 
extern int  pth_RQ; 
extern int  pth_SQ; 
extern int  pth_WQ; 
extern int  pth_current; 
extern int  pth_favournew; 
extern int  pth_loadval; 
extern int  pth_main; 
extern int  pth_sched; 
extern int  __dso_handle; 
/*********************************************************************************************************	
** 系统静态符号表												
*********************************************************************************************************/	
SYMBOL_TABLE_BEGIN												
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
    SYMBOL_ITEM_FUNC(.Lchange_fp) 
    SYMBOL_ITEM_FUNC(.Lchange_ip) 
    SYMBOL_ITEM_FUNC(.Lchange_lr) 
    SYMBOL_ITEM_FUNC(.Lchange_r0) 
    SYMBOL_ITEM_FUNC(.Lchange_r1) 
    SYMBOL_ITEM_FUNC(.Lchange_r2) 
    SYMBOL_ITEM_FUNC(.Lchange_r3) 
    SYMBOL_ITEM_FUNC(.Lchange_r4) 
    SYMBOL_ITEM_FUNC(.Lchange_r5) 
    SYMBOL_ITEM_FUNC(.Lchange_r6) 
    SYMBOL_ITEM_FUNC(.Lchange_r7) 
    SYMBOL_ITEM_FUNC(.Lchange_r8) 
    SYMBOL_ITEM_FUNC(.Lchange_r9) 
    SYMBOL_ITEM_FUNC(.Lchange_sl) 
    SYMBOL_ITEM_FUNC(.Lchange_sp) 
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
    SYMBOL_ITEM_FUNC(_start) 
    SYMBOL_ITEM_FUNC(arch_kernel_enter) 
    SYMBOL_ITEM_FUNC(heap_alloc) 
    SYMBOL_ITEM_FUNC(heap_free) 
    SYMBOL_ITEM_FUNC(heap_init) 
    SYMBOL_ITEM_FUNC(mem_size) 
    SYMBOL_ITEM_FUNC(MIN) 
    SYMBOL_ITEM_FUNC(_calloc_r) 
    SYMBOL_ITEM_FUNC(_fini) 
    SYMBOL_ITEM_FUNC(_free_r) 
    SYMBOL_ITEM_FUNC(_init) 
    SYMBOL_ITEM_FUNC(_malloc_r) 
    SYMBOL_ITEM_FUNC(_realloc_r) 
    SYMBOL_ITEM_FUNC(libc_init) 
    SYMBOL_ITEM_FUNC(memrchr) 
    SYMBOL_ITEM_FUNC(strchrnul) 
    SYMBOL_ITEM_FUNC(xmalloc) 
    SYMBOL_ITEM_FUNC(xstrdup) 
    SYMBOL_ITEM_FUNC(xstrndup) 
    SYMBOL_ITEM_FUNC(xzalloc) 
    SYMBOL_ITEM_FUNC(_close_r) 
    SYMBOL_ITEM_FUNC(_execve_r) 
    SYMBOL_ITEM_FUNC(_exit) 
    SYMBOL_ITEM_FUNC(_fork_r) 
    SYMBOL_ITEM_FUNC(_fstat_r) 
    SYMBOL_ITEM_FUNC(_getpid_r) 
    SYMBOL_ITEM_FUNC(_gettimeofday_r) 
    SYMBOL_ITEM_FUNC(_isatty_r) 
    SYMBOL_ITEM_FUNC(_kill_r) 
    SYMBOL_ITEM_FUNC(_link_r) 
    SYMBOL_ITEM_FUNC(_lseek_r) 
    SYMBOL_ITEM_FUNC(_mkdir_r) 
    SYMBOL_ITEM_FUNC(_open_r) 
    SYMBOL_ITEM_FUNC(_read_r) 
    SYMBOL_ITEM_FUNC(_rename_r) 
    SYMBOL_ITEM_FUNC(_rmdir_r) 
    SYMBOL_ITEM_FUNC(_sbrk_r) 
    SYMBOL_ITEM_FUNC(_stat_r) 
    SYMBOL_ITEM_FUNC(_times_r) 
    SYMBOL_ITEM_FUNC(_unlink_r) 
    SYMBOL_ITEM_FUNC(_wait_r) 
    SYMBOL_ITEM_FUNC(_write_r) 
    SYMBOL_ITEM_FUNC(accept) 
    SYMBOL_ITEM_FUNC(alarm) 
    SYMBOL_ITEM_FUNC(bind) 
    SYMBOL_ITEM_FUNC(chdir) 
    SYMBOL_ITEM_FUNC(closedir) 
    SYMBOL_ITEM_FUNC(connect) 
    SYMBOL_ITEM_FUNC(dup) 
    SYMBOL_ITEM_FUNC(dup2) 
    SYMBOL_ITEM_FUNC(fcntl) 
    SYMBOL_ITEM_FUNC(getcwd) 
    SYMBOL_ITEM_FUNC(getsockopt) 
    SYMBOL_ITEM_FUNC(ioctl) 
    SYMBOL_ITEM_FUNC(listen) 
    SYMBOL_ITEM_FUNC(msleep) 
    SYMBOL_ITEM_FUNC(opendir) 
    SYMBOL_ITEM_FUNC(pause) 
    SYMBOL_ITEM_FUNC(raise) 
    SYMBOL_ITEM_FUNC(readdir) 
    SYMBOL_ITEM_FUNC(recv) 
    SYMBOL_ITEM_FUNC(recvfrom) 
    SYMBOL_ITEM_FUNC(rewinddir) 
    SYMBOL_ITEM_FUNC(schedule) 
    SYMBOL_ITEM_FUNC(seekdir) 
    SYMBOL_ITEM_FUNC(select) 
    SYMBOL_ITEM_FUNC(send) 
    SYMBOL_ITEM_FUNC(sendto) 
    SYMBOL_ITEM_FUNC(setpinfo) 
    SYMBOL_ITEM_FUNC(setsockopt) 
    SYMBOL_ITEM_FUNC(shutdown) 
    SYMBOL_ITEM_FUNC(signal) 
    SYMBOL_ITEM_FUNC(sigsuspend) 
    SYMBOL_ITEM_FUNC(sleep) 
    SYMBOL_ITEM_FUNC(socket) 
    SYMBOL_ITEM_FUNC(syscall_accept) 
    SYMBOL_ITEM_FUNC(syscall_connect) 
    SYMBOL_ITEM_FUNC(syscall_read) 
    SYMBOL_ITEM_FUNC(syscall_recv) 
    SYMBOL_ITEM_FUNC(syscall_recvfrom) 
    SYMBOL_ITEM_FUNC(syscall_select) 
    SYMBOL_ITEM_FUNC(syscall_send) 
    SYMBOL_ITEM_FUNC(syscall_sendto) 
    SYMBOL_ITEM_FUNC(syscall_sigmask) 
    SYMBOL_ITEM_FUNC(syscall_sleep) 
    SYMBOL_ITEM_FUNC(syscall_usleep) 
    SYMBOL_ITEM_FUNC(syscall_write) 
    SYMBOL_ITEM_FUNC(telldir) 
    SYMBOL_ITEM_FUNC(usleep) 
    SYMBOL_ITEM_FUNC(creat) 
    SYMBOL_ITEM_FUNC(isatty) 
    SYMBOL_ITEM_FUNC(mkdir) 
    SYMBOL_ITEM_FUNC(poll) 
    SYMBOL_ITEM_FUNC(rmdir) 
    SYMBOL_ITEM_FUNC(lwip_htonl) 
    SYMBOL_ITEM_FUNC(lwip_htons) 
    SYMBOL_ITEM_FUNC(lwip_ntohl) 
    SYMBOL_ITEM_FUNC(lwip_ntohs) 
    SYMBOL_ITEM_FUNC(ip4_addr_isbroadcast) 
    SYMBOL_ITEM_FUNC(ip4_addr_netmask_valid) 
    SYMBOL_ITEM_FUNC(ipaddr_addr) 
    SYMBOL_ITEM_FUNC(ipaddr_aton) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa) 
    SYMBOL_ITEM_FUNC(ipaddr_ntoa_r) 
    SYMBOL_ITEM_FUNC(pth_mctx_switch) 
    SYMBOL_ITEM_FUNC(pth_mctx_set) 
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
    SYMBOL_ITEM_FUNC(__pthread_sigwait) 
    SYMBOL_ITEM_FUNC(__pthread_sleep) 
    SYMBOL_ITEM_FUNC(__pthread_usleep) 
    SYMBOL_ITEM_FUNC(__pthread_write) 
    SYMBOL_ITEM_FUNC(__pthread_writev) 
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
    SYMBOL_ITEM_FUNC(pthread_kill) 
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
    SYMBOL_ITEM_FUNC(pthread_sigmask) 
    SYMBOL_ITEM_FUNC(pthread_testcancel) 
    SYMBOL_ITEM_FUNC(pthread_yield_np) 
    SYMBOL_ITEM_FUNC(pth_attr_ctrl) 
    SYMBOL_ITEM_FUNC(pth_attr_destroy) 
    SYMBOL_ITEM_FUNC(pth_attr_get) 
    SYMBOL_ITEM_FUNC(pth_attr_init) 
    SYMBOL_ITEM_FUNC(pth_attr_new) 
    SYMBOL_ITEM_FUNC(pth_attr_of) 
    SYMBOL_ITEM_FUNC(pth_attr_set) 
    SYMBOL_ITEM_FUNC(pth_abort) 
    SYMBOL_ITEM_FUNC(pth_cancel) 
    SYMBOL_ITEM_FUNC(pth_cancel_point) 
    SYMBOL_ITEM_FUNC(pth_cancel_state) 
    SYMBOL_ITEM_FUNC(pth_cleanup_pop) 
    SYMBOL_ITEM_FUNC(pth_cleanup_popall) 
    SYMBOL_ITEM_FUNC(pth_cleanup_push) 
    SYMBOL_ITEM_FUNC(pth_key_create) 
    SYMBOL_ITEM_FUNC(pth_key_delete) 
    SYMBOL_ITEM_FUNC(pth_key_destroydata) 
    SYMBOL_ITEM_FUNC(pth_key_getdata) 
    SYMBOL_ITEM_FUNC(pth_key_setdata) 
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
    SYMBOL_ITEM_FUNC(pth_wait) 
    SYMBOL_ITEM_FUNC(pth_sfiodisc) 
    SYMBOL_ITEM_FUNC(pth_accept) 
    SYMBOL_ITEM_FUNC(pth_accept_ev) 
    SYMBOL_ITEM_FUNC(pth_connect) 
    SYMBOL_ITEM_FUNC(pth_connect_ev) 
    SYMBOL_ITEM_FUNC(pth_nanosleep) 
    SYMBOL_ITEM_FUNC(pth_poll) 
    SYMBOL_ITEM_FUNC(pth_poll_ev) 
    SYMBOL_ITEM_FUNC(pth_pread) 
    SYMBOL_ITEM_FUNC(pth_pselect) 
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
    SYMBOL_ITEM_FUNC(pth_select) 
    SYMBOL_ITEM_FUNC(pth_select_ev) 
    SYMBOL_ITEM_FUNC(pth_send) 
    SYMBOL_ITEM_FUNC(pth_send_ev) 
    SYMBOL_ITEM_FUNC(pth_sendto) 
    SYMBOL_ITEM_FUNC(pth_sendto_ev) 
    SYMBOL_ITEM_FUNC(pth_sigmask) 
    SYMBOL_ITEM_FUNC(pth_sigwait) 
    SYMBOL_ITEM_FUNC(pth_sigwait_ev) 
    SYMBOL_ITEM_FUNC(pth_sleep) 
    SYMBOL_ITEM_FUNC(pth_usleep) 
    SYMBOL_ITEM_FUNC(pth_write) 
    SYMBOL_ITEM_FUNC(pth_write_ev) 
    SYMBOL_ITEM_FUNC(pth_writev) 
    SYMBOL_ITEM_FUNC(pth_writev_ev) 
    SYMBOL_ITEM_FUNC(pth_writev_faked) 
    SYMBOL_ITEM_FUNC(pth_writev_iov_advance) 
    SYMBOL_ITEM_FUNC(pth_writev_iov_bytes) 
    SYMBOL_ITEM_FUNC(pth_ctrl) 
    SYMBOL_ITEM_FUNC(pth_exit) 
    SYMBOL_ITEM_FUNC(pth_fdmode) 
    SYMBOL_ITEM_FUNC(pth_init) 
    SYMBOL_ITEM_FUNC(pth_join) 
    SYMBOL_ITEM_FUNC(pth_kill) 
    SYMBOL_ITEM_FUNC(pth_nap) 
    SYMBOL_ITEM_FUNC(pth_once) 
    SYMBOL_ITEM_FUNC(pth_raise) 
    SYMBOL_ITEM_FUNC(pth_resume) 
    SYMBOL_ITEM_FUNC(pth_self) 
    SYMBOL_ITEM_FUNC(pth_spawn) 
    SYMBOL_ITEM_FUNC(pth_suspend) 
    SYMBOL_ITEM_FUNC(pth_thread_cleanup) 
    SYMBOL_ITEM_FUNC(pth_thread_exists) 
    SYMBOL_ITEM_FUNC(pth_version) 
    SYMBOL_ITEM_FUNC(pth_yield) 
    SYMBOL_ITEM_FUNC(pth_msgport_create) 
    SYMBOL_ITEM_FUNC(pth_msgport_destroy) 
    SYMBOL_ITEM_FUNC(pth_msgport_find) 
    SYMBOL_ITEM_FUNC(pth_msgport_get) 
    SYMBOL_ITEM_FUNC(pth_msgport_pending) 
    SYMBOL_ITEM_FUNC(pth_msgport_put) 
    SYMBOL_ITEM_FUNC(pth_msgport_reply) 
    SYMBOL_ITEM_FUNC(pth_pqueue_contains) 
    SYMBOL_ITEM_FUNC(pth_pqueue_delete) 
    SYMBOL_ITEM_FUNC(pth_pqueue_delmax) 
    SYMBOL_ITEM_FUNC(pth_pqueue_favorite) 
    SYMBOL_ITEM_FUNC(pth_pqueue_increase) 
    SYMBOL_ITEM_FUNC(pth_pqueue_init) 
    SYMBOL_ITEM_FUNC(pth_pqueue_insert) 
    SYMBOL_ITEM_FUNC(pth_pqueue_tail) 
    SYMBOL_ITEM_FUNC(pth_pqueue_walk) 
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
    SYMBOL_ITEM_FUNC(pth_sched_eventmanager) 
    SYMBOL_ITEM_FUNC(pth_sched_eventmanager_sighandler) 
    SYMBOL_ITEM_FUNC(pth_scheduler) 
    SYMBOL_ITEM_FUNC(pth_scheduler_drop) 
    SYMBOL_ITEM_FUNC(pth_scheduler_init) 
    SYMBOL_ITEM_FUNC(pth_scheduler_kill) 
    SYMBOL_ITEM_FUNC(pth_asprintf) 
    SYMBOL_ITEM_FUNC(pth_snprintf) 
    SYMBOL_ITEM_FUNC(pth_vasprintf) 
    SYMBOL_ITEM_FUNC(pth_vsnprintf) 
    SYMBOL_ITEM_FUNC(pth_barrier_init) 
    SYMBOL_ITEM_FUNC(pth_barrier_reach) 
    SYMBOL_ITEM_FUNC(pth_cond_await) 
    SYMBOL_ITEM_FUNC(pth_cond_init) 
    SYMBOL_ITEM_FUNC(pth_cond_notify) 
    SYMBOL_ITEM_FUNC(pth_mutex_acquire) 
    SYMBOL_ITEM_FUNC(pth_mutex_init) 
    SYMBOL_ITEM_FUNC(pth_mutex_release) 
    SYMBOL_ITEM_FUNC(pth_mutex_releaseall) 
    SYMBOL_ITEM_FUNC(pth_rwlock_acquire) 
    SYMBOL_ITEM_FUNC(pth_rwlock_init) 
    SYMBOL_ITEM_FUNC(pth_rwlock_release) 
    SYMBOL_ITEM_FUNC(pth_implicit_init) 
    SYMBOL_ITEM_FUNC(pth_syscall_init) 
    SYMBOL_ITEM_FUNC(pth_syscall_kill) 
    SYMBOL_ITEM_FUNC(pth_tcb_alloc) 
    SYMBOL_ITEM_FUNC(pth_tcb_free) 
    SYMBOL_ITEM_FUNC(pth_time) 
    SYMBOL_ITEM_FUNC(pth_time_cmp) 
    SYMBOL_ITEM_FUNC(pth_time_div) 
    SYMBOL_ITEM_FUNC(pth_time_mul) 
    SYMBOL_ITEM_FUNC(pth_time_pos) 
    SYMBOL_ITEM_FUNC(pth_time_t2d) 
    SYMBOL_ITEM_FUNC(pth_time_t2i) 
    SYMBOL_ITEM_FUNC(pth_time_usleep) 
    SYMBOL_ITEM_FUNC(pth_timeout) 
    SYMBOL_ITEM_FUNC(pth_util_cpystrn) 
    SYMBOL_ITEM_FUNC(pth_util_fd_valid) 
    SYMBOL_ITEM_FUNC(pth_util_fds_merge) 
    SYMBOL_ITEM_FUNC(pth_util_fds_select) 
    SYMBOL_ITEM_FUNC(pth_util_fds_test) 
    SYMBOL_ITEM_FUNC(pth_util_sigdelete) 
    SYMBOL_ITEM_FUNC(_ZdaPv) 
    SYMBOL_ITEM_FUNC(_ZdlPv) 
    SYMBOL_ITEM_FUNC(_Znaj) 
    SYMBOL_ITEM_FUNC(_Znwj) 
    SYMBOL_ITEM_FUNC(__cppRtDoCtors) 
    SYMBOL_ITEM_FUNC(__cppRtDoDtors) 
    SYMBOL_ITEM_FUNC(__cppRtDummy) 
    SYMBOL_ITEM_FUNC(_cppRtInit) 
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
    SYMBOL_ITEM_OBJ(pth_state_names) 
    SYMBOL_ITEM_OBJ(pth_internal_version) 
    SYMBOL_ITEM_OBJ(__pth_compat_unit) 
    SYMBOL_ITEM_OBJ(pth_errno_flag) 
    SYMBOL_ITEM_OBJ(pth_errno_storage) 
    SYMBOL_ITEM_OBJ(pth_initialized) 
    SYMBOL_ITEM_OBJ(pth_time_zero) 
    SYMBOL_ITEM_OBJ(ip_addr_any) 
    SYMBOL_ITEM_OBJ(ip_addr_broadcast) 
    SYMBOL_ITEM_OBJ(pth_DQ) 
    SYMBOL_ITEM_OBJ(pth_NQ) 
    SYMBOL_ITEM_OBJ(pth_RQ) 
    SYMBOL_ITEM_OBJ(pth_SQ) 
    SYMBOL_ITEM_OBJ(pth_WQ) 
    SYMBOL_ITEM_OBJ(pth_current) 
    SYMBOL_ITEM_OBJ(pth_favournew) 
    SYMBOL_ITEM_OBJ(pth_loadval) 
    SYMBOL_ITEM_OBJ(pth_main) 
    SYMBOL_ITEM_OBJ(pth_sched) 
    SYMBOL_ITEM_OBJ(__dso_handle) 
SYMBOL_TABLE_END												
#endif                                               
/*********************************************************************************************************	
** END FILE													
*********************************************************************************************************/	
