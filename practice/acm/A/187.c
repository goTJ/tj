/* @JUDGE_ID:   10319NX 187 C */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char list[1000][100];
struct fly{
	int sum;
	int list[1000];
	int val[1000];
	int lim;
}tran[1000];

int getline(char s[])
{
	char c;
	int l=0;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			s[l] = '\0';
			return l;
		}
		s[l++] = c;
	}
	return -1;
}

void initial(void)
{
	memset(tran, 0, sizeof(tran));
}

void input(void)
{
	char in[101];
	int n, i;

	while(getline(in) != -1){
		for(i=n=0; i<3; i++)
			n = n*10+in[i]-'0';
		if(!n)
			break;
		strcpy(list[n], &in[3]);
	}
}

void output(int n)
{
	int i;

	printf("*** Transaction %03d is out of balance ***\n", n);
	for(i=0; i<tran[n].lim; i++)
		printf("%03d %-30s %10.2lf\n", tran[n].list[i], list[tran[n].list[i]], (double)tran[n].val[i]/100);
	printf("999 Out of Balance                 %10.2lf\n", (double)abs(tran[n].sum)/100);
}

int main(void)
{
	char in[101];
	int i, label, n, val;

	input();
	while(getline(in) != -1){
		for(i=label=0; i<3; i++)
			label = label*10+in[i]-'0';
		if(!label)
			break;
		for(n=0; i<6; i++)
			n = n*10+in[i]-'0';
		sscanf(in+i, "%d", &val);
		tran[label].sum += val;
		tran[label].list[tran[label].lim] = n;
		tran[label].val[tran[label].lim] = val;
		tran[label].lim++;
	}
	for(i=0; i<1000; i++){
		if(tran[i].sum != 0){
			output(i);
			i++;
			break;
		}
	}
	for(; i<1000; i++){
		if(tran[i].sum != 0){
			printf("\n");
			output(i);
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
