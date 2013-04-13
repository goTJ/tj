/* @JUDGE_ID:   10319NX 10258 C "§Ú°Q¹½ÂÅ¤Ä, °Q¹½°Q¹½>.<" */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct fly{
	int num;
	int time;
	int solve[10];
	int penalty[10];
	int label;
	int appear;
}con[100];

void initial(void)
{
	int i;

	memset(con, 0, sizeof(con));
	for(i=0; i<100; i++)
		con[i].label = i;
}

int getline(char s[])
{
	int l=0;
	char c;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			s[l] = '\0';
			return l;
		}
		s[l++] = c;
	}
	s[l] = '\0';
	return -1;
}

int comp(const void *p, const void *q)
{
	if(((struct fly *)p)->appear < ((struct fly *)q)->appear)
		return 1;
	if(((struct fly *)p)->appear > ((struct fly *)q)->appear)
		return -1;
	if(((struct fly *)p)->num < ((struct fly *)q)->num)
		return 1;
	if(((struct fly *)p)->num > ((struct fly *)q)->num)
		return -1;
	if(((struct fly *)p)->time > ((struct fly *)q)->time)
		return 1;
	if(((struct fly *)p)->time < ((struct fly *)q)->time)
		return -1;
	if(((struct fly *)p)->label > ((struct fly *)q)->label)
		return 1;
	if(((struct fly *)p)->label < ((struct fly *)q)->label)
		return -1;
	return 1;
}

int main(void)
{
	int times, round;
	int man, prob, time;
	char rus[5];
	char in[100];
	int i;

	scanf("%d\n", &round);
	getline(in);
	for(times=0; times<round; times++){
		initial();
		while(sscanf(in, "%d%d%d%s", &man, &prob, &time, rus) != EOF){
			man--;
			prob--;
			con[man].appear = 1;
			if(rus[0]=='R' || rus[0]=='U' || rus[0]=='E')
				;
			else if(con[man].solve[prob] == 0){
				if(rus[0] == 'C'){
					con[man].num++;
					con[man].solve[prob] = 1;
					con[man].time += con[man].penalty[prob]+time;
				}
				else if(rus[0] == 'I'){
					con[man].penalty[prob] += 20;
				}
			}
			getline(in);
		}
		qsort(con, 100, sizeof(con[0]), comp);
		if(times)
			printf("\n");
		for(i=0; i<100; i++){
			if(!con[i].appear)
				break;
			printf("%d %d %d\n", con[i].label+1, con[i].num, con[i].time);
		}
		getline(in);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
