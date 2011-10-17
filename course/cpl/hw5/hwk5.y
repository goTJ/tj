// vim:ft=yacc:foldmethod=marker
%{
#include <cstdio>
#include <string>
#include <map>
#include <list>
#include <cstring>

using std::string;
using std::map;
using std::list;
extern int _lineno;
extern string _linebuf;
static bool isNoError=true;

// {{{ extern "C"
	int yyparse(void);
	int yylex(void);  
	int yyerror(char *msg)
	{   
		isNoError = false;
		if(!strcmp(msg, "syntax error"))
			return 0;
		fprintf(stderr, "Find error in line %d: %s\n", _lineno, msg);
		fprintf(stderr, "%s\n", _linebuf.c_str());
//		fprintf(stderr, "%*s\n", _linepos, "^");
	}
 // }}}

enum Type{ NO_TYPE, RESERVE, FUNC, INT, FLOAT };
// {{{ struct GeneralType{
struct GeneralType{
	Type t;
	union{
		float fval;
		int ival;
	};
}; // }}}
// {{{ struct Attr{
struct Attr{
	GeneralType gt;
}; // }}}

list<map<string, Attr> > symbol_list;

%}
// {{{ %union{
%union{
	GeneralType gt;
	GeneralType *gtp;
	string *str;
	Type type;
	list<string> *str_list;
	list<GeneralType> *gt_list;
} // }}}
// {{{ tokens
%token ERROR
%token PROGRAM VAR ENDVAR PROCEDURE BEGINN END
%token<type> TYPE
%token<str> NAME ENAME
%token<gt> VAL EVAL
%right ASSIGN
%left '+' '-'
%left '*' '/'
%right UMINUS
// }}}
// {{{ types
%type<str_list> var_list
%type<gt_list> arg_list nonempty_arg_list empty_arg_list
%type<gt> expr
%type<str> all_name
%type<gtp> lval
// }}}
%%
// {{{ start	: head proc_body emp_ln
start	: head proc_body { }
	| head proc_body error { yyerror("Illegal syntax at the end"); }
	| error { yyerror("It's Empty"); yyerrok; }
	;
// {{{ misc
emp_ln0	: emp_ln0 '\n'
	|
	;
emp_ln	: emp_ln '\n'
     	| '\n'
	;
all_name: NAME { $$=$1; }
	| PROGRAM { $$=new string(yytext); }
	| TYPE { $$=new string(yytext); }
	| VAR { $$=new string(yytext); }
	| ENDVAR { $$=new string(yytext); }
	| PROCEDURE { $$=new string(yytext); }
	| BEGINN { $$=new string(yytext); }
	| END { $$=new string(yytext); }
	| ENAME { yyerror(sprint("Invalid ID names: %s", $1->c_str())); $$=$1; }
	;
lval	: all_name{
		$$ = get_symbol_val(*$1);
		if($$ == NULL){
			yyerror(sprint("Using undeclared variables %s", $1->c_str()));
		}else{
			if(!isVar($$->t)){
				$$ = NULL;
				yyerror(sprint("Using invalid ID names %s", $1->c_str()));
			}
		}
	}
	;
// }}}
// {{{ head	: emp_ln PROGRAM all_name '\n'
head	: emp_ln0 PROGRAM all_name emp_ln
     	| error { yyerror("Expect PROGRAM [program_name]"); yyerrok; }
	;
// }}}
// {{{ proc_body	: var_region begin end
proc_body	: var_region proc_region begin state end { del_symbol_table(); }
	  	| var_region proc_region error { yyerror("Illegal syntax: no procedure body"); yyerrok; }
//	  	| error { yyerror("proc body"); yyerrok; }
		;
// {{{ var_region	: var_begin var_body var_end
var_region	: var_begin var_body var_end
	   	|
	   	| var_error_1 var_body var_end
	   	| var_end { yyerror("Expect VAR"); }
		;
var_error_1	: var_line error {  yyerror("Expect VAR"); yyerrok; }
	      	;
var_begin	: VAR '\n'
	   	| error { yyerror("Expect VAR"); yyerrok; }
		;
var_body: var_body var_line '\n'
    	| var_body '\n'
	| 
 	| var_body error '\n' { yyerror("Illegal syntax in declartion"); yyerrok; }
	;
var_line: var_list ':' TYPE ';'{
		for(list<string>::iterator iter=$1->begin(); iter!=$1->end(); iter++)
			add_symbol(*iter, $3);
	}
	| var_list ':' TYPE {
		yyerror("Expect ';'");
		for(list<string>::iterator iter=$1->begin(); iter!=$1->end(); iter++)
			add_symbol(*iter, $3);
	}
	| var_list error { yyerror("Illegal syntax in declartion"); yyerrok; }
	;
var_list: var_list ',' all_name {
		$1->push_back(*$3);
	}
	| var_list all_name {
		yyerror("Expect ','");
		$1->push_back(*$2);
	}
	| NAME {
		$$ = new list<string>;
		$$->push_back(*$1);
	}
	| ENAME {
		yyerror(sprint("Illegal ID names: %s", $1->c_str()));
		$$ = new list<string>;
		$$->push_back(*$1);
	}
	;
var_end	: ENDVAR emp_ln0
	| error { yyerror("Expect ENDVAR"); yyerrok; }
	;
// }}}
// {{{ proc_region	: proc_region proc
proc_region	: proc_region proc
		| proc_region '\n'
		|
		| proc_region error { yyerror("proc_region"); yyerrok; }
		;
proc	: proc_head proc_body
	| error { yyerror("proc"); yyerrok; }
     	;
proc_head	: PROCEDURE all_name emp_ln{
	  		string name=*$2+"_VOID";
	  		add_symbol(name, FUNC);
			new_symbol_table();
		}
		| error {
			yyerror("Expect PROCEDURE [program_name]");
			yyerrok;
			new_symbol_table();
		}
		;
// }}}
begin	: BEGINN '\n' { new_symbol_table(); }
	| error { yyerror("Expect BEGIN"); yyerrok; new_symbol_table(); }
	;
state   : state expr ';' '\n'
	| state expr '\n' { yyerror("Expect ';'"); }
	| state '\n'
	| state begin var_region state end
	| state var_region { yyerror("Illegal syntax: VAR must occurs at block head"); }
	|
	| state error '\n' { yyerror("Illegal syntax"); yyerrok; }
	;
expr    : expr '+' expr { if($1.t == NO_TYPE || $3.t == NO_TYPE){ $$.t = NO_TYPE; }else if($1.t != $3.t){ yyerror("Mismatched data type"); $$.t = NO_TYPE; }else{ $$.t = $1.t; } } // {{{
	| expr '-' expr { if($1.t == NO_TYPE || $3.t == NO_TYPE){ $$.t = NO_TYPE; }else if($1.t != $3.t){ yyerror("Mismatched data type"); $$.t = NO_TYPE; }else{ $$.t = $1.t; } }
	| expr '*' expr { if($1.t == NO_TYPE || $3.t == NO_TYPE){ $$.t = NO_TYPE; }else if($1.t != $3.t){ yyerror("Mismatched data type"); $$.t = NO_TYPE; }else{ $$.t = $1.t; } }
	| expr '/' expr { if($1.t == NO_TYPE || $3.t == NO_TYPE){ $$.t = NO_TYPE; }else if($1.t != $3.t){ yyerror("Mismatched data type"); $$.t = NO_TYPE; }else{ $$.t = $1.t; } }
	| '(' expr ')' { $$.t = $2.t; }
	| '-' expr  %prec UMINUS { $$.t = $2.t; }
	| lval ASSIGN expr { if($1 && $3.t != NO_TYPE && $1->t != $3.t){ yyerror("Mismatched data type"); $$.t = $3.t; }else{ $$.t = NO_TYPE; } }
	| all_name '(' arg_list ')' {
		string name=*$1;
		bool isError=false;
		for(list<GeneralType>::iterator iter=$3->begin(); iter!=$3->end(); iter++){
			if(iter->t == INT){
				name += "_INTEGER";
			}else if(iter->t == FLOAT){
				name += "_REAL";
			}else{
				isError = true;
				break;
			}
		}
		if($3->empty())
			name += "_VOID";
		if(isError || !get_symbol_val(name)){
			yyerror("Undefined procedure or wrong parameters");
		}
	}
	| lval { if($1) $$ = *$1; else $$.t = NO_TYPE; }
	| VAL { $$ = $1; }
	| EVAL { $$.t = NO_TYPE; yyerror("Invalid constant"); }
	| error { yyerror("Illegal syntax"); yyerrok; $$.t = NO_TYPE; }
	; // }}}
end	: END emp_ln0 { del_symbol_table(); }
	| error { yyerror("Expect END"); yyerrok; del_symbol_table(); }
	;
arg_list: nonempty_arg_list // {{{
	| empty_arg_list
	;
nonempty_arg_list	: nonempty_arg_list ',' lval{
				$$->push_back(*$3);
			}
			| nonempty_arg_list ',' VAL{
				$3.t = NO_TYPE;
				$$->push_back($3);
			}
			| nonempty_arg_list lval{
				yyerror("Expect ','");
				$$->push_back(*$2);
			}
			| nonempty_arg_list VAL{
				yyerror("Expect ','");
				$2.t = NO_TYPE;
				$$->push_back($2);
			}
			| lval{
				$$ = new list<GeneralType>;
				$$->push_back(*$1);
			}
			| VAL{
				$$ = new list<GeneralType>;
				$1.t = NO_TYPE;
				$$->push_back($1);
			}
			;
empty_arg_list: { $$ = new list<GeneralType>; }
	      ;
// }}}
// }}}
// }}}
%%
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
	if(t == FLOAT || t == INT)
		return true;
	return false;
} // }}}
// {{{ void init_symbol()
void init_symbol()
{
	Attr reserve;
	reserve.gt.t = RESERVE;

	symbol_list.back()["PROGRAM"] = reserve;
	symbol_list.back()["VAR"] = reserve;
	symbol_list.back()["BEGIN"] = reserve;
	symbol_list.back()["END"] = reserve;
	symbol_list.back()["REAL"] = reserve;
	symbol_list.back()["INTEGER"] = reserve;
} // }}}
// {{{ void init_function()
void init_function()
{
	Attr func;
	func.gt.t = FUNC;

	symbol_list.back()["READ_INTEGER"] = func;
	symbol_list.back()["READ_REAL"] = func;
	symbol_list.back()["WRITE_INTEGER"] = func;
	symbol_list.back()["WRITE_REAL"] = func;
	symbol_list.back()["WRITESP_VOID"] = func;
	symbol_list.back()["WRITELN_VOID"] = func;
} // }}}
// {{{ void new_symbol_table()
void new_symbol_table()
{
	symbol_list.push_back(map<string,Attr>());
} // }}}
// {{{ void del_symbol_table()
void del_symbol_table()
{
	symbol_list.pop_back();
} // }}}
// {{{ GeneralType *get_symbol_val(string name)
GeneralType *get_symbol_val(string name)
{
	for(list<map<string,Attr> >::reverse_iterator riter=symbol_list.rbegin(); riter!=symbol_list.rend(); riter++){
		if(riter->find(name) != riter->end()){
			return &((*riter)[name].gt);
		}
	}
	return NULL;
} // }}}
// {{{ void add_symbol(string name, Type t)
void add_symbol(string name, Type t)
{
	GeneralType *gtp;
	if(gtp=get_symbol_val(name)){
		if(gtp->t == RESERVE)
			yyerror(sprint("Reserved ID names: %s", name.c_str()));
		else if(t == FUNC)
			yyerror(sprint("Duplicatedly declared procedures %s", name.c_str()));
		else
			yyerror(sprint("Duplicatedly declared variables %s", name.c_str()));
	}
	Attr a;
	a.gt.t = t;
	symbol_list.back()[name] = a;
} // }}}
// {{{ int main(int argc, char *argv[])
int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("Usage: %s filename\n", argv[0]);
		exit(0);
	}
	new_symbol_table();
	init_symbol();
	init_function();
        yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		exit(-1);
	}
	yyparse();
	if(isNoError)
		printf("Compile successfully.\n");
	return 0;
} // }}}
