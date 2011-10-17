// vim:ft=cpp:foldmethod=marker
#ifndef MY_SYMBOL_TABLE_H
#define MY_SYMBOL_TABLE_H

#include "library.h"
#include <map>
#include <list>
#include <string>

using std::map;
using std::list;
using std::string;

class SymbolTable{
public:
	virtual void newLayer()=0;
	virtual void delLayer()=0;
	virtual int getLayerSize()=0;
	virtual void addVar(string, GeneralType)=0;
	virtual GeneralType *getVar(string)=0;
};

#endif
