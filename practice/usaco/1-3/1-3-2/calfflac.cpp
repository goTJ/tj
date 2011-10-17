//### PROGRAM
/*
ID: flyherm003
PROG: calfflac
*/
#include<stdio.h>
#include<ctype.h>
#define MAX 20006

char ori[MAX], s[MAX];
int site[MAX];
int len, olen=0;

void transcript(void)
{
	int i;

	len = 0;
	for(i=0; i<olen; i++){
		if(isalpha(ori[i])){
			s[len] = tolower(ori[i]);
			site[len++] = i;
		}
	}
	s[len] = '\0';
}

int main(void)
{
	char c;
	int head=0, tail=0;
	int i, l, r;
	int max;
/**/	freopen("calfflac.in", "r", stdin);
/**/	freopen("calfflac.out", "w", stdout);

	//initial & input
	while((c=getchar()) != EOF)
		ori[olen++] = c;
	transcript();
	max = 0;
	//find odd
	for(i=0; i<len; i++){
		for(l=r=i; l>=0&& r<len&& s[l]==s[r]; l--, r++)
			;
		l++; r--;
		if(r-l+1 > max){
			max = r-l+1;
			head = l;
			tail = r;
		}
	}
	//find even
	for(i=0; i<len; i++){
		for(l=i,r=i+1; l>=0&& r<len&& s[l]==s[r]; l--, r++)
			;
		l++; r--;
		if(r-l+1 > max){
			max = r-l+1;
			head = l;
			tail = r;
		}
	}
	//output
	if(max){
		printf("%d\n", max);
		ori[site[tail]+1] = '\0';
		printf("%s\n", &ori[site[head]]);
	}
	return 0;
}
//### END
