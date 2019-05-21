include ../../makeconfig.mk

DOCUMENTS = AquaPi_RiskAnalysis.pdf

ifeq ($(OS),Windows_NT)
    ; = ;
else
    ; = :
endif

TEXINPUTS = texmf//$(;)

# dubiously speed up by eliminating all implicit rules
.SUFFIXES: 

.PHONY: FORCE

.DEFAULT_GOAL := all

#make TEXINPUTS paths available for child processes
export TEXINPUTS

# Include all dependencies found in the dependency dir
include $(DEPS)

gen_tex:

.PHONY: all
all: gen_tex $(DOCUMENTS)

%.pdf: %.tex $(DEPS_DIR) FORCE
	@echo "--------------"
	@echo Building : $@
	$(LATEXMK) -pdf -quiet -silent -MF $(DEPS_DIR)/$@P $<

.PHONY: clean
clean:
	-$(LATEXMK) -c -deps- -quiet
	-rm -rf $(LATEX_MAKE_CLEAN_OPTIONS)

$(DEPS_DIR):
	mkdir $@

# Resolve dependency to invoke makeglossaries using this Makefiles instead of latexmk (that would require some piece of Perl code to execute properly)
%.gls: %.glo
	makeglossaries $*