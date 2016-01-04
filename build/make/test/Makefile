default: pelopia

TOP = $(abspath ../../..)
MODULE = src/test

include $(TOP)/build/make/Makefile.env

clean:
	rm -rf $(OBJDIR) $(BINDIR)/test-pelopia

SRC = main \
    pelopia \
    boundingbox \
    geojson \
    response \
    distance \
    datanaive \
    textindexnaive \
    normalizernaive \
    dataset \

# libpostal

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

pelopia: makedirs $(LIBDIR)/libpelopia.a $(BINDIR)/test-pelopia 
	cd $(SRCDIR); $(BINDIR)/test-pelopia 
	
$(LIBDIR)/libpelopia.a:
	$(MAKE) --no-print-directory -C ..

$(BINDIR)/test-pelopia: $(OBJ) $(LIBDIR)/libpelopia.a
	$(CXX) -o $@ $^ #/usr/local/lib/libpostal.a
    
valgrind: $(BINDIR)/test-pelopia
	cd $(SRCDIR); valgrind $(BINDIR)/test-pelopia 

.PHONY: std pelopia clean