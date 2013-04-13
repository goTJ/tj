%{
#include <stdlib.h>

int _lineno=0;
string _linebuf;

static int first_time=1;
%}

%x LINEHEAD

Int [0-9]+
Real [0-9]+\.[0-9]+
Name [_a-zA-Z][_a-zA-Z0-9]*
%%
%{
	if(first_time){
		BEGIN LINEHEAD;
		first_time = 0;
	}
%}

<LINEHEAD>.*|\n	{ _linebuf=string(yytext); fprintf(cmmOut, "/* %s */\n", _linebuf.c_str()); _lineno++; yyless(0); BEGIN 0; }
[ \t]		{ }
\n		{ BEGIN LINEHEAD; return '\n'; }
"PROGRAM"	{ return PROGRAM; }
"CONST"		{ return CONST; }
"ENDCONST"	{ return ENDCONST; }
"TYPE"		{ return TYPE; }
"ENDTYPE"	{ return ENDTYPE; }
"VAR"		{ return VAR; }
"ENDVAR"	{ return ENDVAR; }
"PROCEDURE"	{ return PROCEDURE; }
"FUNCTION"	{ return FUNCTION; }
"ARRAY"		{ return ARRAY; }
"OF"		{ return OF; }
"IF"		{ return IF; }
"THEN"		{ return THEN; }
"ELSE"		{ return ELSE; }
"ENDIF"		{ return ENDIF; }
"FOR"		{ return FOR; }
"TO"		{ yylval.num = 1; return TO; }
"DOWNTO"	{ yylval.num = -1; return TO; }
"DO"		{ return DO; }
"WHILE"		{ return WHILE; }
"CASE"		{ return CASE; }
"ENDCASE"	{ return ENDCASE; }
"OTHERWISE"	{ return OTHERWISE; }
"BEGIN"		{ return _BEGIN; }
"END"		{ return END; }
{Real}		{ yylval.gt.arrayBound = new list<Dimension>; yylval.gt.size = 1; yylval.gt.t = FLOAT; yylval.gt.s=_CONST; yylval.gt.fval = atof(yytext); return VAL; }
{Int}		{ yylval.gt.arrayBound = new list<Dimension>; yylval.gt.size = 1; yylval.gt.t = INT; yylval.gt.s=_CONST; yylval.gt.ival = atoi(yytext); return VAL; }
"+"		{ return '+'; }
"-"		{ return '-'; }
"*"		{ return '*'; }
"/"		{ return '/'; }
"MOD"		{ return MOD; }
"("		{ return '('; }
")"		{ return ')'; }
","		{ return ','; }
":"		{ return ':'; }
";"		{ return ';'; }
"="		{ return '='; }
":="		{ return ASSIGN; }
"["		{ return '['; }
"]"		{ return ']'; }
".."		{ return DOTDOT; }
"<->"		{ return SWAP; }
"OR"		{ return OR; }
"AND"		{ return AND; }
"NOT"		{ return NOT; }
"XOR"		{ return XOR; }
">"		{ return '>'; }
"<"		{ return '<'; }
"<="		{ yylval.bop = 'L'; return CMP; }
">="		{ yylval.bop = 'G'; return CMP; }
"<>"		{ yylval.bop = 'N'; return CMP; }
\%\%.*		{ }
{Name}		{ yylval.str=new string(yytext); return NAME; }
\"([^"]|\\.)*\"	{ yytext[strlen(yytext)-1] = '\0'; yylval.str=new string(yytext+1); return STRING; }
.		{ return ERROR; }

%%
