CC ?= gcc
AR ?= ar
CFLAGS ?= -Wall -Wno-unused-function -g -O3
DEFS =
GENPATH = ./autogen
INCS = -I. -I$(GENPATH)
LIBS =

#DEFS += -DENABLE_DEBUG

TARGETS = libshmem_x86_64.a libshmem.a

all: $(TARGETS)

SRC_FILES = $(notdir $(wildcard ./*.c))

OBJS = $(notdir $(SRC_FILES:.c=.o)) 

.PHONY: clean install uninstall

.SUFFIXES:
.SUFFIXES: .S .c .o

.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(INCS) $(LIBS) -c $<

$(GENPATH)/shmem.h:
	$(MAKE) -C $(GENPATH)

$(OBJS): $(GENPATH)/shmem.h

libshmem_x86_64.a: $(OBJS) 
	$(AR) rcv $@ $(OBJS) $(GENPATH)/*.o

libshmem.a: libshmem_x86_64.a
	ln -sf $< $@

.S.o:
	gcc -c $<

clean:
	rm -f *.elf *.o
	$(MAKE) -C $(GENPATH) clean

distclean: clean 
	rm -f $(TARGETS)
	$(MAKE) -C $(GENPATH) distclean
