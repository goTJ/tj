/* @JUDGE_ID:   10319NX   154   C */
#include<stdio.h>
#include<string.h>

struct fly{
	int bin[5];
}city[5005];
int sum[5][5];
int lim=0;

char bbb[5] = { 'P', 'G', 'S', 'A', 'N' };
char color[5] = { 'r', 'o', 'y', 'g', 'b' };

int encode(char c)
{
	int i;

	for(i=0; i<5; i++)
		if(c==bbb[i] || c==color[i])
			return i;
}

int same(int n)
{
	int i, s=0;

	for(i=0; i<5; i++)
		s += sum[i][city[n].bin[i]];
	return s;
}

int main(void)
{
	char s[50], *p;
	char b, w;
	int bin, waste;
	int i, max, t, nnn;

	memset(sum, 0, sizeof(sum));
	while(scanf("%s", s) != EOF){
		if(s[0] == '#')
			break;
		else if(s[0] == 'e'){
			max = 0;
			for(i=0; i<lim; i++){
				if((t=same(i)) > max){
					max = t;
					nnn = i;
				}
			}
			printf("%d\n", nnn+1);
			memset(sum, 0, sizeof(sum));
			lim = 0;
		}
		else{
			for(p=strtok(s, ","); p; p=strtok(NULL, ",")){
				bin = encode(p[2]);
				waste = encode(p[0]);
				city[lim].bin[bin] = waste;
				sum[bin][waste]++;
			}
			lim++;
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
