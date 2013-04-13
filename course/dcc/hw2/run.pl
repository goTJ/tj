#! /usr/bin/perl

$dir = __FILE__;
$dir =~ s/(.+)\/.+$/$1/;
chdir $dir;
system("killall receiver transmitter noiseGenServer repeater 2> /dev/null");
if($#ARGV < 4){
	print "Usage: ".__FILE__." qamfile fromfile tofile Eb/N0 numRepeat\n";
	exit;
}
unlink $ARGV[2];
$Eb = `./countEb $ARGV[0] 1`+0.0;
$N0 = $Eb/$ARGV[3];
$port = int(rand()*9000+1025);
print "Create receiver from port $port to file $ARGV[2]\n";
system("./receiver $ARGV[2] $ARGV[0] $port 1 > /dev/null &");
$portL = $port; $port++;
print "Create noiseGenServer from port $port to port $portL\n";
system("./noiseGenServer $port localhost $portL $N0 > /dev/null &");
for($i=0; $i<$ARGV[4]; $i++){
	$portL = $port; $port++;
	print "Create repeater from port $port to port $portL\n";
	system("./repeater $ARGV[0] $port localhost $portL 1 > /dev/null &");
	$portL = $port; $port++;
	print "Create noiseGenServer from port $port to port $portL\n";
	system("./noiseGenServer $port localhost $portL $N0 > /dev/null &");
}
$portL = $port; $port++;
print "Create transmitter from file $ARGV[1] to port $portL\n";
system("./transmitter $ARGV[1] $ARGV[0] localhost $portL 1 > /dev/null &");

while(($n=`ps ux | grep receiver | wc -l`) >= 3){
	sleep 1;
}
print "\n";
print "Eb = ${Eb}, N0 = $N0\n";
print "Eb/N0 = ".($Eb/$N0)."\n";
system("./countPe $ARGV[1] $ARGV[2]");
print "\n";
system("killall receiver transmitter noiseGenServer repeater 2> /dev/null");
