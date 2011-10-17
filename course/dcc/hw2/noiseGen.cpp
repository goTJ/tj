// vim:ft=cpp:foldmethod=marker
#include "noiseGen.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

// {{{ NoiseGen::NoiseGen(double d){
NoiseGen::NoiseGen(double d){
	_d = d;
	srand(time(NULL));
} // }}}
// {{{ Coor NoiseGen::getNoise(){
Coor NoiseGen::getNoise(){
	double a=(double)rand()/RAND_MAX;
	double b=(double)rand()/RAND_MAX;
	Coor c;

	c.x = sqrt(-2*log(a))*cos(2*M_PI*b)*_d;
	c.y = sqrt(-2*log(a))*sin(2*M_PI*b)*_d;
	return c;
} // }}}
