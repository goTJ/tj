#include<cstdio>
#define N 3006

struct Eq{
	int a, b, c;
	int sign;
};

int main()
{
	int nr;
	Eq r[N];
	int np;
	int px, py;
	int count;
	int round=1;
	int pos, neg;
	int sum;

	while(scanf("%d", &nr) != 0){
		if(nr == 0)
			break;
		count = 0;
		for(int i=0; i<nr; i++)
			scanf("%d%d%d", &(r[i].a), &(r[i].b), &(r[i].c));
		scanf("%d", &np);
		for(int k=0; k<np; k++){
			pos = neg = 0;
			scanf("%d%d", &px, &py);
			for(int i=0; i<nr; i++){
				if(r[i].a*px+r[i].b*py+r[i].c > 0)
					pos++;
				else if(r[i].a*px+r[i].b*py+r[i].c < 0)
					neg++;
			}
			count += (pos*(pos-1)+neg*(neg-1))/2;
		}
		printf("Roadmap %d:\n", round);
		printf("Negative Builders Ltd. will build %d New Roads.\n", count);
		round++;
	}
	return 0;
}
