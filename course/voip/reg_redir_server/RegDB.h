#ifndef REGDB_H
#define REGDB_H

#include "RegData.h"
#include <map>

class RegDB{
public:
	bool insert(RegData &);
	bool query(RegData &);
private:
	map<string, RegData> _data;
};

#endif
