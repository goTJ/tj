#include<stdio.h>
#include<stdlib.h>

struct Coor{
        long long x, y;
};

long long det(const Coor &a, const Coor &b)
{
        return (a.x*b.y - a.y*b.x);
}

long long gcd(long long a, long long b)
{
        long long t;

        while(b){
                t = b;
                b = a%b;
                a = t;
        }
        return a;
}

int main()
{
        Coor node[1000];
        long long n;
        long long i, j;
        long long sum;

        while(scanf("%lld", &n) != EOF && n){
                sum = 0;
                for(i=0; i<n; i++)
                        scanf("%lld%lld", &node[i].x, &node[i].y);
                for(i=0; i<n; i++)
                        sum += det(node[i], node[(i+1)%n]);
                if(sum < 0)
                        sum = -sum;
                sum += 2;
                for(i=0; i<n; i++)
                        sum -= gcd(abs(node[i].x-node[(i+1)%n].x), abs(node[i].y-node[(i+1)%n].y));
                printf("%lld\n", sum/2);
        }
        return 0;
}
