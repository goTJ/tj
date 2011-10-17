// vim:ft=cpp:foldmethod=marker
#include "frame.h"
#include <gil/dip.h>
#include <cmath>
#include <algorithm>

using namespace std;

void Frame::_RgbToHsv(){ // {{{
	hsv.resize(w, h);
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			float r = rgb(x, y)[0];
			float g = rgb(x, y)[1];
			float b = rgb(x, y)[2];
			if(b>g && b>r){
				hsv(x, y)[2] = b;
				if(b == 0){
					hsv(x, y)[0] = 0.0; hsv(x, y)[1] = 0.0;
				}else{
					double min;
					min = (r < g) ? r : g;
					const double delta=b-min;
					hsv(x, y)[0] = 60*(r-g)/delta+240;
					hsv(x, y)[1] = delta/b;
				}
			}else if(g > r){
				hsv(x, y)[2] = g;
				if(g == 0){
					hsv(x, y)[0] = 0; hsv(x, y)[1] = 0;
				}else{
					double min;
					min = (r < b) ? r : b;
					const double delta=g-min;
					hsv(x, y)[0] = 60*(b-r)/delta+120;
					hsv(x, y)[1] = delta/g;
				}
			}else{
				hsv(x, y)[2] = r;
				if(r == 0){
					hsv(x, y)[0] = 0; hsv(x, y)[1] = 0;
				}else{
					double min;
					min = (g < b) ? g : b;
					const double delta=r-min;
					if(delta != 0){
						hsv(x, y)[0] = 60*(g-b)/delta;
						hsv(x, y)[1] = delta/r;
					}else{
						hsv(x, y)[0] = (g-b);
						hsv(x, y)[1] = 0.0;
					}
				}
			}
			if(hsv(x, y)[0] < 0.0)
				hsv(x, y)[0] += 360.0;
		}
	}
} // }}}
void Frame::_RgbToYiq(){ // {{{
	yiq.resize(w, h);
	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			float r = rgb(x, y)[0];
			float g = rgb(x, y)[1];
			float b = rgb(x, y)[2];
			yiq(x, y)[0] = 0.299*r+0.587*g+0.114*b;
			yiq(x, y)[1] = 0.595716*r-0.274453*g-0.321263*b;
			yiq(x, y)[2] = 0.211456*r-0.522591*g+0.311135*b;
		}
	}
} // }}}
Frame::Frame(const string &fn):filename(fn){ // {{{
	read(rgb, filename.c_str());
	w = rgb.width();
	h = rgb.height();

//	_RgbToHsv();
	_RgbToYiq();
} // }}}

int quanRGB(const Float3 &a){ // {{{ // R:G:B = 4:4:4, Range: [0, 64)
	Byte3 b = DefaultConverter<Byte3, Float3>()(a);

	return (b[0]>>6)+((b[1]>>6)<<2)+((b[2]>>6)<<4);
} // }}}
int quanHSV(const Float3 &a){ // {{{ // H:S:V = 18:3:3, Range: [0, 162)
	int h, s, v;
	h = (int)(a[0]/20);
	if(h == 18) h--;
	s = (int)(a[1]*3);
	if(s == 3) s--;
	v = (int)(a[2]*3);
	if(v == 3) v--;
	return h*9+s*3+v;
} // }}}
int quanYIQ(const Float3 &a){ // {{{ // Y:I:Q = 18:3:3, Range: [0, 162)
	FloatImage3 yiq; // Y: [0, 1], I: [-0.5957, 0.5957], Q: [-0.5226, 0.5226]
	int y, i, q;
	y = (int)(a[0]*18);
	if(y == 18) y--;
	i = (int)((a[1]+0.5957)*3/1.1914);
	if(i >= 3) i--;
	q = (int)((a[2]+0.5226)*3/1.0452);
	if(q >= 3) q--;
	return y*9+i*3+q;
} // }}}
bool compInc(const float a, const float b){ // {{{
	return a<b;
} // }}}

void DFrame::_evaluateRGB(const Frame &a, const Frame &b){ // {{{
	int aBins[17][64], bBins[17][64];
	float rgbx2[17], rgbl1[17];
	const int selectRegions=10; // select the 10 smallest regions from 16
	const int wholeIdx=16;
	const int h4=a.h/4;
	const int w4=a.w/4;

	memset(aBins, 0, sizeof(aBins));
	memset(bBins, 0, sizeof(bBins));
	memset(rgbx2, 0, sizeof(rgbx2));
	memset(rgbl1, 0, sizeof(rgbl1));
	// count bins
	for(int y=0; y<a.h; y++){
		int hi=(y/h4)*4;
		for(int x=0; x<a.w; x++){
			int idx=hi+(x/w4);
			aBins[wholeIdx][quanRGB(a.rgb(x,y))]++;
			bBins[wholeIdx][quanRGB(b.rgb(x,y))]++;
			aBins[idx][quanRGB(a.rgb(x,y))]++;
			bBins[idx][quanRGB(b.rgb(x,y))]++;
		}
	}
	// calculate distance
	for(int i=0; i<17; i++){
		for(int j=0; j<64; j++){
			if(aBins[i][j]+bBins[i][j]){
				float sum=bBins[i][j]+aBins[i][j];
				float diff=bBins[i][j]-aBins[i][j];
				rgbx2[i] += diff*diff/sum/sum;
				rgbl1[i] += fabs(diff);
			}
		}
	}
	f.rgbx2 = rgbx2[wholeIdx];
	f.rgbl1 = rgbl1[wholeIdx];
	sort(rgbx2, rgbx2+16, compInc);
	sort(rgbl1, rgbl1+16, compInc);
	for(int i=0; i<selectRegions; i++){
		f.rgbrgx2 += rgbx2[i];
		f.rgbrgl1 += rgbl1[i];
	}
//	printf("RGB: %10.3f, %10.3f, %10.3f, %10.3f\n", f.rgbx2, f.rgbl1, f.rgbrgx2, f.rgbrgl1);
} // }}}
void DFrame::_evaluateHSV(const Frame &a, const Frame &b){ // {{{
	int aBins[17][162], bBins[17][162];
	float hsvx2[17], hsvl1[17];
	const int selectRegions=10; // select the 10 smallest regions from 16
	const int wholeIdx=16;
	const int h4=a.h/4;
	const int w4=a.w/4;

	memset(aBins, 0, sizeof(aBins));
	memset(bBins, 0, sizeof(bBins));
	memset(hsvx2, 0, sizeof(hsvx2));
	memset(hsvl1, 0, sizeof(hsvl1));
	// count bins
	for(int y=0; y<a.h; y++){
		int hi=(y/h4)*4;
		for(int x=0; x<a.w; x++){
			int idx=hi+(x/w4);
			aBins[wholeIdx][quanHSV(a.hsv(x,y))]++;
			bBins[wholeIdx][quanHSV(b.hsv(x,y))]++;
			aBins[idx][quanHSV(a.hsv(x,y))]++;
			bBins[idx][quanHSV(b.hsv(x,y))]++;
		}
	}
	// calculate distance
	for(int i=0; i<17; i++){
		for(int j=0; j<162; j++){
			if(aBins[i][j]+bBins[i][j]){
				float sum=bBins[i][j]+aBins[i][j];
				float diff=bBins[i][j]-aBins[i][j];
				hsvx2[i] += diff*diff/sum/sum;
				hsvl1[i] += fabs(diff);
			}
		}
	}
	f.hsvx2 = hsvx2[wholeIdx];
	f.hsvl1 = hsvl1[wholeIdx];
	sort(hsvx2, hsvx2+16, compInc);
	sort(hsvl1, hsvl1+16, compInc);
	for(int i=0; i<selectRegions; i++){
		f.hsvrgx2 += hsvx2[i];
		f.hsvrgl1 += hsvl1[i];
	}
//	printf("HSV: %10.3f, %10.3f, %10.3f, %10.3f\n", f.hsvx2, f.hsvl1, f.hsvrgx2, f.hsvrgl1);
} // }}}
void DFrame::_evaluateYIQ(const Frame &a, const Frame &b){ // {{{
	int aBins[17][162], bBins[17][162];
	float yiqx2[17], yiql1[17];
	const int selectRegions=10; // select the 10 smallest regions from 16
	const int wholeIdx=16;
	const int h4=a.h/4;
	const int w4=a.w/4;

	memset(aBins, 0, sizeof(aBins));
	memset(bBins, 0, sizeof(bBins));
	memset(yiqx2, 0, sizeof(yiqx2));
	memset(yiql1, 0, sizeof(yiql1));
	// count bins
	for(int y=0; y<a.h; y++){
		int hi=(y/h4)*4;
		for(int x=0; x<a.w; x++){
			int idx=hi+(x/w4);
			aBins[wholeIdx][quanYIQ(a.yiq(x,y))]++;
			bBins[wholeIdx][quanYIQ(b.yiq(x,y))]++;
			aBins[idx][quanYIQ(a.yiq(x,y))]++;
			bBins[idx][quanYIQ(b.yiq(x,y))]++;
		}
	}
	// calculate distance
	for(int i=0; i<17; i++){
		for(int j=0; j<162; j++){
			if(aBins[i][j]+bBins[i][j]){
				float sum=bBins[i][j]+aBins[i][j];
				float diff=bBins[i][j]-aBins[i][j];
				yiqx2[i] += diff*diff/sum/sum;
				yiql1[i] += fabs(diff);
			}
		}
	}
	f.yiqx2 = yiqx2[wholeIdx];
	f.yiql1 = yiql1[wholeIdx];
	sort(yiqx2, yiqx2+16, compInc);
	sort(yiql1, yiql1+16, compInc);
	for(int i=0; i<selectRegions; i++){
		f.yiqrgx2 += yiqx2[i];
		f.yiqrgl1 += yiql1[i];
	}
//	printf("YIQ: %10.3f, %10.3f, %10.3f, %10.3f\n", f.yiqx2, f.yiql1, f.yiqrgx2, f.yiqrgl1);
} // }}}

void DFrameYIQRGX2::evaluate(const Frame &a, const Frame &b){ // {{{
	int aBins[16][162], bBins[16][162];
	float yiqx2[16];
	const int selectRegions=10; // select the N smallest regions from 16
	const int h4=a.h/4;
	const int w4=a.w/4;

	memset(aBins, 0, sizeof(aBins));
	memset(bBins, 0, sizeof(bBins));
	memset(yiqx2, 0, sizeof(yiqx2));
	// count bins
	for(int y=0; y<a.h; y++){
		int hi=(y/h4)*4;
		for(int x=0; x<a.w; x++){
			int idx=hi+(x/w4);
			aBins[idx][quanYIQ(a.yiq(x,y))]++;
			bBins[idx][quanYIQ(b.yiq(x,y))]++;
		}
	}
	// calculate distance
	for(int i=0; i<16; i++){
		for(int j=0; j<162; j++){
			if(aBins[i][j]+bBins[i][j]){
				float sum=bBins[i][j]+aBins[i][j];
				float diff=bBins[i][j]-aBins[i][j];
				yiqx2[i] += diff*diff/sum/sum;
			}
		}
	}
	sort(yiqx2, yiqx2+16, compInc);
	for(int i=0; i<selectRegions; i++)
		f.yiqrgx2 += yiqx2[i];
} // }}}
float DFrameYIQRGX2::comp(const Feature &ft){ // {{{
	return fabs(f.yiqrgx2-ft.yiqrgx2);
} // }}}
