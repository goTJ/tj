#ifndef REGDATA_H
#define REGDATA_H

#include <string>
#include <ctime>

using namespace std;

class RegData{
public:
	friend class RegDB;
	RegData(){};
	RegData(const RegData &r){ _to=r._to; _contact=r._contact; _expires=r._expires; };
	void eraseData(){ _to=_contact=_expires=""; };
protected:
	string _to;
	string _contact;
	string _expires;
	time_t _expireTime;
};

#endif
