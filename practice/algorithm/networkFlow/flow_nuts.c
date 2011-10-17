#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int flow[301][301];
int capacity[301][301];
int source,sink,n;


int fijkstra(int path[]){
	int from[300],D[300],mark[300];
	int rpath[300];
	int i,j,max,next,shortcut;
	for(i=1;i<=n;i++){
		if(i==source){
			D[i]=0;
			from[i]=i;
			mark[i]=1;
			continue;
		}
		D[i] = capacity[source][i]-flow[source][i];
		from[i]=source;
		mark[i]=0;
	}
	while(mark[sink]==0){
		max=0;
		next=0;
		for(i=1;i<=n;i++){
			if(mark[i]==0 && D[i] > max){
				max=D[i];
				next=i;
			}
		}
		if(max==0)
			return 0;
		mark[next]=1;
		for(i=1;i<=n;i++){
			if(mark[i]==0){
				shortcut=capacity[next][i]-flow[next][i];
				if(D[i] < ((D[next] < shortcut)?D[next]:shortcut) ){
					from[i] = next;
					D[i] = (D[next] < shortcut)?D[next]:shortcut;
				}
			}
		}
	}
	rpath[0]=sink;
	for(i=sink,j=1;i!=source;i=from[i],j++)
		rpath[j]=from[i];
	for(i=0;i<j;i++)
		path[i] = rpath[j - i -1];
	path[j]=0;
	return D[sink];
}


int main(void){
	int m,i,u,v,w,flows,add;
	int path[300];
	scanf("%d%d",&n,&m);
	memset(flow,0,sizeof(flow));
	memset(capacity,0,sizeof(capacity));
	for(i=0;i<m;i++){
		scanf("%d%d%d",&u,&v,&w);
		capacity[u][v]+=w;
		capacity[v][u]+=w;            
	}
	scanf("%d%d",&source,&sink);

	/*maxflow*/
	flows=0;
	add=fijkstra(path);
	while(add!=0){
		for(i=0;path[i]!=sink;i++){
			flow[ path[i] ][ path[i+1] ] += add;
			flow[ path[i+1] ][ path[i] ] -= add;
		}
		flows+=add;
		add=fijkstra(path);
	}
	printf("%d\n",flows);
	return 0;
}
