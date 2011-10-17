// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#define PROG_NAME "countSNR"

int main(int argc, char *argv[])
{
	if(argc <= 2){
		printf("Usage: ./%s filename1 filename2\n", PROG_NAME);
		exit(1);
	}
	FILE *fin=fopen(argv[1], "r");
	FILE *fin2=fopen(argv[2], "r");
	short s1, s2;
	double sum=0.0, e=0.0;
	while(fread(&s1, sizeof(short), 1, fin) > 0){
		if(fread(&s2, sizeof(short), 1, fin2) <= 0){
			printf("diff size\n");
			break;
		}
		sum += s1*s1;
		e += (s1-s2)*(s1-s2);
	}
	printf("compare: %s %s\n", argv[1], argv[2]);
	printf("%.4lf, %.4lf\n", sum, e);
	printf("SNR = %.4lf\n", sum/e);
	return 0;
}
