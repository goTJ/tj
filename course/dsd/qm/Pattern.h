#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <string>

using namespace std;

class Pattern{
public:
	friend istream& operator>>(istream&, Pattern&);
	friend ostream& operator<<(ostream&, const Pattern&);
	bool operator==(const Pattern &ptn){ return _s==ptn._s; };
	bool operator<(const Pattern &ptn) const;

	Pattern();
	Pattern(const Pattern&);
	Pattern(const Pattern&, const int&);
	int countOne();
	int similar(const Pattern&) const ;
private:
	string _s;
};

#endif
