CC := g++ # this is the main compiler
# CC := clang --analyze # and comment out the linker last line

CFLAGS := -g -Wall -Wextra -Wpedantic -std=c++14

LEXINC := -Ic_lexer/include
LEXSRCDIR := c_lexer/src
LEXBUILDDIR := c_lexer/build
LEXTARGETDIR := bin
LEXTARGET := c_lexer

LEXSRC := $(shell find $(LEXSRCDIR) -type f -name "*.cpp")
LEXOBJ := $(patsubst $(LEXSRCDIR)/%,$(LEXBUILDDIR)/%,$(LEXSRC:.cpp=.o))

$(LEXTARGETDIR)/$(LEXTARGET) : $(LEXOBJ)
	@echo " Linking..."
	@mkdir -p $(LEXTARGETDIR)
	@echo " $(CC) $^ -o $(LEXTARGETDIR)/$(LEXTARGET)"; $(CC) $^ -o $(LEXTARGETDIR)/$(LEXTARGET)

$(LEXSRCDIR)/$(LEXTARGET).yy.cpp : $(LEXSRCDIR)/$(LEXTARGET).flex
		@echo " flex -o $@  $<"; flex -o $@  $<

$(LEXBUILDDIR)/%.o : $(LEXSRCDIR)/%.cpp
	@mkdir -p $(LEXBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<

clean:
	@echo " Cleaning..."
	@echo " $(RM) -r $(LEXBUILDDIR) $(LEXTARGETDIR) $(LEXSRCDIR)/$(LEXTARGET).yy.cpp"; $(RM) -r $(LEXBUILDDIR) $(LEXTARGETDIR) $(LEXSRCDIR)/$(LEXTARGET).yy.cpp

.PHONY: clean
