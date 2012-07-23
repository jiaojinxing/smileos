

LIB = libjpeg.a
DLL = libjpeg.so


DSYMBOL = -DSMILEOS -DHAVE_PROTOTYPES


DEPEND_DLL =


OBJS = \
jpeg/cdjpeg.o \
jpeg/jaricom.o \
jpeg/jcapimin.o \
jpeg/jcapistd.o \
jpeg/jcarith.o \
jpeg/jccoefct.o \
jpeg/jccolor.o \
jpeg/jcdctmgr.o \
jpeg/jchuff.o \
jpeg/jcinit.o \
jpeg/jcmainct.o \
jpeg/jcmarker.o \
jpeg/jcmaster.o \
jpeg/jcomapi.o \
jpeg/jcparam.o \
jpeg/jcprepct.o \
jpeg/jcsample.o \
jpeg/jctrans.o \
jpeg/jdapimin.o \
jpeg/jdapistd.o \
jpeg/jdarith.o \
jpeg/jdatadst.o \
jpeg/jdatasrc.o \
jpeg/jdcoefct.o \
jpeg/jdcolor.o \
jpeg/jddctmgr.o \
jpeg/jdhuff.o \
jpeg/jdinput.o \
jpeg/jdmainct.o \
jpeg/jdmarker.o \
jpeg/jdmaster.o \
jpeg/jdmerge.o \
jpeg/jdpostct.o \
jpeg/jdsample.o \
jpeg/jdtrans.o \
jpeg/jerror.o \
jpeg/jfdctflt.o \
jpeg/jfdctfst.o \
jpeg/jfdctint.o \
jpeg/jidctflt.o \
jpeg/jidctfst.o \
jpeg/jidctint.o \
jpeg/jmemansi.o \
jpeg/jmemmgr.o \
jpeg/jquant1.o \
jpeg/jquant2.o \
jpeg/jutils.o \
jpeg/rdbmp.o \
jpeg/rdcolmap.o \
jpeg/rdgif.o \
jpeg/rdppm.o \
jpeg/rdrle.o \
jpeg/rdswitch.o \
jpeg/rdtarga.o \
jpeg/transupp.o \
jpeg/wrbmp.o \
jpeg/wrgif.o \
jpeg/wrppm.o \
jpeg/wrrle.o \
jpeg/wrtarga.o


INCDIR  = "../SmileOS/src/include"
INCDIR += "../SmileOS/src/net/include"
INCDIR += "../SmileOS/src/net/include/ipv4"
INCDIR += "./jpeg"


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
