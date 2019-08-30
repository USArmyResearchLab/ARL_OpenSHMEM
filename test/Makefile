CC ?= gcc
DEFS = 

CFLAGS = -g -O3 -Wall -Wno-unused-function

CORES = 16

NP = $(shell seq 1 $(CORES))

INCS = -I. -I../src -I../src/autogen
LIBS = 
LIBS += -L../src -lshmem -lrt -lpthread

# Uncomment and comment out preceeding line to use header only library
#DEFS += -DSHMEM_USE_HEADER_ONLY

FIGURES = ./figures/
SRC_FILES = $(wildcard *.c)
PLOT_FILES = $(notdir $(wildcard $(FIGURES)*.gnuplot))
FIGS = $(PLOT_FILES:.gnuplot=.pdf)

DEPENDS = 

TARGETS = \
	alltoall64.x atomic_add.x atomic_compare_swap_eq.x \
	atomic_compare_swap_neq.x atomic_fetch_add.x atomic_fetch.x \
	atomic_fetch_inc.x atomic_inc.x atomic_set.x atomic_swap.x barrier.x \
	broadcast32.x broadcast64.x collect32.x collect64.x fcollect32.x \
	fcollect64.x get.x get_nb.x get_nb_dual.x hello.x put.x \
	put32.x put64.x put_nb.x put_nb_dual.x reduce.x

OBJ_FILES = $(DEPENDS) $(SRC_FILES:.c=.o)

all: $(TARGETS)

figures: $(FIGS)

info: all $(OBJ_FILES)

report: report.pdf

.PHONY: run test check clean install uninstall

.SUFFIXES:
.SUFFIXES: .c .o .x .S .dat .pdf .tex
#.PRECIOUS: %.dat

.c.o:
	$(CC) -c $(DEFS) $(INCS) $< $(LIBS)

%.x: %.c $(DEPENDS)
	$(CC) $(DEFS) $(INCS) $< $(DEPENDS) -o $@ $(LIBS)

.x.dat:
	../tools/oshrun -np $(CORES) ./$< 1> $@

version.dat: hello.x
	./$< | head -n 1 > $@

git.dat:
	echo -n "git commit: " > $@
	git rev-parse HEAD >> $@

pes.dat:
	echo -n $(CORES) > $@

%.pdf: %.dat $(FIGURES)%.gnuplot
	gnuplot -e "filename='$@'; set loadpath '$(FIGURES)'; load '$*.gnuplot'"

atomics.pdf: atomic_add.dat atomic_compare_swap_neq.dat atomic_compare_swap_eq.dat atomic_swap.dat atomic_fetch_inc.dat atomic_inc.dat atomic_fetch_add.dat atomic_fetch.dat atomic_set.dat $(FIGURES)atomics.gnuplot
	gnuplot -e "filename='$@'; set loadpath '$(FIGURES)'; load 'atomics.gnuplot'"

report.pdf: figures version.dat git.dat pes.dat report.tex
	pdflatex report.tex
	pdflatex report.tex # re-running to get indexes right
	pdflatex report.tex # re-running to get indexes right

run: $(TARGETS)
	@$(foreach x,$(TARGETS),../tools/oshrun -np $(CORES) ./$(x) 2> /dev/null &&) echo "Complete"

test: check
check: $(TARGETS)
	./check.sh

clean:
	rm -f *.aux
	rm -f *.log
	rm -f *.o
	rm -f *.dat

distclean: clean
	rm -f *.x
	rm -f *.pdf
