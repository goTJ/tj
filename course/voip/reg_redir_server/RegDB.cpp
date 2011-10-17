// vim:ft=cpp:foldmethod=marker
#include "RegDB.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

// {{{ bool RegDB::insert(RegData &r){
bool RegDB::insert(RegData &r){
	cout << "RegDB insert: " << r._to << endl;
	char buf[8];
	r._expireTime = time(NULL)+atoi(r._expires.c_str());
	_data[r._to] = r;
	return true;
} // }}}
// {{{ bool RegDB::query(RegData &r){
bool RegDB::query(RegData &r){
	cout << "RegDB query: " << r._to << endl;
	if(_data.find(r._to) == _data.end())
		return false;
	if(_data[r._to]._expireTime < time(NULL)){
		_data.erase(r._to);
		return false;
	}
	r._contact = _data[r._to]._contact;
	return true;
} // }}}
