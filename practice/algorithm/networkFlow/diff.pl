#! /usr/bin/perl

$prog1 = "$ARGV[0]";
$prog2 = "$ARGV[1]";
die "file $prog1 cannot execute!\n" unless(-f $prog1 && -x $prog1);
die "file $prog2 cannot execute!\n" unless(-f $prog2 && -x $prog2);
print "diff $prog1 from $prog2..\n";
for($i=0; $i<3000; $i++){
	print "Case $i:\n";
	$test = `./test`;
	$result1 = `echo '$test' | ./$prog1`;
	$result2 = `echo '$test' | ./$prog2`;
	$result1 = $result1+0;
	$result2 = $result2+0;
	if($result1 != $result2){
		print $test."\n";
		print "result 1($prog1): $result1\n";
		print "result 2($prog2): $result2\n";
		last;
	}
}
