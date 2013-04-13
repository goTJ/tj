// vim:ft=cpp:foldmethod=marker
#ifndef QAM_H
#define QAM_H

#include "struct.h"
#include <string>

using std::string;

// {{{ class Qam{
class Qam{
public:
	Qam(const string &fileName, double halfD);
	~Qam();

	int getBits();
	double getEb();
	Coor encode(int);
	int decode(Coor);
private:
	int _bits;
	double _eb;
	double _halfD;
	Coor *_codeBook;
}; // }}}

#endif
