CROSS_COMPILE=arm-none-eabi-
QEMU=qemu-system-arm

TARGET		= trymos

CSOURCES	:= $(wildcard *.c)
SSOURCES	:= $(wildcard *.S)

COBJECTS	:= $(CSOURCES:.c=.o)
SOBJECTS	+= $(SSOURCES:.S=.o)

CFLAGS		:= -I./include -Wall -march=armv7-a -c
SFLAGS		:= -mcpu=cortex-a9

CC		= $(CROSS_COMPILE)gcc
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
OBJCOPY		= $(CROSS_COMPILE)objcopy

$(TARGET) : $(TARGET).bin
	$(QEMU) -M realview-pbx-a9 -smp 1 -m 128M -kernel $<

$(TARGET).bin : $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf : $(COBJECTS) $(SOBJECTS)
	$(LD) -T load.ld $(COBJECTS) $(SOBJECTS) -o $@

.c.o :
	$(CC) $(CFLAGS) $^ -o $@

.S.o :
	$(AS) $(SFLAGS) $^ -o $@

clean:
	@rm -f *.o $(TARGET).bin $(TARGET).elf
