#! /usr/bin/perl

use strict;
use warnings;
use queryScene;

if($#ARGV < 0){
	print "Usage: ".__FILE__." scene_list result_file\n";
	exit;
}

open(SCENE_IN, $ARGV[0]);
open(RESULT_OUT, "> $ARGV[1]");
foreach(<SCENE_IN>){
	chomp;

	my $scene = $_;
	print "query $scene...";
	my @result = queryScene($scene);
	my $num = ($#result+1)/3;
	print "$num\n";

	print RESULT_OUT "=== $scene - $num ===\n";
	while($#result >= 0){
		my $title = shift @result;
		my $date = shift @result;
		my $url = shift @result;
		print RESULT_OUT "$title [[[$date]]]: $url\n";
	}
}
close(SCENE_IN);
close(RESULT_OUT);
