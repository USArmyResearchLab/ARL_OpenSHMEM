CC ?= gcc
CFLAGS ?= -Wall -Wno-unused-function -g -O3
DEFS ?=
INCS = -I..
LIBS =

CSRCS = $(wildcard *.c)
OBJS = $(notdir $(CSRCS:.c=.o))

SRC_FILES = generate.pl $(wildcard *.m4)

.PHONY: generate build clean distclean

all: generate

generate: shmem.h
	$(MAKE) build

build: $(OBJS)

shmem.h: $(SRC_FILES)
	./generate.pl
	ln -sf ./shmem_api.h ./shmem.h

.c.o:
	$(CC) $(CFLAGS) $(DEFS) $(INCS) $(LIBS) -c $<

clean:
	rm -f *.h *.c *.o

distclean: clean
