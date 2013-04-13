//### PROGRAM
// ¥¢±Ñªº½X>_<
/*
ID: flyherm003
PROG: lamps
*/
#include<stdio.h>
#include<stdlib.h>

int final[6];
int button[4] = { '\77', '\25', '\52', '\11' };
int ans[16];
int alim;
int num;
int lala;

void initial(void)
{
	int i;

	for(i=0; i<6; i++)
		final[i] = -1;
	button[3] = ((1<<((num+2)%3))<<3) + (1<<((num+2)%3));
}

int config(int state)
{
	int n;
	int flag=0;

	scanf("%d", &n);
	while(n != -1){
		n = (num-n)%6;
		if(final[n]!=-1 && final[n]!=state)
			flag = 1;
		final[n] = state;
		scanf("%d", &n);
	}
	return flag;
}

int check(int lamp)
{
	int i;

	for(i=0; i<6; i++){
		if(final[i] != -1){
			if(((lamp>>i)&1)^final[i])
				return 0;
		}
	}
	for(i=0; i<alim; i++)
		if(ans[i] == lamp)
			return 0;
	return 1;
}

void turn(int lamp, int times, int depth)
{
	if(!times){
		if(check(lamp))
			ans[alim++] = lamp;
	}
	else{
		lamp = lamp^button[depth];
		turn(lamp, times-1, depth+1);
		lamp = lamp^button[depth];
		if(depth+times < 4)
			turn(lamp, times, depth+1);
	}
}

void find(int times)
{
	if(times&1){
		turn(63, 1, 0);
		if(times > 2)
			turn(63, 3, 0);
	}
	else{
		turn(63, 0, 0);
		if(times > 0)
			turn(63, 2, 0);
		if(times > 2)
			turn(63, 4, 0);
	}
}

void output()
{
	int i, j;
	char buf[7];

	buf[6] = '\0';
	for(i=0; i<alim; i++){
		for(j=5; j>=0; j--){
			buf[j] = (ans[i]&1)+'0';
			ans[i] >>= 1;
		}
		printf("%s", &(buf[6-num%6]));
		for(j=0; j<num/6; j++)
			printf("%s", buf);
		printf("\n");
	}
}

int comp(const void *p, const void *q)
{
	if(((*(int *)p)&lala) > ((*(int *)q)&lala))
		return 1;
	if(((*(int *)p)&lala) < ((*(int *)q)&lala))
		return -1;
	else
		return *(int *)p - *(int *)q;
}

void sort(void)
{
	int i;

	if(num%6){
		for(i=0,lala=0; i<num%6; i++)
			lala = (lala<<1)|1;
	}
	else
		lala = 63;
	qsort(ans, alim, sizeof(ans[0]), comp);
}

int main(void)
{
	int times;
/**/	freopen("lamps.in", "r", stdin);
/**/	freopen("lamps.out", "w", stdout);

	while(scanf("%d%d", &num, &times) != EOF){
		initial();
		config(1);
		if(config(0)){
			printf("IMPOSSIBLE\n");
			continue;
		}
		find(times);
		if(alim){
			sort();
			output();
		}
		else
			printf("IMPOSSIBLE\n");
	}
	return 0;
}
//### END
