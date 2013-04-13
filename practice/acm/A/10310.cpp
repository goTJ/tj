/* @JUDGE_ID:   10387ZJ 10310 C++ */
#include<stdio.h>

typedef struct coor{
	double x;
	double y;
}coor;

double dis(coor a, coor b)
{
	double x=a.x-b.x, y=a.y-b.y;

	return x*x+y*y;
}

int main(void)
{
	int n, i;
	int notFind;
	coor gopher, dog, hole;

	while(scanf("%d%lf%lf%lf%lf", &n, &gopher.x, &gopher.y, &dog.x, &dog.y) != EOF){
		notFind = 1;
		for(i=0; i<n; i++){
			scanf("%lf%lf", &hole.x, &hole.y);
			if(notFind && dis(gopher, hole)*4-dis(dog, hole) <= 0){
				notFind = 0;
				printf("The gopher can escape through the hole at (%.3f,%.3f).\n", hole.x, hole.y);
			}
		}
		if(notFind)
			printf("The gopher cannot escape.\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
