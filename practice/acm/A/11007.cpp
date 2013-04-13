#include<cstdio>
#include<cstring>
#include<list>

int steps[10000000]; // {{{

struct Node{
	int s;
	int l[7];
};

int ways[6][7] = { 1, 2, 3, 7, 4, 5, 6,
		   1, 2, 3, 5, 6, 7, 4,
		   5, 1, 3, 4, 6, 2, 7,
		   2, 6, 3, 4, 1, 5, 7,
		   1, 3, 7, 4, 5, 2, 7,
		   1, 6, 2, 4, 5, 7, 3 };

int pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000 }; // }}}
int color[256];
int color2val[70];
using std::list;
int initial() // {{{
{
	list<Node> q;
	Node tmp;
	int val;

	for(int i=0; i<10000000; i++)
		steps[i] = -1;
	steps[1234567] = 0;

	tmp.s = 0;
	for(int i=0; i<7; i++)
		tmp.l[i] = i+1;
	q.push_back(tmp);
	while(!q.empty()){
		Node t=q.front();
		for(int i=0; i<6; i++){
			val = 0;
			for(int j=0; j<7; j++){
				tmp.l[j] = t.l[ways[i][j]-1];
				val += tmp.l[j]*pow10[6-j];
			}
			if(steps[val] == -1){
				steps[val] = tmp.s = t.s+1;
				q.push_back(tmp);
			}
		}
		q.pop_front();
	}
	return 0;
} // }}}
void initial2() // {{{
{
	color['W'] = 1;
	color['R'] = 2;
	color['B'] = 4;
	color['O'] = 8;
	color['Y'] = 16;
	color['G'] = 32;

	color2val[35] = 1; // WRG
	color2val[41] = 2; // WOG
	color2val[25] = 3; // WYO
	color2val[22] = 4; // RYB
	color2val[38] = 5; // RBG
	color2val[44] = 6; // BOG
	color2val[56] = 7; // YBO
} // }}}
int turn[3][6][4][2] = { 2, 4, 3, 5, 1, 6,
		   6, 2, 1, 3, 5, 4,
		   1, 3, 5, 4, 6, 2 };


void judge(char m[7][7]) // {{{
{
	int t=0;
	int buf[7];
	int val=0;
	val += color2val[color[m[0][0]]+color[m[4][1]]+color[m[5][0]]]*1000000;
	printf("%c %c %c\n", m[0][0], m[4][1], m[5][0]);
	val += color2val[color[m[4][3]]+color[m[5][2]]+color[m[3][2]]]*100000;
	printf("%c %c %c\n", m[4][3], m[5][2], m[3][2]);
	val += color2val[color[m[4][2]]+color[m[2][3]]+color[m[3][3]]]*10000;
	val += color2val[color[m[0][3]]+color[m[1][1]]+color[m[2][0]]]*1000;
	val += color2val[color[m[5][1]]+color[m[0][2]]+color[m[1][0]]]*100;
	val += color2val[color[m[1][2]]+color[m[3][0]]+color[m[5][3]]]*10;
	val += color2val[color[m[1][3]]+color[m[3][1]]+color[m[2][2]]];
	printf("%d\n", val);
	printf("%d\n", steps[val]);
} // }}}
bool go(char now[7][7], int depth) // {{{
{
	if(depth >= 3)
		return false;
	char tmp1[7][7];
	char tmp2[7][7];
	memmove(tmp1, now, 49);
	for(int i=0; i<4; i++){
		for(int j=0; j<7; j++){
			tmp2[j][] = tmp1[][]
			memmove(tmp2[j], tmp1[turn[depth][j]-1], 7);
		}
		if(tmp2[0][1]=='W' && tmp2[2][1]=='R' && tmp2[4][0]=='Y'){
			printf("Yes!!\n");
			for(int j=0; j<6; j++)
				printf("%s\n", tmp2[j]);
			printf("\n");
			judge(tmp2);
			return true;
		}
		if(go(tmp2, depth+1) == true)
			return true;
		memmove(tmp1, tmp2, 49);
	}
	return false;
} // }}}

int main()
{
	int times;
	char buf[7][7];

//	initial();
	initial2();
	scanf("%d", &times);
	for(int round=0; round<times; round++){
		for(int i=0; i<7; i++)
			scanf("%s", buf[i]);
		go(buf, 0);
		break;
	}
}
