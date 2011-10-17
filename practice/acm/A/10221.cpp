/* @JUDGE_ID:   10319NX 10221 C++ "¦ó¦Ì¯S^^" */
#include<stdio.h>
#include<math.h>

int main(void)
{
	double angle, dis;
	char unit[10];

	while(scanf("%lf%lf%s", &dis, &angle, unit) != EOF){
		if(unit[0] == 'm')
			angle /= 60.0;
		angle = fmod(angle, 360.0);
		angle = (angle < 360.0-angle) ? angle : 360.0-angle;
		dis += 6440.0;
		angle = angle*acos(-1)/180.0;
		printf("%lf %lf\n", dis*angle, 2*dis*sin(angle/2));
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
