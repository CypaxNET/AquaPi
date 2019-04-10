DOC_ROOT=./docu
include makeconfig.mk

DOCU_OUT_DIR=docu_output

LIST_OF_DOCS= \
	$(DOC_ROOT)/01_project_description \
	$(DOC_ROOT)/02_risk_analysis

.DEFAULT_GOAL := all

.PHONY: all docu clean

#
# Build documentation and copy resulting PDFs to documentation output directory
#
docu:
	@echo "--------------"
	@echo Building documentation:
	@for i in $(LIST_OF_DOCS); do echo " " $$i; done
	@for i in $(LIST_OF_DOCS); do $(MAKE) -C $$i; done
	@echo "--------------"
	@mkdir -p $(DOCU_OUT_DIR)
	@echo Composition of PDFs in $(DOCU_OUT_DIR):
	@for i in $(LIST_OF_DOCS); do cp $$i/*.pdf $(DOCU_OUT_DIR) || exit 1; done
	@echo Done.
	@echo List of PDFs in $(DOCU_OUT_DIR):
	@ls -1 $(DOCU_OUT_DIR)
	@echo "--------------"

all: docu 

clean:
	for i in $(LIST_OF_DOCS); do $(MAKE) clean -C $$i; done

