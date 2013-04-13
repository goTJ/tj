/* @JUDGE_ID: 10319NX 10344 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>

int num[5];
int notFind;

void search(int used[], int depth, int val)
{
	int i;

	if(depth == 5){
		if(val == 23){
			printf("Possible\n");
			notFind = 0;
		}
	}
	else{
		for(i=0; i<5; i++){
			if(notFind && !used[i]){
				used[i] = 1;
				search(used, depth+1, val*num[i]);
				search(used, depth+1, val+num[i]);
				search(used, depth+1, val-num[i]);
				used[i] = 0;
			}
		}
	}
}

int main(void)
{
	int i;
	int flag;
	int used[5];

	while(1){
		flag = notFind = 1;
		for(i=0; i<5; i++){
			used[i] = 0;
			scanf("%d", &num[i]);
			if(num[i])
				flag = 0;
		}
		if(flag)
			break;
		for(i=0; i<5; i++){
			if(notFind){
				used[i] = 1;
				search(used, 1, num[i]);
				used[i] = 0;
			}
		}
		if(notFind)
			printf("Impossible\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
