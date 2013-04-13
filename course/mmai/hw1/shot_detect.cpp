// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include "frame.h"

void helppage(const char *prog){ // {{{
	printf("Usage: %s listfile outfile\n", prog);
} // }}}

int main(int argc, char *argv[]){
	char *listfile, *outfile;
	vector<string> imagenames;
	DFrame *dframes;

	if(argc < 3){
		helppage(argv[0]);
		return 0;
	}
	listfile = argv[1];
	outfile = argv[2];
	// Read imagenames in list
	printf("Read images...\n");
	FILE *fin=fopen(listfile, "r");
	char buf[256];

	if(fin == NULL){
		fprintf(stderr, "Can't open listfile\n");
		exit(1);
	}
	string fn;
	while(fgets(buf, 250, fin) != NULL){
		int l=strlen(buf);
		if(buf[l-1] != '\n'){
			fn += buf;
		}else{
			buf[l-1] = '\0';
			fn += buf;
			imagenames.push_back(fn);
			fn.clear();
		}
	}
	fclose(fin);
	// Generate DFrame
	printf("Evaluate difference...\n");
	Frame fr[2];
	int now=1, last=0;
	int dfsize=imagenames.size()-1;
	dframes = new DFrameYIQRGX2[dfsize];
	fr[last] = Frame(imagenames[0]);
	for(int i=0; i<dfsize; i++){
		fr[now] = Frame(imagenames[i+1]);
		dframes[i].evaluate(fr[last], fr[now]);
		{ int t; t=now; now=last; last=t; } // swap
	}
	// Detect shots (By K-means)
	printf("Detect shots...\n");
	const float e=0.01;  // K-means epsilon
	const float Te=0.7;  // Threshold of serious examination
	Feature a[2];
	int *idx[2];
	idx[0] = new int[dfsize], idx[1] = new int[dfsize];
	for(int i=0; i<dfsize; i++)
		a[0] += dframes[i].f;
	a[0] /= dfsize; a[1] = a[0]*(1.0+e); a[0] *= (1.0-e);
	now=1, last = 0;
	for(int i=0; i<dfsize; i++){
		if(dframes[i].comp(a[0]) < dframes[i].comp(a[1])){
			idx[last][i] = 0; // uncutting class
		}else{
			idx[last][i] = 1; // cutting class
		}
	}
	do{
		int n[2]={0};
		a[0].clear(), a[1].clear();
		for(int i=0; i<dfsize; i++){
			a[idx[last][i]] += dframes[i].f;
			n[idx[last][i]]++;
		}
		a[0] /= n[0], a[1] /= n[1];
		for(int i=0; i<dfsize; i++){
			if(dframes[i].comp(a[0]) < dframes[i].comp(a[1])){
				idx[now][i] = 0;
			}else{
				idx[now][i] = 1;
			}
		}
		int i;
		for(i=0; i<dfsize; i++){
			if(idx[last][i] != idx[now][i])
				break;
		}
		if(i == dfsize)
			break;
		{ int t; t=now; now=last; last=t; } // swap
	}while(1);
	// deal with consecutive cut
	printf("avg: %.3f %.3f, %.3f\n", a[0].yiqrgx2, a[1].yiqrgx2, (a[1].yiqrgx2-a[0].yiqrgx2)/(a[1].yiqrgx2+a[0].yiqrgx2));
	const float te=(a[1].yiqrgx2-a[0].yiqrgx2)/(a[1].yiqrgx2+a[0].yiqrgx2);
	int from=-1;
	int *status=idx[last];
	for(int i=0; i<dfsize; i++){
		if(status[i] == 1){
			if(from == -1){
				from = i;
			}
		}else{
			if(i+1<dfsize && status[i+1] == 1){
			}else if(from >= 0){
				for(int j=from; j<i; j++)
					status[j] = 0;
				if(te > Te || i-from > 10)  // magic number...Orz
					status[(from+i-1)>>1] = 1;
				from = -1;
			}
		}
	}

	FILE *fout=fopen(outfile, "w");
	if(fout == NULL){
		fprintf(stderr, "Can't write %s\n", outfile);
		exit(1);
	}
	for(int i=0; i<=dfsize; i++){
		fprintf(fout, "%s\n", imagenames[i].c_str());
		if(i<dfsize && status[i])
			fprintf(fout, "=============================\n");
	}
	fclose(fout);
	delete[] idx[0], delete[] idx[1];
	return 0;
}
