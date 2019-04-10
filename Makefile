DOC_ROOT=./docu
CODE_ROOT=./code
HTDOC_DIR=./code/htdocs
current_dir = $(PWD)

include makeconfig.mk

DOCU_OUT_DIR=docu_output

LIST_OF_DOCS= \
	$(DOC_ROOT)/01_project_description \
	$(DOC_ROOT)/02_risk_analysis

LIST_OF_QPROJECTS= \
	$(CODE_ROOT)/aquaBackend \
	$(CODE_ROOT)/aquaRequest

CGI_FILES=\
	$(CODE_ROOT)/aquaRequest/aquaRequest.cgi

HTTP_DIRECTORIES=\
	jquery/scheduler/* \
	jquery/* \
	js/* \
	css/* \
	images/* \
	ico/* \

HTTP_FILES=\
	schedule*.html \
	imageshow.txt \
	index.php \
	languages.xml

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
	@for i in $(LIST_OF_QPROJECTS); do echo "  Building " $$i && cd $(current_dir) && cd $$i && qmake && make && echo '-----------------------------------------' done
	@cd $(current_dir)
	@echo '#########################################'

all: qprojects docu

clean:
	@for i in $(LIST_OF_DOCS); do $(MAKE) clean -C $$i; done
	@for i in $(LIST_OF_QPROJECTS); do $(MAKE) clean -C $$i; done

install:
	@echo '#########################################'
	@echo 'OVERWRITING CGI'
	@for i in $(CGI_FILES); do sudo cp $$i /usr/lib/cgi-bin/; done
	@sudo chown www-data:www-data /usr/lib/cgi-bin/*.cgi
	@echo '-----------------------------------------'
	@echo 'OVERWRITING WEB APPLICATION'
	@for i in $(HTTP_DIRECTORIES); do echo "Copying directories " $$i; done
	for i in $(HTTP_DIRECTORIES); do sudo cp $(HTDOC_DIR)/$$i /var/www/; done
	@for i in $(HTTP_FILES); do echo "Copying files " $$i; done
	for i in $(HTTP_FILES); do sudo cp $(HTDOC_DIR)/$$i /var/www/; done
	@sudo chown -R www-data:www-data /var/www/
	@echo '#########################################'

#cd ~/AquarPi/code/htdocs
#sudo cp -R jquery/scheduler/* /var/www/jquery/scheduler/
#sudo cp -R jquery/* /var/www/jquery/
#sudo cp -R js/* /var/www/js/
#sudo cp -R css/* /var/www/css/
#sudo cp -R images/* /var/www/images/
#sudo cp -R ico/* /var/www/ico/
#sudo cp schedule*.html /var/www/
#sudo cp imageshow.txt /var/www/imageshow.txt
#sudo cp index.php /var/www/index.php
#sudo cp languages.xml /var/www/languages.xml
#	cd ~/AquaPi/code/aquaRequest
#	sudo cp aquaRequest.cgi /usr/lib/cgi-bin/aquaRequest.cgi
#	sudo chown www-data:www-data /usr/lib/cgi-bin/aquaRequest.cgi