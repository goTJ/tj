// vim:ft=yacc:foldmethod=marker
%{
#include <cstdio>
#include <string>
#include <list>
#include "inc.h"
#include "library.h"
#include "emit.h"
#include "SymbolTable.h"
#include "OneSymbolTableByAVLTree.h"

using std::string;
using std::list;
extern int _lineno;
extern string _linebuf;
bool isNoError=true;
FILE *cmmOut;
list<ProcInfo> procInfoStack;
ProcInfo procInfo;
int procNum=0;

// {{{ extern "C"
	int yyparse(void);
	int yylex(void);  
	int yyerror(char *msg)
	{   
		isNoError = false;
		if(!strcmp(msg, "syntax error"))
			return 0;
		printf("Find error in line %d: %s\n", _lineno, msg);
		printf("%s\n", _linebuf.c_str());
//		printf("%*s\n", _linepos, "^");
	}
 // }}}

SymbolTable *symbolTable=new OneSymbolTableByAVLTree();
RegManager regM;

%}
%union{ /* {{{ */
	string *str;
	GeneralType gt;
	list<string> *str_list;
	list<GeneralType> *gt_list;
	char bop;
	int num;
	int regNum;
	int labelNum;
	struct{
		int label1;
		int label2;
	} labels;
	struct{
		int label;
		int label2;
		GeneralType gt;
	} labGt;
} /* }}} */
// {{{ tokens and types
%token ERROR
%token PROGRAM CONST ENDCONST TYPE ENDTYPE VAR ENDVAR PROCEDURE FUNCTION _BEGIN END ARRAY DOTDOT OF IF THEN ELSE ENDIF FOR TO DOWNTO DO WHILE CASE ENDCASE OTHERWISE
%token<num> TO
%token<str> NAME STRING
%token<gt> VAL
%token<bop> CMP
// boolean expression rules
%left OR
%left XOR
%left AND
%left NOT
// expression rules
%right SWAP
%right ASSIGN
%left '+' '-'
%left '*' '/' MOD
%right UMINUS

%type<str> all_name proc_head_head func_head_head
%type<str_list> var_list
%type<gt> expr
%type<gt> const_val var_array_list var_type alist array boexp
%type<bop> boop
%type<labelNum> if_head while_head
%type<labels> if_else for_range
%type<labGt> while_do case_head case_list case_list_head case_head
%type<gt_list> arg_list nonempty_arg_list arg_line nonempty_arg_line var_line
// }}}
%%
start	: head proc_body // {{{
	| head proc_body error { yyerror("Illegal syntax at the end."); }
	| error { yyerror("It's not pascal-like file."); yyerrok; }
	; // }}}
// {{{ misc
emp_ln0	: emp_ln0 '\n'
	|
	;
emp_ln	: emp_ln '\n'
     	| '\n'
	;
all_name: NAME { $$=$1; }
	| PROGRAM { $$=new string("PROGRAM"); }
	;
const_val	: VAL { $$ = $1; }
	  	| '-' VAL {
			$$ = $2;
			if($$.t == INT)
				$$.ival = -$$.ival;
			else
				$$.fval = -$$.fval;
		}
		;
// }}}
head	: emp_ln0 PROGRAM all_name emp_ln { // {{{
     		fprintf(cmmOut, "#define MAX__S 992106\n");
     		fprintf(cmmOut, "#include \"cmm.c\"\n");
		newProc("main", "main", 0);
		symbolTable->newLayer();
     	}
     	| error { yyerror("Expect PROGRAM [program_name]"); yyerrok; }
	; // }}}
proc_body	: def_region state_region { // {{{
			procInfo.tstart -= symbolTable->getLayerSize();
	  		symbolTable->delLayer();
			emitPROCEND();
			if(!procInfoStack.empty()){
				procInfo = procInfoStack.back();
				procInfoStack.pop_back();
			}
		}
	  	;
def_region	: const_region type_region var_region proc_region {
	  		fprintf(cmmOut, "%s()\n{\n", procInfo.pname.c_str());
			if(procInfo.pname == "main")
				emitINIT(GLOBAL_SIZE);
			emitSETSP(procInfo.tstart);
		}
	  	;
// }}}
// {{{ const_region
const_region	: const_begin const_body const_end
	     	|
		;
const_begin	: CONST '\n'
	    	;
const_body	: const_body const_line '\n'
	   	| const_body '\n'
		|
		| const_body error '\n' { yyerror("Illegal syntax in const definition"); yyerrok; }
	   	;
const_line	: all_name '=' all_name ';' {
	   		GeneralType *gtp;
	   		gtp = symbolTable->getVar(*$3);
			if(gtp->s == _CONST){
				symbolTable->addVar(*$1, *gtp);
			}else{
				yyerror("Mismatched data type");
				yyerrok;
			}
		}
	   	| all_name '=' VAL ';' {
			$3.size = 0;
	   		symbolTable->addVar(*$1, $3);
		}
	   	;
const_end	: ENDCONST emp_ln
	  	;
// }}}
// {{{ type_region
type_region	: type_begin type_body type_end
	    	|
		;
type_begin	: TYPE '\n'
	    	;
type_body	: type_body type_line '\n'
	   	| type_body '\n'
		|
		| type_body error '\n' { yyerror("Illegal syntax in type definition"); yyerrok; }
	   	;
type_line	: all_name '=' var_type ';' {
	  		GeneralType gt;
			gt.t = TYPEDEF;
			gt.typeGtp = new GeneralType();
			*(gt.typeGtp) = $3;
			gt.size = 0;
			symbolTable->addVar(*$1, gt);
		}
	   	;
type_end	: ENDTYPE emp_ln
	  	;
// }}}
// {{{ var_region
var_region	: var_begin var_body var_end
	   	|
		;
var_begin	: VAR '\n'
	    	;
var_body	: var_body var_line ';' '\n'
	   	| var_body '\n'
		|
		| var_body error '\n' { yyerror("Illegal syntax in declartion"); yyerrok; }
	   	;
var_line	: var_list ':' var_type {
	 		$$ = new list<GeneralType>;
			$3.s = LOCAL;
			$3.depth = 0;
			int size=1;
			int offset=0;
			for(list<Dimension >::reverse_iterator riter=$3.arrayBound->rbegin(); riter!=$3.arrayBound->rend(); riter++){
				riter->size = size;
				offset += -(riter->l)*size;
				size *= (riter->u-riter->l+1);
			}
			for(list<string>::iterator iter=$1->begin(); iter!=$1->end(); iter++){
				$3.offset = procInfo.tstart+offset-procInfo.lstart;
				$3.oriOffset = procInfo.tstart-procInfo.lstart;
				$3.size = size;
				symbolTable->addVar(*iter, $3);
				procInfo.tstart += size;
				$$->push_back($3);
			}
		}
		| var_list error { yyerror("Illegal syntax in declartion"); yyerrok; }
	   	;
var_type	: all_name {
	 		GeneralType *gtp;
			if(*$1 == "INTEGER"){
				$$.t = INT;
				$$.arrayBound = new list<Dimension >();
			}else if(*$1 == "REAL"){
				$$.t = FLOAT;
				$$.arrayBound = new list<Dimension >();
			}else if(!(gtp=symbolTable->getVar(*$1))){
				yyerror("Unknown type");
			}else if(gtp->t != TYPEDEF){
				yyerror("It's not type");
			}else{
				$$ = *(gtp->typeGtp);
			}
		}
	 	| ARRAY '[' var_array_list ']' OF all_name {
			$$ = $3;
			if(*$6 == "INTEGER"){
				$$.t = INT;
			}else if(*$6 == "REAL"){
				$$.t = FLOAT;
			}
		}
	 	;
var_array_list	: const_val DOTDOT const_val {
	       		if($1.t != INT || $3.t != INT || $1.ival > $3.ival){
				yyerror("Wrong array index");
			}else{
				$$.arrayBound = new list<Dimension >();
				Dimension d;
				d.l = $1.ival;
				d.u = $3.ival;
				$$.arrayBound->push_back(d);
			}
	       	}
	       	| var_array_list ',' const_val DOTDOT const_val {
	       		if($3.t != INT || $5.t != INT || $3.ival > $5.ival){
				yyerror("Wrong array index");
			}else{
				$$.arrayBound = $1.arrayBound;
				Dimension d;
				d.l = $3.ival;
				d.u = $5.ival;
				$$.arrayBound->push_back(d);
			}
		}
	       	;
var_list	: var_list ',' all_name {
	 		$1->push_back(*$3);
		}
	   	| NAME {
			$$ = new list<string>;
			$$->push_back(*$1);
		}
	   	;
var_end		: ENDVAR emp_ln
	  	;
// }}}
// {{{ proc_region
proc_region	: proc_region proc
		| proc_region '\n'
		|
		| proc_region error { yyerror("proc_region"); yyerrok; }
		;
proc	: proc_head proc_body
     	;
proc_head	: proc_head_head '(' arg_line ')' ';' emp_ln {
	  		GeneralType *gtp=symbolTable->getVar(*$1);
			gtp->arg_list = $3;
			gtp->retVal = NULL;
			symbolTable->addVar(*$1, *gtp);
		}
		| func_head_head '(' arg_line ')' ':' var_type ';' emp_ln {
	  		GeneralType *gtp=symbolTable->getVar(*$1);
			gtp->arg_list = $3;
			gtp->retVal = new GeneralType($6);
			gtp->retVal->offset = -2;
			gtp->retVal->s = LOCAL;
			symbolTable->addVar(*$1, *gtp);
			gtp = symbolTable->getVar(*$1);
		}
		| error
		;
proc_head_head	: PROCEDURE all_name {
	       		GeneralType gt;
			gt.t = FUNC;
			gt.s = LOCAL;
			procInfoStack.push_back(procInfo);
			char buf[20];
			sprintf(buf, "_%d", procNum);
			gt.pname = new string(*$2+buf);
			newProc(*$2, *$2+buf, 2);
			procNum++;
	       		symbolTable->addVar(*$2, gt);
			symbolTable->newLayer();
			$$ = $2;
		}
func_head_head	: FUNCTION all_name {
	       		GeneralType gt;
			gt.t = FUNC;
			gt.s = LOCAL;
			procInfoStack.push_back(procInfo);
			char buf[20];
			sprintf(buf, "_%d", procNum);
			gt.pname = new string(*$2+buf);
			newProc(*$2, *$2+buf, 2);
			procNum++;
	       		symbolTable->addVar(*$2, gt);
			symbolTable->newLayer();
			$$ = $2;
	       	}
		;
arg_line: nonempty_arg_line { $$ = $1; }
	| { $$ = new list<GeneralType>; }
	;
nonempty_arg_line	: nonempty_arg_line ';' var_line {
				$$->insert($$->end(), $3->begin(), $3->end());
			}
			| var_line {
				$$ = new list<GeneralType>;
				$$->insert($$->end(), $1->begin(), $1->end());
			}
			;
// }}}
// {{{ state region
state_region	: begin var_region state end
	     	| error { yyerror("state region error"); }
	     	;
one_state	: expr ';' '\n' { freetemp($1); }
		| expr '\n' { yyerror("Expect ';'"); freetemp($1); }
		| if_region ';' '\n'
		| for_region
		| while_region
		| case_region ';' '\n'
		| state_region
		| '\n' one_state
	  	;
state   : state one_state
	| state '\n'
	| state var_region { yyerror("Illegal syntax: VAR must occurs at block head"); }
	|
	| state error '\n' { yyerror("Illegal syntax"); yyerrok; }
	;
begin	: _BEGIN '\n' { symbolTable->newLayer(); }
	| error { yyerror("Expect BEGIN"); yyerrok; symbolTable->newLayer(); }
	;
end	: END emp_ln0 { procInfo.tstart -= symbolTable->getLayerSize(); symbolTable->delLayer(); }
	| error { yyerror("Expect END"); yyerrok; procInfo.tstart -= symbolTable->getLayerSize(); symbolTable->delLayer(); }
	;
// }}}
if_region	: if_head state if_else state ENDIF { // {{{
			emitJUMP($3.label1);
			emitLABEL($1);
			emitJUMP($3.label2);
			emitLABEL($3.label1);
		}
		| if_head state ENDIF {
			emitLABEL($1);
		}
		| error { yyerror("if-region error"); yyerrok; }
		;
if_head	: IF boexp '\n' THEN '\n' {
		$$ = newLabel(); // labelFalse
		emitIFHEAD($2, $$);
		freetemp($2);
	}
	;
if_else	: ELSE '\n' {
		$$.label1 = newLabel(); // labelEnd
		$$.label2 = newLabel(); // labelElse
		emitJUMP($$.label1);
		emitLABEL($$.label2);
	}
	; // }}}
for_region	: FOR for_range DO '\n' one_state { // {{{
	   		emitJUMP($2.label1);
			emitLABEL($2.label2);
		}
	   	;
for_range	: all_name ASSIGN expr TO expr {
			GeneralType *gtp;
			if(!(gtp=symbolTable->getVar(*$1))){
				yyerror("Undeclared");
			}else if(gtp->t != INT || $3.t != INT || $5.t != INT){
				yyerror("Data type must be INTEGER");
			}else{
				int labelEnd=newLabel();
				int labelBegin=newLabel();
				int labelLoop=newLabel();
				GeneralType constGt;
				constGt.t = INT; constGt.s = _CONST;
				GeneralType tempGt;
				tempGt = newtemp(INT);

				emitASSIGN('-', tempGt, $3, $5);
				emitCOND(($4==1)?'L':'G', tempGt, $3, $5);
				emitIFHEAD(tempGt, labelEnd);
				emitASSIGN('=', *gtp, $3);
				emitJUMP(labelBegin);
				emitLABEL(labelLoop);
				emitCOND('N', tempGt, *gtp, $5);
				emitIFHEAD(tempGt, labelEnd);
				constGt.ival = 1;
				emitASSIGN(($4==1)?'+':'-', *gtp, constGt);
				emitLABEL(labelBegin);
				$$.label1 = labelLoop;
				$$.label2 = labelEnd;
			}
		}
		; // }}}
while_region	: while_head while_do one_state { // {{{
	     		emitJUMP($1);
			emitLABEL($2.label);
			freetemp($2.gt);
		}
	     	;
while_head	: WHILE {
	   		$$ = newLabel(); // labelBegin
			emitLABEL($$);
		}
	   	;
while_do	: boexp DO '\n' {
	 		$$.label = newLabel(); // labelFalse
			$$.gt = $1;
			emitIFHEAD($1, $$.label);
	 	}
	 	; // }}}
case_region	: case_list case_end { // {{{
	    		emitLABEL($1.label);
	    	}
	    	;
case_list	: case_list_head one_state {
	  		$$ = $1;
			emitJUMP($1.label);
			emitLABEL($1.label2);
	  	}
	  	| case_head {
			$$ = $1;
		}
	  	;
case_list_head	: case_list const_val ':' {
	  		if($1.gt.t != INT){
				yyerror("Data type must be INTEGER");
			}else{
				$$ = $1;
				$$.label2 = newLabel(); // labelFalse
				GeneralType tempGt;
				tempGt = newtemp(INT);
				emitCOND('=', tempGt, $$.gt, $2);
				emitIFHEAD(tempGt, $$.label2);
				freetemp(tempGt);
			}
	       	}
	       	;
case_head	: CASE expr OF '\n' {
	  		if($2.t != INT){
				yyerror("Data type must be INTEGER");
			}else{
				$$.gt = $2;
				$$.label = newLabel(); // labelEnd
			}
	  	}
	  	;
case_end	: ENDCASE
	 	| OTHERWISE ':' one_state ENDCASE
	 	; // }}}
expr	: expr '+' expr { // {{{
     		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = newtemp($1.t);
			emitASSIGN('+', $$, $1, $3);
			freetemp($1);
			freetemp($3);
		}
	}
	| expr '-' expr {
     		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = newtemp($1.t);
			emitASSIGN('-', $$, $1, $3);
			freetemp($1);
			freetemp($3);
		}
	}
	| expr '*' expr {
     		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = newtemp($1.t);
			emitASSIGN('*', $$, $1, $3);
			freetemp($1);
			freetemp($3);
		}
	}
	| expr '/' expr {
     		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = newtemp($1.t);
			emitASSIGN('/', $$, $1, $3);
			freetemp($1);
			freetemp($3);
		}
	}
	| expr MOD expr {
     		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else if($1.t != INT){
			yyerror("Data type must be INTEGER");
		}else{
			$$ = newtemp($1.t);
			emitASSIGN('%', $$, $1, $3);
			freetemp($1);
			freetemp($3);
		}
	} // }}}
	| array SWAP array { // {{{
		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			GeneralType tempGt;
			tempGt = newtemp($1.t);
			GeneralType temp2Gt;
			temp2Gt = newtemp($1.t);
			emitASSIGN('r', tempGt, $1, *($1.tempGtp));
			emitASSIGN('r', temp2Gt, $3, *($3.tempGtp));
			emitASSIGN('l', $1, *($1.tempGtp), temp2Gt);
			emitASSIGN('l', $3, *($3.tempGtp), tempGt);
		}
	}
	| all_name SWAP all_name {
		GeneralType *gtp1, *gtp2;
		if(!(gtp1=symbolTable->getVar(*$1))){
			yyerror("Wrong variable");
		}else if(!(gtp2=symbolTable->getVar(*$3))){
			yyerror("Wrong variable");
		}else if(gtp1->t != gtp2->t){
			yyerror("Mismatched data type");
		}else if(gtp1->t != INT && gtp1->t != FLOAT){
			yyerror("Wrong variable");
		}else if(!compareArray(gtp1->arrayBound, gtp2->arrayBound)){
			yyerror("Mismatched data type");
		}else{
			GeneralType tempGt;
			tempGt = newtemp(gtp1->t);
			GeneralType temp2Gt;
			tempGt = newtemp(gtp1->t);
			GeneralType addrGt;
			addrGt = newtemp(INT);
			GeneralType condGt;
			condGt = newtemp(INT);
			GeneralType countGt;
			countGt = newtemp(INT);
			GeneralType constGt;
			constGt.t = INT; constGt.s = _CONST;

			constGt.ival = 0;
			emitASSIGN('=', tempGt, constGt);
			constGt.ival = gtp1->size;
			int labelBegin=newLabel();
			int labelEnd=newLabel();
			emitLABEL(labelBegin);
			emitCOND('<', condGt, countGt, constGt);
			emitIFHEAD(condGt, labelEnd);
				constGt.ival = gtp1->oriOffset;
				emitASSIGN('+', addrGt, countGt, constGt);
				emitASSIGN('r', tempGt, *gtp1, addrGt);
				constGt.ival = gtp2->oriOffset;
				emitASSIGN('+', addrGt, countGt, constGt);
				emitASSIGN('r', temp2Gt, *gtp2, addrGt);
				emitASSIGN('l', *gtp2, addrGt, tempGt);
				constGt.ival = gtp1->oriOffset;
				emitASSIGN('+', addrGt, countGt, constGt);
				emitASSIGN('l', *gtp1, addrGt, temp2Gt);
				constGt.ival = 1;
				emitASSIGN('+', countGt, constGt);
			emitJUMP(labelBegin);
			emitLABEL(labelEnd);
			freetemp(tempGt); freetemp(temp2Gt); freetemp(addrGt); freetemp(condGt); freetemp(countGt);
		}
	} // }}}
	| '-' expr  %prec UMINUS { // {{{
		$$ = newtemp($2.t);
		emitASSIGN('m', $$, $2);
	}
	| '(' expr ')' {
		$$ = $2;
	}
	| all_name ASSIGN expr {
		GeneralType *gtp;
		if(!(gtp=symbolTable->getVar(*$1))){
			yyerror("Undeclared");
		}else if(gtp->t == FUNC && *$1==procInfo.name){
			if(gtp->retVal->t != $3.t){
				yyerror("Mismatched data type");
			}else{
				$$ = *(gtp->retVal);
				emitASSIGN('=', *(gtp->retVal), $3);
				freetemp($3);
			}
		}else if(gtp->t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = *gtp;
			emitASSIGN('=', *gtp, $3);
			freetemp($3);
		}
	}
	| array ASSIGN expr {
		if($1.t != $3.t){
			yyerror("Mismatched data type");
		}else{
			$$ = $1;
			emitASSIGN('l', $1, *($1.tempGtp), $3);
			freetemp($3);
		}
	} // }}}
     	| array { // {{{
		$$ = newtemp($1.t);
		emitASSIGN('r', $$, $1, *($1.tempGtp));
	}
	| all_name '(' arg_list ')' {
		GeneralType *gtp;
		if(*$1 == "READ"){ // {{{
			for(list<GeneralType>::iterator iter=$3->begin(); iter!=$3->end(); iter++){
				if(iter->t != INT && iter->t != FLOAT){
					yyerror("Wrong argument type");
				}else if(iter->s == _CONST || iter->s == TEMP){
					yyerror("Wrong argument: not lval");
				}else{
					emitREAD(*iter);
				}
				freetemp(*iter);
			}
		}else if(*$1 == "WRITE"){
			bool isFirst=true;
			for(list<GeneralType>::iterator iter=$3->begin(); iter!=$3->end(); iter++){
				if(iter->t != INT && iter->t != FLOAT){
					yyerror("Wrong argument type");
				}else{
					if(!isFirst)
						emitWRITE(" ");
					emitWRITE(*iter);
					isFirst = false;
				}
			}
		}else if(*$1 == "WRITESP"){
			if($3->size()){
				yyerror("Wrong argument list");
			}else{
				emitWRITE(" ");
			}
		}else if(*$1 == "WRITELN"){
			if($3->size()){
				yyerror("Wrong argument list");
			}else{
				emitWRITE("\\n");
			} // }}}
		}else if(!(gtp=symbolTable->getVar(*$1))){
			yyerror("Undeclare procedure/function");
		}else if(gtp->t != FUNC){
			yyerror("It's not procedure/function");
		}else if(!compareArg(gtp->arg_list, $3)){
			yyerror("Wrong arguments");
		}else{
			for(list<GeneralType>::iterator iter1=gtp->arg_list->begin(),iter2=$3->begin(); iter1!=gtp->arg_list->end()&&iter2!=$3->end(); iter1++,iter2++){
				iter1->s = TEMP;
				iter1->offset += 2+procInfo.temp.size();
				emitASSIGN('=', *iter1, *iter2);
				iter1->offset -= 2+procInfo.temp.size();
				iter1->s = LOCAL;
			}
			emitFUNC(*(gtp->pname));
			if(gtp->retVal){
				$$ = newtemp(gtp->retVal->t);
				gtp->retVal->s = TEMP;
				gtp->retVal->offset += 2+procInfo.temp.size()-1;
				emitASSIGN('=', $$, *(gtp->retVal));
				gtp->retVal->offset -= 2+procInfo.temp.size()-1;
				gtp->retVal->s = LOCAL;
			}
		}
		for(list<GeneralType>::iterator iter=$3->begin(); iter!=$3->end(); iter++)
			freetemp(*iter);
	}
	| all_name '(' STRING ')' {
		if(*$1 == "WRITESTRING"){
			emitWRITE($3->c_str());
		}else{
			yyerror("Unknow procedure or wrong parameter");
		}
	}
     	| all_name {
		GeneralType *gtp;
		if(!(gtp=symbolTable->getVar(*$1))){
			yyerror("Undeclared");
		}else{
			$$ = *gtp;
		}
	}
	| VAL {
		$$ = $1;
	}
     	; // }}}
arg_list: nonempty_arg_list { $$ = $1; } // {{{
	| { $$ = new list<GeneralType>; };
	;
nonempty_arg_list	: arg_list ',' expr {
				$$->push_back($3);
			}
			| expr {
				$$ = new list<GeneralType>;
				$$->push_back($1);
			}
			; // }}}
array	: alist ']' { // {{{
      		$$ = $1;
	}
      	;
alist	: alist ',' expr {
		if($1.arrayBound->empty()){
			yyerror("Array index error");
		}else{
			GeneralType constGt;
			constGt.t = INT; constGt.s = _CONST;
			GeneralType tempGt;
			tempGt = newtemp(INT);
			$$ = $1;
			$$.arrayBound = new list<Dimension>(*($1.arrayBound));

			constGt.ival = $$.arrayBound->front().size;
			emitASSIGN('*', tempGt, $3, constGt);
			emitASSIGN('+', *($$.tempGtp), tempGt);

			$$.arrayBound->pop_front();
			freetemp(tempGt);
		}
		freetemp($3);
      	}
      	| all_name '[' expr {
		GeneralType *gtp;
		if(!(gtp=symbolTable->getVar(*$1))){
			yyerror("Undeclared");
		}else if(!(gtp->arrayBound) || gtp->arrayBound->empty()){
			yyerror("Variable is not array or dimension error");
		}else{
			GeneralType constGt;
			constGt.t = INT; constGt.s = _CONST;
			GeneralType tempGt;
			tempGt = newtemp(INT);
			$$ = *gtp;
			$$.arrayBound = new list<Dimension>(*(gtp->arrayBound));
			$$.tempGtp = new GeneralType();
			*($$.tempGtp) = newtemp(INT);
			constGt.ival = $$.offset;
			emitASSIGN('=', *($$.tempGtp), constGt);

			constGt.ival = $$.arrayBound->front().size;
			emitASSIGN('*', tempGt, $3, constGt);
			emitASSIGN('+', *($$.tempGtp), tempGt);

			$$.arrayBound->pop_front();
			freetemp(tempGt);
		}
		freetemp($3);
	}
	; // }}}
boexp	: '(' boexp ')' { // {{{
      		$$ = $2;
	}
      	| expr boop expr {
		$$ = newtemp(INT);
		emitCOND($2, $$, $1, $3);
	}
      	| boexp OR boexp {
		emitASSIGN('|', $1, $3);
		freetemp($3);
		$$ = $1;
	}
      	| boexp XOR boexp {
		emitASSIGN('^', $1, $3);
		freetemp($3);
		$$ = $1;
	}
      	| boexp AND boexp {
		emitASSIGN('&', $1, $3);
		freetemp($3);
		$$ = $1;
	}
      	| NOT boexp {
		emitASSIGN('!', $2);
		$$ = $2;
	}
	| error { yyerror("Boolean expression error"); }
      	;
boop	: '=' { $$ = '='; }
     	| '<' { $$ = '<'; }
     	| '>' { $$ = '>'; }
     	| CMP { $$ = $1; }
	; // }}}
%%
#include "lex.yy.c"

bool compareArray(list<Dimension> *a, list<Dimension> *b) // {{{
{
	if(!a){
		if(b->size() != 1)
			return false;
		return true;
	}
	if(!b){
		if(a->size() != 1)
			return false;
		return true;
	}
	if(a->size() != b->size())
		return false;
	for(list<Dimension>::iterator iter1=a->begin(),iter2=b->begin(); iter1!=a->end()&&iter2!=b->end(); iter1++,iter2++){
		if(iter1->l != iter2->l)
			return false;
		if(iter1->u != iter2->u)
			return false;
		if(iter1->size != iter2->size)
			return false;
	}
	return true;
} // }}}
bool compareArg(list<GeneralType> *a, list<GeneralType> *b) // {{{
{
	if(a->size() != b->size())
		return false;
	for(list<GeneralType>::iterator iter1=a->begin(),iter2=b->begin(); iter1!=a->end()&&iter2!=b->end(); iter1++,iter2++){
		if(iter1->t != iter2->t)
			return false;
			/*
		if(!compareArray(iter1->arrayBound, iter2->arrayBound))
			return false;
			*/
	}
	return true;
} // }}}
void newProc(string name, string pname, int offset) // {{{
{
	procInfo.name = name;
	procInfo.pname = pname;
	procInfo.lstart = offset;
	procInfo.tstart = offset;
} // }}}
GeneralType newtemp(Type t) // {{{
{
	GeneralType gt;
	gt.t = t;
	gt.s = TEMP;
	gt.size = 1;
	gt.arrayBound = new list<Dimension>;
	for(int i=0; i<procInfo.temp.size(); i++){
		if(!procInfo.temp[i]){
			procInfo.temp[i] = true;
//			printf("go newtemp: %d\n", i);
			gt.offset = gt.oriOffset = i;
			return gt;
		}
	}
	emitSETSP(1);
	procInfo.temp.push_back(true);
//	printf("go newtemp: %d\n", procInfo.temp.size()-1);
	gt.offset = gt.oriOffset = procInfo.temp.size()-1;
	return gt;
} // }}}
void freetemp(GeneralType gt) // {{{
{
	if(gt.s == TEMP){
//		printf("go freetemp: %d\n", gt.offset);
		delete gt.arrayBound;
		if(gt.offset == procInfo.temp.size()-1){
			int i;
			for(i=procInfo.temp.size()-2; i>=0; i--){
				if(procInfo.temp[i])
					break;
			}
			emitSETSP(i-procInfo.temp.size()+1);
			procInfo.temp.resize(i+1);
		}else{
			procInfo.temp[gt.offset] = false;
		}
	}
} // }}}
void initSymbolTable() // {{{
{
	GeneralType reserveGT;
	reserveGT.t = RESERVE;
	char *reserveWord[] = { "PROGRAM", "VAR", "ENDVAR", "CONST", "ENDCONST", "TYPE", "ENDTYPE",
				"BEGIN", "END" };
	int reserveNum = 9;
	GeneralType funcGT;
	funcGT.t = RESERVE;
	char *funcWord[] = { "READ", "WRITE", "WRITESP", "WRITELN", "WRITESTRING" };
	int funcNum = 5;

	symbolTable->newLayer();
	for(int i=0; i<reserveNum; i++)
		symbolTable->addVar(reserveWord[i], reserveGT);
	for(int i=0; i<funcNum; i++)
		symbolTable->addVar(funcWord[i], funcGT);
} // }}}
int main(int argc, char *argv[]) // {{{
{
	if(argc < 2){
		printf("Usage: %s filename\n", argv[0]);
		exit(0);
	}
        yyin = fopen(argv[1], "r");
	if(yyin == NULL){
		fprintf(stderr, "Can't open file %s\n", argv[1]);
		exit(-1);
	}
	cmmOut=fopen("cmmcode.c", "w");
	initSymbolTable();
	yyparse();
	fclose(cmmOut);
	if(isNoError)
		printf("Compile successfully.\n");
	return 0;
} // }}}
