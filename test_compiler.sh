#!/bin/bash

# script to test the lexer

echo "========================================"
echo " Cleaning the temporaries and outputs"
make clean
rm -rf ./c_compiler/test/{out,ref}
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

mkdir -p c_compiler/test/out
mkdir -p c_compiler/test/ref

for i in c_compiler/test/in/*.c; do
	echo "==========================="
    echo ""
    echo "Input file : ${i}"
	BASENAME=$(basename $i .c);
    cat $i | ./bin/c_compiler > c_compiler/test/out/$BASENAME.s  2> c_compiler/test/out/$BASENAME.stderr.txt

	mips-linux-gnu-gcc -O0 -S -c c_compiler/test/in/$BASENAME.c -o c_compiler/test/ref/$BASENAME.s
	mips-linux-gnu-gcc -O0 -static c_compiler/test/ref/$BASENAME.s -o c_compiler/test/ref/$BASENAME
	
    mips-linux-gnu-gcc -O0 -static c_compiler/test/out/$BASENAME.s -o c_compiler/test/out/$BASENAME

	qemu-mips c_compiler/test/ref/$BASENAME
	REFOUTPUT=$?
	
	qemu-mips c_compiler/test/out/$BASENAME
	TESTOUTPUT=$?

	if [ "$TESTOUTPUT" = "$REFOUTPUT" ]; then
	    PASSED=$(( PASSED+1 ))
	else
	    echo -e "\nERROR"
	fi

	echo -e "output: $TESTOUTPUT\n"

	CHECKED=$(( CHECKED+1 ))
done

echo "########################################"
echo "Passed ${PASSED} out of ${CHECKED}".
echo ""
