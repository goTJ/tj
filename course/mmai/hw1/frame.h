// vim:ft=cpp:foldmethod=marker
#ifndef SHOT_DETECTOR_H
#define SHOT_DETECTOR_H

#include <string>
#include <gil/gil.h>

using namespace std;
using namespace gil;

class Frame{ // {{{
public:
	string filename;
	FloatImage3 rgb; // R, G, B: [0, 1]
	FloatImage3 hsv; // H: [0, 360), S, V: [0, 1]
	FloatImage3 yiq; // Y: [0, 1], I: [-0.5957, 0.5957], Q: [-0.5226, 0.5226]
	int w, h;

	Frame(const string &filename);
	Frame(){};
private:
	void _RgbToYiq();
	void _RgbToHsv();
}; // }}}
struct Feature{ // {{{
	float rgbx2;
	float rgbl1;
	float rgbrgx2;
	float rgbrgl1;
	float hsvx2;
	float hsvl1;
	float hsvrgx2;
	float hsvrgl1;
	float yiqx2;
	float yiql1;
	float yiqrgx2;
	float yiqrgl1;

	Feature(){ clear(); }
	void clear(){
		rgbx2 = 0.0;
		rgbl1 = 0.0;
		rgbrgx2 = 0.0;
		rgbrgl1 = 0.0;
		hsvx2 = 0.0;
		hsvl1 = 0.0;
		hsvrgx2 = 0.0;
		hsvrgl1 = 0.0;
		yiqx2 = 0.0;
		yiql1 = 0.0;
		yiqrgx2 = 0.0;
		yiqrgl1 = 0.0;
	}
	Feature &operator=(const Feature &a){
		rgbx2 = a.rgbx2;
		rgbl1 = a.rgbl1;
		rgbrgx2 = a.rgbrgx2;
		rgbrgl1 = a.rgbrgl1;
		hsvx2 = a.hsvx2;
		hsvl1 = a.hsvl1;
		hsvrgx2 = a.hsvrgx2;
		hsvrgl1 = a.hsvrgl1;
		yiqx2 = a.yiqx2;
		yiql1 = a.yiql1;
		yiqrgx2 = a.yiqrgx2;
		yiqrgl1 = a.yiqrgl1;
		return *this;
	}
	Feature &operator+=(const Feature &a){
		rgbx2 += a.rgbx2;
		rgbl1 += a.rgbl1;
		rgbrgx2 += a.rgbrgx2;
		rgbrgl1 += a.rgbrgl1;
		hsvx2 += a.hsvx2;
		hsvl1 += a.hsvl1;
		hsvrgx2 += a.hsvrgx2;
		hsvrgl1 += a.hsvrgl1;
		yiqx2 += a.yiqx2;
		yiql1 += a.yiql1;
		yiqrgx2 += a.yiqrgx2;
		yiqrgl1 += a.yiqrgl1;
		return *this;
	}
	Feature operator+(const float e){
		Feature result;
		result.rgbx2 = rgbx2+e;
		result.rgbl1 = rgbl1+e;
		result.rgbrgx2 = rgbrgx2+e;
		result.rgbrgl1 = rgbrgl1+e;
		result.hsvx2 = hsvx2+e;
		result.hsvl1 = hsvl1+e;
		result.hsvrgx2 = hsvrgx2+e;
		result.hsvrgl1 = hsvrgl1+e;
		result.yiqx2 = yiqx2+e;
		result.yiql1 = yiql1+e;
		result.yiqrgx2 = yiqrgx2+e;
		result.yiqrgl1 = yiqrgl1+e;
		return result;
	}
	Feature operator*(const float e){
		Feature result;
		result.rgbx2 = rgbx2*e;
		result.rgbl1 = rgbl1*e;
		result.rgbrgx2 = rgbrgx2*e;
		result.rgbrgl1 = rgbrgl1*e;
		result.hsvx2 = hsvx2*e;
		result.hsvl1 = hsvl1*e;
		result.hsvrgx2 = hsvrgx2*e;
		result.hsvrgl1 = hsvrgl1*e;
		result.yiqx2 = yiqx2*e;
		result.yiql1 = yiql1*e;
		result.yiqrgx2 = yiqrgx2*e;
		result.yiqrgl1 = yiqrgl1*e;
		return result;
	}
	Feature &operator-=(const float e){
		rgbx2 -= e;
		rgbl1 -= e;
		rgbrgx2 -= e;
		rgbrgl1 -= e;
		hsvx2 -= e;
		hsvl1 -= e;
		hsvrgx2 -= e;
		hsvrgl1 -= e;
		yiqx2 -= e;
		yiql1 -= e;
		yiqrgx2 -= e;
		yiqrgl1 -= e;
		return *this;
	}
	Feature &operator*=(const float e){
		rgbx2 *= e;
		rgbl1 *= e;
		rgbrgx2 *= e;
		rgbrgl1 *= e;
		hsvx2 *= e;
		hsvl1 *= e;
		hsvrgx2 *= e;
		hsvrgl1 *= e;
		yiqx2 *= e;
		yiql1 *= e;
		yiqrgx2 *= e;
		yiqrgl1 *= e;
		return *this;
	}
	Feature &operator/=(const float e){
		rgbx2 /= e;
		rgbl1 /= e;
		rgbrgx2 /= e;
		rgbrgl1 /= e;
		hsvx2 /= e;
		hsvl1 /= e;
		hsvrgx2 /= e;
		hsvrgl1 /= e;
		yiqx2 /= e;
		yiql1 /= e;
		yiqrgx2 /= e;
		yiqrgl1 /= e;
		return *this;
	}
}; // }}}
class DFrame{ // {{{
public:
	Feature f;

	DFrame(){};
	~DFrame(){};
	virtual void evaluate(const Frame &a, const Frame &b)=0;
	virtual float comp(const Feature &f)=0;
protected:
	void _evaluateRGB(const Frame &a, const Frame &b);
	void _evaluateHSV(const Frame &a, const Frame &b);
	void _evaluateYIQ(const Frame &a, const Frame &b);
}; // }}}
class DFrameYIQRGX2: public DFrame{ // {{{
public:
	void evaluate(const Frame &a, const Frame &b);
	float comp(const Feature &f);
private:
}; // }}}

#endif
