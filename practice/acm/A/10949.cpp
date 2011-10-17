// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#define N 20022

int pos[128][N];
int min[N];

int main()
{
	int times;
	int len[128];
	int sLen[2];
	char buf[N];

	fgets(buf, N, stdin);
	sscanf(buf, "%d", &times);
	for(int round=0; round<times; round++){
		// {{{ initialize
		int h, w;
		char map[55][55];
		fgets(buf, N, stdin);
		sscanf(buf, "%d%d\n", &h, &w);
		for(int i=0; i<h; i++)
			fgets(map[i], 25, stdin);
		for(int i=0; i<128; i++)
			len[i] = 0;
		int maxLen;
		maxLen = 0;
		// }}}
		for(int k=0; k<2; k++){
			int x, y, n;
			fgets(buf, N, stdin);
			sscanf(buf, "%d%d%d\n", &n, &x, &y);
			x--, y--;
			sLen[k] = n+1;
			fgets(buf, N, stdin);
			for(int i=0; i<=n; i++){
				int t=map[x][y];
				if(k == 0){
					pos[t][len[t]] = i;
					len[t]++;
				}else{
					int n=1;
					int old=-1;
					for(int j=0; j<len[t]; j++){
						int p=pos[t][j];
						if(p <= old)
							continue;
						while(p > min[n] && n <= maxLen)
							n++;
						old = min[n];
						min[n] = p;
						if(n > maxLen){
							maxLen++;
							break;
						}
					}
				}
				switch(buf[i]){ // {{{
					case 'S': x++; break;
					case 'N': x--; break;
					case 'E': y++; break;
					case 'W': y--; break;
				} // }}}
			}
		}
		printf("Case %d: %d %d\n", round+1, sLen[0]-maxLen, sLen[1]-maxLen);
	}
	return 0;
}
