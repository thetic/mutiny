# Minimal Sphinx Makefile

SPHINXBUILD   ?= sphinx-build
SOURCEDIR      = .
BUILDDIR       = _build
SPHINXOPTS    ?= -W

help:
	@$(SPHINXBUILD) -M help $(SOURCEDIR) $(BUILDDIR) $(SPHINXOPTS) $(O)

.PHONY: help clean Makefile

clean:
	@$(SPHINXBUILD) -M clean $(SOURCEDIR) $(BUILDDIR) $(SPHINXOPTS) $(O)
	rm -rf api doxyoutput

%: Makefile
	@$(SPHINXBUILD) -M $@ $(SOURCEDIR) $(BUILDDIR) $(SPHINXOPTS) $(O)
