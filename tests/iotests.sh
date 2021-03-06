#!/bin/bash

echo "================ Running IO Tests ================"
echo ""

INPUTS=`ls tests/inputs`
EXECUTABLE=''
TEST_NAME=''
DIFF=''

NUM_PASSED=0
NUM_FAILED=0

# get_tweets test
echo "get_tweets: get_tweets"
x=`( ./get_tweets > tests/tmpout.txt ) & sleep 5; kill $!`

if [ -s tests/tmpout.txt ]; then
	NUM_PASSED=`expr $NUM_PASSED + 1`
	echo "............................................PASSED"
else
	NUM_FAILED=`expr $NUM_FAILED + 1`
	echo "............................................FAILED"
fi

for FILE in $INPUTS
do
	EXECUTABLE=`echo $FILE | cut -f1 -d.`
	TEST_NAME=`echo $FILE | cut -f2 -d.`

	echo "$EXECUTABLE: $TEST_NAME"

	cat tests/inputs/$FILE | ./$EXECUTABLE > tests/tmpout.txt

	DIFF=`diff <(sort tests/tmpout.txt) <(sort tests/outputs/$FILE) 2> /dev/null`

	cat tests/outputs/$FILE > /dev/null 2> /dev/null

	if [ $? -eq 0 ]; then
		if [ -z "$DIFF" ]; then
			NUM_PASSED=`expr $NUM_PASSED + 1`
			echo "............................................PASSED"
		else
			NUM_FAILED=`expr $NUM_FAILED + 1`
			echo "............................................FAILED"
			echo "expected:"
			cat <(sort tests/outputs/$FILE)
			echo "actual:"
			cat <(sort tests/tmpout.txt)
		fi
	else
		NUM_FAILED=`expr $NUM_FAILED + 1`
		echo ".............................................ERROR"
	fi

done

PERCENTAGE=`expr $(expr $NUM_PASSED \* 100) / $(expr $NUM_PASSED + $NUM_FAILED)` 

echo ""
echo "$NUM_PASSED PASSED, $NUM_FAILED FAILED ($PERCENTAGE%)"

rm tests/tmpout.txt
