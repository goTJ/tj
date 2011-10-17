#! /usr/bin/perl

$PROG_DIR = '/home/master/96/roastfish/public-html/MMAI/mmai_prog';

open(LIST_FIN, "$PROG_DIR/config") or die "Can't open config file";

if($#ARGV < 0){
	print "Usage: ".__FILE__." image_dir\n";
	exit;
}

$IMG_DIR = $ARGV[0];
$LISTFILE = 'list';
$HTMLFILE = 'result.html';

system("find $IMG_DIR -type f -iname '*.jpg' -o -iname '*.gif' -o -iname '*.png' > $IMG_DIR/$LISTFILE");

print "Classifying...\n";
foreach(<LIST_FIN>){
	chomp;
	s/#[\s\S]+$//;
	($cat, $prog, $title) = split(/\s+/);
	next unless(defined($cat));

	# print "$cat, $prog, $title\n";
	print STDERR "Warning: Can't execute $prog\n" and next unless(-x "$PROG_DIR/$prog" || $prog eq '(none)');
	if($cat eq 'preproc'){
		system("$PROG_DIR/$prog $IMG_DIR/$LISTFILE");
	}elsif($cat eq 'init'){
		system("$PROG_DIR/$prog $IMG_DIR/$LISTFILE");
		$titles{''} = $title;
		unlink("$IMG_DIR/$LISTFILE");
	}else{
		$cat =~ /(.+)_\d+$/;
		$titles{$cat} = $titles{$1}.' - '.$title;
		if($prog ne '(none)'){
			system("$PROG_DIR/$prog $IMG_DIR/$LISTFILE$cat");
			unlink("$IMG_DIR/$LISTFILE$cat");
		}
	}
}

close(LIST_FIN);

# Generate HTML page
print "Generating HTML page...\n";
@listArray = sort(split(/\n/, `find $IMG_DIR -name $LISTFILE\*`));
open(HTML_FOUT, "> $IMG_DIR/$HTMLFILE");
print HTML_FOUT '
<html>
<head>
<title>'.$titles{''}.'</title>
</head>

<body bgcolor="#ddffee">
';
foreach(<@listArray>){
	$listname = $_;
	/$LISTFILE(.+)/;
	$title = $titles{$1};
	print HTML_FOUT "<h1>$title</h1>\n";

	$count = 0;
	open(FIN, $listname);
	foreach(<FIN>){
		chomp;
		if(m#$IMG_DIR/?([\s\S]+)(\.jpg|\.gif|\.png)#i){
			#system("convert $_ -resize 400x400 $IMG_DIR/t_$1$2");
			print HTML_FOUT '<a href="'.$1.$2.'" target="_blank"><img src="'.$1.$2.'" width="12%"></a>¡@'."\n";
			$count++;
			if($count > 6){
				print HTML_FOUT "<p>\n";
				$count = 0;
			}
		}
	}
	close(FIN);
	unlink($listname);
	print HTML_FOUT '¡@<p>¡@<p>¡@<p>';
}
print HTML_FOUT '
</body>
</html>
';
close(HTML_FOUT);
