#! /usr/bin/perl

if($#ARGV < 0){
	print "Usage: ".__FILE__." list\n";
	exit;
}

$LISTNAME = $ARGV[0];
open(LIST_FIN, $LISTNAME) or die "Can't open list file";
open(LIST_1_FOUT, "> $LISTNAME_1");
open(LIST_2_FOUT, "> $LISTNAME_2");

foreach(<LIST_FIN>){
	chomp;
	$filename = $_;
	if(-x "$filename.info"){
	}else{
		print LIST_2_FOUT "$filename\n";
	}
}

close(LIST_FIN);
