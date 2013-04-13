/* @JUDGE_ID:   10387ZJ 306 C */
#include<stdio.h>
#include<string.h>

int key[32][205];
int lim;

int getline(char in[])
{
	int l=0;
	char c;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			in[l] = '\0';
			return l;
		}
		in[l++] = c;
	}
	return -1;
}

void make_key(void)
{
	int i, j;

	for(i=0; i<lim; i++){
		scanf("%d", &key[0][i]);
		key[0][i]--;
	}
	for(i=1; i<32; i++){
		for(j=0; j<lim; j++)
			key[i][j] = key[i-1][key[i-1][j]];
	}
}

int main(void)
{
	char in[205], tmp[205];
	int times;
	int i, p;

	while(scanf("%d", &lim) != EOF){
		if(!lim)
			break;
		make_key();
		while(scanf("%d", &times) != EOF){
			if(!times)
				break;
			getchar();
			for(i=getline(in); i<lim; i++)
				in[i] = ' ';
			p = 0;
			while(times){
				if(times & 1){
					for(i=0; i<lim; i++)
						tmp[key[p][i]] = in[i];
					for(i=0; i<lim; i++)
						in[i] = tmp[i];
				}
				p++;
				times >>= 1;
			}
			for(i=0; i<lim; i++)
				printf("%c", in[i]);
			printf("\n");
		}
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
