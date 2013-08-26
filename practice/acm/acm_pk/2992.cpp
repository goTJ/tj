#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#define MAX 433
int array[433];
int prime[MAX];
int board[MAX];
int b_l = 0;
long long int mem[MAX][MAX];
using namespace std;
struct ele {
	int x;
	int num;
	struct ele *next;
};

struct ele *gggg[MAX];
int eeee[MAX];

void getP()
{
	int i,j;
	for ( i=0 ; i<MAX ; i++ )
		prime[i]=1;
	prime[0]=0, prime[1]=0;
	for ( i=2 ; i<MAX ; i++ )
		if ( prime[i]==1 )
		{
			board[b_l] = i;
			b_l++;
			for ( j=i*2 ; j<MAX ; j+=i)
				prime[j]=0;
		}
}

int main(){
	int n,m,k, i, j;
	int tt=1;
	char tmp[1000];

	getP();
	for ( i=0 ; i<MAX ; i++ )
		for ( j=0 ; j<MAX ; j++ )
			mem[i][j] = -1;

	for ( i=0 ; i<MAX ; i++ )
			eeee[i] = 0;

	for ( i=0 ; i<MAX ; i++ )
		gggg[i] = 0;
	for ( i=2 ; i<MAX ; i++ )
	{
		int now = i;
		struct ele *p;
		for ( j=2 ; j<MAX ; j++ )
			if ( prime[j]==1 )
			{
				while (now%j == 0 )
					now/=j, eeee[j]++;
			}

		gggg[i] = (struct ele *)malloc(sizeof(ele));
		gggg[i]->x = 2;
		gggg[i]->num = eeee[2];
		gggg[i]->next = 0;
		p = gggg[i];

		for ( j=3 ; j<MAX ; j++ )
			if ( prime[j]==1 )
				if ( eeee[j]>0 )
				{
					p->next = (struct ele *)malloc(sizeof(ele));
					p=p->next;
					p->x = j;
					p->num = eeee[j];
					p->next = 0;
				}
	}


	while (scanf("%d%d", &n, &k)==2)
		{
			struct ele *p;
			long long int ans;

			if ( k < n/2-1 )
				k = n-k;

		

				if ( mem[n][k] !=-1 )
				{
	      cout << mem[n][k]<< endl;

					continue;
				}

		for ( i=0 ; i<MAX ; i++ )
			array[i]=0;

		for ( p=gggg[n] ; p!=0 ; p=p->next)
			array[p->x] += p->num;
	
		for ( p=gggg[k] ; p!=0 ; p=p->next)
			array[p->x] -= p->num;
		for ( p = gggg[n-k] ; p!=0 ; p=p->next)
			array[p->x] -= p->num;

/*		for ( i=n ; i>n-k ; i-- )
		{
			int now = i;
			for ( j=0 ; j<b_l ; j++ )
			{
				int tmp = board[j];
				if ( now==1 )
					break;
				while ( now%tmp == 0 )
					array[tmp]++, now/=tmp;
			}
		}
		for ( i=k ; i>=2 ; i-- )
		{
			int now = i;
			for ( j=0 ; j<b_l ; j++ )
			{
				int tmp = board[j];
				if ( now == 1 )
					break;
				while (now%tmp==0 )
					array[tmp]--, now/=tmp;
			}
		}*/
		for ( i=2, ans=1; i<MAX ; i++ )
			if ( array[i] > 0 )
			ans*=(array[i]+1);
		cout << ans<< endl;
		mem[n][k] = ans;
	}
	return 0;			
}
