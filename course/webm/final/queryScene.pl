#! /usr/bin/perl

use strict;
use warnings;
use queryScene;

if($#ARGV < 0){
	print "Usage: ".__FILE__." scene_list result_file\n";
	exit;
}

my $scene = $ARGV[0];
my @result = queryScene($scene);
my $num = ($#result+1)/3;

open(RESULT_OUT, "> j");
print "=== $scene - $num ===\n";
print RESULT_OUT "=== $scene - $num ===\n";
while($#result >= 0){
	my $title = shift @result;
	my $date = shift @result;
	my $url = shift @result;
	print RESULT_OUT "$title [[[$date]]]: $url\n";
}
close(RESULT_OUT);
