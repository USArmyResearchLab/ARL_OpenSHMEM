# Makefile for oshrun

#include ../../version
VERSION_STRING=$(version_string)
VERSION_NAME=$(version_name)

exec_prefix=${prefix}

### this useless define is needed to silence useless autoconf warning
datarootdir=${prefix}/share

DATE = $(shell date +"%Y%m%d.%H%M")
BUILD_STRING=$(DATE)

DEFS += -D__BUILD_STRING="\"$(BUILD_STRING)\""
DEFS += -D__VERSION_STRING="\"$(VERSION_STRING)\""
DEFS += -D__VERSION_NAME="\"$(VERSION_NAME)\""

######################################################################

INSTALL_BIN_DIR=${exec_prefix}/bin
INSTALL_LIB_DIR=${exec_prefix}/lib
INSTALL_INCLUDE_DIR=${prefix}/include
INSTALL_MAN_DIR=${datarootdir}/man

INSTALL_BINS = oshrun

INCS += -I../../include

CFLAGS += -O1 -fPIC 

TARGET = oshrun

all: $(TARGET)

.PHONY: install uninstall clean

.SUFFIXES:
.SUFFIXES: .c .o 

oshrun_sh.h: oshrun.sh
	perl -pi -e 'chomp if eof' oshrun.sh # must remove newline at end of file
	sed -e "s/__VERSION_STRING/$(VERSION_STRING)/" \
		-e "s/__VERSION_NAME/$(VERSION_NAME)/" \
		-e "s/__BUILD_STRING/$(BUILD_STRING)/" oshrun.sh > tmp_oshrun.sh
	xxd -i tmp_oshrun.sh > oshrun_sh.h

oshrun: oshrun.c oshrun_sh.h
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
	rm -f oshrun_sh.h tmp_oshrun.sh

distclean: clean
	rm -f $(TARGET)

