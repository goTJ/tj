#! /usr/bin/perl

open(FOUT, "> fail_list.txt");
open(FOUT2, "> success_list.txt");
$count = 0;
while(<>){
	chomp;
	$title = $_;
	m#=== (.+) - (\d+) ===#;
	$scene = $1;
	$num = $2;
#	print "$scene, $num\n";
	if($num < 100){
		print FOUT "$scene: $num\n";
		for($i=0; $i<$num; $i++){
			$line = <>;
		}
	}else{
		print FOUT2 "$scene\n";
		$count++;
		if($num >= 300){
			print "=== $scene - 300 ===\n";
		}else{
			print "=== $scene - $num ===\n";
		}
		for($i=0; $i<$num; $i++){
			$line = <>;
			print $line if($i < 300);
		}
	}
}
close(FOUT);
close(FOUT2);
