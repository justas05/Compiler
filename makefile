CC := g++ # this is the main compiler
# CC := clang --analyze # and comment out the linker last line

CFLAGS := -g -Wall -Wextra -Wpedantic -std=c++14

LEXINC := -Ic_lexer/include
LEXBUILDDIR := c_lexer/build
LEXSRCDIR := c_lexer/src

# Make the c_lexer
bin/c_lexer : $(LEXBUILDDIR)/main.o $(LEXBUILDDIR)/c_lexer.o $(LEXBUILDDIR)/c_lexer.yy.o
	@echo "Linking..."
	@echo " mkdir -p bin"; mkdir -p bin
	@echo " $(CC) $^ -o bin/c_lexer"; $(CC) $^ -o bin/c_lexer

$(LEXBUILDDIR)/%.o : $(LEXSRCDIR)/%.cpp
	@echo "Building sources..."
	@echo " mkdir -p $(LEXBUILDDIR)"; mkdir -p $(LEXBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<

$(LEXBUILDDIR)/%.yy.o : $(LEXSRCDIR)/%.yy.cpp
	@echo "Building lex..."
	@echo " mkdir -p $(LEXBUILDDIR)"; mkdir -p $(LEXBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<

$(LEXSRCDIR)/%.yy.cpp : $(LEXSRCDIR)/%.flex
	@echo "Creating flex"
	@echo " flex -o $@  $<"; flex -o $@  $<

clean:
	@echo "Cleaning..."
	@echo " rm -r $(LEXBUILDDIR) bin $(LEXSRCDIR)/c_lexer.yy.cpp"; rm -r $(LEXBUILDDIR) bin $(LEXSRCDIR)/c_lexer.yy.cpp

.PHONY: clean

.PRECIOUS: $(LEXSRCDIR)/%.yy.cpp
