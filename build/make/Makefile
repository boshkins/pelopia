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
    TextIndexNaive \
    NormalizerNaive \
    GeoScorer \
    TextScorer \
    CoordinateIndex \
    LocationFilter \
    GeoTextScorer \
    FilteringGeoTextScorer \

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

clean:
	rm -rf $(LIBDIR)/libpelopia.a

libpelopia: makedirs $(LIBDIR)/libpelopia.a

$(LIBDIR)/libpelopia.a: $(OBJ)
	ar -rc $@ $^

test: std
	$(MAKE) -C test

accept: std
	-$(MAKE) -C acceptance

lang: std
	@$(MAKE) -C language

.PHONY: std libpelopia test clean accept lang
