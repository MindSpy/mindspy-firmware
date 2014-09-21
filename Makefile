#_______________________________________________________________________________
#
#			  ItOpen Energia makefile
#             (heavily based on edam's Arduino makefile)
#_______________________________________________________________________________
#                                                                    version 0.1
#
# Copyright (C) 2013 Alessandro Pasotti
# Copyright (C) 2011, 1012 Tim Marston <tim@ed.am>.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#_______________________________________________________________________________
#
#
# This is a general purpose makefile for use with TI's MSP430 Launchpad hardware and
# and Energia Arduino clone software
#
# This makefile can be used as a drop-in replacement for the Energia IDE's
#
# The Energia software is required.  If you are using
#
#   export ENERGIADIR=~/somewhere/energia
#
# You will also need to set BOARD to the type of Arduino you're building for.
# Type `make boards` for a list of acceptable values.  You could set a default
# in your ~/.profile if you want, but it is suggested that you specify this at
# build time, especially if you work with different types of Arduino.  For
# example:
#
#   $ export ENERGIABOARD=lpmsp430g2553
#   $ make
#
# You may also need to set SERIALDEV if it is not detected correctly.
#
# The presence of a .ino (or .pde) file causes the arduino.mk to automatically
# determine values for SOURCES, TARGET and LIBRARIES.  Any .c, .cc and .cpp
# files in the project directory (or any "util" or "utility" subdirectories)
# are automatically included in the build and are scanned for Arduino libraries
# that have been #included. Note, there can only be one .ino (or .pde) file.
#
# Alternatively, if you want to manually specify build variables, create a
# Makefile that defines SOURCES and LIBRARIES and then includes arduino.mk.
# (There is no need to define TARGET).  Here is an example Makefile:
#
#   SOURCES := main.cc other.cc
#   LIBRARIES := EEPROM
#   include ~/src/energia.mk
#
# Here is a complete list of configuration parameters:
#
# ENERGIACONST The Energia/Arduino software version, as an integer, used to define the
#              ARDUINO/ENERGIA version constant. This defaults to 101 if undefined.
#
# BOARDFAMILY  this can be msp430 or lm4f (default)
#
#
# ENERGIADIR   The path where the Arduino software is installed on your system.
#
#
# ENERGIABOARD        Specify a target board type.  Run `make boards` to see available
#              board types.
#
# LIBRARIES    A list of Energia libraries to build and include.  This is set
#              automatically if a .ino (or .pde) is found.
#
# SERIALDEV    The unix device name of the serial device that is the Arduino.
#              If unspecified, an attempt is made to determine the name of a
#              connected Arduino's serial device.
#
# SOURCES      A list of all source files of whatever language.  The language
#              type is determined by the file extension.  This is set
#              automatically if a .ino (or .pde) is found.
#
# TARGET       The name of the target file.  This is set automatically if a
#              .ino (or .pde) is found, but it is not necessary to set it
#              otherwise.
#
# This makefile also defines the following goals for use on the command line
# when you run make:
#
# all          This is the default if no goal is specified.  It builds the
#              target.
#
# target       Builds the target.
#
# upload       Uploads the last built target to an attached Launchpad.
#
# clean        Deletes files created during the build.
#
# boards       Display a list of available board names, so that you can set the
#              BOARD environment variable appropriately.
#
# monitor      Start `screen` on the serial device.  This is meant to be an
#              equivalent to the Arduino serial monitor.
#
# size         Displays size information about the bulit target.
#
# <file>       Builds the specified file, either an object file or the target,
#              from those that that would be built for the project.
#_______________________________________________________________________________
#

include *.mk

# default arduino software directory, check software exists
ifndef ENERGIADIR
ENERGIADIR := $(firstword $(wildcard ~/energia /usr/share/energia ~/apps/energia))
endif
ifndef BOARDFAMILY
$(error BOARDFAMILY is not set correctly; valid values are lm4f msp430.)
endif
ifeq "$(wildcard $(ENERGIADIR)/hardware/$(BOARDFAMILY)/boards.txt)" ""
$(error ENERGIADIR is not set correctly; energia software not found)
endif

# default arduino version
ARDUINOCONST ?= 101
ENERGIACONST ?= 12


# auto mode?
INOFILE := $(wildcard *.ino *.pde)
ifdef INOFILE
ifneq "$(words $(INOFILE))" "1"
$(error There is more than one .pde or .ino file in this directory!)
endif

# automatically determine sources and targeet
TARGET := $(basename $(INOFILE))
SOURCES := $(INOFILE) \
	$(wildcard *.c *.cc *.cpp) \
	$(wildcard $(addprefix util/, *.c *.cc *.cpp)) \
	$(wildcard $(addprefix utility/, *.c *.cc *.cpp))

# automatically determine included libraries
LIBRARIES := $(filter $(notdir $(wildcard $(HOME)/sketchbook/libraries/*)), \
    $(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))

# automatically determine included libraries
LIBRARIES += $(filter $(notdir $(wildcard $(ENERGIADIR)/hardware/$(BOARDFAMILY)/libraries/*)), \
	$(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))




endif

# no serial device? make a poor attempt to detect an arduino
SERIALDEVGUESS := 0
ifeq "$(SERIALDEV)" ""
ifeq "$(ENERGIABOARD)" "lpmsp430f5529_25"
	SERIALDEV:= /dev/ttyACM1
else
	SERIALDEV:= /dev/ttyACM0
endif

#SERIALDEV := $(firstword $(wildcard \
#	/dev/ttyACM? /dev/ttyUSB? /dev/tty.usbserial* /dev/tty.usbmodem*))
SERIALDEVGUESS := 1
endif

# software
#CC := msp430-gcc

COMPILER_PREFIX := $(ENERGIADIR)/hardware/tools/$(BOARDFAMILY)/bin/
ifeq "$(BOARDFAMILY)" "lm4f"
BOARD_PREFIX := arm-none-eabi-
else
ifeq "$(BOARDFAMILY)" "msp430"
BOARD_PREFIX := msp430-
endif
endif

CC := $(COMPILER_PREFIX)$(BOARD_PREFIX)gcc
CXX := $(COMPILER_PREFIX)$(BOARD_PREFIX)g++
LD := $(COMPILER_PREFIX)$(BOARD_PREFIX)ld
AR := $(COMPILER_PREFIX)$(BOARD_PREFIX)ar
OBJCOPY := $(COMPILER_PREFIX)$(BOARD_PREFIX)objcopy
MSPDEBUG := $(COMPILER_PREFIX)mspdebug
GDB := $(COMPILER_PREFIX)$(BOARD_PREFIX)gdb
MSP430SIZE := $(COMPILER_PREFIX)$(BOARD_PREFIX)size
FLASH := $(COMPILER_PREFIX)lm4flash

# files
ifndef OUTDIR
OUTDIR := target/
endif
LIBOUTDIR := $(OUTDIR)lib/
DEPOUTDIR := $(OUTDIR)dep/
TARGET := $(if $(TARGET),$(TARGET),a.out)
OBJECTS := $(addprefix $(OUTDIR),$(addsuffix .o, $(basename $(SOURCES))))
DEPFILES := $(patsubst %, $(DEPOUTDIR)%.dep, $(SOURCES))
ENERGIACOREDIR := $(ENERGIADIR)/hardware/$(BOARDFAMILY)/cores/$(BOARDFAMILY)
ENERGIALIB := $(LIBOUTDIR)arduino.a
ENERGIALIBLIBSDIR := $(ENERGIADIR)/hardware/$(BOARDFAMILY)/libraries
ENERGIALIBLIBSPATH := $(foreach lib, $(LIBRARIES), \
	 $(HOME)/sketchbook/libraries/$(lib)/ $(ENERGIADIR)/libraries/$(lib)/ $(ENERGIACOREDIR)/libraries/$(lib) $(ENERGIALIBLIBSDIR)/$(lib) )
ENERGIALIBOBJS := $(foreach dir, $(ENERGIACOREDIR) $(ENERGIALIBLIBSPATH) $(ENERGIACOREDIR)/driverlib, \
	$(patsubst %, $(LIBOUTDIR)%.o, $(wildcard $(addprefix $(dir)/, *.c *.cpp))))


# no board?
ifndef ENERGIABOARD
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error ENERGIABOARD is unset.  Type 'make boards' to see possible values)
endif
endif
endif

# obtain board parameters from the arduino boards.txt file
BOARDS_FILE := $(ENERGIADIR)/hardware/$(BOARDFAMILY)/boards.txt
BOARD_BUILD_MCU := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.mcu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_FCPU := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.f_cpu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_VARIANT := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.variant=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_SPEED := \
	$(shell sed -ne "s/$(ENERGIABOARD).upload.speed=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_PROTOCOL := \
	$(shell sed -ne "s/$(ENERGIABOARD).upload.protocol=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_LD_SCRIPT := \
  $(shell sed -ne "s/$(ENERGIABOARD).ldscript=\(.*\)/\1/p" $(BOARDS_FILE))


# invalid board?
ifeq "$(BOARD_BUILD_MCU)" ""
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error ENERGIABOARD is invalid.  Type 'make boards' to see possible values)
endif
endif
endif

# flags
MSPDEBUG_PROTOCOL:= rf2500
MSPDEBUG_LD:=

ifeq "$(ENERGIABOARD)" "lpmsp430f5529_25"
	MSPDEBUG_LD:=LD_LIBRARY_PATH=~/energia-0101E0010/hardware/tools/msp430/bin/
	MSPDEBUG_PROTOCOL:= tilib
endif

CPPFLAGS := -Os -Wall -ggdb
CPPFLAGS +=  -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -fsingle-precision-constant  -mfloat-abi=hard -mfpu=fpv4-sp-d16
ifeq "$(BOARDFAMILY)" "lm4f"
CPPFLAGS += -mcpu=$(BOARD_BUILD_MCU)
CPPFLAGS += -mthumb
LINKFLAGS := -mcpu=$(BOARD_BUILD_MCU) -Os -nostartfiles -nostdlib -Wl,-gc-sections -T$(ENERGIACOREDIR)/$(BOARD_LD_SCRIPT) -Wl,--entry=ResetISR -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant 
else
CPPFLAGS += -mmcu=$(BOARD_BUILD_MCU)
LINKFLAGS := -mmcu=$(BOARD_BUILD_MCU) -Os -Wl,-gc-sections,-u,main -lm
endif
CPPFLAGS += -DF_CPU=$(BOARD_BUILD_FCPU) -DARDUINO=$(ARDUINOCONST)  -DENERGIA=$(ENERGIACONST)
CPPFLAGS += -I. -Iutil -Iutility -I$(ENERGIACOREDIR)
CPPFLAGS += -I$(ENERGIADIR)/hardware/$(BOARDFAMILY)/variants/$(BOARD_BUILD_VARIANT)/
CPPFLAGS += -I$(HOME)/sketchbook/hardware/$(BOARDFAMILY)/variants/$(BOARD_BUILD_VARIANT)/
CPPFLAGS += $(addprefix -I$(HOME)/sketchbook/libraries/,  $(LIBRARIES))
CPPFLAGS += $(addprefix -I$(ENERGIADIR)/hardware/$(BOARDFAMILY)/libraries/, $(LIBRARIES))
CPPDEPFLAGS = -MMD -MP -MF $(DEPOUTDIR)$<.dep
CPPINOFLAGS := -x c++ -include $(ENERGIACOREDIR)/Arduino.h
MSPDEBUGFLAGS :=  $(MSPDEBUG_PROTOCOL) 'erase' 'load $(TARGET).elf' 'exit'

# figure out which arg to use with stty
STTYFARG := $(shell stty --help > /dev/null 2>&1 && echo -F || echo -f)

# include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPFILES)
endif

# default rule
.DEFAULT_GOAL := all

#_______________________________________________________________________________
#                                                                          RULES

.PHONY:	all target upload clean boards monitor size proto

all: target

proto:
	$(MAKE) -C $@ regs_pb.c
	mv proto/regs_pb.[hc] ./

target:  $(OUTDIR)$(TARGET).bin

upload: $(OUTDIR)$(TARGET).bin
	@echo "\nUploading to board..."
	$(FLASH) $<


clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET).elf $(TARGET).bin $(ENERGIALIB) *~
	rm -rf $(OUTDIR)

boards:
	@echo Available values for BOARD:
	@sed -ne '/^#/d; /^[^.]\+\.name=/p' $(BOARDS_FILE) | \
		sed -e 's/\([^.]\+\)\.name=\(.*\)/\1            \2/' \
			-e 's/\(.\{14\}\) *\(.*\)/\1 \2/'

monitor:
	stty raw 9600 -F $(SERIALDEV)
	cat $(SERIALDEV)

size: $(OUTDIR)$(TARGET).elf
	echo && $(MSP430SIZE) $(OUTDIR)$(TARGET).elf

rebuild: clean all

debug:
	$(MSPDEBUG) rf2500 gdb
	cgdb  -d $(GDB) $(OUTDIR)$(TARGET).elf


# building the target


$(OUTDIR)$(TARGET).elf: regs_pb.c $(ENERGIALIB) $(OBJECTS)
	$(CXX) $(LINKFLAGS) $(OBJECTS) $(ENERGIALIB) -o $@ -lgcc -lc -lgcc -lm -lrdimon

$(OUTDIR)$(TARGET).bin: $(OUTDIR)$(TARGET).elf
	$(OBJCOPY) -O binary $< $@ 

$(OUTDIR)%.o: %.c
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.c) $(CPPDEPFLAGS) -o $@ $<

$(OUTDIR)%.o: %.cpp
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

$(OUTDIR)%.o: %.cc
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

$(OUTDIR)%.o: %.C
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

$(OUTDIR)%.o: %.ino
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $(CPPINOFLAGS) $<

$(OUTDIR)%.o: %.pde
	mkdir -p $(DEPOUTDIR)$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ -x c++ -include $(ENERGIACOREDIR)/Arduino.h $<

# building the arduino library

$(ENERGIALIB): $(ENERGIALIBOBJS)
	$(AR) rcs $@ $?

$(LIBOUTDIR)%.c.o: %.c
	mkdir -p $(dir $@)
	$(COMPILE.c) -o $@ $<

$(LIBOUTDIR)%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

$(LIBOUTDIR)%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

$(LIBOUTDIR)%.C.o: %.C
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<
