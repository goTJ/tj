// vim:ft=cpp:foldmethod=marker
#ifndef NOISEGEN_H
#define NOISEGEN_H

#include "struct.h"

class NoiseGen{
public:
	NoiseGen(double);

	Coor getNoise();
private:
	double _d;
};

#endif
