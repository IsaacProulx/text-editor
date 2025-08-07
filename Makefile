SRCDIR := src
BUILDDIR := build

main: $(SRCDIR)/main.c | $(BUILDDIR)
	gcc $(SRCDIR)/main.c -o $(BUILDDIR)/text-editor

$(BUILDDIR):
	mkdir $(BUILDDIR)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
