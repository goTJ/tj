// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cmath>
#include "qam.h"

// {{{ inline double distance(const Coor &a, const Coor &b){
inline double distance(const Coor &a, const Coor &b){
	return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
} // }}}
// {{{ Qam::Qam(const string &fileName, double halfD){
Qam::Qam(const string &fileName, double halfD){
	FILE *fin=fopen(fileName.c_str(), "r");

	_halfD = halfD;
	fscanf(fin, "%d", &_bits);
	Coor coor;
	switch(_bits){
		case 4:
			_codeBook = new Coor[1<<_bits];
			for(coor.y=3; coor.y>=-3; coor.y-=2){
				for(coor.x=-3; coor.x<=3; coor.x+=2){
					char buf[20];
					int n=0;
					fscanf(fin, "%s", buf);
					for(char *p=buf; *p; p++)
						n = (n<<1)+(*p-'0');
					_codeBook[n] = coor;
				}
			}
			/*
			for(int i=0; i<16; i++)
				printf("%d: %.0lf %.0lf\n", i, _codeBook[i].x, _codeBook[i].y);
			printf("===\n");
			*/
			break;
		case 8:
			_codeBook = new Coor[1<<_bits];
			for(coor.y=15; coor.y>=-15; coor.y-=2){
				for(coor.x=-15; coor.x<=15; coor.x+=2){
					char buf[20];
					int n=0;
					fscanf(fin, "%s", buf);
					for(char *p=buf; *p; p++)
						n = (n<<1)+(*p-'0');
					_codeBook[n] = coor;
				}
			}
			break;
		default:
			printf("Unknow QAM.\n");
			return;
	}
	_eb = 0.0;
	for(int i=0; i<(1<<_bits); i++)
		_eb += _codeBook[i].x*_codeBook[i].x+_codeBook[i].y*_codeBook[i].y;
	_eb *= _halfD*_halfD/(1<<_bits);
} // }}}
// {{{ Qam::~Qam(){
Qam::~Qam(){
	delete[] _codeBook;
} // }}}
// {{{ int Qam::getBits(){
int Qam::getBits(){
	return _bits;
} // }}}
// {{{ double Qam::getEb(){
double Qam::getEb(){
	return _eb;
} // }}}
// {{{ double Qam::encode(int){
Coor Qam::encode(int n){
	Coor c;
	c.x = _codeBook[n].x*_halfD;
	c.y = _codeBook[n].y*_halfD;
	return c;
} // }}}
// {{{ int Qam::decode(double){
int Qam::decode(Coor c){
	c.x /= _halfD;
	c.y /= _halfD;
	int minI=0;
	double minD=distance(c, _codeBook[0]);

	for(int i=1; i<(1<<_bits); i++){
		double tmpD=distance(c, _codeBook[i]);
		if(tmpD < minD){
			minI = i;
			minD = tmpD;
		}
	}
	return minI;
} // }}}
