default: std

std: libpelopia

TOP = $(abspath ../../../..)
MODULE = language/python

include $(TOP)/build/make/Makefile.env

SRC = pelopia_python

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

clean:
	rm -rf $(LIBDIR)/libpelopia.so $(OBJDIR)

libpelopia: makedirs $(LIBDIR)/libpelopia.so

$(LIBDIR)/libpelopia.so: $(OBJ) $(LIBDIR)/libpelopia.a
	$(CXX) -shared -o $@ $^ 

.PHONY: std libpelopia clean
