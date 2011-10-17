// vim:ft=cpp:foldmethod=marker
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#define DIM 4
#define VQ_SIZE 1024
#define EPSILON 0.01

using std::string;

struct Signal{
	short *data;
	short *belong;
	int len;
};

// {{{ int sum(Signal val[], int len)
int sum(Signal val[], int len)
{
	int i;
	int sum=0;
	for(i=0; i<len; i++)
		sum += val[i].len;
	return sum;
} // }}}
// {{{ int sum(int val[], int len)
int sum(int val[], int len)
{
	int i;
	int sum=0;
	for(i=0; i<len; i++)
		sum += val[i];
	return sum;
} // }}}
// {{{ int sum(short val[], int len)
int sum(short val[], int len)
{
	int i;
	int sum=0;
	for(i=0; i<len; i++)
		sum += val[i];
	return sum;
} // }}}
// {{{ void add(double a[], short b[])
void add(double a[], short b[])
{
	for(int i=0; i<DIM; i++)
		a[i] += b[i];
} // }}}
// {{{ void sub(double a[], int b)
void sub(double a[], int b)
{
	for(int i=0; i<DIM; i++)
		a[i] /= b;
} // }}}
// {{{ double distance(short a[], double b[])
double distance(short a[], double b[])
{
	double sum=0;
	for(int i=0; i<DIM; i++)
		sum += (a[i]-b[i])*(a[i]-b[i]);
	return sqrt(sum);
} // }}}

double V[VQ_SIZE][DIM]; // VQ-$DIM-$VQ_SIZE-$EPSILON.txt
int main(int argv, char *argc[])
{
	// {{{ read training data: *data[argv], len[argv]
	argv--;
	Signal *signal=(Signal *)malloc(sizeof(Signal)*argv);
	for(int i=0; i<argv; i++){
		FILE *fin=fopen(argc[i+1], "rb");
		fseek(fin, 0, SEEK_END);
		signal[i].len=ftell(fin)/sizeof(short);
		rewind(fin);
		signal[i].data = (short *)malloc(signal[i].len*sizeof(short));
		signal[i].belong = (short *)malloc((signal[i].len-DIM+1)*sizeof(short));
		fread(signal[i].data, sizeof(short), signal[i].len, fin);
		fclose(fin);
	} // }}}
	// {{{ k-means
	int size=1;
	int total=sum(signal, argv)-(DIM-1)*argv;
	double split_step=EPSILON/sqrt(DIM);
	printf("%d\n", total);
	for(int i=0; i<DIM; i++){
		V[0][i] = 0;
		for(int j=0; j<argv; j++)
			V[0][i] += (double)sum(&signal[j].data[i], signal[j].len-DIM+1)/total;
	}
	while(size < VQ_SIZE){
		double tmpV[VQ_SIZE][DIM]={0};
		int numV[VQ_SIZE]={0};
		memcpy(V[size], V[0], size*DIM*sizeof(double));
		for(int i=0; i<size; i++){
			for(int j=0; j<DIM; j++){
				V[i][j] += split_step;
				V[i+size][j] -= split_step;
			}
		}
		size <<= 1;
		printf("size: %d\n", size);
		for(int i=0; i<argv; i++){
			for(int j=0; j<signal[i].len-DIM+1; j++){
				double minD=distance(&signal[i].data[j], V[0]);
				int minI=0;
				for(int k=1; k<size; k++){
					double tmpD;
					if((tmpD=distance(&signal[i].data[j], V[k])) < minD){
						minD = tmpD;
						minI = k;
					}
				}
				add(tmpV[minI], &signal[i].data[j]);
				numV[minI]++;
				signal[i].belong[j] = minI;
			}
		}
		for(int i=0; i<size; i++){
			if(numV[i] == 0)
				continue;
			sub(tmpV[i], numV[i]);
		}
		memcpy(V, tmpV, sizeof(double)*size*DIM);
	} // }}}
	// {{{ training
	double d1=0.0,d2=EPSILON;
	while(fabs(d2-d1) >= EPSILON){
		printf("%lf, %lf => %lf\n", d1, d2, fabs(d2-d1));
		double tmpV[VQ_SIZE][DIM]={0};
		int numV[VQ_SIZE]={0};
		for(int i=0; i<argv; i++){
			for(int j=0; j<signal[i].len-DIM+1; j++){
				double minD=distance(&signal[i].data[j], V[0]);
				int minI=0;
				for(int k=1; k<VQ_SIZE; k++){
					double tmpD;
					if((tmpD=distance(&signal[i].data[j], V[k])) < minD){
						minD = tmpD;
						minI = k;
					}
				}
				add(tmpV[minI], &signal[i].data[j]);
				numV[minI]++;
				signal[i].belong[j] = minI;
			}
		}
		for(int i=0; i<size; i++){
			if(numV[i] == 0){
				printf("0");
				continue;
			}
			sub(tmpV[i], numV[i]);
		}
		memcpy(V, tmpV, sizeof(double)*VQ_SIZE*DIM);
		d1 = d2;
		d2 = 0.0;
		for(int i=0; i<argv; i++){
			for(int j=0; j<signal[i].len-DIM+1; j++)
				d2 += distance(&signal[i].data[j], V[signal[i].belong[j]]);
		}
	} // }}}
	// {{{ initialize outFileName
	string outFileName="VQ-";
	{
		char buf[30];
		sprintf(buf, "%d", DIM);
		outFileName += buf;
		outFileName += "-";
		sprintf(buf, "%d", VQ_SIZE);
		outFileName += buf;
		outFileName += "-";
		sprintf(buf, "%.2lf", EPSILON);
		outFileName += buf;
		for(unsigned int i=0; i<outFileName.length(); i++){
			if(outFileName[i] == '.')
				outFileName[i] = '_';
		}
		outFileName += ".txt";
	}
	printf("%s\n", outFileName.c_str());
	// }}}
	freopen(outFileName.c_str(), "w", stdout);
	printf("%d %d %.2lf\n", DIM, VQ_SIZE, EPSILON);
	for(int i=0; i<VQ_SIZE; i++){
		for(int j=0; j<DIM; j++)
			printf("%10.3lf ", V[i][j]);
		printf("\n");
	}
	return 0;
}
