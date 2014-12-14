OBJECTS=blink.o 
PYOBJ  =pulseWidthTable.h

GCC_DIR =  /home/mark/ti/gcc/bin
SUPPORT_FILE_DIRECTORY = /home/mark/ti/gcc/include

DEVICE  = msp430g2553
CC      = $(GCC_DIR)/msp430-elf-gcc
GDB     = $(GCC_DIR)/msp430-elf-gdb
PY      = python

CFLAGS = -I $(SUPPORT_FILE_DIRECTORY) -mmcu=$(DEVICE) -O2
LFLAGS = -L $(SUPPORT_FILE_DIRECTORY)
MAPFLAGS = -Wl,-Map=$(DEVICE).map,--cref
MINRT = -minrt

all: $(OBJECTS) $(PYOBJ)
	$(PY) $(basename $(PYOBJ)).py 
	$(CC) $(CFLAGS) $(LFLAGS) $(MINRT) $? -o $(DEVICE).o

blink.o: pulseWidthTable.h

.PHONY: load
load: 
	sudo mspdebug rf2500 prog $(DEVICE).o

debug: all
	$(GDB) $(DEVICE).o

.PHONY: clean
clean:
	rm *.o pulseWidthTable.h
