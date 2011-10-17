#! /usr/bin/perl

my $servaddr = '140.112.30.44';
my $servport = '3000';

use CGI;
use IO::Socket;
$query = new CGI;
$query->import_names('R');

print "Content-type: text/html\r\n";
print "\r\n";
print '<html><head>
<title>FJB Monitor</title>
<link rel="stylesheet" href="style.css" type="text/css" />
<style type="text/css"> /* {{{ */
body
{ 
	background: #EEEEEE;
}
input.button, button.button, select.button{
	border: 1px solid #8cacbb;
	color: Black;
	background-color: white;
	vertical-align: middle;
	text-decoration:none;
	font-size: 14px;
	cursor: pointer;
	height: 22px !important;
	max-height: 22px !important;
	min-height: 22px !important;
	margin: 1px;
	display: inline;
}
textarea.edit {
	font-family:monospace;
	border: 1px solid #8cacbb;
	color: Black;
	background-color: white;
	font-size:14px;
	padding: 3px;
	width:100%;
}
</style> <!-- }}} -->
</head>
<body>
<!-- logo {{{ -->
<div class="logo">

<span class="title">FJB Monitor - Fully Jamed Buffer Monitor</span>
<span class="update">
Update: 2006/06/29
</span>
</div><br/>
<!-- logo }}} -->';

my $sock = IO::Socket::INET->new(
		PeerAddr => $servaddr,
		PeerPort => $servport,
		Proto    => 'tcp');
if(!$sock){
	print "Can't connect to server: $!\n";
	print "</body></html>";
	exit;
}

@req = ();
print $sock "list\r\n";
my $num = <$sock>;
print '<div class="main">';
print 'Num of Cam: '.$num."<br>";
print '<form>';
print '<input type="submit" class="button" value="Refresh">';
print '</form>';
print '<form>';
print 'Cam Address: ';
print '<select name="cam">';
for($i=0; $i<$num; $i++){
	my $buf = <$sock>;
	$buf =~ s/\r\n$//;
	@data = split / /, $buf;
	if($R::cam eq $data[0]){
		@req = @data;
		print '<option selected="selected">'.$data[0];
	}else{
		print "<option >$data[0]";
	}
}
print '</select> ';
print '<input type="submit" class="button" value="Connect!">';
print '</form>';
if(@req){
	print '
	<div class="lbox">
	<h3>Cam '.$R::cam.'</h3>
	<p align="center">
	<script language="javascript">
	var BaseURL = "/~b92106/";
	var DisplayWidth = "320";
	var DisplayHeight = "240";
	var File = "cgi-bin/mjpg.pl?addr=140.112.30.44&port='.$req[-1].'";
	var output = "";
	if ((navigator.appName == "Microsoft Internet Explorer")&&(navigator.platform != "MacPPC")&&(navigator.platform != "Mac68k"))
	{ 
	// If Internet Explorer for Windows then use ActiveX 
	output =  "<OBJECT ID=\"CamImage\" WIDTH="
	output += DisplayWidth;
	output += " HEIGHT=";
	output += DisplayHeight;
	output += " CLASSID=CLSID:917623D1-D8E5-11D2-BE8B-00104B06BDE3 ";
	output += "CODEBASE=\"";
	output += BaseURL;
	output += "activex/AxisCamControl.ocx#Version=1,0,1,34\">";
	output += "<PARAM NAME=\"URL\" VALUE=\"";
	output += BaseURL;
	output += File;
	output += "\"> <BR><B>Axis ActiveX Camera Control</B><BR>";
	output += "The AXIS ActiveX Camera Control, which enables you ";
	output += "to view live image streams in Microsoft Internet";
	output += " Explorer, could not be registered on your computer.";
	output += "<BR></OBJECT>"; 
	} 
	else 
	{
	// If not IE for Windows use the browser itself to display
	output = "<IMG SRC=\"";
	output += BaseURL;
	output += File;
	output += "\" HEIGHT=\"";
	// The above dummy cgi-parameter helps some versions of NS
	output += DisplayHeight;
	output += "\" WIDTH=\"";
	output += DisplayWidth;
	output += "\" ALT=\"Moving Image Stream\">";
	} 
	output += "<p/>";

	document.write(output);
	</script></p>';
}
print "</div></div></body></html>";

$sock->close;
