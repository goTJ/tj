// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include "bitio.h"

using std::string;


// {{{ inline double distance(short a[], double b[], const int &len)
inline double distance(short a[], double b[], const int &len)
{
	double sum=0;
	for(int i=0; i<len; i++)
		sum += (a[i]-b[i])*(a[i]-b[i]);
	return sqrt(sum);
} // }}}

int main(int argv, char *argc[])
{
	if(sizeof(short) != 2){
		printf("Must run on a 32-bit machine.\n");
		return 0;
	}
	if(argv < 3){
		printf("Usage: %s VQ_filename vqpcm_filename\n", argc[0]);
		return 0;
	}
	double **VQ;
	int vqSize, vqDim, codeBit;
	// {{{ initialize VQ, codeBit
	{
		FILE *fvq=fopen(argc[1], "r");
		double e;
		fscanf(fvq, "%d%d%lf", &vqDim, &vqSize, &e);
		VQ = (double **)malloc(sizeof(double *)*vqSize);
		for(int i=0; i<vqSize; i++){
			VQ[i] = (double *)malloc(sizeof(double)*vqDim);
			for(int j=0; j<vqDim; j++)
				fscanf(fvq, "%lf", &VQ[i][j]);
		}
		for(codeBit=1; (1<<codeBit)<vqSize; codeBit++)
			;
		fclose(fvq);
	}
	// }}}
	// {{{ initialize string outFileName(argc[1]), FILE *fout
	string outFileName(argc[2]); // $argc[1].pcm
	FILE *fout;
	{
		outFileName.erase(outFileName.length()-6, 6);
		outFileName += ".pcm";
		fout = fopen(outFileName.c_str(), "wb");
		printf("Generating %s.\n", outFileName.c_str());
	} // }}}
	FILE *fin=fopen(argc[2], "rb");
	int idx;
	while((idx=input(codeBit, fin)) != -1){
		for(int i=0; i<vqDim; i++){
			short v=(short)VQ[idx][i];
			fwrite(&v, sizeof(short), 1, fout);
		}
	}
	fclose(fin);
	fclose(fout);
	return 0;
	return 0;
}
