#! /usr/bin/perl

use strict;
use warnings;
use Encode;
use LWP::UserAgent;

sub fetchPage{
	my $ua = LWP::UserAgent->new;
	my $response = $ua->get($_[0]);

	local $_ = encode('big5', decode('utf8', $response->content));

	my $content='';
	if(m#<div class="(mainEntryBody|innertext|main)">(.+?)<br clear#s){
		$content = $2;
	}elsif(m#<div class="article-content">(.+?)</div>#s){
		$content = $1;
	}
	my $result='';
	$result .= $1 while($content =~ m/(([\xA1-\xF9][\x40-\x7E\xA1-\xFE])+)/sg);
	return $result;
}
1;
