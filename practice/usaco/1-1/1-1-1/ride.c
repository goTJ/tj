//### PROGRAM
/*
ID: flyherm003
PROG: ride 
*/
#include<stdio.h>

int judge(unsigned char *tmp)
{
	int i, n=1;

	for(i=0; tmp[i] != 0; i++){
		tmp[i] -= 'A'-1;
		n = (n*tmp[i])%47;
	}
	return n;
}

int main(void)
{
	unsigned char tmp[100], tmp2[100];
	FILE *out = fopen("ride.out", "w");
	FILE *in = fopen("ride.in", "r");
	
	while(fscanf(in, "%s", tmp) != EOF){
		fscanf(in, "%s", tmp2);
		if(judge(tmp) == judge(tmp2))
			fprintf(out, "GO\n");
		else
			fprintf(out, "STAY\n");
	}
	return 0;
}
//### END
