#! /usr/bin/sh

for i in `seq 2 8`
do
	for m in 'm' 'a' 'u'
	do
		for j in '100070' '100087'
		do
			./transmitterDPCM -$i -$m $j.pcm
			./receiverDPCM -$i -$m ${j}_D_${m}_$i.dpcm
		done
	done
done
