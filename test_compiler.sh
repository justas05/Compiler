#!/bin/bash

# script to test the lexer

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean
echo " Force building lexer"
make -B bin/c_compiler

if [[ "$?" -ne 0 ]]; then
    	echo "Build failed.";
fi

echo ""
echo ""
echo "========================================="
echo " Testing compiler"

PASSED=0
CHECKED=0

for i in c_compiler/test/in/*.c; do
	echo "==========================="
    	echo ""
    	echo "Input file : ${i}"
	BASENAME=$(basename $i .c);
    	cat $i | ./bin/c_compiler > c_compiler/test/out/$BASENAME.stdout.s  2> c_compiler/test/out/$BASENAME.stderr.txt

    	diff <(cat c_compiler/test/ref/$BASENAME.stdout.s) <(cat c_compiler/test/out/$BASENAME.stdout.s) > c_compiler/test/out/$BASENAME.diff.txt
	
    	if [[ "$?" -ne "0" ]]; then
        	echo -e "\nERROR"
    	else
       		PASSED=$(( ${PASSED}+1 ));
    	fi
	
    	CHECKED=$(( ${CHECKED}+1 ));
done

echo "########################################"
echo "Passed ${PASSED} out of ${CHECKED}".
echo ""
