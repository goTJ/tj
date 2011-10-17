/*   @JUDGE_ID:   10387ZJ   102   C*/

#include<stdio.h>

enum { B ,G ,C };
int a[3][3];
char b[][3] = { B,C,G,
		B,G,C,
		C,B,G,
		C,G,B,
		G,B,C,
		G,C,B };

int  input()
{
	int i,j;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if(scanf("%d",&a[i][j])==EOF)
				return 0;
	return 1;
}

int judge(int n)
{
	int i,j;
	unsigned long num=0;

	for(i=0;i<3;i++)
		for(j=0;j<3;j++)
			if(j!=b[n][i])
				num+=a[i][j];
	return num;
}

int main()
{
	unsigned long min,num,ans;
	int i,j;

	while(input()!=0){
		min=judge(0);
		ans=0;
		for(i=1;i<6;i++){
			num=judge(i);
			if(num<min){
				min=num;
				ans=i;
			}
		}
		for(i=0;i<3;i++){
			if(b[ans][i]==C)
				printf("C");
			else if(b[ans][i]==G)
				printf("G");
			else printf("B");	
		}
		printf(" %lu\n",min);
	}
	return 0;
}
/*END_OF_SOURCE_CODE*/
