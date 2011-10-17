// vim:ft=cpp:foldmethod=marker
#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <cstdlib>
#include <cmath>

// {{{ class Quan{
class Quan{
public:
	virtual ~Quan(){};
	virtual unsigned int encode(int val)=0;
	virtual int decode(unsigned int code)=0;
}; // }}}
// {{{ class QuanByUni: public Quan{
class QuanByUni: public Quan{
public:
	QuanByUni(int totalBit, int codeBit){
		_tBit=totalBit; _cBit=codeBit; _iBit=totalBit-_cBit; _mask=(1<<(codeBit-1))-1;
		_val = ((1<<(_iBit-1)) > 500) ? 500 : (1<<(_iBit-1));
	}
	unsigned int encode(int val){
		bool sign=(val>=0)?false:true;
		if(abs(val) == (1<<(_tBit-1)))
			val = (abs(val)-1)>>_iBit;
		else
			val = abs(val)>>_iBit;
		unsigned int code=val;
		if(sign)
			code |= (1<<(_cBit-1));
		return code;
	}
	int decode(unsigned int code){
		bool sign=(code&(1<<(_cBit-1)))?true:false;
		code &= (1<<(_cBit-1))-1;
		int val=(code<<_iBit)+_val;
		return (sign)?-val:val;
	}
private:
	int _tBit; // total bit
	int _cBit; // code bit
	int _iBit; // ignore bit = _tBit-_cBit-1
	int _mask;
	int _val;
}; // }}}
// {{{ class QuanByMu: public Quan{
class QuanByMu: public Quan{
public:
	QuanByMu(int totalBit, int codeBit){
		_quanByUni = new QuanByUni(totalBit, codeBit);
		_max_val = 1<<(totalBit-1);
	}
	unsigned int encode(int val){
		int sign=(val>=0)?1:-1;
		double v = (double)abs(val)/_max_val;
		return _quanByUni->encode((int)(log10(1+_mu*v)/log10(1+_mu)*_max_val*sign));
	}
	int decode(unsigned int code){
		double val=(double)_quanByUni->decode(code);
		int sign=(val>=0)?1:-1;
		return (int)((pow(10, fabs(val)/_max_val*log10(1+_mu))-1)/_mu*_max_val*sign);
	}
private:
	static const int _mu;
	QuanByUni *_quanByUni;
	int _max_val; // abs(max)
}; // }}}
// {{{ class QuanByA: public Quan{
class QuanByA: public Quan{
public:
	QuanByA(int totalBit, int codeBit){
		_quanByUni = new QuanByUni(totalBit, codeBit);
		_max_val = 1<<(totalBit-1);
	}
	unsigned int encode(int val){
		int sign=(val>=0)?1:-1;
		double v = (double)abs(val)/_max_val;
		if(v <= 1/_A)
			return _quanByUni->encode((int)((_A*v)/(1+log(_A))*_max_val*sign));
		else
			return _quanByUni->encode((int)((1+log(_A*v))/(1+log(_A))*_max_val*sign));
	}
	int decode(unsigned int code){
		double val=(double)_quanByUni->decode(code)/_max_val;
		int sign=(val>=0)?1:-1;
		val = fabs(val);
		if(val <= 1/_A)
			return (int)(val*(1+log(_A))/_A*_max_val*sign);
		else
			return (int)(exp(val*(1+log(_A))-1)/_A*_max_val*sign);
	}
private:
	static const double _A;
	QuanByUni *_quanByUni;
	int _max_val; // abs(max)
}; // }}}

const double QuanByA::_A = 87.6;
const int QuanByMu::_mu=255;
#endif
