# Makefile for oshrun
exec_prefix=${prefix}

### this useless define is needed to silence useless autoconf warning
datarootdir=${prefix}/share

BUILD=$(shell date +"%Y%m%d.%H%M")

## this seems fragile
VENDOR_INC = ../src/autogen/shmem_api_top.m4
VENDOR=$(shell grep -A 1 "VENDOR_STRING" $(VENDOR_INC) | awk -F'"' '$$0=$$2')

DEFS += -DBUILD_STRING="\"$(BUILD)\""
DEFS += -DVENDOR_STRING="\"$(VENDOR)\""

######################################################################

INSTALL_BIN_DIR=${exec_prefix}/bin
INSTALL_LIB_DIR=${exec_prefix}/lib
INSTALL_INCLUDE_DIR=${prefix}/include
INSTALL_MAN_DIR=${datarootdir}/man

INSTALL_BINS = oshrun

INCS += -I./

CFLAGS += -Wextra -O3 -fPIC

TARGET = oshrun

all: $(TARGET)

.PHONY: install uninstall clean

.SUFFIXES:
.SUFFIXES: .c .o 

oshrun: oshrun.c
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -o oshrun oshrun.c

.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -c $<

install:
	test -d $(INSTALL_BIN_DIR) || install -m 755 -d $(INSTALL_BIN_DIR)
	install -m 755 $(INSTALL_BINS) $(INSTALL_BIN_DIR)

uninstall:
	rm -f $(INSTALL_BIN_DIR)/$(BIN_NAME)

clean:
	rm -f *.o *.s

distclean: clean
	rm -f $(TARGET)

