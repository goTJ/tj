#! /usr/bin/perl

use strict;
use warnings;
use fetchPage;

while(<>){
	chomp;
	m#=== (.+) - (\d+) ===#;
	my $scene = $1;
	my $num = $2;
	my %author;

	for(my $i=0; $i<$num; $i++){
		my $line = <>;
		if($line =~ /(.+) \[\[\[.+(http.+?)$/s){
			my $title = $1;
			my $url = $2;
			my $r = fetchPage($url);
			if($r){
				my $result='';
				$result .= $1 while($title =~ m/(([\xA1-\xF9][\x40-\x7E\xA1-\xFE])+)/sg);
				$result .= "¡Ð$r";
				$url =~ m#net/(.+?)/#;
				if(defined($author{$1})){
					$author{$1} .= "¡þ$result";
				}else{
					$author{$1} .= $result;
				}
			}
		}
	}
	print "=== $scene ===\n";
	foreach my $k(keys %author){
		if(length($author{$k}) > 100){
			print $author{$k}."\n";
		}
	}
}
