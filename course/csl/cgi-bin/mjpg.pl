#! /usr/bin/perl

use CGI;
use IO::Socket;
$query = new CGI;
$query->import_names('R');
 
#syswrite STDOUT, "Content-type: text/html\r\n"; syswrite STDOUT, "\r\n";
$lsnSock = newSocket();
my $sock = IO::Socket::INET->new(
		PeerAddr => $R::addr,
		PeerPort => $R::port,
		Proto    => 'tcp')
	or die "Can't connect: $!\n";
print $sock "PINKY ".$lsnSock->sockport."\r\n";

$session = $lsnSock->accept;

syswrite STDOUT, "Content-Type: multipart/x-mixed-replace; boundary=--myboundary\r\n";

$buf = <$session>;
if($buf =~ /NEW (.+)\r\n/){
	$data = '';
	open(FIN, $1);
	$data .= $_ foreach(<FIN>);
	close(FIN);
	syswrite STDOUT, "\r\n";
	syswrite STDOUT, "--myboundary\r\n";
	syswrite STDOUT, "Content-Type: image/jpeg\r\n";
	syswrite STDOUT, "Content-Length: ".length($data)."\r\n";
	syswrite STDOUT, "\r\n";
	syswrite STDOUT, $data;
	$buf = <$session>;
}

do{
	if($buf =~ /--myboundary (\d+)/){
		syswrite STDOUT, "\r\n";
		syswrite STDOUT, "--myboundary\r\n";
		syswrite STDOUT, "Content-Type: image/jpeg\r\n";
		syswrite STDOUT, "Content-Length: ".$1."\r\n";
		syswrite STDOUT, "\r\n";
	}else{
		syswrite STDOUT, $buf;
	}
}while($buf = <$session>);

print $sock "NO ".$lsnSock->sockport."\r\n";

sub newSocket #{{{
{
	my($portNum) = @_;
	my $socket;
	if($portNum && $portNum =~ /^\d+$/){
		$socket = IO::Socket::INET->new( Listen    => 20,
			LocalPort => $portNum,
			Timeout   => 60*1,
			Reuse     => 1)
			or die "Can't create listening socket: $!\n";
		return $socket;
	}else{
		while(1){
			$portNum=int(rand(8000)+1050);
			$socket = IO::Socket::INET->new( Listen    => 20,
				LocalPort => $portNum,
				Timeout   => 60*1,
				Reuse     => 1)
				and return $socket;
		}
	}
} # }}}
