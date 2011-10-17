// vim:ft=cpp:foldmethod=marker
#include <string.h>
#include <stdio.h>
#include "library.h"

RegManager::RegManager() // {{{
{
	_iReg[0] = false;
	_iReg[1] = false;
	_iReg[2] = false;
	_iReg[3] = true;
	_iReg[4] = true;
	_iReg[5] = true;
	_iReg[6] = true;
	_iReg[7] = true;
	_iReg[8] = false;
	_iReg[9] = false;
	_fReg[0] = true;
	_fReg[1] = true;
	_fReg[2] = true;
	_fReg[3] = true;
	_fReg[4] = true;
	_fReg[5] = true;
	_fReg[6] = true;
	_fReg[7] = true;
	_fReg[8] = true;
	_fReg[9] = true;
} // }}}
int RegManager::allocReg(Type t) // {{{
{
	if(t == INT){
		for(int i=0; i<10; i++){
			if(_iReg[i]){
				_iReg[i] = false;
				return i;
			}
		}
		return -1;
	}else if(t == FLOAT){
		for(int i=0; i<10; i++){
			if(_fReg[i]){
				_fReg[i] = false;
				return i;
			}
		}
		return -1;
	}
	return -1;
} // }}}
void RegManager::freeReg(Type t, int regNum) // {{{
{
	if(t == INT)
		_iReg[regNum] = true;
	if(t == FLOAT)
		_fReg[regNum] = true;
} // }}}
char *sprint(char *format, const char *name) // {{{
{
	char *buf=new char[strlen(format)+strlen(name)];
	sprintf(buf, format, name);
	return buf;
} // }}}
int newLabel() // {{{
{
	static int current_label=0;

	return current_label++;
} // }}}
