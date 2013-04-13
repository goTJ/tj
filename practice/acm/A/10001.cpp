// vim:ft=cpp:foldmethod=marker
#include <cstdio>

int main()
{
	int k, n;
	char s[50];
	int path[2][8];
	int now, next;
	int way[8][2];

	for(int i=0; i<8; i++){
		way[i][0] = ((i&3)<<1);
		way[i][1] = ((i&3)<<1)+1;
	}
	while(scanf("%d%d%s", &k, &n, s) == 3){
////		printf("%x, %d, %s\n", k, n, s);
		now = 0, next = 1;
		for(int i=0; i<8; i++)
			path[now][i] = 0;
		for(int i=0; i<8; i++){
			if(((k&(1<<i))>>i) == s[n-1]-'0'){
////				printf("yes(%d): %d %d\n", i, ((k&(1<<i))>>i), s[n-1]-'0');
				path[now][way[i][0]] |= (1<<i);
				path[now][way[i][1]] |= (1<<i);
			}
		}
////		for(int i=0; i<8; i++)
////			printf("%x ", path[now][i]);
////		printf("\n");
		for(int i=0; i<n-1; i++){
////			printf("i: %d\n", i);
			for(int j=0; j<8; j++)
				path[next][j] = 0;
			for(int j=0; j<8; j++){
				if(((k&(1<<j))>>j) == s[i]-'0'){
////					printf("yes(%d): %d %d, %x\n", j, ((k&(1<<j))>>j), s[i]-'0', path[now][j]);
					path[next][way[j][0]] |= path[now][j];
					path[next][way[j][1]] |= path[now][j];
				}
			}
			int t; // for swap
			t = now; now = next; next = t;
////			for(int j=0; j<8; j++)
////				printf("%x ", path[now][j]);
////			printf("\n");
		}
		int yes=0;
		for(int i=0; i<8; i++){
			if(path[now][i] & (1<<i)){
				yes = 1;
				break;
			}
		}
		if(yes){
			printf("REACHABLE\n");
		}else{
			printf("GARDEN OF EDEN\n");
		}
	}
	return 0;
}
