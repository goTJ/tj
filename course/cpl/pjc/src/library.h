// vim:ft=cpp:foldmethod=marker
#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <string>
#include <vector>
#include <list>
#include <utility>

using std::string;
using std::vector;
using std::list;
using std::pair;

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)

enum Type{ NO_TYPE, RESERVE, FUNC, INT, FLOAT, TYPEDEF, RECORD};
enum Scope{ _CONST, LOCAL, TEMP };
enum BoolOP{ EQ, NE, GT, LT, GE, LE };

struct Dimension{
	int l, u, size;
};

struct ProcInfo{
	string name;
	string pname;
	int lstart;
	int tstart;
	vector<bool> temp;
};

struct GeneralType{ // {{{
	Type t;
	Scope s;
	union{
		GeneralType *typeGtp;
		struct{
			int offset;
			int oriOffset;
			int size;
			list<Dimension> *arrayBound;
			int depth;
			GeneralType *tempGtp;
		};
		struct{
			string *pname;
			list<GeneralType> *arg_list;
			GeneralType *retVal;
		};
		int ival;
		float fval;
	};
}; // }}}
class RegManager{ // {{{
public:
	RegManager();
	int allocReg(Type);
	void freeReg(Type, int);
private:
	bool _iReg[10];
	bool _fReg[10];
}; // }}}

int newLabel();
char *sprint(char *format, const char *name);

#endif
