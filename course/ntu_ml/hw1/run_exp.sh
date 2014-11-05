#! /bin/sh

rm -f result
for i in `seq 1 2000`; do
  ./pla < ntumlone%2Fhw1%2Fhw1_18_train.dat > w
  cat w ntumlone%2Fhw1%2Fhw1_18_test.dat | ./validator >> result
done

cut -d ' ' -f 1 result | perl -e '$sum = 0.0; foreach(<>){ $sum += $_; } print $sum / 2000, "\n";'
cut -d ' ' -f 2 result | perl -e '$sum = 0.0; foreach(<>){ $sum += $_; } print $sum / 2000, "\n";'
