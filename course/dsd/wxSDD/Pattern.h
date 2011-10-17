#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <string>

using namespace std;

class Pattern{
public:
	bool operator==(const Pattern &ptn){ return _s==ptn._s; };
	bool operator<(const Pattern &ptn) const;

	Pattern();
	Pattern(const Pattern&);
	Pattern(const Pattern&, const int&);
	void setPtn(string);
	string getPtn();
	int countOne();
	int similar(const Pattern&) const ;
private:
	string _s;
};

#endif
