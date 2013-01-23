

LIB = libruntime.a
DLL = libruntime.so


DSYMBOL = -DSMIELOS -D__LINUX_ERRNO_EXTENSIONS__


DEPEND_DLL = 


OBJS = \
crt0.o \
../kern/heap.o \
../libc/libc_fix.o \
../kern/posix_fix.o \
../kern/sys_call.o \
../net/core/def.o \
../net/core/ipv4/ip_addr.o \
pth/pthread.o \
pth/pth_attr.o \
pth/pth_cancel.o \
pth/pth_clean.o \
pth/pth_compat.o \
pth/pth_data.o \
pth/pth_debug.o \
pth/pth_errno.o \
pth/pth_event.o \
pth/pth_ext.o \
pth/pth_high.o \
pth/pth_lib.o \
pth/pth_mctx_arm920t_c.o \
pth/pth_msg.o \
pth/pth_pqueue.o \
pth/pth_ring.o \
pth/pth_sched.o \
pth/pth_string.o \
pth/pth_sync.o \
pth/pth_syscall.o \
pth/pth_tcb.o \
pth/pth_time.o \
pth/pth_util.o \
pth/pth_vers.o \
pth/pth_mctx_arm920t_asm.o \
cpp/cppEabiLib.o \
cpp/cppMemLib.o \
cpp/cppRtBegin.o \
cpp/cppRtEnd.o \
cpp/cppSupLib.o


INCDIR  = "../include"
INCDIR += "../net/include"
INCDIR += "../net/include/ipv4"


COMMONFLAGS = -Wall -c -fmessage-length=0 -mcpu=arm920t -g3 -gdwarf-2 -fPIC

ASFLAGS  = -x assembler-with-cpp $(DSYMBOL) $(addprefix -I,$(INCDIR)) $(COMMONFLAGS)

CFLAGS   = $(DSYMBOL) $(addprefix -I,$(INCDIR)) -O3 -Os $(COMMONFLAGS)

CXXFLAGS = $(DSYMBOL) $(addprefix -I,$(INCDIR)) -O3 -Os -fno-exceptions -fno-rtti $(COMMONFLAGS)

ARFLAGS  = -r


CC  = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
AS  = arm-none-eabi-gcc
AR  = arm-none-eabi-ar
LD  = arm-none-eabi-g++


#all: $(LIB) $(DLL)
all: $(LIB)

$(LIB): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC Archiver'
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

$(DLL): $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC Linker'
	$(LD) -mcpu=arm920t -nostartfiles -nostdlib -nodefaultlibs -pie -shared -o $(DLL) $(OBJS) $(DEPEND_DLL) -lm -lgcc
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	rm -rf $(LIB) $(OBJS) $(DLL)

cleanobjs:
	rm -rf $(OBJS)
