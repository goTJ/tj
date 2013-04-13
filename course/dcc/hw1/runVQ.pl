#! /usr/bin/perl -w

@FILE = ('100070', '100087');
foreach(`ls VQ-*.txt`){
	chop;
	/VQ-(\d+)-(\d+)-.+txt/;
	$vqFile = $_;
	foreach(@FILE){
		system("./transmitterVQ $vqFile $_.pcm");
		system("./receiverVQ $vqFile ${_}_VQ_$1_$2.vqpcm");
	}
}
