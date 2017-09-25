#!/bin/bash
echo "Test starting..." > test.log
rm results.log
while read asm_file golden in_cmd
do
	echo Assembling $asm_file >> test.log
	./assemble $asm_file machine_code.obj >> test.log
	../lc3bsim2 machine_code.obj < $in_cmd
	diff -q dumpsim $golden >> results.log;
done
if [ -s results.log ]; then
echo FAIL: results.log is not empty
fi

