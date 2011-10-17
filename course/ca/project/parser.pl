# vim:ft=perl:foldmethod=marker
#################################################################################
# BRF parser v0.1								#
# Abstract: This program parse a MIPS-- code into object file			#
# Usage: parser.pl FILENAME							#
# Auther: ±i³Í´Q, ÁÂ¨ôèû, ²ø¨å¿Ä						#
# Date: 05/04/06								#
#################################################################################
use strict;

# {{{ Const
my $FILENAME=$ARGV[0];
my $TEMPFILE=$FILENAME.".data.temp";
my $DATA_OFFSET = 0x800000;
my @R_FORMAT = (6, 5, 5, 5, 5, 6);
my @I_FORMAT = (6, 5, 5, 16);
my @J_FORMAT = (6, 26);
my $DATA_TYPE = '.word .asciiz';
open(SRC, "< $FILENAME") or die "Can't open file $FILENAME: $!";
# }}}

# {{{ Global variable
my $data_pos = $DATA_OFFSET;
my $text_pos = 0x0;
my $line_num = 0;
my $ori_sente;
# }}}

# {{{ static variable
# for output_binary
my $buf_index=0;
my $buf_remain=8;
my @buf=();
my $BUF_SIZE=32;

# for parse_data
my $now_data_type;
my $iscontinue=0;
# }}}

# {{{ tables
my %label;

# {{{ my %inst;
my $rrr_type = [qw/r r r/];
my $rr_type = [qw/r r/];
my $rri_type = [qw/r r i/];
my $r_type = [qw/r/];
my $rm_type = [qw/r m/];
my $ri_type = [qw/r i/];
my $i_type =[qw/i/];
my %inst;
$inst{add}[0] = $rrr_type;
$inst{add}[1] = [0, '$2', '$3', '$1', 0, 0x20];

$inst{addu}[0] = $rrr_type;
$inst{addu}[1] = [0, '$2', '$3', '$1', 0, 0x21];

$inst{'and'}[0] = $rrr_type;
$inst{'and'}[1] = [0, '$2', '$3', '$1', 0, 0x24];

$inst{jr}[0] = $r_type;
$inst{jr}[1] = [0, '$1', 0, 0, 0, 0x08];

$inst{nor}[0] = $rrr_type;
$inst{nor}[1] = [0, '$2', '$3', '$1', 0, 0x27];

$inst{'or'}[0] = $rrr_type;
$inst{'or'}[1] = [0, '$2', '$3', '$1', 0, 0x25];

$inst{slt}[0] = $rrr_type;
$inst{slt}[1] = [0, '$2', '$3', '$1', 0, 0x2a];

$inst{sltu}[0] = $rrr_type;
$inst{sltu}[1] = [0, '$2', '$3', '$1', 0, 0x2b];

$inst{sll}[0] = $rri_type;
$inst{sll}[1] = [0, '$2', 0, '$1', '$3', 0x00];

$inst{srl}[0] = $rrr_type;
$inst{srl}[1] = [0, '$2', 0, '$1', '$3', 0x02];

$inst{'sub'}[0] = $rrr_type;
$inst{'sub'}[1] = [0, '$2', '$3', '$1', 0, 0x22];

$inst{subu}[0] = $rrr_type;
$inst{subu}[1] = [0, '$2', '$3', '$1', 0, 0x23];



$inst{addi}[0] = $rri_type;
$inst{addi}[1] = [0x08, '$2', '$1', '$3'];

$inst{addiu}[0] = $rri_type;
$inst{addiu}[1] = [0x08, '$2', '$1', '$3'];

$inst{andi}[0] = $rri_type;
$inst{andi}[1] = [0x0c, '$2', '$1', '$3'];

$inst{ori}[0] = $rri_type;
$inst{ori}[1] = [0x0d, '$2', '$1', '$3'];

$inst{slti}[0] = $rri_type;
$inst{slti}[1] = [0x0a, '$2', '$1', '$3'];


$inst{sltiu}[0] = $rri_type;
$inst{sltiu}[1] = [0x0b, '$2', '$1', '$3'];

 
$inst{beq}[0] = $rri_type;
$inst{beq}[1] = [0x04, '$1', '$2', '$3'];

$inst{bne}[0] = $rri_type;
$inst{bne}[1] = [0x05, '$1', '$2', '$3'];


$inst{lbu}[0] = $rm_type;
$inst{lbu}[1] = [0x24, '$2', '$1', '$3'];

$inst{lhu}[0] = $rm_type;
$inst{lhu}[1] = [0x25, '$2', '$1', '$3'];

$inst{lui}[0] = $ri_type;
$inst{lui}[1] = [0x0f, 0, '$1', '$2'];

$inst{lw}[0] = $rm_type;
$inst{lw}[1] = [0x23, '$2', '$1', '$3'];

$inst{lb}[0] = $rm_type;
$inst{lb}[1] = [0x20, '$2', '$1', '$3'];

$inst{lh}[0] = $rm_type;
$inst{lh}[1] = [0x21, '$2', '$1', '$3'];

$inst{sb}[0] = $rm_type;
$inst{sb}[1] = [0x28, '$2', '$1', '$3'];

$inst{sh}[0] = $rm_type;
$inst{sh}[1] = [0x29, '$2', '$1', '$3'];

$inst{sw}[0] = $rm_type;
$inst{sw}[1] = [0x2b, '$2', '$1', '$3'];

$inst{j}[0] = $i_type;
$inst{j}[1] = [0x02,'$1'];

$inst{jal}[0] = $i_type;
$inst{jal}[1] = [0x03,'$1'];


$inst{mov}[0] = $rr_type;
$inst{mov}[1] = [0, 0, 0, '$1', 0, 0x20];
$inst{mov}[2] = [0, '$2', 0, '$1', 0, 0x20];


$inst{movi}[0] = $ri_type;
$inst{movi}[1] = [0x08, 0, '$1', 0];
$inst{movi}[2] = [0x08, 0, '$1', '$2'];
# }}}
# {{{ my %similar_inst;
my %similar_inst;
$similar_inst{add} = [ qw/addi/ ];
$similar_inst{addui} = [qw/addiu/];
$similar_inst{sltui} = [qw/sltiu/];
# }}}
# {{{ my %reg;
my %reg;
my $i;
for($i=0; $i<32; $i++){
	$reg{"\$$i"} = $i;
}
$reg{'$zero'} = 0;
$reg{'$v0'} = 2;
$reg{'$v1'} = 3;
$reg{'$a0'} = 4;
$reg{'$a1'} = 5;
$reg{'$a2'} = 6;
$reg{'$a3'} = 7;
$reg{'$t0'} = 8;
$reg{'$t1'} = 9;
$reg{'$t2'} = 10;
$reg{'$t3'} = 11;
$reg{'$t4'} = 12;
$reg{'$t5'} = 13;
$reg{'$t6'} = 14;
$reg{'$t7'} = 15;
$reg{'$s0'} = 16;
$reg{'$s1'} = 17;
$reg{'$s2'} = 18;
$reg{'$s3'} = 19;
$reg{'$s4'} = 20;
$reg{'$s5'} = 21;
$reg{'$s6'} = 22;
$reg{'$s7'} = 23;
$reg{'$t8'} = 24;
$reg{'$t9'} = 25;
$reg{'$gp'} = 28;
$reg{'$sp'} = 29;
$reg{'$fp'} = 30;
$reg{'$ra'} = 31;
# }}}

# }}}

### PROCEDURE ### {{{

# open temp file for storing data
open(TAR, "> $TEMPFILE") or die "Can't create temperate file: $!";

# {{{ first pass
my $istext = -1;	# 1: in text segment, 0: in data segment, -1: neither
while($_ = rdln_wo_cmt()){
	/(\S+?)\s(.*)/;
	if($1 eq ".data"){
		output_error(index($ori_sente, $1), "Don't append words after .data") if($2 =~ /(\S+)/);
		$istext = 0;
	}elsif($1 eq ".text"){
		output_error(index($ori_sente, $1), "Don't append words after .text") if($2 =~ /(\S+)/);
		$istext = 1;
	}elsif($_ =~ /(\S)/){
		if($istext == 1){
			parse_text($_);
		}elsif($istext == 0){
			parse_data($_);
		}else{
			output_error(index($ori_sente, $1), "Please enter .data or .text first.");
		}
	}
}
# }}}

output_binary(0, -1);	# flush buf into temp file
close(TAR);		# close temp file
# {{{ get object filename and open it
my $tmp = rindex($FILENAME, ".");
my $OBJNAME;
if($tmp != -1){
	$OBJNAME = substr($FILENAME, 0, $tmp).".o";
}else{
	$OBJNAME = $FILENAME.".o";
}
open(TAR, "> $OBJNAME") or die "Can't write file $OBJNAME: $!";
# }}}
output_binary($text_pos, 32);	# write text segment size into object file
output_binary($data_pos-$DATA_OFFSET, 32);	# write data segment size into object file

seek(SRC, 0, 0);	# reset position before repass
# {{{ second pass
my $istext = -1;	# 1: in text segment, 0: in data segment, -1: neither
while($_ = rdln_wo_cmt()){
	/(\S+?)\s(.*)/;
	if($1 eq ".data"){
		$istext = 0;
	}elsif($1 eq ".text"){
		$istext = 1;
	}elsif($_ =~ /(\S)/){
		if($istext == 1){
			parse_text2($_);
		}
	}
}
# }}}

output_binary(0, -1);	# flush buf into object file
close(SRC);		# close source file
# {{{ append data segment to object file
open(SRC, "< $TEMPFILE");
my $buf;
sysread SRC, $buf, $data_pos-$DATA_OFFSET;
syswrite TAR, $buf, $data_pos-$DATA_OFFSET;
close(SRC);
unlink($TEMPFILE);	# remove temp file
# }}}
close(TAR);

### END ### }}}

### FUNCTION LIST ### {{{

# {{{ sub rdln_wo_cmt{
# readline from source code(SRC) without comments.
# PARAM: none
# RETURN: the sentence without comments. none if EOF.
# NOTE: $ori_sente store the ordinary sentence.
# NOTE: $line_num record the line number of this sentence in source code.
sub rdln_wo_cmt{
	if($_ = <SRC>){
		$line_num++;
		s/\t/        /g;	# replace <tab> with 8 spaces
		$ori_sente = $_;
		s/#.*//;	# ingore comments
		return $_;
	}
	return "";
}
# }}}

# {{{ sub output_error{
# output error message and exit.
# PARAM 0: the position of sentence where error occured. don't display if the value is -1.
# PARAM 1: additional message of the error. (optional)
# RETURN: none
sub output_error{
	print STDERR "brfspim: parse error on line $line_num of file $FILENAME\n";
	print STDERR $ori_sente;
	print STDERR ((" "x$_[0])."^\n") if($_[0] != -1);
	print STDERR "$_[1]\n" if($_[1]);
	close(SRC);		# close temp file
	unlink($TEMPFILE);	# remove temp file
	exit 1;
}
# }}}

# {{{ sub parse_data{
# parse data segment sentence for first pass
# PARAM 0: the sentence to be parsed.
# RETURN: none
sub parse_data{
	$_ = $_[0];
	# judge label
	if(s/(.*)://){
		output_error(index($ori_sente, ":"), "Illegal label name.") unless($1 =~ /^\s*(\w[\w\d]+)\s*$/);
		output_error(index($ori_sente, ":"), "Redefine label.") if(defined($label{$1}));
		$label{$1} = $data_pos;
	}
	# judge data
	if(/(\S+?)\s/){
		if(!$iscontinue){
			if(index($1, '.') == 0){
				output_error(index($ori_sente, $1)+1, "Unknown data type.") if(index($DATA_TYPE, $1) == -1);
				$now_data_type=$1;
				s/\s*$1//;
			}else{
				output_error(index($ori_sente, $1), "Missing comma?") if(!s/^\s*,//);
			}
		}
		$iscontinue = (s/,\s*$//) ? 1 : 0;
		my @tmp=split ',';
		if($now_data_type eq '.word'){
			foreach(@tmp){
				my $val = get_num($_);
				/^\s*(.+)/;
				output_error(index($ori_sente, $1), "Invalid number.") if($val eq "Invalid");
				output_binary($val, 32);
				$data_pos += 4;
			}
		}elsif($now_data_type eq '.asciiz'){
			foreach(@tmp){
				/^\s*(.+)/ and output_error(index($ori_sente, $1)) if(!/"(.+)"/ && !/'(.+)'/);
				foreach(split '', $1){
					output_binary(ord($_), 8);
					$data_pos += 1;
				}
				output_binary(0, 8);
				$data_pos += 1;
			}
		}else{
			output_error(index($ori_sente, $1));
		}
	}
}
# }}}

# {{{ sub parse_text{
# parse text segment sentence for first pass
# PARAM 0: the sentence to be parsed.
# RETURN: none
sub parse_text{
	$_ = $_[0];
	# judge label
	if(s/(.*)://){
		output_error(index($ori_sente, ":"), "Illegal label name.") unless($1 =~ /^\s*(\w[\w\d]+)\s*$/);
		output_error(index($ori_sente, ":"), "Redefine label.") if(defined($label{$1}));
		$label{$1} = $text_pos;
	}
	# judge instructions
	if(/(\S+?)\s/){
		output_error(index($ori_sente, $1), "Unknown instruction.") if(!defined($inst{$1}));
		$text_pos += $#{$inst{$1}}*4;
	}
}
# }}}

# {{{ sub parse_text2{
# parse text segment sentence for second pass
# PARAM 0: the sentence to be parsed.
# RETURN: none
sub parse_text2{
	$_ = $_[0];
	# skip label
	s/(.*)://;
	# judge instruction
	if(/(\S+?)\s/){
		my @col=separate($_);
		my @node=get_type_and_val(@col);	# @{$node[0]} stores type and @{$node[1]} stores value.
		if(check_type($node[0], $inst{$col[0]}[0])){
			my $i;
			for($i=1; $i<=$#{$inst{$col[0]}}; $i++){
				output_inst($inst{$col[0]}[$i], $node[1]);
			}
		}else{
			my $is_fixed=0;
			foreach(@{$similar_inst{$col[0]}}){
				if(check_type($node[0], $inst{$_}[0])){
					$is_fixed = 1;
					print STDERR "brfspim: syntex error on line $line_num of file $FILENAME\n";
					print STDERR "Fixed by replacing instruction '$col[0]' with '$_'.\n";
					my $i;
					for($i=1; $i<=$#{$inst{$_}}; $i++){
						output_inst($inst{$_}[$i], $node[1]);
					}
					last;
				}
			}
			if(!$is_fixed){
				output_error(index($ori_sente, $col[0]), "Wrong usage!");
			}
		}
	}
}
# }}}

# {{{ sub separate{
# split sentence by space or comma.
# PARAM 0: the sentence to be parsed.
# RETURN: the array generated by spliting sentence.
sub separate{
	if(index($_[0], ",") != -1){
		/(\S+?)\s(.*)/;
		my $tmp = $1;
		my $tmp2 = $2;
		$tmp2 =~ s/\s//g;
		return ($tmp, split(',', $tmp2));
	}else{
		return split ' ', $_[0];
	}
}
# }}}

# {{{ sub get_type_and_val{
# judge the type and the value of every word in the array.
# PARAM 0: the array of words.
# RETURN: an array points two array, the first array stores the type, the second stores the value.
sub get_type_and_val{
	my $i;
	my @tmp;
	for($i=1; $i<=$#_; $i++){
		$_ = $_[$i];
		my $val = get_num($_);
		if($val ne "Invalid"){
			push @{$tmp[0]}, "i";
			push @{$tmp[1]}, $val;
		}elsif(defined($label{$_})){
			push @{$tmp[0]}, "i";
			push @{$tmp[1]}, $label{$_};
		}elsif(defined($reg{$_})){
			push @{$tmp[0]}, "r";
			push @{$tmp[1]}, $reg{$_};
		}elsif(/(\d*)\((.+)\)/ && defined($reg{$2})){
			push @{$tmp[0]}, "m";
			push @{$tmp[1]}, $reg{$2}, (($1)?$1:0);
		}else{
			push @{$tmp[0]}, "e";	# error
			push @{$tmp[1]}, 0;
		}
	}
	return @tmp;
}
# }}}

# {{{ sub check_type{
# compare two array if they are the same type.
# PARAM 0: one of the array to be compared.
# PARAM 1: the other.
# RETURN: 1 if the are the same, 0 otherwise.
sub check_type{
	return 0 if($#{$_[0]} != $#{$_[1]});
	my $i;
	for($i=0; $i<=$#{$_[0]}; $i++){
		return 0 if($_[0][$i] ne $_[1][$i]);
	}
	return 1;
}
# }}}

# {{{ sub output_inst{
# output instructoin into object file
# PARAM 0: the array representing the columns of the instruction.
# PARAM 1: the array storing the arguments of the instruction.
# RETURN: none
sub output_inst{
	my @format;
	if($#{$_[0]} == 5){
		@format = @R_FORMAT;
	}elsif($#{$_[0]} == 3){
		@format = @I_FORMAT;
	}else{
		@format = @J_FORMAT;
	}
	my $i, my $val, my $tmp;
	for($i=0; $i<=$#format; $i++){
		$val = ($_[0][$i] =~ /\$(\d)/) ? $_[1][$1-1] : $_[0][$i];
		output_binary($val, $format[$i]);
	}
}
# }}}

# {{{ sub output_binary{
# convert input into binary and output into target(TAR) file.
# PARAM 0: the bits representing by integer.
# PARAM 1: the number of bits to be output. -1 for flush buffer.
sub output_binary{
	my ($val, $size) = @_;
	$val &= (2**$size)-1;
	while($size > 0){
		my $len=($size<$buf_remain) ? $size : $buf_remain;
		$buf[$buf_index] = ($buf[$buf_index]<<$buf_remain) | ($val>>($size-$len));
		$buf_remain -= $len;
		if($buf_remain <= 0){
			$buf_index++;
			$buf[$buf_index] = 0;
			$buf_remain += 8;
		}
		$size -= $len;
		$val &= (2**$size)-1;
	}
	if(($size == -1) || ($buf_index > $BUF_SIZE)){
		while($buf_index > 0){
			syswrite TAR, chr(shift(@buf));
			$buf_index--;
		}
		if($size == -1 && $buf_remain < 8){
			$buf[0] <<= $buf_remain;
			syswrite TAR, chr(shift(@buf));
			$buf_remain = 8;
		}
	}
}
# }}}

# {{{ sub get_num{
# tranfer string into number.
# PARAM 0: the string to be tranfered.
# RETURN: the number of string. "Invalid" if the string is invalid.
sub get_num{
	$_ = $_[0];
	if(/^\s*([0-1]+)b\s*$/){
		return eval("0b$1");
	}elsif(/^\s*-{0,1}([1-9][0-9]*|0)\s*$/){
		return $_+0;
	}elsif(/^\s*([0-9][0-9a-f]*)h\s*$/){
		return eval("0x$1");
	}else{
		return "Invalid";
	}
}
# }}}

### END ### }}}
