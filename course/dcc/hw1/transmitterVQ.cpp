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
		printf("Usage: %s VQ_filename pcm_filename\n", argc[0]);
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
	string outFileName(argc[2]); // $argc[1]_4.dpcm
	// {{{ initialize string outFileName(argc[1]); // $argc[1]_VQ_$vqDim_$vqSize.vqpcm
	{
		char buf[20];
		outFileName.erase(outFileName.length()-4, 4);
		outFileName += "_VQ_";
		sprintf(buf, "%d", vqDim);
		outFileName += buf;
		outFileName += "_";
		sprintf(buf, "%d", vqSize);
		outFileName += buf;
		outFileName += ".vqpcm";
		printf("Generating %s.\n", outFileName.c_str());
	} // }}}
	FILE *fout = fopen(outFileName.c_str(), "wb");
	FILE *fin=fopen(argc[2], "rb");
	short *signal=(short *)malloc(sizeof(short)*vqDim);
	while(!feof(fin)){
		int n=fread(signal, sizeof(short), vqDim, fin);
		double minD=distance(signal, VQ[0], n);
		int minI=0;
		for(int i=1; i<vqSize; i++){
			double tmpD;
			if((tmpD=distance(signal, VQ[i], n)) < minD){
				minD = tmpD;
				minI = i;
			}
		}
		output(minI, codeBit, fout);
	}
	output(0, -1, fout);
	fclose(fin);
	fclose(fout);
	return 0;
	return 0;
}
