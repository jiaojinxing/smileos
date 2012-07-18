

LIB = libfreetype.a
DLL = libfreetype.so


DSYMBOL = -DFT2_BUILD_LIBRARY -DHAVE_FCNTL_H


DEPEND_DLL =


OBJS = \
freetype/src/autofit/autofit.o \
freetype/src/bdf/bdf.o \
freetype/src/cff/cff.o \
freetype/src/base/ftbase.o \
freetype/src/base/ftbbox.o \
freetype/src/base/ftbdf.o \
freetype/src/base/ftbitmap.o \
freetype/src/base/ftfstype.o \
freetype/src/base/ftgasp.o \
freetype/src/cache/ftcache.o \
freetype/src/base/ftglyph.o \
freetype/src/base/ftgxval.o \
freetype/src/base/ftlcdfil.o \
freetype/src/gzip/ftgzip.o \
freetype/src/base/ftinit.o \
freetype/src/lzw/ftlzw.o \
freetype/src/base/ftmm.o \
freetype/src/base/ftotval.o \
freetype/src/base/ftpfr.o \
freetype/src/base/ftstroke.o \
freetype/src/base/ftsynth.o \
freetype/src/base/fttype1.o \
freetype/src/base/ftwinfnt.o \
freetype/src/base/ftxf86.o \
freetype/src/pcf/pcf.o \
freetype/src/pfr/pfr.o \
freetype/src/psaux/psaux.o \
freetype/src/pshinter/pshinter.o \
freetype/src/psnames/psmodule.o \
freetype/src/raster/raster.o \
freetype/src/sfnt/sfnt.o \
freetype/src/smooth/smooth.o \
freetype/src/truetype/truetype.o \
freetype/src/type1/type1.o \
freetype/src/cid/type1cid.o \
freetype/src/type42/type42.o \
freetype/src/winfonts/winfnt.o \
freetype/src/base/ftsystem.o


INCDIR  = "E:/SmileOS/SmileOS/src/include"
INCDIR += "E:/SmileOS/SmileOS/src/net/include"
INCDIR += "E:/SmileOS/SmileOS/src/net/include/ipv4"
INCDIR += "E:/SmileOS/libfreetype/freetype/include"
INCDIR += "E:/SmileOS/libfreetype/freetype/include/freetype/config"


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
