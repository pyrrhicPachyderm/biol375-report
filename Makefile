SHELL := /bin/bash
LATEXMK_FLAGS = --pdf --cd
maindoc := report
RM := rm -f

c_scripts := scripts/critchlow-metrics

figures := figures/richness-regression.pdf figures/abundance-regression.pdf figures/site-ordination.pdf figures/taxa-ordination.pdf figures/shell-abundance-regression.pdf
results := results/richness-regression.tex results/abundance-regression.tex results/permanova.tex results/permdisp.tex results/shell-abundance-regression.tex


all: $(maindoc).pdf

$(maindoc).pdf: $(maindoc).tex $(maindoc).bib reference-style.tex $(figures) $(results)
	latexmk $(LATEXMK_FLAGS) --jobname="$(basename $@)" $<
pvc: $(maindoc).pdf
	latexmk $(LATEXMK_FLAGS) --jobname="$(basename $<)" $< --pvc
clean:
	@latexmk $(LATEXMK_FLAGS) -c -silent
	@(\
		shopt -s globstar;\
		$(RM) **/*.aux **/*.fls **/*.fdb_latexmk;\
		$(RM) **/*.bbl **/*.run.xml **/*.auxlock;\
		$(RM) **/*.dep **/*.dpth **/*.log **/*.md5 **/*-figure*.pdf;\
	)
	@(\
		shopt -s globstar;\
		$(RM) $(c_scripts);\
		$(RM) **/*.o **/*.d\
	)
	@$(RM) data/processed/*
	@$(RM) figures/*
	@$(RM) results/*
Clean: clean
	@$(RM) $(maindoc).pdf
	@$(RM) notes/presence-absence-notes.pdf
clear:
	clear
	clear

.PHONY: all pvc clean Clean clear

notes/presence-absence-notes.pdf: notes/presence-absence-notes.tex
	#Need shell's basename to remove the directory, and make's basename to remove the extension
	latexmk $(LATEXMK_FLAGS) --jobname="$(basename $(shell basename $@))" $<

notes: notes/presence-absence-notes.pdf

.PHONY: notes

data/processed/taxa-list.csv: data/raw/taxa-list.csv scripts/csv-fill-missing scripts/csv-strip-empty
	< $< \
		./$(word 2,$^) 0 \
		| ./$(word 3,$^) \
		> $@

data/processed/site-distance-matrix.csv: scripts/critchlow-metrics data/processed/taxa-list.csv
	./$< -t tau $(word 2,$^) $@
data/processed/taxa-distance-matrix.csv: scripts/critchlow-metrics data/processed/taxa-list.csv
	./$< tau $(word 2,$^) $@
data/processed/taxa-list-tidy.csv: scripts/gather data/processed/taxa-list.csv
	./$< -t $(word 2,$^) $@
data/processed/taxa-abundance.csv: scripts/summarise data/processed/taxa-list-tidy.csv
	./$< sum $(word 2,$^) $@
data/processed/taxa-richness.csv: scripts/summarise data/processed/taxa-list-tidy.csv
	./$< count $(word 2,$^) $@

data/processed/taxa-list-shelled.csv: scripts/variable-multiply data/processed/taxa-list-tidy.csv data/raw/taxa-shell-data.csv
	./$< $(word 2,$^) $(word 3,$^) $@
data/processed/taxa-abundance-shelled.csv: scripts/summarise data/processed/taxa-list-shelled.csv
	./$< sum $(word 2,$^) $@

data/processed/pfankuch-tidy.csv: scripts/gather data/raw/pfankuch.csv
	./$< -t -i 1 $(word 2,$^) $@
data/processed/pfankuch-total.csv: scripts/summarise data/processed/pfankuch-tidy.csv
	./$< sum $(word 2,$^) $@

figures/richness-regression.pdf: scripts/regression data/processed/taxa-richness.csv data/processed/pfankuch-total.csv
	./$< quasipoisson $(word 2,$^) $(word 3,$^) -p $@ -x "Pfankuch Stability Score" -y "Taxonomic Richness" -w 9 -v 3.8
figures/abundance-regression.pdf: scripts/regression data/processed/taxa-abundance.csv data/processed/pfankuch-total.csv
	./$< quasipoisson $(word 2,$^) $(word 3,$^) -p $@ -x "Pfankuch Stability Score" -y "Total Coded Abundance" -w 9 -v 3.8

results/richness-regression.tex: scripts/regression data/processed/taxa-richness.csv data/processed/pfankuch-total.csv
	./$< quasipoisson $(word 2,$^) $(word 3,$^) -t $@
results/abundance-regression.tex: scripts/regression data/processed/taxa-abundance.csv data/processed/pfankuch-total.csv
	./$< quasipoisson $(word 2,$^) $(word 3,$^) -t $@

figures/shell-abundance-regression.pdf: scripts/regression data/processed/taxa-abundance-shelled.csv data/processed/pfankuch-total.csv data/processed/taxa-abundance.csv
	./$< binomial $(word 2,$^) $(word 3,$^) -b $(word 4,$^) -p $@ -x "Pfankuch Stability Score" -y "Proportion of Shelled Individuals" -w 9 -v 3.8
results/shell-abundance-regression.tex: scripts/regression data/processed/taxa-abundance-shelled.csv data/processed/pfankuch-total.csv data/processed/taxa-abundance.csv
	./$< binomial $(word 2,$^) $(word 3,$^) -b $(word 4,$^) -t $@

figures/site-ordination.pdf: scripts/mds data/processed/site-distance-matrix.csv data/raw/stream-stability.csv
	./$< $(word 2,$^) -p $@ -f $(word 3,$^) -e
figures/taxa-ordination.pdf: scripts/mds data/processed/taxa-distance-matrix.csv
	./$< $(word 2,$^) -p $@

results/permanova.tex: scripts/mds data/processed/site-distance-matrix.csv data/raw/stream-stability.csv
	./$< $(word 2,$^) -f $(word 3,$^) -a $@
results/permdisp.tex: scripts/mds data/processed/site-distance-matrix.csv data/raw/stream-stability.csv
	./$< $(word 2,$^) -f $(word 3,$^) -d $@

######################################################################################################
#Stuff for making C files. Making extensive use of Peter Miller's "Recursive Make Considered Harmful".
######################################################################################################
CC := gcc
CFLAGS += -Wall -Wpedantic -Wstrict-prototypes -Wextra -Wno-unused-parameter -O3
CDIRS := scripts
CEXECS := $(c_scripts)
CLIBS := -lgsl -lgslcblas -lm

###############################
#A big ugly pile of make hacks.
#All the stuff that might need editing as the project changes should be above here.
###############################
CFLAGS += $(patsubst %,-I%,$(CDIRS))
#Find all C files
CSRC := $(shell find -name "*.c")
#Find all .c files except those that lead to excutables (all the ones without mains)
CSUPPSRC := $(shell find -name "*.c" $(shell printf "! -name %s.c " $(shell basename -s.out $(CEXECS))))
#Determine the object files
COBJ := $(patsubst %.c,%.o,$(CSRC))
CSUPPOBJ := $(patsubst %.c,%.o,$(CSUPPSRC))
#Link the object files
#Note that this will fail if any of the C files contain main functions but *don't* have a corresponding listing in CEXECS
#It also assumes all executables depend on all object files, but I can't think how to fix that nicely.
%: %.o $(CSUPPOBJ)
	$(CC) $^ -o $@ $(CLIBS)
#Include the autogenerated dependencies files
include $(COBJ:.o=.d)
#Rule to build dependencies files
%.d: %.c depend
	./depend $(shell dirname $*.c) $(CFLAGS) $*.c > $@
