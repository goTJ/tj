/* C--, version 2.3, June 2, 2006 */
#include <stdio.h>
/* stack element type */
typedef int ITYPE;
typedef float FTYPE;
typedef union u_type { ITYPE ival; FTYPE fval;} S__TYPE;
S__TYPE *STACK__S; /* stack */
ITYPE SP__S; /* stack pointer */
/* integer registers */
ITYPE I__0,I__1,I__2,I__3,I__4,I__5,I__6,I__7,I__8,I__9;
FTYPE F__0,F__1,F__2,F__3,F__4,F__5,F__6,F__7,F__8,F__9;

/* initial stack */
void INIT__S(void)
{
  STACK__S = (S__TYPE *) malloc(sizeof(S__TYPE) * (MAX__S+1));
  SP__S = 0;
}

/* return top of stack pointer */
ITYPE CURRENT__SP(void)
{
  return(SP__S);
}

/* returns the int value at stack pointer + i */
ITYPE VAL__S(i)
ITYPE i;
{
  return(STACK__S[SP__S+i].ival);
}

/* returns the int value at STACK[i] */
ITYPE AVAL__S(i)
ITYPE i;
{
  return(STACK__S[i].ival);
}

/* returns the float value at stack pointer + i */
FTYPE FVAL__S(i)
ITYPE i;
{
  return(STACK__S[SP__S+i].fval);
}

/* returns the float value at STACK[i] */
FTYPE FAVAL__S(i)
ITYPE i;
{
  return(STACK__S[i].fval);
}

/* set new stack pointer to be current stack pointer $+ i$ */
void SETSP__S(i)
ITYPE i;
{
  SP__S += i;
}

/* set new stack pointer to be  $i$ */
void ASETSP__S(i)
ITYPE i;
{
  SP__S = i;
}

/* set the int value at stack pointer $+ i$ to the int value $k$ */
void SSET__S(i,k)
ITYPE i;
ITYPE k;
{
  STACK__S[SP__S+i].ival = k;
}

/* set the int value at STACK[i] to the int value $k$ */
void ASSET__S(i,k)
ITYPE i;
ITYPE k;
{
  STACK__S[i].ival = k;
}

/* set the int value at stack pointer $+ i$ to the int value $k$ */
void FSSET__S(i,k)
ITYPE i;
FTYPE k;
{
  STACK__S[SP__S+i].fval = k;
}

/* set the int value at STACK[i] to the int value $k$ */
void FASSET__S(i,k)
ITYPE i;
FTYPE k;
{
  STACK__S[i].fval = k;
}

/* push int value k into stack */
void PUSH__S(k)
ITYPE k;
{
  SP__S += 1;
  STACK__S[SP__S].ival = k;
}

/* push float value k into stack */
void FPUSH__S(k)
FTYPE k;
{
  SP__S += 1;
  STACK__S[SP__S].fval = k;
}

/* pop int value from stack */
ITYPE POP__S(void)
{
  return(STACK__S[SP__S--].ival);
}

/* pop float value from stack */
FTYPE FPOP__S(void)
{
  return(STACK__S[SP__S--].fval);
}
