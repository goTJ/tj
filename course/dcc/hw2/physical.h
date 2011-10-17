// vim:ft=cpp:foldmethod=marker
#ifndef PHYSICAL_H
#define PHYSICAL_H

#include "struct.h"

// {{{ class Physical{
class Physical{
public:
	Sample encode(const Coor&);
	Coor decode(const Sample&);
private:
}; // }}}

#endif
