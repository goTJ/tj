#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{               
        int v,e;                        
        freopen("input.txt","w",stdout);
        scanf("%d %d",&v,&e);
        printf("%d\n",v);
        srand(time(0));
        while(e--)
                printf("%d %d\n",rand()%v+1,rand()%v+1);
	return 0;
}
