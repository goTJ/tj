// vim:ft=cpp:foldmethod=marker
#include "Set.h"
#include <iostream>

// {{{ ostream& operator<<(ostream& out, const Set& ptn)
ostream& operator<<(ostream& out, const Set& set){
	for(unsigned int i=0; i<set._size; i++)
		out << ((set._set[i>>set._capBit]&(1<<(i&set._capMask)))>>(i&set._capMask));
	return out;
} // }}}
// {{{ bool Set::operator[](const int idx) const {
bool Set::operator[](const unsigned int idx) const {
	return (_set[idx>>_capBit] & (1<<(idx&_capMask)));
} // }}}
// {{{ Set &Set::operator=(const Set &s){
Set &Set::operator=(const Set &s){
	_isDirty = s._isDirty;
	_count = s._count;
	for(unsigned int i=0; i<_len; i++)
		_set[i] = s._set[i];
	return (*this);
} // }}}
// {{{ Set operator|(Set s) const {
Set Set::operator|(const Set &s) const {
	Set *new_s=new Set(_size);

	new_s->_isDirty = true;
	for(unsigned int i=0; i<_len; i++)
		new_s->_set[i] = _set[i] | s._set[i];
	return *new_s;
} // }}}

// {{{ Set::Set(const int num){
Set::Set(const unsigned int num){
////	cout << "\tSet(const int)" << endl;
	set(num);
} // }}}
// {{{ Set::Set(const Set &s){
Set::Set(const Set &s){
////	cout << "\tSet(const Set &): " << s._size << endl;
	_isDirty = s._isDirty;
	_size = s._size;
	_len = s._len;
	_count = s._count;
	_set = new unsigned int[_len];
	for(unsigned int i=0; i<_len ; i++)
		_set[i] = s._set[i];
} // }}}
// {{{ Set::~Set(){
Set::~Set(){
	delete[] _set;
} // }}}
// {{{ void Set::set(const int num){
void Set::set(const int num){
	_isDirty = false;
	_count = 0;
	_size = num;
	_len = (num-1+_cap)>>_capBit;
	_set = new unsigned int[_len];
	for(unsigned int i=0; i<_len; i++)
		_set[i] = 0;
} // }}}
// {{{ int Set::size() const {
unsigned int Set::size() const {
	return _size;
} // }}}
// {{{ void Set::setElement(int idx, bool val){
void Set::setElement(unsigned int idx, bool val){
	if(val && !(_set[idx>>_capBit]&(1<<(idx&_capMask))))
		_count++;
	else if(!val && (_set[idx>>_capBit]&(1<<(idx&_capMask))))
		_count--;
	_setElement(idx, val);
} // }}}
// {{{ int Set::count(){
unsigned int Set::count(){
	if(_isDirty){
		_count = 0;
		for(unsigned int i=0; i<_len; i++){
			unsigned int t=_set[i];
			while(t){
				_count += t&1;
				t >>= 1;
			}
		}
		_isDirty = false;
	}
	return _count;
} // }}}
// {{{ int Set::front(){
int Set::front() const {
	for(unsigned int i=0; i<_len; i++){
		if(!_set[i])
			continue;
		for(unsigned int j=0,val=1; j<_cap; j++,val<<=1){
			if(_set[i]&val)
				return (i<<_capBit)+j;
		}
	}
	return -1;
} // }}}
// {{{ void Set::del(const int idx){
void Set::del(const unsigned idx){
	if((*this)[idx])
		_count--;
	_size--;
	if(idx != _size)
		_setElement(idx, (*this)[_size]);
	_setElement(_size, false);
} // }}}
// {{{ int Set::cmpSet(const Set &) const {
int Set::cmpSet(const Set &s) const {
	bool ge=true, le=true;

	for(unsigned int i=0; i<_len; i++){
		if(!ge && !le)
			return 0;
		if((_set[i]|s._set[i]) != _set[i])
			ge = false;
		if((_set[i]|s._set[i]) != s._set[i])
			le = false;
	}
	if(ge)
		return 1;
	if(le)
		return -1;
	return 0;
} // }}}

// {{{ void Set::_setElement(int idx, bool val){
void Set::_setElement(unsigned int idx, bool val){
	if(val)
		_set[idx>>_capBit] |= 1<<(idx&_capMask);
	else
		_set[idx>>_capBit] &= ~(1<<(idx&_capMask));
} // }}}
