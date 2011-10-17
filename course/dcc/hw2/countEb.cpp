// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cmath>
#include "qam.h"
#define PROG_NAME "countEb"

int main(int argc, char *argv[])
{
	double E0=10000.0, halfD;
	if(argc <= 1){
		printf("Usage: ./%s qamfile E0\n", PROG_NAME);
		exit(1);
	}else if(argc >= 3){
		sscanf(argv[2], "%lf", &E0);
	}
	halfD = sqrt(E0);
	Qam qam(argv[1], halfD);
	printf("%.2lf\n", qam.getEb());
	return 0;
}
