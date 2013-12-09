# Name: Makefile
# Author: Randy Glenn <randy@surrealitylabs.com
# Copyright: 2013. All rights reserved.
# License: GPL 3.0

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
PROGRAMMER_PI = -c linuxspi -P /dev/spidev0.0
BATCH_MODE ?= nonbatch
OBJECTS    = main.o debounce.o charlieplex.o xmas_common.o
CHRISTMAS-TREE-OBJECTS = christmas_tree.o
SNOWMAN-OBJECTS = snowman.o
SNOWFLAKE-OBJECTS = snowflake.o
CANDYCANE-OBJECTS = candycane.o
FUSES	   = -U lfuse:w:0xe2:m -U hfuse:w:0x5f:m -U efuse:w:0xff:m

ifeq ($(BATCH_MODE),pigrammer)
	AVRDUDE = avrdude $(PROGRAMMER_PI) -p $(DEVICE) -B 10 -F
else
	AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 10 -F
endif
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	christmas-tree.hex	christmas-tree-size  snowman.hex  snowman-size  snowflake.hex  snowflake-size  candycane.hex  candycane-size

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

christmas-tree-flash:	christmas-tree.hex  christmas-tree-size
	$(AVRDUDE) -U flash:w:christmas-tree.hex:i

snowman-flash:   snowman.hex snowman-size
	$(AVRDUDE) -U flash:w:snowman.hex:i

snowflake-flash:   snowflake.hex snowflake-size
	$(AVRDUDE) -U flash:w:snowflake.hex:i

candycane-flash:   candycane.hex candycane-size
	$(AVRDUDE) -U flash:w:candycane.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

christmas-tree-size:	christmas-tree.elf
	avr-size --mcu=$(DEVICE) --format=avr christmas-tree.elf
	
snowman-size:	snowman.elf
	avr-size --mcu=$(DEVICE) --format=avr snowman.elf

snowflake-size:	snowflake.elf
	avr-size --mcu=$(DEVICE) --format=avr snowflake.elf

candycane-size:	candycane.elf
	avr-size --mcu=$(DEVICE) --format=avr candycane.elf

ident:
	$(AVRDUDE)

clean:
	rm -f $(OBJECTS)
	rm -f christmas-tree.hex christmas-tree.elf $(CHRISTMAS-TREE-OBJECTS)
	rm -f snowman.hex snowman.elf $(SNOWMAN-OBJECTS)
	rm -f snowflake.hex snowflake.elf $(SNOWFLAKE-OBJECTS)
	rm -f candycane.hex candycane.elf $(CANDYCANE-OBJECTS)

# file targets:
christmas-tree.elf: $(OBJECTS) $(CHRISTMAS-TREE-OBJECTS)
	$(COMPILE) -o christmas-tree.elf $(OBJECTS) $(CHRISTMAS-TREE-OBJECTS)

snowman.elf: $(OBJECTS) $(SNOWMAN-OBJECTS)
	$(COMPILE) -o snowman.elf $(OBJECTS) $(SNOWMAN-OBJECTS)

snowflake.elf: $(OBJECTS) $(SNOWFLAKE-OBJECTS)
	$(COMPILE) -o snowflake.elf $(OBJECTS) $(SNOWFLAKE-OBJECTS)

candycane.elf: $(OBJECTS) $(CANDYCANE-OBJECTS)
	$(COMPILE) -o candycane.elf $(OBJECTS) $(CANDYCANE-OBJECTS)

christmas-tree.hex: christmas-tree.elf
	rm -f christmas-tree.hex
	avr-objcopy -j .text -j .data -O ihex christmas-tree.elf christmas-tree.hex

snowman.hex: snowman.elf
	rm -f snowman.hex
	avr-objcopy -j .text -j .data -O ihex snowman.elf snowman.hex

snowflake.hex: snowflake.elf
	rm -f snowflake.hex
	avr-objcopy -j .text -j .data -O ihex snowflake.elf snowflake.hex

candycane.hex: candycane.elf
	rm -f candycane.hex
	avr-objcopy -j .text -j .data -O ihex candycane.elf candycane.hex
