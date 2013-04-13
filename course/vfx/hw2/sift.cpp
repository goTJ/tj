// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cmath>
#include "gil/gil.h"

#define NUM_OCTAVE 3

#define min(a,b) ((a<b)?a:b)
#define max(a,b) ((a>b)?a:b)
#define pow2(a) ((a)*(a))

// {{{ Feature List
struct FeatureList{
	int x, y;
	int k;
	FeatureList *next;
};

inline void addFeature(FeatureList *fList, int x, int y, int k)
{
	FeatureList *tmp=new FeatureList;

	tmp->x = x;
	tmp->y = y;
	tmp->k = k;
	tmp->next = fList->next;
	fList->next = tmp;
}
// }}}
// {{{ Feature ListX
struct FeatureListX{
	int x, y;
	int k;
	double scale;
	double ori;
	int des[128];
	bool isDone;
	FeatureListX *next;
};

inline FeatureListX *newFeatureListX()
{
	FeatureListX *tmp=new FeatureListX;

	tmp->next = NULL;
	return tmp;
}

inline void addFeatureX(FeatureListX *fListX, int x, int y, int k, double scale, double ori)
{
	FeatureListX *tmp=new FeatureListX;

	tmp->x = x;
	tmp->y = y;
	tmp->k = k;
	tmp->scale = scale;
	tmp->ori = ori;
	tmp->next = fListX->next;
	tmp->isDone = false;
	fListX->next = tmp;
}
// }}}

// {{{ Function
// {{{ inline void analyse(double **a, int h, int w)
inline void analyse(double **a, int h, int w)
{
	double min=a[0][0];
	double max=a[0][0];
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			if(a[i][j] < min)
				min = a[i][j];
			if(a[i][j] > max)
				max = a[i][j];
		}
	}
	printf("analyse: min=%lf, max=%lf\n", min, max);
} // }}}
// {{{ bool isEqual(double a, double b)
bool isEqual(double a, double b)
{
	if((a-b)<0.00001 && (b-a)<0.00001)
		return true;
	return false;
} // }}}
// {{{ inline double **new2D(int m, int n)
inline double **new2D(int m, int n)
{
	double **tmp;

	tmp = new double*[m];
	for(int i=0; i<m; i++)
		tmp[i] = new double[n];
	return tmp;
} // }}}
// {{{ inline double **new2D(int m, int n, double v)
inline double **new2D(int m, int n, double v)
{
	double **tmp;

	tmp = new2D(m, n);
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++)
			tmp[i][j] = v;
	}
	return tmp;
} // }}}
// {{{ inline void delete2D(double **a, int m, int n)
inline void delete2D(double **a, int m, int n)
{
	for(int i=0; i<m; i++)
		delete[] a[i];
	delete[] a;
} // }}}
// {{{ inline void outputImage(double **a, int h, int w, char *filename)
inline void outputImage(double **a, int h, int w, char *filename)
{
	gil::FloatImage3 image(w, h);

	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			if(a[i][j] >= -999){
				image(j, i)[0] = a[i][j];
				image(j, i)[1] = a[i][j];
				image(j, i)[2] = a[i][j];
			}else{
				// features
				image(j-1, i)[0] = 1;
				image(j-1, i)[1] = 0;
				image(j-1, i)[2] = 0;
				image(j, i)[0] = 1;
				image(j, i)[1] = 0;
				image(j, i)[2] = 0;
				image(j+1, i)[0] = 1;
				image(j+1, i)[1] = 0;
				image(j+1, i)[2] = 0;
				image(j, i-1)[0] = 1;
				image(j, i-1)[1] = 0;
				image(j, i-1)[2] = 0;
				image(j, i+1)[0] = 1;
				image(j, i+1)[1] = 0;
				image(j, i+1)[2] = 0;
			}
		}
	}
	if(!gil::write(image, filename))
		fprintf(stderr, "write file to %s failed.\n", "tmp.jpg");
} // }}}
// {{{ inline double **imageCopy(gil::FloatImage1 image, int *h, int *w)
inline double **imageCopy(gil::FloatImage1 image, int *h, int *w)
{
	double **data=new2D(image.height(), image.width());
	*h = image.height();
	*w = image.width();

	for(int i=0; i<*h; i++){
		for(int j=0; j<*w; j++)
			data[i][j] = image(j,i);
	}
	return data;
} // }}}
// {{{ inline double **sizeDouble(double **data, int h, int w, int *hh, int *ww)
inline double **sizeDouble(double **data, int h, int w, int *hh, int *ww)
{
	double **data2;
	*hh = h*2-1;
	*ww = w*2-1;
	data2 = new2D(*hh, *ww);
	for(int i=0; i<h*2-1; i++){
		if(i&1){
			for(int j=0; j<w*2-1; j++){
				if(j&1){
					data2[i][j] = (data[i/2][j/2]+data[i/2][j/2+1]+data[i/2+1][j/2]+data[i/2+1][j/2+1])/4;
				}else{
					data2[i][j] = (data[i/2][j/2]+data[i/2+1][j/2])/2;
				}
			}
		}else{
			for(int j=0; j<w*2-1; j++){
				if(j&1){
					data2[i][j] = (data[i/2][j/2]+data[i/2][j/2+1])/2;
				}else{
					data2[i][j] = data[i/2][j/2];
				}
			}
		}
	}
	return data2;
} // }}}
// {{{ inline double **sizeHalf(double **data, int h, int w, int *hh, int *ww)
inline double **sizeHalf(double **data, int h, int w, int *hh, int *ww)
{
	double **tmp=new2D(h>>1, w>>1);
	*hh = h>>1;
	*ww = w>>1;
	for(int i=0; i<*hh; i++){
		for(int j=0; j<*ww; j++)
			tmp[i][j] = data[i*2][j*2];
	}
	return tmp;
} // }}}
// {{{ inline double **gau2D(double sigma, int *bound)
inline double **gau2D(double sigma, int *bound)
{
	double **tmp;

	*bound=(int)(sigma*2)+2;
	int n=*bound;
	tmp = new2D(n, n);
//	printf("sigma: %.2lf, bound: %d\n", sigma, *bound);
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			tmp[i][j]  = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
//			printf("%.4lf, ", tmp[i][j]);
		}
//		printf("\n");
	}
	return tmp;
} // }}}
// {{{ inline double **gauFilter(double **data, int h, int w, double sigma)
inline double **gauFilter(double **data, int h, int w, double sigma, int *bound)
{
//	printf("Gaussian sigma: %lf\n", sigma);
	double **filter=gau2D(sigma, bound);
	double **result;
	int n=*bound;
	result = new2D(h, w, 0.0);
	for(int i=n-1; i<h-n+1; i++){
		for(int j=n-1; j<w-n+1; j++){
			for(int ii=i-n+1; ii<i+n-1; ii++){
				for(int jj=j-n+1; jj<j+n-1; jj++)
					result[i][j] += data[ii][jj]*filter[abs(ii-i)][abs(jj-j)];
			}
		}
	}
	delete2D(filter, n, n);
	return (double**)result;
} // }}}
// {{{ inline double subMatrix(double **a, double **b, int h, int w)
inline double **subMatrix(double **a, double **b, int h, int w)
{
	double **tmp;

	tmp = new2D(h, w);
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++)
			tmp[i][j] = a[i][j]-b[i][j];
	}
	return tmp;
} // }}}
// }}}
// {{{ Procedure
// {{{ FILE *getKeyFilePoint(char *name)
FILE *getKeyFilePoint(char *name)
{
	int i;
	for(i=strlen(name)-1; i>=0; i--){
		if(name[i] == '.')
			break;
	}
	if(i == -1)
		i = strlen(name);
	char *tmp=new char[i+5];
	strncpy(tmp, name, i);
	tmp[i] = '.';
	tmp[i+1] = 'k';
	tmp[i+2] = 'e';
	tmp[i+3] = 'y';
	tmp[i+4] = '\0';
	FILE *fout=fopen(tmp, "w");
	delete[] tmp;
	return fout;
} // }}}
// {{{ inline FeatureList *getFeature(double ***dog, int dogBound[], int h, int w)
inline FeatureList *getFeature(double ***dog, int dogBound[], int h, int w)
{
	FeatureList *fList=new FeatureList;
	fList->next = NULL;
	for(int k=1; k<=NUM_OCTAVE; k++){
		for(int i=dogBound[k]*2; i<h-dogBound[k]*2; i++){
			for(int j=dogBound[k]*2; j<w-dogBound[k]*2; j++){
				double **dogk=dog[k];
				// {{{ check if it's local extrema
				int larger=0;
				int smaller=0;
				int count=-1;
				for(int kk=k-1; kk<=k+1; kk++){
					for(int ii=i-1; ii<=i+1; ii++){
						for(int jj=j-1; jj<=j+1; jj++){
							if(dogk[i][j] < dog[kk][ii][jj])
								smaller++;
							else if(dogk[i][j] > dog[kk][ii][jj])
								larger++;
							count++;
							if(larger < count && smaller < count)
								break;
						}
					}
				} // }}}
				if((larger==26 || smaller==26)){
					// {{{ accurate keypoint localization
					int tx=j, ty=i;
					double dx=(dogk[ty][tx+1]-dogk[ty][tx-1])/2;
					double dy=(dogk[ty+1][tx]-dogk[ty-1][tx])/2;
					double dxx=(dogk[ty][tx+1]-2*dogk[ty][tx]+dogk[ty][tx-1])/4;
					double dyy=(dogk[ty+1][tx]-2*dogk[ty][tx]+dogk[ty-1][tx])/4;
					count=11;
					while((fabs(dx/dxx) > 0.5 || fabs(dy/dyy) > 0.5) && count){
						if(dx/dxx > 0.5 && tx<w-dogBound[k]*2) tx++;
						if(dx/dxx < -0.5 && tx>dogBound[k]*2) tx--;
						if(dy/dyy > 0.5 && ty<h-dogBound[k]*2) ty++;
						if(dy/dyy < -0.5 && ty>dogBound[k]*2) ty--;
						dx = (dogk[ty][tx+1]-dogk[ty][tx-1])/2;
						dy = (dogk[ty+1][tx]-dogk[ty-1][tx])/2;
						dxx=(dogk[ty][tx+1]-2*dogk[ty][tx]+dogk[ty][tx-1])/4;
						dyy=(dogk[ty+1][tx]-2*dogk[ty][tx]+dogk[ty-1][tx])/4;
						count--;
					} // }}}
					dxx=(dogk[ty][tx+1]-2*dogk[ty][tx]+dogk[ty][tx-1])/4;
					dyy=(dogk[ty+1][tx]-2*dogk[ty][tx]+dogk[ty-1][tx])/4;
					double dxy=(dogk[ty-1][tx-1]+dogk[ty+1][tx+1]-dogk[ty-1][tx+1]-dogk[ty+1][tx-1])/4;
					const int r=10;
					if(fabs(dogk[ty][tx]) > 0.03 && (pow2(dxx+dyy)/(dxx*dyy-dxy*dxy)) < pow2(r+1)/r){
						addFeature(fList, tx, ty, k);
					}
				}
			}
		}
	}
	return fList;
} // }}}
// {{{ inline double **evalMag(double **gau, int h, int w, int bound)
inline double **evalMag(double **gau, int h, int w, int bound)
{
	double **tmp;

	tmp = new2D(h, w);
	for(int i=bound; i<h-bound; i++){
		for(int j=bound; j<w-bound; j++)
			tmp[i][j] = sqrt(pow2(gau[i][j+1]-gau[i][j-1])+pow2(gau[i+1][j]-gau[i-1][j]));
	}
	return tmp;
} // }}}
// {{{ inline double **evalTheta(double **gau, int h, int w, int bound)
inline double **evalTheta(double **gau, int h, int w, int bound)
{
	double **tmp;

	tmp = new2D(h, w);
	for(int i=bound; i<h-bound; i++){
		for(int j=bound; j<w-bound; j++){
			tmp[i][j] = atan2(gau[i+1][j]-gau[i-1][j], gau[i][j+1]-gau[i][j-1]);
			if(isEqual(tmp[i][j], M_PI))
				tmp[i][j] = -M_PI;
		}
	}
	return tmp;
} // }}}
// {{{ inline void evalOri(FeatureList *fList, int *numFeatureX, double **mag[], double **theta[], double gauSigma[], FeatureListX *fListX, double scale)
inline void evalOri(FeatureList *fList, int *numFeatureX, double **mag[], double **theta[], double gauSigma[], FeatureListX *fListX, double scale)
{
	double **gauF=NULL;
	int bound=0;
	int lastPk=-1;
	FeatureList *p=fList->next;
	delete fList;
	FeatureList *pp;
	while(p){
		int pk=p->k; int py=p->y; int px=p->x;
		if(pk != lastPk){
			gauF = gau2D(gauSigma[pk]*1.5, &bound);
			lastPk = pk;
		}
		double bins[36]={0};
		double sum[36]={0};
		double weight[36]={0};
		// {{{ evaluate bins
		for(int i=py-bound+1; i<py+bound-1; i++){
			for(int j=px-bound+1; j<px+bound-1; j++){
				int idx=(int)floor((theta[pk][i][j]+M_PI)/(2*M_PI)*36);
				sum[idx] += mag[pk][i][j]*gauF[abs(i-py)][abs(j-px)];
				weight[idx] += ((theta[pk][i][j]+M_PI)/(2*M_PI)*36-idx)*mag[pk][i][j]*gauF[abs(i-py)][abs(j-px)];
				bins[idx] += mag[pk][i][j]*gauF[abs(i-py)][abs(j-px)];
			}
		}
		// }}}
		// {{{ find maxima
		double maxPeak=0.0;
		for(int i=0; i<36; i++){
			if(bins[i] < bins[(i+35)%36] || bins[i] < bins[(i+1)%36])
				bins[i] = 0.0;
			if(bins[i] > maxPeak)
				maxPeak = bins[i];
		}
		if(isEqual(maxPeak, 0.0)){
			p = p->next;
			continue;
		} // }}}
		for(int i=0; i<36; i++){
			if(bins[i] > maxPeak*0.8){
				addFeatureX(fListX, px, py, pk, gauSigma[pk]*scale, (i+weight[i]/sum[i])/36*2*M_PI-M_PI);
				(*numFeatureX)++;
			}
		}
		pp = p;
		p = p->next;
		delete pp;
	}
} // }}}
// {{{ inline void evalDes(FeatureListX *fListX, double **mag[], double **theta[], double scale)
inline void evalDes(FeatureListX *fListX, double **mag[], double **theta[], double scale)
{
	int boundD;
	double **gauD=gau2D(8, &boundD);
	FeatureListX *p=fListX->next;
	double max=-1e10, min=1e10;
	while(p){
		if(p->isDone)
			break;
		double cosP=cos(p->ori);
		double sinP=sin(p->ori);
		double bins2[8]={0};
		int count=0;
		int px=p->x, py=p->y, pk=p->k;
		for(int i=-6; i<=6; i+=4){
			for(int j=-6; j<=6; j+=4){
				for(int ii=i-2; ii<=i+1; ii++){
					for(int jj=j-2; jj<=j+1; jj++){
						int rx=(int)(cosP*(jj+0.5)-sinP*(ii+0.5));
						int ry=(int)(sinP*(jj+0.5)+cosP*(ii+0.5));
						int idx=(int)floor((theta[pk][py+ry][px+rx]+M_PI)/(2*M_PI)*8);
						bins2[idx] += gauD[abs(ry)][abs(rx)]*mag[pk][py+ry][px+rx];
					}
				}
				for(int k=0; k<8; k++){
					p->des[count++] = (int)(bins2[k]*10000)&0xff;
					if(bins2[k] > max)
						max = bins2[k];
					if(bins2[k] < min)
						min = bins2[k];
				}
			}
		}
		p->x = (int)(p->x*scale);
		p->y = (int)(p->y*scale);
		p->isDone = true;
		p = p->next;
	}
	printf("%lf, %lf\n", max, min);
} // }}}
// }}}

int main(int argc, char *argv[])
{
	// {{{ initialize
	if(argc < 2){
		printf("Usage: %s [image name]\n", argv[0]);
		return 0;
	}
	FILE *fout=getKeyFilePoint(argv[1]);
	gil::FloatImage1 image;
	if(!read(image, argv[1])){
		fprintf(stderr, "Can't read image file: %s", argv[1]);
		return 1;
	}
	int ori_h, ori_w;
	double **ori_data;
	int h, w;
	double **data;
	ori_data = imageCopy(image, &ori_h, &ori_w);
	data = sizeDouble(ori_data, ori_h, ori_w, &h, &w);
	printf("Original size: %dx%d\n", ori_h, ori_w);
	printf("Double image size\n");
	// }}}
	double scale=0.5;
	FeatureListX *fListX=newFeatureListX();
	int numFeatureX=0;
	while(min(h,w) >= 32){
		printf("=== size: %dx%d ===\n", h, w);
		// {{{ calculate gau & dog for each octave
		printf("Calculate gaussian & DOG for each octave...\n");
		double **dog[NUM_OCTAVE+2];
		double **gau[NUM_OCTAVE+3];
		int dogBound[NUM_OCTAVE+3];
		int gauBound[NUM_OCTAVE+3];
		double gauSigma[NUM_OCTAVE+3];
		for(int i=0; i<NUM_OCTAVE+3; i++){
			gauSigma[i] = pow(2, (double)(i)/NUM_OCTAVE)*sqrt(2);
			gau[i] = gauFilter(data, h, w, gauSigma[i], &gauBound[i]);
		}
		for(int i=0; i<NUM_OCTAVE+2; i++){
			dog[i] = subMatrix(gau[i+1], gau[i], h, w);
			dogBound[i] = max(gauBound[i], gauBound[i+1]);
		} // }}}
		// {{{ find maxima points
		printf("Find maxima points...\n");
		FeatureList *fList=getFeature(dog, dogBound, h, w);
		for(int i=0; i<=NUM_OCTAVE+1; i++)
			delete2D(dog[i], h, w);
		// }}}
		// {{{ evaluate mag & theta
		printf("Evaluate mag & theta...\n");
		double **mag[NUM_OCTAVE+3];
		double **theta[NUM_OCTAVE+3];
		for(int i=0; i<NUM_OCTAVE+3; i++){
			mag[i] = evalMag(gau[i], h, w, gauBound[i]);
			theta[i] = evalTheta(gau[i], h, w, gauBound[i]);
		} // }}}
		// {{{ evaluate orientation
		printf("Evaluate orientation...\n");
		evalOri(fList, &numFeatureX, mag, theta, gauSigma, fListX, scale);
		printf("Num of features: %d\n", numFeatureX);
		// }}}
		// {{{ evaluate descriptor
		printf("Evaluate descriptor...\n");
		evalDes(fListX, mag, theta, scale);
		// }}}
		double **tmp=data;
		int th=h, tw=w;
		data = sizeHalf(tmp, th, tw, &h, &w);
		delete2D(tmp, th, tw);
		scale *= 2;
	}
	// {{{ output result to file
	printf("Output result to file...\n");
	fprintf(fout, "%d 128\n", numFeatureX);
	FeatureListX *p=fListX->next, *pp;
	while(p){
		ori_data[p->y][p->x] = -1e10;
		fprintf(fout, "%d %d %.2lf %.3lf\n", p->y, p->x, p->scale, p->ori);
		for(int i=0; i<128; i++){
			fprintf(fout, "%d ", p->des[i]);
			if((i&0xf) == 0xf)
				fprintf(fout, "\n");
		}
		pp = p;
		p = p->next;
		delete pp;
	} // }}}
	char filename[200];
	strcpy(filename, "/home/student/92/b92106/htdocs/tmp/");
	strcat(filename, argv[1]);
	outputImage(ori_data, ori_h, ori_w, filename);
	return 0;
} // }}}
