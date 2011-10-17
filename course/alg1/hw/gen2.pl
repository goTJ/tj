#! /usr/bin/perl -w

$prog = "gen2.pl";

if($#ARGV < 0){
	print "Usage: $prog num\n";
	exit;
}

$num = $ARGV[0];
open(OUT, "> test_data.txt");
print OUT "$num";
for($i=0; $i<$num; $i++){
	print OUT "\n" if(!($i%10));
	printf OUT "%.3f ", rand($num);
}
print OUT "\n";
close(OUT);
