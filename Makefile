
CROSS_COMPILE = arm-none-eabi-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -Os  -DCPU_S32K144LFT0MLLT -std=gnu99
CFLAGS += -I $(shell pwd)/sdk/device/  -I $(shell pwd)/sdk/driver/inc/
CFLAGS += -mcpu=cortex-m4 -mthumb 
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -g

LDFLAGS := -Tsdk/device/linker/S32K144_64_flash.ld
LDFLAGS += -mcpu=cortex-m4 -mthumb
#LDFLAGS += -specs=nano.specs
#LFLAGS  += -u_printf_float #Use this option if you want print float
LDFLAGS += -Wl,--gc-sections -Wl,-Map=s32k144.map,--cref
export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := s32k144.elf

obj-y += sdk/
obj-y += user/

all : 
	make -C ./ -f $(TOPDIR)/Makefile.build
	$(CC) $(LDFLAGS) -o $(TARGET) built-in.o
	$(OBJCOPY) -O binary -S $(TARGET) s32k144.bin 

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bin")
	rm -f $(TARGET)

distclean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bin")
	rm -f $(shell find -name "*.d")
	rm -f $(shell find -name "*.map")
	rm -f $(TARGET)