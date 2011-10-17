// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#define PROG_NAME "countPe"

int main(int argc, char *argv[])
{
	if(argc <= 2){
		printf("Usage: ./%s filename1 filename2\n", PROG_NAME);
		exit(1);
	}
	FILE *fin=fopen(argv[1], "r");
	FILE *fin2=fopen(argv[2], "r");
	unsigned int c1, c2;
	int v;
	double sum=0.0, e=0.0;
	while(fread(&c1, sizeof(char), 1, fin) > 0){
		if(fread(&c2, sizeof(char), 1, fin2) <= 0){
			printf("diff size\n");
			break;
		}
		sum += 8.0;
		v = c1^c2;
		for(int i=0; i<8; i++){
			if(v&1)
				e++;
			v >>= 1;
		}
	}
	printf("compare: %s %s\n", argv[1], argv[2]);
	printf("total char = %.0lf, wrong char = %.0lf, Pe = %.10lf\n", sum, e, e/sum);
	return 0;
}
