#include<cstdio>
#include<cctype>
#include<cstring>

bool find(char s[], int len, int dir[2], int r, int c, char map[][55])
{
	for(int i=0; i<len ;i++,r+=dir[0],c+=dir[1]){
		if(map[r][c] != s[i])
			return false;
	}
	return true;
}

int main()
{
	int round;
	scanf("%d", &round);
	for(int times=0; times<round; times++){
		char map[55][55];
		int maxRow, maxCol;
		scanf("%d%d", &maxRow, &maxCol);
		for(int i=0; i<maxRow; i++){
			scanf("%s", map[i]);
			for(int j=0; map[i][j]; j++)
				map[i][j] = tolower(map[i][j]);
		}
		int n;
		scanf("%d", &n);
		for(int i=0; i<n; i++){
			char word[55];
			int wLen;
			scanf("%s", word);
			wLen = strlen(word);
			for(int j=0; word[j]; j++)
				word[j] = tolower(word[j]);
			bool isFound=false;
			int r, c;
			for(r=0; r<maxRow; r++){
				for(c=0; c<maxCol; c++){
					int dir[8][2] = { {1,1}, {1,0}, {1,-1}, {0,1}, {0,-1}, {-1,1}, {-1,0}, {-1,-1}};
					for(int j=0; !isFound && j<8; j++)
						isFound = find(word, wLen, dir[j], r, c, map);
					if(isFound)
						break;
				}
				if(isFound)
					break;
			}
			printf("%d %d\n", r+1, c+1);
		}
	}
	return 0;
}
