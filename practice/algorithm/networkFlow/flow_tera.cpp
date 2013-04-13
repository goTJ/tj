#include <stdio.h>
#include <limits.h>

#define MIN(a,b)	(a>b?b:a)

int main()
{
	int nodes,edges;
	int weight[300][300];
	int source,target;
	int flow;

	scanf ("%d%d",&nodes,&edges);

	for (int tmpa=0;tmpa<nodes;tmpa++)
		for (int tmpb=0;tmpb<nodes;tmpb++)
			weight[tmpa][tmpb] = 0;

	for (int tmp=0;tmp<edges;tmp++){
		int from,to,weigh;
		scanf ("%d%d%d",&from,&to,&weigh);
		weight[from-1][to-1] = (weight[to-1][from-1] += weigh);
	}

	scanf ("%d%d",&source,&target);

	source--; target--;

	flow = 0;

	while(1){
		int cap[300];
		int from[300];
		bool togo[300];

		for (int tmp=0;tmp<nodes;tmp++){
			cap[tmp] = 0;
			from[tmp] = -1;
			togo[tmp] = false;
		}
		cap[source] = INT_MAX;
		togo[source] = true;

		for (int tmp=0;tmp<nodes;tmp++){
			int extract = 0;
			for (int tmp2=0;tmp2<nodes;tmp2++)
				if (!togo[extract] || (togo[tmp2] && cap[tmp2]>cap[extract]))
					extract = tmp2;
			if (!togo[extract])
				break;
			togo[extract] = false;
			for (int tmp2=0;tmp2<nodes;tmp2++)
				if (MIN(weight[extract][tmp2],cap[extract])>cap[tmp2]){
					cap[tmp2] = MIN(weight[extract][tmp2],cap[extract]);
					from[tmp2] = extract;
					togo[tmp2] = true;
				}
			if (cap[target])
				break;
		}
		if (!cap[target])
			break;

		flow += cap[target];

		int tmp = target;
		while (from[tmp] != -1){
			weight[from[tmp]][tmp] -= cap[target];
			weight[tmp][from[tmp]] += cap[target];
			tmp = from[tmp];
		}
	}
	printf ("%d\n",flow);

	return 0;
}
