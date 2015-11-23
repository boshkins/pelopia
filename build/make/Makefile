default: std

std: libpelopia

all: std test accept

TOP = $(abspath ../..)
MODULE = src

include $(TOP)/build/make/Makefile.env

SRC = \
    Pelopia \
    BoundingBox \
    Dataset \
    GeocodeJSON \
    GeocodeJSON_Reader \
    GeocodeJSON_Reader_Rapid_DOM \
    Response \
    Data \
    DataNaive \

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

clean:
	rm -f $(LIBDIR)/libpelopia.a $(OBJ)
	@$(MAKE) --no-print-directory -C test clean

libpelopia: makedirs $(LIBDIR)/libpelopia.a

$(LIBDIR)/libpelopia.a: $(OBJ)
	ar -rc $@ $^ 

test: std
	$(MAKE) -C test
    
accept: std    
	$(MAKE) -C acceptance

.PHONY: std libpelopia test clean accept
