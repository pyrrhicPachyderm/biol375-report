SHELL := /bin/bash
LATEXMK_FLAGS = --pdf --cd
maindoc := report
RM := rm -f

all: $(maindoc).pdf

$(maindoc).pdf: $(maindoc).tex $(maindoc).bib reference-style.tex
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
		$(RM) **/*-cropped.pdf;\
	)
Clean: clean
	@$(RM) $(maindoc).pdf
	@$(RM) notes/presence-absence-notes.pdf

.PHONY: all pvc clean Clean

notes/presence-absence-notes.pdf: notes/presence-absence-notes.tex
	#Need shell's basename to remove the directory, and make's basename to remove the extension
	latexmk $(LATEXMK_FLAGS) --jobname="$(basename $(shell basename $@))" $<

notes: notes/presence-absence-notes.pdf

.PHONY: notes
