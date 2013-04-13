#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define maxV 500001                                  //max countber of vertex
#define bool char
#define true 1
#define false 0

typedef struct graphnode{                        //Declare structure of vertex
	int vertex;
	struct graphnode *next;
}Node;
Node *node[maxV];                             //node[] is the node vertex and z is the tail vertex 
Node *nodeT[maxV];                             //node[] is the node vertex and z is the tail vertex 

FILE *fp;

void initial(int nv);                            //initial the adjacency-list
void adjlist(void);                              //Build the adjacency-list of input graph
void dumplist(int nv);                           //Show the adjacency-list

void strongly_connect_component(int nv);

int main()
{
	int nv;
	clock_t begin, end;

	fp= fopen("input.txt","r");
	fscanf(fp,"%d",&nv);
	initial(nv);
	adjlist();
////	printf("\n       Adjacency LIST\n");
////	dumplist(nv);
	begin = clock();
	strongly_connect_component(nv);
	end = clock();
	printf("time: %.2fms\n", (double)(end-begin)/CLOCKS_PER_SEC*1000);
	return 0;
}


void initial(int nv)
{
	int i;

	for(i=1;i<=nv;i++){
		node[i] = (Node *)malloc(sizeof(Node));
		node[i]->next=NULL;
		nodeT[i] = (Node *)malloc(sizeof(Node));
		nodeT[i]->next=NULL;
	}

}

void adjlist(void)
{
	int v1,v2;
	Node *ptr;

////	printf("    EDGEs\n");
	while(fscanf(fp,"%d%d",&v1,&v2)!=EOF){
////		printf("         %2d ---> %2d\n",v1,v2);
		ptr = (Node *)malloc(sizeof(Node));
		ptr->vertex = v2;
		ptr->next = node[v1]->next;
		node[v1]->next = ptr;

		ptr = (Node *)malloc(sizeof(Node));
		ptr->vertex = v1;
		ptr->next = nodeT[v2]->next;
		nodeT[v2]->next = ptr;

		// Remove comment for d-graph
		//   ptr=(Node *) malloc(sizeof(Node));
		//	 ptr->vertex=v1;
		//	 ptr->next=node[v2]->next;
		//	 node[v2]->next=ptr;
	}
	fclose(fp);
}

void dumplist(int nv)
{
	int i;
	Node *ptr;

	for(i=1; i<=nv; i++){
		ptr = node[i]->next;
		printf("    node[%2d]  ",i);
		while(ptr != NULL){
			printf("  -->%2d", ptr->vertex);
			ptr=ptr->next;
		}
		printf("\n");
	}
	printf("\n");
}

void strongly_connect_component(int nv)
{
	int i;
	Node *p;
	Node s[maxV];
	int pos;
	bool used[maxV];
	int f[maxV];
	int count;

	// initialize used array
	for(i=1; i<=nv; i++)
		used[i] = false;
	// first DFS to trace G by simulating stack method
	count = 1;
	for(i=1; i<=nv; i++){
		if(!used[i]){
			used[i] = true;
			pos = 0;
			s[pos].vertex = i;
			s[pos].next = node[i]->next;
			pos++;
			while(pos){
				while(s[pos-1].next){
					if(!used[s[pos-1].next->vertex]){
						used[s[pos-1].next->vertex] = true;
						s[pos].vertex = s[pos-1].next->vertex;
						s[pos].next = node[s[pos].vertex]->next;
						s[pos-1].next = s[pos-1].next->next;
						pos++;
					}else{
						s[pos-1].next = s[pos-1].next->next;
					}
				}
				f[count++] = s[pos-1].vertex;
				pos--;
			}
		}
	}
	// initialize used and reachable array
	for(i=1; i<=nv; i++)
		used[i] = false;
	pos = 0;
	// second DFS to trace G^T by simulating stack method
	for(i=nv; i>=1; i--){
		if(!used[f[i]]){
			used[f[i]] = true;
			s[pos++].vertex = f[i];
			count = 0;
			while(pos){
				pos--;
				p = nodeT[s[pos].vertex]->next;
				while(p){
					if(!used[p->vertex]){
						s[pos++].vertex = p->vertex;
						used[p->vertex] = true;
					}
					p = p->next;
				}
				count++;
			}
			printf("%d ", count);
		}
	}
	printf("\n");
}
