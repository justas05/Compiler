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

echo ""
echo ""
echo "========================================="
echo " Testing parser"

PASSED=0
CHECKED=0

for i in c_parser/test/in/*.c; do
	echo "==========================="
    	echo ""
    	echo "Input file : ${i}"
	BASENAME=$(basename $i .c);
    	cat $i | ./bin/c_parser > c_parser/test/out/$BASENAME.stdout.xml  2> c_parser/test/out/$BASENAME.stderr.txt

    	diff <(cat c_parser/test/ref/$BASENAME.stdout.xml) <(cat c_parser/test/out/$BASENAME.stdout.xml) > c_parser/test/out/$BASENAME.diff.txt
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
