/* @JUDGE_ID:   10319NX 434 C */
#include<stdio.h>

int main(void)
{
	int a[13], b[13];
	int n, i, j, times;
	int ma, mb;
	int min, max;

	scanf("%d", &times);
	while(times--){
		scanf("%d", &n);
		ma = mb = 0;
		for(i=0; i<n; i++){
			scanf("%d", &(a[i]));
			if(a[i] > ma)
				ma = a[i];
		}
		for(i=0; i<n; i++){
			scanf("%d", &(b[i]));
			if(b[i] > mb)
				mb = b[i];
		}
		min = max = 0;
		for(i=0; i<n; i++){
			for(j=0; j<n; j++){
				if(b[j] < a[i])
					max += b[j];
				else
					max += a[i];
			}
		} 
		for(i=0; i<n; i++){
			min += a[i];
			for(j=0; j<n; j++)
				if(a[i] == b[j]){
					b[j] = 0;
					break;
				}
		}
		for(i=0; i<n; i++)
			min += b[i];
printf("Matty needs at least %d blocks, and can add at most %d extra blocks.\n"
		, min, max-min);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
