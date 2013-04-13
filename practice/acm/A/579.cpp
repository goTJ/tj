#include<stdio.h>
#include<math.h>

int main()
{
	int h, m;
	double ha, ma, d;

	while(scanf("%d:%d", &h, &m) == 2){
		if(!h && !m)
			break;
		ha = (h%12)*30+m*0.5;
		ma = m*6;
		d = ha>ma ? ha-ma : ma-ha;
		printf("%.3lf\n", (d>180.0)?(360-d):d);
	}
	return 0;
}
