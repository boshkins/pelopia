default: accept

TOP = $(abspath ../../..)
MODULE = acceptance
EXE = $(BINDIR)/acceptance

include $(TOP)/build/make/Makefile.env

SRC = main

OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o,$(SRC)))

clean:
	@rm -rf $(OBJDIR) $(EXE)

$(BINDIR)/acceptance: $(OBJ) $(LIBDIR)/libpelopia.a
	$(CXX) -o $@ $^ 

accept: makedirs $(EXE)
	$(EXE)

.PHONY: clean accept
