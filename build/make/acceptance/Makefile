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
	cd $(SRCDIR); $(EXE)
    
valgrind: $(EXE)
	cd $(SRCDIR); valgrind --leak-check=full --show-leak-kinds=all $(EXE)     

.PHONY: clean accept
