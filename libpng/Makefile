

LIB = libpng.a
DLL = libpng.so


DSYMBOL = -DSMILEOS


DEPEND_DLL =


OBJS = \
libpng/png.o \
libpng/pngerror.o \
libpng/pngget.o \
libpng/pngmem.o \
libpng/pngpread.o \
libpng/pngread.o \
libpng/pngrio.o \
libpng/pngrtran.o \
libpng/pngrutil.o \
libpng/pngset.o \
libpng/pngtrans.o \
libpng/pngwio.o \
libpng/pngwrite.o \
libpng/pngwtran.o \
libpng/pngwutil.o


INCDIR  = "../SmileOS/src/include"
INCDIR += "../SmileOS/src/net/include"
INCDIR += "../SmileOS/src/net/include/ipv4"
INCDIR += "./libpng"
INCDIR += "../libz/zlib"

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
