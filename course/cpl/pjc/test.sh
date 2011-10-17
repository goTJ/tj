#! /bin/sh

PROG='./pcompiler';

echo "test 1"
${PROG} tests/program1.p;
./p.out;
echo "test 2"
${PROG} tests/program2.p;
./p.out < tests/input2_1 > tests/output2_1
./p.out < tests/input2_2 > tests/output2_2
echo "test 3"
${PROG} tests/program3.p;
./p.out;
echo "test 4"
${PROG} tests/program4.p;
./p.out < tests/input4_1 > tests/output4_1
echo "test 5"
${PROG} tests/program5.p;
./p.out < tests/input5_1 > tests/output5_1
./p.out < tests/input5_2 > tests/output5_2
