// vim:ft=cpp:foldmethod=marker
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "library.h"
#include "SymbolTable.h"
#include <map>
#include <list>
#include <string>

using std::map;
using std::list;
using std::string;

class OneSymbolTableByAVLTree: public SymbolTable{
public:
	OneSymbolTableByAVLTree();
	void newLayer();
	void delLayer();
	int getLayerSize();
	GeneralType *getVar(string name);
	void addVar(string name, GeneralType gt);
private:
	map<string, list<GeneralType> > _st;
	list<list<string> > _history;
};

#endif
