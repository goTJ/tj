// vim:ft=yacc:foldmethod=marker
%{
#include <cstdio>
#include <string>
#include <map>
#include <list>

using std::string;
using std::map;
using std::list;
extern int _lineno;
extern int _linepos;
extern string _linebuf;

// {{{ extern "C"
	int yyparse(void);
	int yylex(void);  
	int yyerror(char *msg)
	{   
		fprintf(stderr, "Find error in line %d: %s\n", _lineno, msg);
		fprintf(stderr, "%s\n", _linebuf.c_str());
		fprintf(stderr, "%*s\n", _linepos, "^");
	}
 // }}}

enum Type{ RESERVE, FUNC, FLOAT };
// {{{ struct GeneralType{
struct GeneralType{
	Type t;
	union{
		float fval;
	};
}; // }}}
// {{{ struct Attr{
struct Attr{
	GeneralType gt;
}; // }}}

map<string, Attr> symbol;

%}
// {{{ %union{
%union{
	GeneralType gt;
	GeneralType *gtp;
	string *str;
	Type type;
	list<string> *str_list;
} // }}}

// {{{ tokens
%token ERROR
%token<str> FUNC0
%token PROGRAM VAR BEGINN END
%token<type> TYPE
%token<str> NAME
%token<gt> VAL
%right ASSIGN
%left '+' '-'
%left '*' '/'
%right UMINUS
// }}}
// {{{ types
%type<gt> expr
%type<gtp> lval
%type<str_list> var_list
%type<str> all_name
// }}}
%% // {{{ PDA
start	: empty_lines head empty_lines var_region state empty_lines // {{{
	| error
	; // }}}
all_name: NAME { $$=new string(yytext); } // {{{
	| PROGRAM { $$=new string(yytext); }
	| TYPE { $$=new string(yytext); }
	| VAR { $$=new string(yytext); }
	| BEGINN { $$=new string(yytext); }
	| END { $$=new string(yytext); }
	; // }}}
lval	: all_name { // {{{
		$$ = get_symbol_val(*$1);
		if($$ == NULL){
			yyerror(sprint("Using undeclared variables %s", $1->c_str()));
			YYERROR;
		}else{
			if(!isVar($$->t)){
				yyerror(sprint("using invalid ID names %s", $1->c_str()));
				YYERROR;
			}
		}
	}
	; // }}}
/*
lval	: all_name { $$ = get_symbol_val(*$1); } // {{{
	; // }}}
*/
empty_lines	: empty_lines '\n' // {{{
	    	| /* empty */
		; // }}}
head	: PROGRAM NAME '\n' // {{{
	; // }}}
var_region	: var_head var_body // {{{
	; // }}}
var_head: VAR '\n' // {{{
	; // }}}
var_body: var_body var_line '\n' // {{{
    	| var_body '\n'
	| 
    	| var_body error '\n' { yyerrok; }
	; // }}}
var_line: var_list ':' TYPE ';' { // {{{
		for(list<string>::iterator iter=$1->begin(); iter!=$1->end(); iter++){
			if(get_symbol_val(*iter) != NULL){
				yyerror(sprint("Variable %s declared twice", iter->c_str()));
				YYERROR;
				break;
			}else{
				add_symbol(*iter, $3);
			}
		}
	}
	; // }}}
var_list: var_list ',' NAME { // {{{
		$1->push_back(yytext);
	}
	| NAME{
		$$ = new list<string>;
		$$->push_back(yytext);
	}
	; // }}}
state	: BEGINN '\n' lines END // {{{
	; // }}}
lines   : lines expr ';' '\n' // {{{
        | lines '\n'
        |
        | lines error '\n' { yyerrok; }
        ; // }}}
expr    : expr '+' expr { $$.fval = $1.fval + $3.fval; } // {{{
	| expr '-' expr { $$.fval = $1.fval - $3.fval; }
	| expr '*' expr { $$.fval = $1.fval * $3.fval; }
	| expr '/' expr { $$.fval = $1.fval / $3.fval; }
        | '(' expr ')' { $$.fval = $2.fval; }
        | '-' expr  %prec UMINUS { $$.fval = -$2.fval; }
	| lval ASSIGN expr { *$1 = $3; $$ = $3; }
//	| all_name '(' arg_list ')' { $$.fval = 0.0; }
	| all_name '(' lval ')' {
		if(*$1 == "READ"){
			scanf("%f", &($3->fval));
		}else if(*$1 == "WRITE"){
			printf("%f", $3->fval);
		}else{
			yyerror(sprint("Using undeclared function %s() or wrong parameter", $1->c_str()));
			YYERROR;
		}
	}
        | lval { $$ = *$1; }
        | VAL { $$ = $1; }
	| FUNC0 {
		$$.fval = 0.0;
		if(*$1 == "WRITESP()"){
			printf(" ");
		}else if(*$1 == "WRITELN()"){
			printf("\n");
		}else{
			yyerror(sprint("Using undeclared function %s or wrong parameter", $1->c_str()));
			YYERROR;
		}
	}
	; // }}}
/*
arg_list: arg_list all_name
	| all_name
	;
*/
%% // }}}
#include "lex.yy.c"

// {{{ char *sprint(char *format, const char *name)
char *sprint(char *format, const char *name)
{
	char *buf=new char[strlen(format)+strlen(name)];
	sprintf(buf, format, name);
	return buf;
} // }}}
// {{{ bool isVar(Type t)
bool isVar(Type t)
{
	if(t == RESERVE || t == FUNC)
		return false;
	return true;
} // }}}
// {{{ void init_symbol()
void init_symbol()
{
	Attr reserve;
	reserve.gt.t = RESERVE;

	symbol["PROGRAM"] = reserve;
	symbol["VAR"] = reserve;
	symbol["BEGIN"] = reserve;
	symbol["END"] = reserve;
	symbol["REAL"] = reserve;
} // }}}
// {{{ void init_function()
void init_function()
{
	Attr func;
	func.gt.t = FUNC;

	symbol["READ"] = func;
	symbol["WRITE"] = func;
	symbol["WRITESP"] = func;
	symbol["WRITELN"] = func;
} // }}}
// {{{ void add_symbol(string name, Type t)
void add_symbol(string name, Type t)
{
	if(symbol.find(name) != symbol.end()){
		printf("exists\n");
	}else{
		Attr a;
		a.gt.t = t;
		symbol[name] = a;
	}
} // }}}
// {{{ GeneralType *get_symbol_val(string name)
GeneralType *get_symbol_val(string name)
{
	if(symbol.find(name) == symbol.end()){
		return NULL;
	}else{
		return &(symbol[name].gt);
	}
} // }}}

int main()
{
	init_symbol();
	init_function();
        yyin = fopen("program.p", "r");
	yyparse();
	return 0;
}
