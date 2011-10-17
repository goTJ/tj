#! /usr/bin/sh

for i in `seq 2 8`
do
	for m in 'm' 'a' 'u'
	do
		for j in '100070' '100087'
		do
			./transmitterLP -$i -$m $j.pcm
			./receiverLP -$i -$m ${j}_LP_${m}_$i.lppcm
		done
	done
done
