#! /usr/bin/perl -w

for($i=0; $i<@ARGV; $i++){
	die "file $ARGV[$i] cannot execute!\n" unless(-f $ARGV[$i] && -x $ARGV[$i]);
}
print "diff ";
for($i=0; $i<@ARGV; $i++){
	print "$ARGV[$i] ";
}
print "\n";
for($i=0; $i<30; $i++){
	print "Case ".($i+1).":\n";
	$Correct = "yes";
	$test = `./test`;
	for($j=0; $j<@ARGV; $j++){
		$result[$j] = `echo '$test' | ./$ARGV[$j]`;
		$result[$j] += 0;
	}
	for($j=1; $j<@ARGV; $j++){
		if($result[$j-1] != $result[$j]){
			for($k=0; $k<@ARGV; $k++){
				print "\tresult ".($k+1)."($ARGV[$k]): $result[$k]\n";
			}
			$Correct = "no";
			last;
		}
	}
	last if($Correct eq "no");
	print "\t$result[0]\n";
	sleep(1);
}
