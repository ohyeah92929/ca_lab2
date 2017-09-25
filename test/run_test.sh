#!/bin/bash
echo "Test starting..." > test.log
rm results.log
while read input output expected err_code
do
	echo Assembling $input >> test.log
	../assemble $input $output >> test.log
	if [ $? != $err_code ]; then
	echo FAIL: Got error code $? different from expected $err_code for $input
        fi
	if [ $err_code = 0 ]; then
	diff -q $output $expected >> results.log;
	fi
done
if [ -s results.log ]; then
echo FAIL: results.log is not empty
fi

