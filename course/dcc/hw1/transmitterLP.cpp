// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <climits>
#include <string>
#include <cctype>
#include <list>
#include <deque>
#include "quantize.h"
#include "bitio.h"
#include "lp_config.h"

using std::string;
using std::list;
using std::deque;

deque<double> X;
list<double> PROD_X[P_SIZE+1];
double R[P_SIZE]; // R[i]=E(x[n]x[n-i-1])/E(x[n]^2)
double RR[P_SIZE][P_SIZE]; // RR[i][j]=R(abs(i-j))
double invRR[P_SIZE][P_SIZE]; // invRR = inv(RR);
double W[P_SIZE]; // W=inv(RR)*R

// {{{ void display()
void display()
{
	printf("X: ");
	for(int i=0; i<P_SIZE+1; i++)
		printf("%.2lf ", X[i]);
	printf("\n");
	/*
	printf("PROD_X: \n");
	for(int i=0; i<P_SIZE+1; i++){
		printf("%2d: ", i);
		for(list<double>::iterator iter=PROD_X[i].begin(); iter!=PROD_X[i].end(); iter++)
			printf("%.2lf ", *iter);
		printf("\n");
	}
	printf("R: ");
	for(int i=0; i<P_SIZE; i++)
		printf("%.2lf ", R[i]);
	printf("\n");
	printf("RR: \n");
	for(int i=0; i<P_SIZE; i++){
		for(int j=0; j<P_SIZE; j++)
			printf("%.2lf ", RR[i][j]);
		printf("\n");
	}
	printf("invRR: \n");
	for(int i=0; i<P_SIZE; i++){
		for(int j=0; j<P_SIZE; j++)
			printf("%.2lf ", invRR[i][j]);
		printf("\n");
	}
	printf("W: ");
	for(int i=0; i<P_SIZE; i++)
		printf("%.2lf ", W[i]);
	printf("\n");
	*/
} // }}}
// {{{ double sum(list<double> &n)
double sum(list<double> &n)
{
	double sum=0.0;
	for(list<double>::iterator iter=n.begin(); iter!=n.end(); iter++)
		sum += *iter;
	return sum;
} // }}}
// {{{ bool equal(double a, double b)
inline bool equal(double a, double b)
{
	return (a-b)<0.00001&&(a-b)>-0.00001;
} // }}}
// {{{ inline void swap(double a[], double b[], int n)
inline void swap(double a[], double b[], int n)
{
	double tmp;

	for(int i=0; i<n; i++){
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
	}
} // }}}
// {{{ inline void div(double a[], double v, int n)
inline void div(double a[], double v, int n)
{
	for(int i=0; i<n; i++)
		a[i] /= v;
} // }}}
// {{{ inline void sub(double a[], double b[], double w, int n)
inline void sub(double a[], double b[], double w, int n)
{
	for(int i=0; i<n; i++)
		a[i] -= b[i]*w;
} // }}}
// {{{ bool inv(double invM[][P_SIZE], double m[][P_SIZE], int n)
bool inv(double invM[][P_SIZE], double m[][P_SIZE], int n)
{
	if(n > P_SIZE)
		n = P_SIZE;
	// invM = I
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++)
			invM[i][j] = 0.0;
		invM[i][i] = 1.0;
	}
	// Gauss
	for(int count=0; count<n; count++){
		int i=count;
		while(equal(m[i][i], 0.0)){
			i++;
			if(i == n)
				return false;
		}
		if(i != count){
			swap(invM[count], invM[i], n);
			swap(m[count], m[i], n);
		}
		i = count;
		div(invM[i], m[i][i], n);
		div(m[i], m[i][i], n);
		for(int j=0; j<n; j++){
			if(j!=i && !equal(m[j][i], 0.0)){
				sub(invM[j], invM[i], m[j][i], n);
				sub(m[j], m[i], m[j][i], n);
			}
		}
	}
	return true;
} // }}}
// {{{ void prod(double c[], double a[][P_SIZE], double b[], int n)
void prod(double c[], double a[][P_SIZE], double b[], int n)
{
	if(n > P_SIZE)
		n = P_SIZE;
	for(int i=0; i<n; i++){
		c[i] = a[i][0]*b[0];
		for(int j=1; j<n; j++)
			c[i] += a[i][j]*b[j];
	}
} // }}}

int main(int argv, char *argc[])
{
	if(sizeof(short) != 2){
		printf("Must run on a 32-bit machine.\n");
		return 0;
	}
	int codeBit=4;
	Quan *quan;
	char quanType='m';
	int fileIdx=-1;
	{ // {{{ initialize quan, codeBit
		for(int i=1; i<argv; i++){
			if(argc[i][0] == '-'){
				switch(argc[i][1]){
					case 'U':
					case 'u':
						quanType='u';
						break;
					case 'A':
					case 'a':
						quanType='a';
						break;
					default:
						if(isdigit(argc[i][1]))
							codeBit = atoi(&argc[i][1]);
				}
			}else{
				fileIdx = i;
			}
		}
		switch(quanType){
			case 'u':
				quan = new QuanByUni(sizeof(short)*8, codeBit);
				break;
			case 'a':
				quan = new QuanByA(sizeof(short)*8, codeBit);
				break;
			default:
				quan = new QuanByMu(sizeof(short)*8, codeBit);
		}
	} // }}}
	// {{{ help page
	if(fileIdx == -1){
		printf("Usage: %s [-m|-a|-u] [-number] pcm_filename\n", argc[0]);
		printf("-u: quantizate by uniform\n");
		printf("-m: quantizate by mu-law\n");
		printf("-a: quantizate by mu-law\n");
		printf("-number: number of bit per sample (suggestion: 4~8)\n");
		return 0;
	} // }}}
	// {{{ initialize string outFileName(argc[1]); // $argc[1]_$CODE_BIT.lppcm
	string outFileName(argc[fileIdx]); // $argc[1]_4.lppcm
	{
		char buf[20];
		outFileName.erase(outFileName.length()-4, 4);
		outFileName += "_LP_";
		sprintf(buf, "%c", quanType);
		outFileName += buf;
		outFileName += "_";
		sprintf(buf, "%d", codeBit);
		outFileName += buf;
		outFileName += ".lppcm";
		printf("Generating %s\n", outFileName.c_str());
	} // }}}
	FILE *fout=fopen(outFileName.c_str(), "wb");
	FILE *fin=fopen(argc[fileIdx], "rb");
	// {{{ initialize LP
	for(int i=0; i<P_SIZE+E_SIZE&&!feof(fin); i++){
		short signal;
		fread(&signal, sizeof(short), 1, fin);
		double ir=0.0;
		for(int j=0; j<P_SIZE; j++)
			ir += X[j]*W[j];
		if(ir > 10000.0)
			ir = 10000;
		else if(ir < -10000.0)
			ir = -10000;
		short d=signal-(short)ir;
		unsigned int code=quan->encode(d);
		output(code, codeBit, fout);
		short dd=quan->decode(code);
		X.push_front((short)(dd+(short)ir));
////		printf("%d %d %.0lf %d %d %d\n", signal, d, ir, code, dd, (short)X[0]);
		for(int j=0; j<=i&&j<P_SIZE+1; j++)
			PROD_X[j].push_back(X[0]*X[j]);
		double exn=sum(PROD_X[0])/PROD_X[0].size();
		for(int j=0; j<i&&j<P_SIZE; j++)
			R[j] = sum(PROD_X[j+1])/PROD_X[j+1].size()/exn;
		for(int j=0; j<i&&j<P_SIZE; j++){
			RR[j][j] = 1.0;
			for(int k=j+1; k<i&&k<P_SIZE; k++)
				RR[j][k] = RR[k][j] = R[k-j-1];
		}
		if(!inv(invRR, RR, i))
			printf("Orz\n");
		prod(W, invRR, R, i);
//		display();
	} // }}}
	int count=0;
	while(!feof(fin)){
////		printf("=== %d ===\n", count);
		short signal;
		fread(&signal, sizeof(short), 1, fin);
		double ir=0.0;
		for(int j=0; j<P_SIZE; j++)
			ir += X[j]*W[j];
		if(ir > 10000.0)
			ir = 10000;
		else if(ir < -10000.0)
			ir = -10000;
		short d=signal-(short)ir;
		unsigned int code=quan->encode(d);
		output(code, codeBit, fout);
		short dd=quan->decode(code);
		X.push_front(dd+(short)ir);
//		printf("%d %d ", signal, d);
////		printf("%.0lf %d %d %.0lf\n", ir, code, dd, X[0]);
		X.pop_back();
		for(int j=0; j<P_SIZE+1; j++){
			PROD_X[j].push_back(X[0]*X[j]);
			PROD_X[j].pop_front();
		}
		double exn=sum(PROD_X[0])/PROD_X[0].size();
		for(int j=0; j<P_SIZE; j++)
			R[j] = sum(PROD_X[j+1])/PROD_X[j+1].size()/exn;
		for(int j=0; j<P_SIZE; j++){
			RR[j][j] = 1.0;
			for(int k=j+1; k<P_SIZE; k++)
				RR[j][k] = RR[k][j] = R[k-j-1];
		}
		if(!inv(invRR, RR, P_SIZE))
			;
//			printf("Orz: %d\n", signal);
		prod(W, invRR, R, P_SIZE);
//		display();
//		if(count == 11111)
//			break;
		count++;
	}
	output(0, -1, fout);
	fclose(fin);
	fclose(fout);
	return 0;
}
