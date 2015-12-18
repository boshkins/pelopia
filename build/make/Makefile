default: std

std: libpelopia

all: std test accept lang

TOP = $(abspath ../..)
MODULE = src

include $(TOP)/build/make/Makefile.env

SRC = \
    Pelopia \
    BoundingBox \
    Distance \
    Dataset \
    GeocodeJSON \
    GeocodeJSON_Reader \
    GeocodeJSON_Reader_Rapid_DOM \
    Response \
    ResponseInternal \
    Data \
    DataNaive \

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

clean:
	rm -rf $(LIBDIR)/libpelopia.a $(OBJDIR)
	@$(MAKE) --no-print-directory -C test clean
	@$(MAKE) --no-print-directory -C language/python clean

libpelopia: makedirs $(LIBDIR)/libpelopia.a

$(LIBDIR)/libpelopia.a: $(OBJ)
	ar -rc $@ $^ 

test: std
	$(MAKE) -C test
    
accept: std    
	$(MAKE) -C acceptance
    
lang: std
	@$(MAKE) --no-print-directory -C language/python

.PHONY: std libpelopia test clean accept lang
 