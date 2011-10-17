/*
   先輸入n, 然後緊接著n個數字, 找出最長遞增子序列(Longest Increasing Seqence)

   Time complexity: O(nlgn)
*/
#include<stdio.h>

int Bsearch(const int best[], const int &max, const int &t)
{
	int up=max, low=1, mid;

	while(up >= low){
		mid = (up+low)/2;
		if(t < best[mid] && (mid == 1 || t > best[mid-1]))
			return mid;
		else if(t > best[mid])
			low = mid+1;
		else if(t < best[mid-1])
			up = mid-1;
		else
			return -1;
	}
	return -1;
}

int main()
{
	int s[100], ans[100];
        int best[100], len[100];
        int max;
        int n, i;
	int j;

        scanf("%d", &n);
        scanf("%d", &s[0]);
	best[1] = s[0];
        len[0] = max = 1;
        for(i=1; i<n; i++){
		printf("%d: %d\n\t", i, max);
		for(j=1; j<=max; j++)
			printf("%d ", best[j]);
		printf("\n");
		scanf("%d", &s[i]);
		if(s[i] > best[max]){
			max++;
			best[max] = s[i];
			len[i] = max;
		}
		else{
			int index = Bsearch(best, max, s[i]);
			if(index != -1){
				best[index] = s[i];
				len[i] = index;
			}
		}
        }
	printf("------ANSWER------\n");
	printf("max: %d\n", max);
	int t=max;
	for(i=n-1; i>=0; i--){
		if(len[i] == t){
			ans[t] = s[i];
			t--;
		}
	}
	for(i=1; i<=max; i++)
		printf("%d ", ans[i]);
	printf("\n");
        return 0;
}
