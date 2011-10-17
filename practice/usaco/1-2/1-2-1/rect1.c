/*### PROGRAM*/
/*
ID: flyherm003
PROG: rect1
start: 
finish: 
*/
#include<stdio.h>
#include<string.h>

struct fly{
	int lr;
	int lc;
	int ur;
	int uc;
	int color;
}tab[100000];
int lim;
int row_max, col_max;
int area[2503];

void add_tab(int lr, int lc, int ur, int uc, int color)
{
	tab[lim].lr = lr;
	tab[lim].lc = lc;
	tab[lim].ur = ur;
	tab[lim].uc = uc;
	tab[lim++].color = color;
	area[color] += (ur-lr)*(uc-lc);
}

void del_tab(int n)
{
	area[tab[n].color] -= (tab[n].ur-tab[n].lr) * (tab[n].uc-tab[n].lc);
	tab[n] = tab[--lim];
}

void initial(void)
{
	lim = 0;
	memset(area, 0, sizeof(area));
	add_tab(0, 0, row_max, col_max, 1);
}

void cut(int lr,int lc,int ur,int uc,int color,int j)
{
	int i, tmp;
	int tlr=tab[j].lr, tlc=tab[j].lc, tur=tab[j].ur, tuc=tab[j].uc
		, tcolor=tab[j].color;

	if(tlr < lr && tuc>tlc){
		tmp = (lr<tur) ? lr : tur;
		add_tab(tlr, tlc, tmp, tuc, tcolor);
		tlr = tmp;
	}
	if(tur > ur && tuc>tlc){
		tmp = (ur>tlr) ? ur : tlr;
		add_tab(tmp, tlc, tur, tuc, tcolor);
		tur = tmp;
	}
	if(tlc < lc && tur>tlr){
		tmp = (lc<tuc) ? lc : tuc;
		add_tab(tlr, tlc, tur, tmp, tcolor);
		tlc = tmp;
	}
	if(tuc > uc && tur>tlr){
		tmp = (uc>tlc) ? uc : tlc;
		add_tab(tlr, tmp, tur, tuc, tcolor);
		tuc = tmp;
	}
}

int main(void)
{
	int n, i;
	int tlr, tlc, tur, tuc, tcolor;
	int j, tlim;
	int k;
	freopen("rect1.in",  "r",  stdin); 
	freopen("rect1.out",  "w",  stdout); 

	while(scanf("%d%d%d", &row_max, &col_max, &n) != EOF){
		initial();
		for(i=0; i<n; i++){
			scanf("%d%d%d%d%d", &tlr, &tlc, &tur, &tuc, &tcolor);
			tlim = lim;
			add_tab(tlr, tlc, tur, tuc, tcolor);
			for(j=0; j<tlim; j++)
				cut(tlr, tlc, tur, tuc, tcolor, j);
			for(j=0; j<tlim; j++)
				del_tab(j);
		}
		for(i=0; i<=2500; i++)
			if(area[i])
				printf("%d %d\n", i, area[i]);
	}
	return 0;
}
/*### END*/
