#! /usr/bin/perl -w

@accout = (0, 10319, 10387, 10318);
print "Please enter accout(1--10319, 2--10387, 3--10318): ";
$index = <>;
chomp $index;
die "Wrong accout index number!!" if($index != 1 && $index != 2 && $index != 3);

# get solved problem list
print "get solved problem list...\n";
system("wget -q http://acm.uva.es/cgi-bin/OnlineJudge?AuthorInfo:$accout[$index]");
open(LIST, "OnlineJudge?AuthorInfo:$accout[$index]");
$begin = "no";
foreach(<LIST>){
	@word = split;
	foreach(@word){
		$begin = "yes" if($_ eq "solved");
		$begin = "no" if(/br/);
		if($begin eq "yes"){
			s/(\<.+\>)//g;
			s/(\<.+)//g;
			s/(.+\>)//g;
			s/(\D)//g;
			push(@list, $_) if($_);
		}
	}
}
close(LIST);
unlink("OnlineJudge?AuthorInfo:$accout[$index]");

# get my code list
print "get my code list...\n";
@solve = split(' ', `ls ???.c* ?????.c*`);
foreach(@solve){
	s/\D//g;
}

# find unsended codes & send it
print "find unsended codes & send it...\n";
foreach(@solve){
	$name = $_;
	$send = "no";
	foreach(@list){
		$send = "yes" if($_ eq $name);
	}
	if($send eq "no"){
#		print $name."\n";
		system("acmmail$index $name");
		sleep(10);
	}
}

print "finished!!\n";
