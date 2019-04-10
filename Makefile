DOC_ROOT=./docu
CODE_ROOT=./code
CURRENT_DIR = $(PWD)

WWW_TARGET_DIR=/var/www
CGI_TARGET_DIR=/usr/lib/cgi-bin

include makeconfig.mk

DOCU_OUT_DIR=docu_output

LIST_OF_DOCS= \
	$(DOC_ROOT)/01_project_description \
	$(DOC_ROOT)/02_risk_analysis

LIST_OF_QPROJECTS= \
	$(CODE_ROOT)/aquaBackend \
	$(CODE_ROOT)/aquaRequest

LIST_OF_HTDOC_DIRS=\
	code/htdocs/*

LIST_OF_CGI_FILES=\
	$(CODE_ROOT)/aquaRequest/aquaRequest.cgi

.DEFAULT_GOAL := qprojects

.PHONY: all docu clean

#
# Build documentation and copy resulting PDFs to documentation output directory
#
docu:
	@echo '#########################################'
	@echo Building documentation
	@for i in $(LIST_OF_DOCS); do echo " " $$i; done
	@for i in $(LIST_OF_DOCS); do $(MAKE) -C $$i; done
	@echo '-----------------------------------------'
	@mkdir -p $(DOCU_OUT_DIR)
	@echo Composition of PDFs in $(DOCU_OUT_DIR):
	@for i in $(LIST_OF_DOCS); do cp $$i/*.pdf $(DOCU_OUT_DIR) || exit 1; done
	@echo Done.
	@echo List of PDFs in $(DOCU_OUT_DIR):
	@ls -1 $(DOCU_OUT_DIR)
	@echo '#########################################'

qprojects:
	@echo '#########################################'
	@echo Building Qt projects
	@for i in $(LIST_OF_QPROJECTS); do echo "  Building " $$i && cd $(CURRENT_DIR) && cd $$i && qmake && make && echo '-----------------------------------------' done
	@cd $(CURRENT_DIR)
	@echo '#########################################'

all: qprojects docu

clean:
	@for i in $(LIST_OF_DOCS); do $(MAKE) clean -C $$i; done
	@for i in $(LIST_OF_QPROJECTS); do $(MAKE) clean -C $$i; done

install:
	@echo '#########################################'
	@echo 'OVERWRITING CGI'
	@for i in $(LIST_OF_CGI_FILES); do sudo cp $$i $(CGI_TARGET_DIR)/; done
	@sudo chown www-data:www-data $(CGI_TARGET_DIR)/*.cgi
	@echo '-----------------------------------------'
	@echo 'OVERWRITING WEB APPLICATION'
	@for i in $(LIST_OF_HTDOC_DIRS); do echo "Copying " $$i && sudo cp -R $(HTDOC_DIR)/$$i $(WWW_TARGET_DIR)/; done
	@sudo chown -R www-data:www-data $(WWW_TARGET_DIR)/
	@echo '#########################################'
