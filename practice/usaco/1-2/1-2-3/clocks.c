//### PROGRAM
/*
ID: flyherm003
PROG: clocks
*/
#include<stdio.h>
#include<string.h>

enum { A, B, C, D, E, F, G, H, I };

int used[9];
int map[9];
void (*move[9])();

void jjj(void)
{
	int i;

	for(i=0; i<9; i++)
		printf("%d ", map[i]);
	printf("\n");
}

void m0()
{
	map[A] = (map[A]+3)%12;
	map[B] = (map[B]+3)%12;
	map[D] = (map[D]+3)%12;
	map[E] = (map[E]+3)%12;
}

void m1()
{
	map[A] = (map[A]+3)%12;
	map[B] = (map[B]+3)%12;
	map[C] = (map[C]+3)%12;
}

void m2()
{
	map[B] = (map[B]+3)%12;
	map[C] = (map[C]+3)%12;
	map[E] = (map[E]+3)%12;
	map[F] = (map[F]+3)%12;
}

void m3()
{
	map[A] = (map[A]+3)%12;
	map[D] = (map[D]+3)%12;
	map[G] = (map[G]+3)%12;
}

void m4()
{
	map[B] = (map[B]+3)%12;
	map[D] = (map[D]+3)%12;
	map[E] = (map[E]+3)%12;
	map[F] = (map[F]+3)%12;
	map[H] = (map[H]+3)%12;
}

void m5()
{
	map[C] = (map[C]+3)%12;
	map[F] = (map[F]+3)%12;
	map[I] = (map[I]+3)%12;
}

void m6()
{
	map[D] = (map[D]+3)%12;
	map[E] = (map[E]+3)%12;
	map[G] = (map[G]+3)%12;
	map[H] = (map[H]+3)%12;
}

void m7()
{
	map[G] = (map[G]+3)%12;
	map[H] = (map[H]+3)%12;
	map[I] = (map[I]+3)%12;
}

void m8()
{
	map[E] = (map[E]+3)%12;
	map[F] = (map[F]+3)%12;
	map[H] = (map[H]+3)%12;
	map[I] = (map[I]+3)%12;
}

void initial(void)
{
	int i;

	memset(used, 0, sizeof(used));
	move[0] = m0;
	move[1] = m1;
	move[2] = m2;
	move[3] = m3;
	move[4] = m4;
	move[5] = m5;
	move[6] = m6;
	move[7] = m7;
	move[8] = m8;
}

int check()
{
	int i;

	for(i=0; i<9; i++)
		if(map[i])
			return 0;
	return 1;
}

void go(int num, int times, int deep, int way[])
{
	int i;

////	printf("go go: deep: %d\n", deep);
////	jjj();
	if(check()){
////		printf("get(%d)\n", deep);
		for(i=0; i<deep-1; i++)
			printf("%d ", way[i]+1);
		printf("%d\n", way[i]+1);
		exit(0);
	}
	else{
		/*/
		for(i=0; i<deep; i++)
			printf("%d ", way[i]);
		printf("\n");
		*/
		if(times < 2){
			move[num]();
			way[deep] = num;
			go(num, times+1, deep+1, way);
			move[num]();
			move[num]();
			move[num]();
		}
		for(i=num+1; i<9; i++){
			move[i]();
			way[deep] = i;
			go(i, 0, deep+1, way);
			move[i]();
			move[i]();
			move[i]();
		}

	}
}

int main(void)
{
	int i, way[30];
	freopen("clocks.in",  "r",  stdin);
	freopen("clocks.out",  "w",  stdout);

	while(1){
		for(i=0; i<9; i++)
			if(scanf("%d", &map[i]) == EOF)
				exit(0);
		initial();
		go(0, -1, 0, way);
	}
	return 0;
}
//### END
