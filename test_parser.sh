#!/bin/bash

# script to test the lexer

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean
echo " Force building lexer"
make -B bin/c_parser

if [[ "$?" -ne 0 ]]; then
    echo "Build failed.";
fi

echo "========================================="
echo " Testing lexer"

cat c_parser/test/test_parser.c | ./bin/c_parser | tee c_lexer/test/output.xml
