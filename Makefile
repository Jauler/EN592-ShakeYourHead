TARGET=ShakeYourHead

CSRCS=$(shell find . -name "*.c")

ASMSRCS=$(shell find . -name "*.S")

CFLAGS+=-Wall
CFLAGS+=-mcpu=cortex-m4
CFLAGS+=-mthumb
CFLAGS+=-g
CFLAGS+=-I./Lib/

CFLAGS+=-DSTM32F40_41xxx
CFLAGS+=-DUSE_STDPERIPH_DRIVER

LDFLAGS+=-TSTM32_flash.ld
LDFLAGS+=-nostartfiles
LDFLAGS+=-mcpu=cortex-m4
LDFLAGS+=-mthumb
LDFLAGS+=-g
LDFLAGS+=-lm

DEPFLAGS+=-MT $@ -MMD -MP -MF $*.d

STFLASH=st-flash


ifeq ($(CC),cc)
	CC=gcc
endif

SIZE?=size
OBJCOPY?=objcopy
CROSS?=arm-none-eabi-

OBJS=$(CSRCS:.c=.o)
OBJS+=$(ASMSRCS:.S=.o)

all: executable binary info

executable: $(OBJS) Makefile
	$(CROSS)$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET).elf

binary: executable Makefile
	$(CROSS)$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

info: binary
	$(CROSS)$(SIZE) -A $(TARGET).elf

flash: binary
	$(STFLASH) write $(TARGET).bin 0x08000000

%.o : %.c %.d Makefile
	$(CROSS)$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

%.o : %.S Makefile
	$(CROSS)$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(CSRCS:.c=.d)
	rm -f $(TARGET).elf
	rm -f $(TARGET).bin

%.d: ;
.PRECIOUS: %.d

-include $(patsubst %,%.d,$(basename $(CSRCS)))



