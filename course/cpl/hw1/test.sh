#! /usr/local/bin/bash

if [ ! -x $1 ]; then
	exit
fi

echo 'I, X, C, M, 1, 10, 100' | $1 > tmp

if [ "`cat tmp`" == "1, 10, 100, 1000, I, X, C" ]; then
	echo "The program works normally.";
else
	echo "The program works abnormally.";
fi

rm -f tmp
