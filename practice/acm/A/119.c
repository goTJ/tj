/*###########################################################################*/
/*       title: Greedy                                                       */
/*                                                                           */
/*           start: 2001年  7月 2日 周一 07時01分43秒 CST                    */
/*                                                                           */
/*                finish: */
/*                                                                           */
/*                                        @JUDGE_ID:   10319NX   119   C     */
/*                                                                           */
/*###########################################################################*/
#include<stdio.h>
#include<string.h>

struct fly{
	char name[15];
	int money;
}people[15];
int num;

int tell(char tmp[])
{
	int i, j, n, flag;

	for(i=0; i<num; i++){
		flag = 0;
		if(strlen(tmp) != strlen(people[i].name))
			continue;
		for(j=0; people[i].name[j] != '\0'; j++)
			if(people[i].name[j] == tmp[j])
				flag++;
		if(flag == strlen(tmp))
			n = i;
	}
	return n;
}

int main(void)
{
	int i, j, from, to, money, n, times=0;
	char tmp[15];

	while(scanf("%d", &num) != EOF){
		if(times++)
			printf("\n");
		for(i=0; i<num; i++){
			scanf("%s", people[i].name);
			people[i].money = 0;
		}
		for(i=0; i<num; i++){
			scanf("%s", tmp);
			from = tell(tmp);
			scanf("%d", &money);
			scanf("%d", &n);
			if(n){
				people[from].money -= money;
				for(j=0; j<n; j++){
					scanf("%s", &tmp);
					people[tell(tmp)].money += money/n;
				}
				people[from].money += money-(money/n)*n;
			}
		}
		for(i=0; i<num; i++)
			printf("%s %d\n", people[i].name, people[i].money);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
