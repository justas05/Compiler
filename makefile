CC := g++ # this is the main compiler
# CC := clang --analyze # and comment out the linker last line

CFLAGS := -g -Wall -Wextra -Wpedantic -std=c++14

LEXINC := -Ic_lexer/include
LEXBUILDDIR := c_lexer/build
LEXSRCDIR := c_lexer/src

PARINC := -Ic_parser/include
PARBUILDDIR := c_parser/build
PARSRCDIR := c_parser/src

COMPINC := -Ic_compiler/include
COMPBUILDDIR := c_compiler/build
COMPSRCDIR := c_compiler/src

all : bin/c_lexer bin/c_parser

# Make the c_lexer
bin/c_lexer : $(LEXBUILDDIR)/main.o $(LEXBUILDDIR)/c_lexer.o $(LEXBUILDDIR)/c_lexer.yy.o
	@echo "Linking..."
	@echo " mkdir -p bin"; mkdir -p bin
	@echo " $(CC) $^ -o $@"; $(CC) $^ -o $@

$(LEXBUILDDIR)/%.o : $(LEXSRCDIR)/%.cpp
	@echo "Building sources..."
	@echo " mkdir -p $(LEXBUILDDIR)"; mkdir -p $(LEXBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<

$(LEXBUILDDIR)/%.yy.o : $(LEXSRCDIR)/%.yy.cpp
	@echo "Building lex..."
	@echo " mkdir -p $(LEXBUILDDIR)"; mkdir -p $(LEXBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(LEXINC) -c -o $@ $<

$(LEXSRCDIR)/c_lexer.yy.cpp : $(LEXSRCDIR)/c_lexer.flex
	@echo "Creating flex"
	@echo " flex -o $@  $<"; flex -o $@  $<

# Make the c_parser
bin/c_parser : $(PARBUILDDIR)/parser_main.o $(PARBUILDDIR)/c_parser.tab.o $(PARBUILDDIR)/c_lexer.yy.o
	@echo "Linking..."
	@echo " mkdir -p bin"; mkdir -p bin
	@echo " $(CC) $^ -o $@"; $(CC) $^ -o $@

$(PARBUILDDIR)/%.o : $(PARSRCDIR)/%.cpp
	@echo "Building sources..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARBUILDDIR)/%.yy.o : $(PARSRCDIR)/%.yy.cpp
	@echo "Building lex..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARBUILDDIR)/c_parser.tab.o : $(PARSRCDIR)/c_parser.tab.cpp
	@echo "Building yacc..."
	@echo " mkdir -p $(PARBUILDDIR)"; mkdir -p $(PARBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(PARINC) -c -o $@ $<

$(PARSRCDIR)/c_lexer.yy.cpp : $(PARSRCDIR)/c_lexer.flex
	@echo "Creating lex..."
	@echo " flex -o $@  $<"; flex -o $@  $<

$(PARSRCDIR)/c_parser.tab.cpp $(PARSRCDIR)/c_parser.tab.hpp : $(PARSRCDIR)/c_parser.y
	@echo "Creating yacc..."
	@echo " bison -v -d $< -o $(PARSRCDIR)/c_parser.tab.cpp"; bison -v -d $< -o $(PARSRCDIR)/c_parser.tab.cpp

# Make the c_parser
bin/c_compiler:	$(COMPBUILDDIR)/compiler_main.o $(COMPBUILDDIR)/statement.o \
		$(COMPBUILDDIR)/function.o $(COMPBUILDDIR)/expression.o \
		$(COMPBUILDDIR)/initializer.o $(COMPBUILDDIR)/translation_unit.o \
		$(COMPBUILDDIR)/declaration.o $(COMPBUILDDIR)/type.o \
		$(COMPBUILDDIR)/c_parser.tab.o $(COMPBUILDDIR)/c_lexer.yy.o
	@echo "Linking..."
	@echo " mkdir -p bin"; mkdir -p bin
	@echo " $(CC) $^ -o $@"; $(CC) $^ -o $@

$(COMPBUILDDIR)/%.o : $(COMPSRCDIR)/%.cpp
	@echo "Building sources..."
	@echo " mkdir -p $(COMPBUILDDIR)"; mkdir -p $(COMPBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<

$(COMPBUILDDIR)/%.yy.o : $(COMPSRCDIR)/%.yy.cpp
	@echo "Building lex..."
	@echo " mkdir -p $(COMPBUILDDIR)"; mkdir -p $(COMPBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<

$(COMPBUILDDIR)/c_parser.tab.o : $(COMPSRCDIR)/c_parser.tab.cpp
	@echo "Building yacc..."
	@echo " mkdir -p $(COMPBUILDDIR)"; mkdir -p $(COMPBUILDDIR)
	@echo " $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<"; $(CC) $(CFLAGS) $(COMPINC) -c -o $@ $<

$(COMPSRCDIR)/c_lexer.yy.cpp : $(COMPSRCDIR)/c_lexer.flex
	@echo "Creating lex..."
	@echo " flex -o $@  $<"; flex -o $@  $<

$(COMPSRCDIR)/c_parser.tab.cpp $(COMPSRCDIR)/c_parser.tab.hpp : $(COMPSRCDIR)/c_parser.y
	@echo "Creating yacc..."
	@echo " bison -v -d $< -o $(COMPSRCDIR)/c_parser.tab.cpp"; bison -v -d $< -o $(COMPSRCDIR)/c_parser.tab.cpp

clean :
	@echo "Cleaning..."
	@echo " rm -r $(LEXBUILDDIR) bin $(LEXSRCDIR)/c_lexer.yy.cpp"; rm -r bin $(LEXBUILDDIR) $(LEXSRCDIR)/c_lexer.yy.cpp $(PARBUILDDIR) $(PARSRCDIR)/c_lexer.yy.cpp $(PARSRCDIR)/c_parser.tab.cpp $(PARSRCDIR)/c_parser.tab.hpp $(PARSRCDIR)/c_parser.output

.PHONY : clean

.PRECIOUS : $(LEXSRCDIR)/%.yy.cpp $(PARSRCDIR)/%.yy.cpp $(PARSRCDIR)/%.tab.cpp $(PARSRCDIR)/%.tab.hpp
