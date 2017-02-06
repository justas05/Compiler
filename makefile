CC := g++ # this is the main compiler
# CC := clange --analyze # and comment out the linker last line

CFLAGS := -g -Wall -Wextra -Wpedantic -std=c++14
INC := -Iinclude

bin/c_lexer: build/main.o build/c_lexer.o build/c_lexer.yy.o
	@echo " Linking..."
	@mkdir -p bin
	@echo " $(CC) $^ -o bin/c_lexer"; $(CC) $^ -o bin/c_lexer

build/c_lexer.yy.o: src/c_lexer.yy.cpp
	@mkdir -p build
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/main.o: src/main.cpp
	@mkdir -p build
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/c_lexer.o: src/c_lexer.cpp
	@mkdir -p build
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

src/c_lexer.yy.cpp: src/c_lexer.flex
	@echo " flex -o $@  $<"; flex -o $@  $<

clean:
	@echo " Cleaning..."
	@echo " $(RM) -r build bin src/c_lexer.yy.cpp"; $(RM) -r build bin src/c_lexer.yy.cpp

.PHONY: clean
