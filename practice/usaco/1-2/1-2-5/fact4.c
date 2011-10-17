//### PROGRAM
/*
id: flyherm003
prog: fact4
*/
#include<stdio.h>

int ten[4];      /* 2, 5, 10 */

int judge(int n)
{
        int t; 

        for(; n%2==0; ten[0]++, n/=2); 
        for(; n%5==0; ten[1]++, n/=5); 
        t = (ten[0]<ten[1]) ? (ten[0]):(ten[1]); 
        ten[0] -= t; 
        ten[1] -= t; 
        ten[2] += t; 
        return n%10; 
}

int pow(int n, int base)
{
        int i, s=1; 

        for(i=0; i<n; i++)
                s = base*s%10; 
        return s; 
}

int main(void)
{
        int n, i, t, right; 
////	freopen("fact4.in",  "r",  stdin); 
////	freopen("fact4.out",  "w",  stdout); 

        while(scanf("%d", &n)!=EOF) {
                right = 1; 
                for(i=1; i<=n; i++) {
                        t = judge(i); 
                        right = right*t%10; 
                }
                t = pow(ten[0], 2); 
                right = right*t%10; 
                t = pow(ten[1], 5); 
                right = right*t%10; 
                printf("%d\n", right); 
        }
        return 0; 
}
