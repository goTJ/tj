%{
#include <stdlib.h>

int _lineno=0;
//int _linepos=0;
string _linebuf;

static int first_time=1;
%}

%x LINEHEAD

Real [0-9]+\.[0-9]+
Int [1-9][0-9]*|0
Name [_a-zA-Z][_a-zA-Z0-9]*
WrongConst [0-9]+|[0-9]*\.[0-9]*
WrongName [_a-zA-Z0-9]*
%%
%{
	if(first_time){
		BEGIN LINEHEAD;
		first_time = 0;
	}
%}

<LINEHEAD>.*|\n	{ _linebuf=string(yytext); _lineno++; yyless(0); BEGIN 0; }
[ \t]		{ }
\n		{ BEGIN LINEHEAD; return '\n'; }
"PROGRAM"	{ return PROGRAM; }
"VAR"		{ return VAR; }
"ENDVAR"	{ return ENDVAR; }
"INTEGER"	{ yylval.type = INT; return TYPE; }
"REAL"		{ yylval.type = FLOAT; return TYPE; }
"PROCEDURE"	{ return PROCEDURE; }
"BEGIN"		{ return BEGINN; }
"END"		{ return END; }
{Real}		{ yylval.gt.t = FLOAT; yylval.gt.fval = atof(yytext); return VAL; }
{Int}		{ yylval.gt.t = INT; yylval.gt.ival = atoi(yytext); return VAL; }
{Name}		{ yylval.str=new string(yytext); return NAME; }
{WrongConst}	{ return EVAL; }
{WrongName}	{ yylval.str=new string(yytext); return ENAME; }
"+"		{ return '+'; }
"-"		{ return '-'; }
"*"		{ return '*'; }
"/"		{ return '/'; }
"("		{ return '('; }
")"		{ return ')'; }
","		{ return ','; }
":"		{ return ':'; }
";"		{ return ';'; }
":="		{ return ASSIGN; }
\%\%.*		{ }
.		{ return ERROR; }

%%
