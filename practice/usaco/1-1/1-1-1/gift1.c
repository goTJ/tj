//### PROGRAM
/*
ID: flyherm003
PROG: gift1
*/
#include<stdio.h>
#include<string.h>

int num;
struct usr {
	char name[14];
	int money;
}; 
struct usr a[10];

int who( char *name)
{
	int i;

	for(i=0; i<num; i++)
		if(strcmp( name, a[i].name) == 0)
			return i;
	return 0;
}

void judge(char from[], int mon, char to[])
{
	a[who(from)].money -= mon;
	a[who(to)].money += mon;
}

int main(void)
{
	int i, j, mon, toto;
	char from[14], to[14];
	FILE *out=fopen("gift1.out", "w");
	FILE *in=fopen("gift1.in", "r");
	
	while(fscanf(in, "%d", &num) != EOF){
		for(i=0; i<num; i++)
			fscanf(in, "%s", a[i].name);
		for(i=0; i<num; i++){
			fscanf(in, "%s%d%d", from, &mon, &toto);
			for(j=0; j<toto; j++){
				fscanf(in, "%s", to);
				judge(from, mon/toto, to);
			}
		}
		for(i=0; i<num; i++)
			fprintf(out, "%s %d\n", a[i].name, a[i].money);
	}
	return 0;
}
//### END
