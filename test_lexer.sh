#!/bin/bash

# script to test the lexer

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean
echo " Force building lexer"
make bin/c_lexer -B

if [[ "$?" -ne 0 ]]; then
    echo "Build failed.";
fi

echo "========================================="
echo " Testing lexer"

cpp test/test_lex.c test/pre_processed_test_lex.c
cat test/pre_processed_test_lex.c | ./bin/c_lexer | tee test/output.json
#cat test/test_lex.c | ./bin/c_lexer | tee test/output.json
