#include<stdio.h>
#include<stdlib.h>

typedef struct{
	int x;
	int y;
}coor;

coor icon[53];

struct fly{
	int level;
	char code;
	coor ul;
	coor dr;
}region[27];

int rlim, ilim;

int comp(const void *p, const void *q)
{
	return ((struct fly *)q)->level-((struct fly *)p)->level;
}

void insert_region(void)
{
	scanf("%d%d%d%d\n", &region[rlim].ul.x, &region[rlim].ul.y,
			&region[rlim].dr.x, &region[rlim].dr.y);
	region[rlim].level = rlim;
	region[rlim].code = rlim+'A';
	rlim++;
	qsort(region, rlim, sizeof(region[0]), comp);
}

void insert_icon(void)
{
	scanf("%d%d\n", &icon[ilim].x, &icon[ilim].y);
	ilim++;
}

int in_region(int n, int x, int y)
{
	int lx=region[n].ul.x, rx=region[n].dr.x
		, uy=region[n].ul.y, dy=region[n].dr.y;
	if(x < lx || x > rx)
		return 0;
	if(y < uy || y > dy)
		return 0;
	return 1;
}

void up_region(int n)
{
	int i;

	for(i=0; i<n; i++)
		region[i].level--;
	region[n].level = rlim-1;
	qsort(region, rlim, sizeof(region[0]), comp);
}

int sol(int a, int b)
{
	if(a > b)
		return a-b;
	else
		return b-a;
}

int d_icon(int n, int x, int y)
{
	int tx, ty;

	tx = sol(icon[n].x, x);
	ty = sol(icon[n].y, y);

	return tx*tx+ty*ty;
}

int main(void)
{
	char c;
	int i, flag, tx, ty;
	int d_max, t;

	rlim = ilim = 0;
	while((c=getchar())!='#' && c!=EOF){
		if(c == 'R')
			insert_region();
		else if(c == 'I')
			insert_icon();
		else{
			flag = 1;
			scanf("%d%d\n", &tx, &ty);
			for(i=0; i<rlim; i++)
				if(in_region(i, tx, ty)){
					printf("%c\n", region[i].code);
					up_region(i);
					flag = 0;
					break;
				}
			if(flag){
				d_max = 300000;
				for(i=0; i<ilim; i++)
					if((t=d_icon(i, tx, ty)) < d_max)
						d_max = t;
				for(i=0; i<ilim; i++)
					if((t=d_icon(i, tx, ty)) == d_max)
						printf("%3d", i+1);
				printf("\n");
			}
		}
	}
	return 0;
}
