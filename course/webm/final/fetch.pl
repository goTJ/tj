#! /usr/bin/perl

use strict;
use Encode;
use LWP::UserAgent;
my $ua = LWP::UserAgent->new;
my @query;
my %used;

open(FOUT, "> result.txt");
open(STDOUT_OUT, "> out.txt");
foreach(<>){
	chomp;

	undef(@query);
	undef(%used);
	my $scene = $_;
	push @query, URI->new(encode('utf8', decode('big5', "$scene+遊")));
	push @query, URI->new(encode('utf8', decode('big5', "$scene+旅")));
	
	my $count = 0;
	foreach(<@query>){
		my $q = $_;
		print "$scene: $q\n";
		print FOUT "=== $scene ===\n";
		my $nPage = 25;
		for(my $i=1; $i<=$nPage; $i++){
			print "$i ";
			print "\n" unless($i%10);
			# sleep 1 unless($i%10);
			my $queryUrl = "http://www.pixnet.net/sb/$q/$i";
			my $response = $ua->get($queryUrl);
			local $_ = encode('big5', decode('utf8', $response->content));

			while(m#class="num".+?href="(.+?)".+?_blank">(.+?)</a>.+?date">(.+?)</span>.+?grid2#sg){
				my($url, $title, $date) = ($1, $2, $3);
				next if($title =~ /此文設定不開放/);
				if(!defined($used{$url})){
					next unless($title=~ m#($scene.*遊)|(遊.*$scene)|($scene.*旅)#s);
					print FOUT "$title [[[$date]]]: $url\n";
					$used{$url} = 1;
					$count++;
				}
			}
		}
		print "\n";
	}
	print "total: $count\n";
	print STDOUT_OUT "$scene: $count\n";
}
close(FOUT);
