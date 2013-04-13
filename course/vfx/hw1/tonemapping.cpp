// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cmath>
#include "gil/gil.h"

int main(int argc, char *argv[])
{
	if(argc < 5){
		printf("Usage: %s [Float Image] [Byte Image] [Gamma] [Amplifier]\n", argv[0]);
		return 0;
	}
	// {{{ initialize
	gil::FloatImage3 image;
	if(!read(image, argv[1])){
		fprintf(stderr, "Can't read image file: %s", argv[1]);
		return 1;
	}
	double ***rgb;
	const int h = image.height();
	const int w = image.width();
	const double gama=atof(argv[3]);
	const double mag=atof(argv[4]);
	printf("height: %d, width: %d\n", h, w);
	rgb = new double**[h];
	for(int i=0; i<h; i++){
		rgb[i] = new double*[w];
		for(int j=0; j<w; j++)
			rgb[i][j] = new double[3];
	}
	double min=999.0, max=-999.0;
	double sum=0.0;
	int count[11]={0};
	// }}}
	// {{{ gama correction
	for(int i=0; i<h; ++i){
		for(int j=0; j<w; ++j){
			for(int k=0; k<3; k++){
				rgb[i][j][k] = pow(image(j,i)[k], gama);
				max = (rgb[i][j][k]>max) ? rgb[i][j][k] : max;
				min = (rgb[i][j][k]<min) ? rgb[i][j][k] : min;
			}
		}
	}
	// }}}
	printf("max: %lf, min: %lf\n", max, min);
	// {{{ magnification
	for(int i=0; i<h; ++i){
		for(int j=0; j<w; ++j){
			double lMax=-999.0;
			for(int k=0; k<3; k++){
				rgb[i][j][k] = (rgb[i][j][k]-min)/(max-min)*mag;
				if(rgb[i][j][k] > 1.0)
					rgb[i][j][k] = 1.0;
				sum += rgb[i][j][k];
				lMax = (rgb[i][j][k] > lMax) ? rgb[i][j][k] : lMax;
			}
			count[(int)(lMax*10)]++;
		}
	}
	// }}}
	printf("adv: %lf\n", sum/w/h/3);
	for(int i=0; i<10; i++)
		printf("%d: %d\n", i, count[i]);
	printf("outOfBound: %d\n", count[10]);
	// {{{ write image
	for(int i=0; i<h; ++i){
		for(int j=0; j<w; ++j){
			for(int k=0; k<3; k++)
				image(j,i)[k] = rgb[i][j][k];
		}
	}
	if( !gil::write(image, argv[2]) ){
		fprintf(stderr, "write file to %s failed.\n", "tmp.jpg");
		return 1;
	}
	// }}}
	// {{{ destroy
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++)
			delete[] rgb[i][j];
		delete[] rgb[i];
	}
	delete[] rgb;
	// }}}
	return 0;
}
