#ifndef SET_H
#define SET_H

#include <iostream>

using namespace std;

class Set{
public:
	friend ostream& operator<<(ostream&, const Set&);
	bool operator[](const unsigned int) const ;
	Set &operator=(const Set &s);
	Set operator|(const Set &s) const ; // Union

	Set(const unsigned int num);
	Set(const Set &);
	~Set();
	void set(const int num);
	unsigned int size() const ;
	void setElement(unsigned int idx, bool val);
	unsigned int count();
	int front() const ;
	void del(const unsigned int idx);
	int cmpSet(const Set &) const ;
private:
	void _setElement(unsigned int idx, bool val);

	unsigned int *_set;
	unsigned int _size;
	unsigned int _len;
	bool _isDirty;
	unsigned int _count;
	static const unsigned int _capBit;
	static const unsigned int _cap;	// capacity: 2^5
	static const unsigned int _capMask;
};

#endif
