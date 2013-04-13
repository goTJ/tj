#ifndef EMIT_H
#define EMIT_H

#include "library.h"
#include <string>

using std::string;

void emitINIT(int globalSize);
void emitSETSP(int offset);

void emitASSIGN(char op, GeneralType, GeneralType, GeneralType);
void emitASSIGN(char op, GeneralType, GeneralType);
void emitASSIGN(char op, GeneralType);

void emitREAD(GeneralType a);
void emitWRITE(GeneralType a);
void emitWRITE(const char *s);

void emitLABEL(int labelNum);
void emitJUMP(int labelNum);
void emitCOND(char op, GeneralType a, GeneralType b, GeneralType c);
void emitIFHEAD(GeneralType a, int labelFalse);
void emitPROCEND();
void emitFUNC(string name);

#endif
