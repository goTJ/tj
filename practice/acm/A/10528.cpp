#include<stdio.h>
#include<string.h>

bool tab[12][12];
int step[7] = {2, 2, 1, 2, 2, 2, 1};
int sign[256];
char *name[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

void initial()
{
	int i, j;
	int t;

	for(i=0; i<12; i++){
		for(j=0; j<12; j++)
			tab[i][j] = false;
	}
	for(i=0; i<12; i++){
		t = 0;
		for(j=0; j<7; j++){
			tab[i][(i+t)%12] = true;
			t += step[j];
		}
	}
	sign['C'] = 0;
	sign['D'] = 2;
	sign['E'] = 4;
	sign['F'] = 5;
	sign['G'] = 7;
	sign['A'] = 9;
	sign['B'] = 11;
}

int main()
{
	char in[4001];
	bool used[12];
	char *p;
	int i, j;
	bool can;

	initial();
	while(fgets(in, 4000, stdin)){
		if(in[1]=='N')
			break;
		for(i=0; i<12; i++)
			used[i] = false;
		p = strtok(in, " ");
		while(p){
			if(p[1] == '#')
				used[sign[p[0]]+1] = true;
			else
				used[sign[p[0]]] = true;
			p = strtok(NULL, " ");
		}
		for(i=0; i<12; i++){
			can = true;
			for(j=0; j<12; j++){
				if(used[j]==true && tab[i][j]==false){
					can = false;
					break;
				}
			}
			if(can)
				printf("%s ", name[i]);
		}
		printf("\n");
	}
	return 0;
}
