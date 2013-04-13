// vim:ft=cpp:foldmethod=marker
#include <stdio.h>
#include "emit.h"
#include "library.h"

extern FILE* cmmOut;
extern ProcInfo procInfo;
extern bool isNoError;

void loadAddr(GeneralType a, int regNum) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(a.t==INT)?"I":"F";
        const char *prefixF=(a.t==INT)?"":"F";

        switch(a.s){
                case LOCAL:
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.lstart+a.offset);
                        fprintf(cmmOut, "%s__%d = %sAVAL__S(I__9);\n", prefixV, regNum, prefixF);
                        break;
                case TEMP:
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.tstart+a.offset);
                        fprintf(cmmOut, "%s__%d = %sAVAL__S(I__9);\n", prefixV, regNum, prefixF);
                        break;
                case _CONST:
                        if(a.t == INT)
                                fprintf(cmmOut, "%s__%d = %d;\n", prefixV, regNum, a.ival);
                        else
                                fprintf(cmmOut, "%s__%d = %f;\n", prefixV, regNum, a.fval);
        }
} // }}}
void loadAddr(GeneralType a, GeneralType b, int regNum) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(a.t==INT)?"I":"F";
        const char *prefixF=(a.t==INT)?"":"F";

        switch(a.s){
                case LOCAL:
                        loadAddr(b, 8);
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.lstart);
                        fprintf(cmmOut, "I__9 = I__9+I__8;\n");
                        fprintf(cmmOut, "%s__%d = %sAVAL__S(I__9);\n", prefixV, regNum, prefixF);
                        break;
                case TEMP:
                        loadAddr(b, 8);
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.tstart);
                        fprintf(cmmOut, "I__9 = I__9+I__8;\n");
                        fprintf(cmmOut, "%s__%d = %sAVAL__S(I__9);\n", prefixV, regNum, prefixF);
                        break;
        }
} // }}}
void saveAddr(GeneralType a, int regNum) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(a.t==INT)?"I":"F";
        const char *prefixF=(a.t==INT)?"":"F";

        switch(a.s){
                case LOCAL:
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.lstart+a.offset);
                        fprintf(cmmOut, "%sASSET__S(I__9, %s__%d);\n", prefixF, prefixV, regNum);
                        break;
                case TEMP:
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.tstart+a.offset);
                        fprintf(cmmOut, "%sASSET__S(I__9, %s__%d);\n", prefixF, prefixV, regNum);
                        break;
        }
} // }}}
void saveAddr(GeneralType a, GeneralType b, int regNum) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(a.t==INT)?"I":"F";
        const char *prefixF=(a.t==INT)?"":"F";

        switch(a.s){
                case LOCAL:
                        loadAddr(b, 8);
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.lstart);
                        fprintf(cmmOut, "I__9 = I__9+I__8;\n");
                        fprintf(cmmOut, "%sASSET__S(I__9, %s__%d);\n", prefixF, prefixV, regNum);
                        break;
                case TEMP:
                        loadAddr(b, 1);
                        fprintf(cmmOut, "I__9 = I__0+%d;\n", procInfo.tstart);
                        fprintf(cmmOut, "I__9 = I__9+I__8;\n", regNum);
                        fprintf(cmmOut, "%sASSET__S(I__9, %s__%d);\n", prefixF, prefixV, regNum);
                        break;
        }
} // }}}

void emitINIT(int globalSize) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "INIT__S();\nSETSP__S(%d);\nI__0=%d;\n", globalSize, globalSize);
} // }}}
void emitSETSP(int offset) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "SETSP__S(%d);\n", offset);
} // }}}

void emitASSIGN(char op, GeneralType a, GeneralType b, GeneralType c) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(b.t==INT)?"I":"F";
        const char *prefixF=(b.t==INT)?"":"F";
        const char *zero=(a.t==INT)?"0":"0.0";

        switch(op){
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                        fprintf(cmmOut, "/* a = b %c c */\n", op);
                        loadAddr(b, 1);
                        loadAddr(c, 2);
                        fprintf(cmmOut, "%s__1 = %s__1 %c %s__2;\n", prefixV, prefixV, op, prefixV);
                        saveAddr(a, 1);
                        break;
                case 'r':
                        fprintf(cmmOut, "/* a = b[c] */\n");
                        loadAddr(b, c, 1);
                        saveAddr(a, 1);
                        break;
                case 'l':
                        fprintf(cmmOut, "/* a[b] = c */\n");
                        loadAddr(c, 1);
                        saveAddr(a, b, 1);
                        break;
        }
} // }}}
void emitASSIGN(char op, GeneralType a, GeneralType b) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(b.t==INT)?"I":"F";
        const char *prefixF=(b.t==INT)?"":"F";

        switch(op){
                case '=':
                        fprintf(cmmOut, "/* a %c b */\n", op);
                        loadAddr(b, 1);
                        saveAddr(a, 1);
                        break;
                case 'm':
                        fprintf(cmmOut, "/* a = -b */\n");
                        loadAddr(b, 1);
                        fprintf(cmmOut, "%s__1 = %s__1 * -1;\n", prefixV, prefixV);
                        saveAddr(a, 1);
                        break;
                case '+':
                case '-':
                case '*':
                case '/':
                case '%':
                case '&':
                case '|':
                case '^':
                        fprintf(cmmOut, "/* a %c= b */\n", op);
                        loadAddr(a, 1);
                        loadAddr(b, 2);
                        fprintf(cmmOut, "%s__1 = %s__1 %c %s__2;\n", prefixV, prefixV, op, prefixV);
                        saveAddr(a, 1);
                        break;

        }
} // }}}
void emitASSIGN(char op, GeneralType a) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(a.t==INT)?"I":"F";
        const char *prefixF=(a.t==INT)?"":"F";

        switch(op){
                case '!':
                        fprintf(cmmOut, "/* a = -a */\n");
                        loadAddr(a, 1);
                        fprintf(cmmOut, "I__1 = I__1+1;\n");
                        fprintf(cmmOut, "I__1 = I__1&1;\n");
                        saveAddr(a, 1);
                        break;
        }
} // }}}

void emitWRITE(GeneralType a) // {{{
{
	if(!isNoError)
		return;
        const char *prefixA=(a.t==INT)?"d":"f";
        const char *prefixV=(a.t==INT)?"I":"F";

        loadAddr(a, 1);
        fprintf(cmmOut, "printf(\"%%%s\", %s__1);\n", prefixA, prefixV);
} // }}}
void emitWRITE(const char *s) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "printf(\"%s\");\n", s);
} // }}}
void emitREAD(GeneralType a) // {{{
{
	if(!isNoError)
		return;
        const char *prefixA=(a.t==INT)?"d":"f";
        const char *prefixV=(a.t==INT)?"I":"F";

        fprintf(cmmOut, "scanf(\"%%%s\", &%s__1);\n", prefixA, prefixV);
        saveAddr(a, 1);
} // }}}

void emitLABEL(int labelNum) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "LAB%d:\n", labelNum);
} // }}}
void emitJUMP(int labelNum) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "goto LAB%d;\n", labelNum);
} // }}}
void emitCOND(char op, GeneralType a, GeneralType b, GeneralType c) // {{{
{
	if(!isNoError)
		return;
        const char *prefixV=(b.t==INT)?"I":"F";
        const char *prefixF=(b.t==INT)?"":"F";
        const char *zero=(a.t==INT)?"0":"0.0";
        int labelTrue=newLabel(); // true
        int labelEnd=newLabel(); // end

        switch(op){
                case '<':
                case '>':
                case '=':
                case 'G':
                case 'L':
                case 'N':
                        GeneralType constGt;
                        constGt.t = INT;
                        constGt.s = _CONST;

                        fprintf(cmmOut, "/* a = b %c c */\n", op);
                        loadAddr(b, 1);
                        loadAddr(c, 2);
                        fprintf(cmmOut, "%s__1 = %s__1 - %s__2;\n", prefixV, prefixV, prefixV);
                        if(op == 'G')
                                fprintf(cmmOut, "if ( %s__1 >= %s ) ", prefixV, zero);
                        else if(op == 'L')
                                fprintf(cmmOut, "if ( %s__1 <= %s ) ", prefixV, zero);
                        else if(op == 'N' || op == '=')
                                fprintf(cmmOut, "if ( %s__1 == %s ) ", prefixV, zero);
                        else
                                fprintf(cmmOut, "if ( %s__1 %c %s ) ", prefixV, op, zero);
                        emitJUMP(labelTrue);
                        constGt.ival = (op=='N')?1:0;
                        emitASSIGN('=', a, constGt);
                        emitJUMP(labelEnd);
                        emitLABEL(labelTrue);
                        constGt.ival = (op=='N')?0:1;
                        emitASSIGN('=', a, constGt);
                        emitLABEL(labelEnd);
                        break;
        }
} // }}}
void emitIFHEAD(GeneralType a, int labelFalse) // {{{
{
	if(!isNoError)
		return;
        loadAddr(a, 1);
//        fprintf(cmmOut, "printf(\"I__1 = %%d\\n\", I__1);\n");
        fprintf(cmmOut, "if ( I__1 == 0 ) ");
        emitJUMP(labelFalse);
} // }}}
void emitPROCEND() // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "I__0 = I__0 + 0;\n");
        fprintf(cmmOut, "}\n");
} // }}}
void emitFUNC(string name) // {{{
{
	if(!isNoError)
		return;
        fprintf(cmmOut, "I__0 = I__0 + %d;\n", procInfo.tstart+procInfo.temp.size());
        fprintf(cmmOut, "%s();\n", name.c_str());
        fprintf(cmmOut, "I__0 = I__0 - %d;\n", procInfo.tstart+procInfo.temp.size());
} // }}}
