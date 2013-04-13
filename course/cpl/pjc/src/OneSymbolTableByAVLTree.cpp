// vim:ft=cpp:foldmethod=marker
#include "OneSymbolTableByAVLTree.h"
#include "library.h"

OneSymbolTableByAVLTree::OneSymbolTableByAVLTree(){ // {{{
} // }}}
void OneSymbolTableByAVLTree::newLayer(){ // {{{
	_history.push_front(list<string>());
} // }}}
void OneSymbolTableByAVLTree::delLayer(){ // {{{
	for(list<string>::iterator iter=_history.front().begin(); iter!=_history.front().end(); iter++){
		_st[*iter].pop_front();
	}
	_history.pop_front();
} // }}}
int OneSymbolTableByAVLTree::getLayerSize(){ // {{{
	int size=0;
	for(list<string>::iterator iter=_history.front().begin(); iter!=_history.front().end(); iter++){
		GeneralType *gtp=getVar(*iter);
		if(gtp->t == INT || gtp->t == FLOAT)
			size += gtp->size;	
	}
	return size;
} // }}}
GeneralType *OneSymbolTableByAVLTree::getVar(string name){ // {{{
	if(_st.find(name) != _st.end()){
		if(!_st[name].empty()){
			return &(_st[name].front());
		}
	}
	return NULL;
} // }}}
void OneSymbolTableByAVLTree::addVar(string name, GeneralType gt){ // {{{
	GeneralType *gtp;
	if(gtp=getVar(name)){
		/*
		if(gtp->t == RESERVE)
			yyerror(sprint("Reserved ID names: %s", name.c_str()));
		else
			yyerror(sprint("Variables %s is declared before.", name.c_str()));
			*/
	}
	_st[name].push_front(gt);
	_history.front().push_front(name);
} // }}}
