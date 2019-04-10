ifndef ROOT_DIRECTORY
	ROOT_DIRECTORY := .
endif

ifndef LATEX_OUT_DIR
	LATEX_OUT_DIR := $(ROOT_DIRECTORY)/docu_output
endif

DEPS_DIR = .deps
LATEXMK = @latexmk -recorder -use-make -deps \
	-interaction=nonstopmode -file-line-error -halt-on-error -shell-escape \
	-e '@cus_dep_list = ();' \
	-e 'show_cus_dep();'
DEPS = $(wildcard $(DEPS_DIR)/*)


# Latex temp files to clean
LATEX_MAKE_CLEAN_OPTIONS = .deps *.run.xml *.glsdefs *.ist *.glg *.glo *.gls *.aux *.upa *blx.bib *.bbl *.fls *.log *.out *.upb *.fdb_* *.toc *.lof *.lot
