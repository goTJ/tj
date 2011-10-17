#! /usr/bin/perl -w

use strict;
use warnings;
use Encode;
use LWP::UserAgent;

sub queryScene{
	my $ua = LWP::UserAgent->new;
	my $scene = shift;
	my @query;
	my %used;
	my %author;
	my @result;

	push @query, URI->new(encode('utf8', decode('big5', "$scene+遊")));
	push @query, URI->new(encode('utf8', decode('big5', "$scene+旅")));
	
	my $count = 0;
	my $np = 0;
	my $nPage = 50;
	for(my $i=1; $i<=$nPage; $i++){
		foreach(<@query>){
			my $q = $_;
			# sleep 1 unless($i%10);
			my $queryUrl = "http://www.pixnet.net/sb/$q/$i";
			my $response = $ua->get($queryUrl);
			local $_ = encode('big5', decode('utf8', $response->content));

			while(m#class="num".+?href="(.+?)".+?_blank">(.+?)</a>.+?date">(.+?)</span>.+?grid2#sg){
				my($url, $title, $date) = ($1, $2, $3);
				next if($title =~ /此文設定不開放/);
				if(!defined($used{$url})){
					next unless($title=~ m#($scene.*遊)|(遊.*$scene)|($scene.*旅)#s);
					push @result, $title;
					push @result, $date;
					push @result, $url;
					$used{$url} = 1;
					$url =~ m#net/(.+?)/#;
					if(!defined($author{$1})){
						$author{$1} = 1;
						$count++;
					}
				}
			}
			$np++;
			last if($np > 30 && $count < 70);
			last if($np > 60 && $count < 150);
			last if($count > 300);
		}
		last if($np > 30 && $count < 75);
		last if($np > 60 && $count < 150);
		last if($count > 300);
	}
	print "total: $np\n";
	return @result;
}
1;
