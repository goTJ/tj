use Tk;
$TOBINARY_PROC = "./tobinary";
$PARSER_PROC = "perl parser.pl";

$mw = MainWindow->new;
# Create necessary widgets
$f = $mw->Frame->pack(-side => 'top', -fill => 'x');
$f->Label(-text => "Filename:")->pack(-side => 'left', -anchor => 'w');
$f->Entry(-textvariable => \$filename)->pack(-side => 'left', -anchor => 'w', -fill => 'x', -expand => 1);
$f->Button(-text => "Load", -command => \&load_file)->pack(-side => 'left', -anchor => 'e');
$f->Button(-text => "Exit", -command => sub { exit; } )->pack(-side => 'right');
$mw->Label(-textvariable => \$info, -relief => 'ridge')->pack(-side => 'top', -fill => 'x');
$code_area = $mw->Scrolled("Text", -width => 100, -height => 10)->pack(-side => 'top', -fill => 'both', -expand => 1);
$mw->Label(-text => "Message", -relief => 'ridge')->pack(-side => 'top', -fill => 'x');
#$mw->Label(-text => "Message", -justify => "center")->pack(-side => 'top', -anchor => 'w');
$msg_area = $mw->Scrolled("Text", -width => 100, -height => 10)->pack(-side => 'top', -fill => 'both', -expand => 1);
$mw->Label(-text => "Object File", -relief => 'ridge')->pack(-side => 'top', -fill => 'x');
#$mw->Label(-text => "Object File", -justify => "center")->pack(-side => 'top', -anchor => 'w');
$obj_area = $mw->Scrolled("Text", -width => 100, -height => 10)->pack(-side => 'top', -fill => 'both', -expand => 1);
$msg_area->delete("1.0", "end");

MainLoop;

# load_file checks to see what the filename is and loads it if possible
sub load_file {
	$info = "Loading file '$filename'...";
	$code_area->delete("1.0", "end");
	$obj_area->delete("1.0", "end");
	$msg_area->insert("end", "parse $filename...\n");
	if (!open(FH, "$filename")){
		$code_area->insert("end", "ERROR: Could not open $filename\n"); 
		return; 
	}
	$line_num = 1;
	while(<FH>){
		$code_area->insert("end", sprintf("%2d: ", $line_num).$_);
		$line_num++;
	}
	close (FH);
	$info = "File '$filename' loaded";

	$result = `$PARSER_PROC $filename 2>&1`;
	$msg_area->insert("end", $_."\n") foreach(split "\n", $result);

	if(index($result, "error") == -1){
		$msg_area->insert("end", "parse $filename OK.\n");
		my $tmp = rindex($filename, ".");
		my $objname;
		if($tmp != -1){
			$objname = substr($filename, 0, $tmp).".o";
		}else{
			$objname = $filename.".o";
		}
		$obj_area->insert("end", $_."\n") foreach(split "\n", `$TOBINARY_PROC < $objname`);
	}
}
