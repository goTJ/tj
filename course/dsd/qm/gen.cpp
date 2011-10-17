// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <cstdlib>
#include <ctime>

void output(int tmp, int num)
{
	if(num){
		output(tmp>>1, num-1);
		printf("%d", tmp&1);
	}
}

int main()
{
	int num, n, m;
	int max;
	bool *used;

	srand(time(0));
	scanf("%d", &num);
	max = 1<<num;
	n = rand()%max;
	m = rand()%n;
	n -= m;
	used = new bool[max];
	for(int i=0; i<max; i++)
		used[i] = false;
	printf("%d %d %d\n", num, n, m);
	for(int i=0; i<m+n; i++){
		int tmp;
		do{
			tmp = rand()%max;
		}while(used[tmp]);
		used[tmp] = true;
		output(tmp, num);
		printf("\n");
	}
	return 0;
}
