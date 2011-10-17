// vim:ft=cpp:foldmethod=marker
#include "Pattern.h"
#include <iostream>
using namespace std;

// {{{ bool Pattern::operator<(const Pattern &ptn) const {
bool Pattern::operator<(const Pattern &ptn) const {
	return _s < ptn._s;
} // }}}

// {{{ Pattern::Pattern(){
Pattern::Pattern(){
} // }}}
// {{{ Pattern::Pattern(const Pattern &ptn){
Pattern::Pattern(const Pattern &ptn){
	_s = ptn._s;
} // }}}
// {{{ Pattern::Pattern(const Pattern&, const Pattern&, const int&){
Pattern::Pattern(const Pattern &ptn, const int &pos){
	_s = ptn._s;
	_s[pos] = '-';
} // }}}
// {{{ void setPtn(const string &ptn){
void Pattern::setPtn(string ptn){
	_s = ptn;
} // }}}
// {{{ string &Pattern::getPtn(){
string Pattern::getPtn(){
	return _s;
} // }}}
// {{{ int Pattern::countOne()
int Pattern::countOne(){
	int sum=0;

	for(unsigned int i=0; i<_s.size(); i++){
		if(_s[i] == '1')
			sum++;
	}
	return sum;
} // }}}
// {{{ bool Pattern::similar(const &Pattern) const {
int Pattern::similar(const Pattern &ptn) const {
	bool isDiff=false;
	int pos=-1;

	for(unsigned int i=0; i<_s.size(); i++){
		if(_s[i] == ptn._s[i]){
			;
		}else if(((_s[i]=='1' && ptn._s[i]=='0')||(ptn._s[i]=='1'&&_s[i]=='0')) && !isDiff){
			isDiff = true;
			pos = i;
		}else{
			return -1;
		}
	}
	return pos;
} // }}}
