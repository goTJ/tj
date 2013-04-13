/* @JUDGE_ID:   10319NX 186 C */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>

char city[105][25];
struct hermit{
	char name[15];
	int dis;
}road[205][205];
int num=0;
struct fly{
	int node[105];
	int lim;
	int dis;
}map[205][205];

int get_line(char in[])
{
	char c;
	int pos=0;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			in[pos] = '\0';
			return 1;
		}
		in[pos++] = c;
	}
	return 0;
}

void initial(void)
{
	int i, j;

	memset(map, 0, sizeof(map));
	for(i=0; i<205; i++)
		for(j=0; j<205; j++){
			map[i][j].dis = INT_MAX;
			map[i][j].node[0] = j;
			map[i][j].lim = 1;
		}
}

int search_city(char s[])
{
	int i;

	for(i=0; i<num; i++)
		if(!strcmp(s, city[i]))
			return i;
	strcpy(city[num], s);
	num++;
	return num-1;
}

void link(int f, int m, int t)
{
	int i, n=0;

	for(i=0; i<map[f][m].lim; i++)
		map[f][t].node[n++] =  map[f][m].node[i];
	for(i=0; i<map[m][t].lim; i++)
		map[f][t].node[n++] =  map[m][t].node[i];
	map[f][t].lim = n;
}

void floyd(void)
{
	int i, j, k;
	int t;

	for(k=0; k<num; k++){
	    for(i=0; i<num; i++){
		for(j=0; j<num; j++){
	if(map[i][k].dis!=INT_MAX && map[k][j].dis!=INT_MAX && 
		(t=map[i][k].dis+map[k][j].dis) < map[i][j].dis){
				map[i][j].dis = t;
				link(i, k, j);
			}
		}
	    }
	}
}

void output(int a, int b)
{
	printf("%-20s %-20s %-10s %5d\n"
		, city[a], city[b], road[a][b].name, road[a][b].dis);
}

int main(void)
{
	char in[80], *from, *to, *r;
	int dis, a, b, first;
	int i, j, times=0;

	initial(); while(1){
		fgets(in, 80, stdin);
		if(in[0] == '\n')
			break;
		from = strtok(in, ",");
		to = strtok(NULL, ",");
		r = strtok(NULL, ",");
		dis = atoi(strtok(NULL, ","));
		a = search_city(from);
		b = search_city(to);
		if(dis < map[a][b].dis){
			map[a][b].dis = dis;
			map[b][a].dis = dis;
			road[a][b].dis = dis;
			road[b][a].dis = dis;
			strcpy(road[a][b].name, r);
			strcpy(road[b][a].name, r);
		}
	}
	floyd();
	while(get_line(in)){
		if(times)
			printf("\n\n");
		from = strtok(in, ",");
		to = strtok(NULL, ",");
		a = search_city(from);
		b = search_city(to);
	printf("From                 To                   Route      Miles\n");
	printf("-------------------- -------------------- ---------- -----\n");
		first = a;
		for(i=0; i<map[a][b].lim; i++){
			output(first, map[a][b].node[i]);
			first = map[a][b].node[i];
		}
	printf("                                                     -----\n");
	printf("                                          Total      %5d\n"
					, map[a][b].dis);
		times++;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
