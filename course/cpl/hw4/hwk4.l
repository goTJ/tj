%{
#include <stdlib.h>

int _lineno=1;
int _linepos=0;
string _linebuf;
%}

%x LINEHEAD

Real [0-9]+\.[0-9]+
Name [_a-zA-Z][_a-zA-Z0-9]*
%%

<LINEHEAD>.*|\n	{ _linebuf=string(yytext); _lineno++; _linepos=0; yyless(0); BEGIN 0; }
[ \t]		{ _linepos += yyleng; }
\n		{ BEGIN LINEHEAD; return '\n'; }
"PROGRAM"	{ _linepos += yyleng; return PROGRAM; }
"VAR"		{ _linepos += yyleng; return VAR; }
"REAL"		{ _linepos += yyleng; yylval.type = FLOAT; return TYPE; }
"BEGIN"		{ _linepos += yyleng; return BEGINN; }
"END"		{ _linepos += yyleng; return END; }
{Real}		{ _linepos += yyleng; yylval.gt.t = FLOAT; yylval.gt.fval = atof(yytext); return VAL; }
{Name}		{ _linepos += yyleng; return NAME; }
{Name}"()"	{ _linepos += yyleng; yylval.str=new string(yytext); return FUNC0; };
"+"		{ _linepos += yyleng; return '+'; }
"-"		{ _linepos += yyleng; return '-'; }
"*"		{ _linepos += yyleng; return '*'; }
"/"		{ _linepos += yyleng; return '/'; }
"("		{ _linepos += yyleng; return '('; }
")"		{ _linepos += yyleng; return ')'; }
","		{ _linepos += yyleng; return ','; }
":"		{ _linepos += yyleng; return ':'; }
";"		{ _linepos += yyleng; return ';'; }
":="		{ _linepos += yyleng; return ASSIGN; }
\%\%.*		{ _linepos += yyleng; }
.		{ _linepos += yyleng; return ERROR; }

%%
