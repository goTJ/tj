// vim:ft=cpp:foldmethod=marker
#include "physical.h"
#include <cmath>

// {{{ Sample Physical::encode(const Coor&){
Sample Physical::encode(const Coor &c){
	Sample s;
	for(int i=0; i<8; i++)
		s.t[i] = c.x*0.5*cos(M_PI/8*i)+c.y*0.5*sin(M_PI/8*i);
	return s;
} // }}}
// {{{ Coor Physical::decode(const Sample&){
Coor Physical::decode(const Sample &s){
	Coor c;
	c.x = c.y = 0.0;
	for(int i=0; i<8; i++){
		c.x += s.t[i]*cos(M_PI/8*i);
		c.y += s.t[i]*sin(M_PI/8*i);
	}
	c.x /= 2;
	c.y /= 2;
	return c;
} // }}}
