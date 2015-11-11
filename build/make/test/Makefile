default: pelopia

TOP = $(abspath ../../..)
MODULE = src/test

include $(TOP)/build/make/Makefile.env

clean:
	rm -rf $(OBJDIR) $(BINDIR)/test-pelopia

SRC = main

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

pelopia: makedirs $(LIBDIR)/libpelopia.a $(BINDIR)/test-pelopia 
	$(BINDIR)/test-pelopia 
	
$(LIBDIR)/libpelopia.a:
	$(MAKE) --no-print-directory -C ..

$(BINDIR)/test-pelopia: $(OBJ) $(LIBDIR)/libpelopia.a
	g++ -o $@ $^ /usr/local/lib/libpostal.a

.PHONY: std pelopia clean