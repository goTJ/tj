//### PROGRAM
/*
ID: flyherm003
PROG: runround
*/
#include<stdio.h>
#include<string.h>

char s[12];
int leng, used[10];

void jjj(void)
{
	int i;

	for(i=leng-1; i>=0; i--)
		printf("%d", s[i]);
	printf("\n");
	/*
	printf("used: ");
	for(i=0; i<10; i++)
		printf("%d: %d\n", i, used[i]);
	printf("\n");
	*/
}

void reverse()
{
	int i, tmp;

//	printf("go reverse\n");
	leng = strlen(s);
	for(i=0; i*2<leng; i++){
		tmp = s[i];
		s[i] = s[leng-i-1];
		s[leng-i-1] = tmp;
	}
}

char find(char n, int used[])
{
	int i;

	for(i=n+1; i<10; i++)
		if(!used[i]){
			used[i] = 1;
			return i;
		}
	return 0;
}

int get_new(int n, int used[])
{
	int i;
	char tmp;

//	printf("go get_new: %d--%d --> ", n, s[n]);
	used[n] = 0;
	if(!(tmp=find(s[n], used))){
//		printf("noon\n");
		return 0;
	}
	s[n] = 0;
	s[n] = tmp;
//	printf("%d->%d\n", n, s[n]);
//	jjj();
	for(i=n-1; i>=0; i--)
		s[i] = find(0, used);
	return 1;
}

void initial()
{
	int i, j, k;

//	printf("go initial\n");
	for(i=0; i<leng; i++)
		s[i] -= '0';
	memset(used, 0, sizeof(used));
	for(i=leng-1; i>=0; i--){
		if(used[s[i]]){
//			printf("rerere: %d--%d\n", i, s[i]);
			for(j=i; !get_new(j, used); j++){
				if(j == leng-1){
//					printf(">.<\n");
					leng++;
					if(leng>10)
						exit(0);
					memset(used, 0, sizeof(used));
					for(k=leng; k>=0; k--){
						s[k] = leng-k;
						used[leng-k] = 1;
					}
					return;
				}
			}
			return;
		}
		else
			used[s[i]] = 1;
	}
}

void next(int used[])
{
	int i, j;
	char tmp;

	for(i=0; i<leng; i++){
		used[s[i]] = 0;
		if((tmp=find(s[i], used))){
			s[i] = tmp;
			for(j=i-1; j>=0; j--)
				s[j] = find(0, used);
			return;
		}
	}
	leng++;
	if(leng>10)
		exit(0);
	memset(used, 0, sizeof(used));
	for(i=leng-1; i>=0; i--)
		s[i] = leng-i;
}

int test(char s[])
{
	char way[12];
	int i=leng-1, time=0;

//	jjj();
	memset(way, 0, sizeof(way));
	while(!way[(int)i]){
//		printf("%d:%d ", i, s[i]);
		way[(int)i] = 1;
		i = i-s[i]+leng*10;
		i = i%leng;
		time++;
	}
//	printf("%d:%d ", i, s[i]);
//	printf("\n");
	if(time==leng && i==leng-1)
		return 1;
	return 0;
}

int main(void)
{
	freopen("runround.in",  "r",  stdin);
	freopen("runround.out",  "w",  stdout);

	while(scanf("%s", s) != EOF){
		leng = strlen(s);
		reverse();
		initial();
//		jjj();
		do{
			next(used);
//			jjj();
		}while(!test(s));
		jjj();
	}
	return 0;
}
//### END
