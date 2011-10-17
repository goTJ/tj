/*   @JUDGE_ID:   10319NX   10106   C */
#include<stdio.h>
#include<stdlib.h>

enum { D_MAX = 254 };

void recircle(char a[], int length)
{
	int i;
	char tmp;

	length--;
	for(i=0; i*2<=length; i++){
		tmp = a[i];
		a[i] = a[length-i];
		a[length-i] = tmp;
	}
}

int main(void)
{
	char num_a[D_MAX],  num_b[D_MAX], ans[504];
	int length_a, length_b;
	int i, j;
	int k;

	while(scanf("%s%s", &num_a, &num_b) == 2){
		length_a = strlen(num_a);
		length_b = strlen(num_b);
		memset(ans, 0, sizeof(ans));
		recircle(num_a, length_a);
		recircle(num_b, length_b);
		for(i=0; i<length_a; i++)
			num_a[i] -= '0';
		for(i=0; i<length_b; i++)
			num_b[i] -= '0';
		/*judge*/
		if(length_a==1 && !num_a[0])
			printf("0\n");
		else if(length_b==1 && !num_b[0])
			printf("0\n");
		else{
			for(i=0; i<length_a; i++){
			   for(j=0; j<length_b; j++){
				ans[i+j] += (char)((int)num_a[i]*(int)num_b[j]);
				/*/
				for(k=0; k<length_a+length_b; k++)
					printf("%d ", (int)ans[k]);
				printf("\n");
				*/
			   }
			   for(k=0; k<length_a+length_b; k++)
				 while(ans[k] > 9){
					 ans[k+1]++;
					 ans[k] -= 10;
				 }
			}
		}
		for(i=501; !ans[i]; i--);
		for(; i>=0; i--)
			printf("%d", (int)ans[i]);
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
