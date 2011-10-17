#include<stdio.h>
#include<string.h>

struct Coor{
	int r, c;
};

struct Node{
	char action;
	int state[3][3];
	int father;
	Coor zero;
};

Node node[362880];
int lim=0;
bool used[362880];
int way[4][2] = { {1, 0}, {0, 1}, {0, -1}, {-1, 0}};
char sign[4] = { 'U', 'L', 'R', 'D' };

inline void addNode(int state[][3], const Coor &zero, const int &father, const char &action)
{
	int i, j;

	for(i=0; i<3; i++){
		for(j=0; j<3; j++)
			node[lim].state[i][j] = state[i][j];
	}
	node[lim].zero = zero;
	node[lim].father = father;
	node[lim].action = action;
	lim++;
}

void swap(int &a, int &b)
{
	int t;

	t = a;
	a = b;
	b = t;
}

int encode(int state[][3])
{
	int order[9];
	int i, j;
	int sum=0;
	int n;

	for(i=0; i<9; i++)
		order[i] = i;
	for(n=1,i=1; i<9; i++)
		n *= i;
	for(i=0; i<9; i++){
		sum += order[state[i/3][i%3]]*n;
		for(j=state[i/3][i%3]; j<9; j++)
			order[j]--;
		if(8-i)
			n /= (8-i);
	}
	return sum;
}

void output(int pos)
{
	if(pos > 0){
		output(node[pos].father);
		printf("%c", node[pos].action);
	}
}

int main()
{
	int pos;
	int s[3][3];
	int i, j;
	int ii, jj;
	int t;
	Coor temp;

	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			scanf("%d", &s[i][j]);
			printf("%d ", s[i][j]);
			if(!s[i][j]){
				temp.r = i;
				temp.c = j;
			}
		}
	}
	printf("\n");
	used[encode(s)] = true;
	addNode(s, temp, -1, 'S');
	for(pos=0; pos<lim; pos++){
		for(i=0; i<4; i++){
			temp.r = node[pos].zero.r + way[i][0];
			temp.c = node[pos].zero.c + way[i][1];
			if(temp.r >= 0 && temp.r < 3 && temp.c >= 0 && temp.c < 3){
				swap(node[pos].state[node[pos].zero.r][node[pos].zero.c], node[pos].state[temp.r][temp.c]);
				t = encode(node[pos].state);
				if(!used[t]){
					used[t] = true;
					addNode(node[pos].state, temp, pos, sign[i]);
				}
				swap(node[pos].state[node[pos].zero.r][node[pos].zero.c], node[pos].state[temp.r][temp.c]);
			}
		}
	}
	lim--;
	printf("lim: %d\n", lim);
	for(i=0; i<3; i++){
		for(j=0; j<3; j++)
			printf("%d ", node[lim].state[i][j]);
	}
	printf("\n");
	/*
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			for(ii=0; ii<3; ii++){
				for(jj=0; jj<3; jj++){
					if(node[lim].state[ii][jj] == s[i][j]){
						printf("%d ", ii*3+jj);
					}
				}
			}
		}
	}
	*/
	printf("\n");
	output(lim);
	printf("\n");
	return 0;
}
