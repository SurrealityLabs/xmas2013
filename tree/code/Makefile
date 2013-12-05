# Name: Makefile
# Author: <insert your name here>
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = attiny25
CLOCK      = 8000000
PROGRAMMER = -c usbtiny
OBJECTS    = main.o
FUSES	   = -U lfuse:w:0xe2:m -U hfuse:w:0x5f:m -U efuse:w:0xff:m

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 10 -F
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	christmas-tree.hex	size

main.o:

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:christmas-tree.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

size:	christmas-tree.elf
	avr-size --mcu=$(DEVICE) --format=avr christmas-tree.elf
	
ident:
	$(AVRDUDE)

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID christmas-tree.hex

clean:
	rm -f christmas-tree.hex christmas-tree.elf $(OBJECTS)

# file targets:
christmas-tree.elf: $(OBJECTS)
	$(COMPILE) -o christmas-tree.elf $(OBJECTS)

christmas-tree.hex: christmas-tree.elf
	rm -f christmas-tree.hex
	avr-objcopy -j .text -j .data -O ihex christmas-tree.elf christmas-tree.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	christmas-tree.elf
	avr-objdump -d christmas-tree.elf

cpp:
	$(COMPILE) -E main.c
