/* @JUDGE_ID:   10319NX 200 C */
#include<stdio.h>
#include<string.h>

struct fly{
	int after[50];
	int lim;
	int before;
}order[30];

void initial(void)
{
	int i;

	memset(order, 0, sizeof(order));
	for(i=0; i<30; i++)
		order[i].before = -1;
}

int main(void)
{
	char in[30], b[30];
	int i, j, n, m;
	int flag;

	b[0] = '\0';
	initial();
	while(scanf("%s", in) != EOF){
		if(in[0] == '#'){
			flag = 1;
			while(flag){
				flag = 0;
			   for(i=0; i<26; i++){
				if(order[i].before == 0){
				     order[i].before = -1;
				     printf("%c", i+'A');
				     flag = 1;
				     for(j=0; j<order[i].lim; j++)
					order[order[i].after[j]].before--;
				}
			   }
			}
			printf("\n");
			return 0;
		}
		else{
			i = 0;
			while(b[i]!='\0' && in[i]==b[i])
				i++;
			n = in[i]-'A';
			m = b[i]-'A';
			if(b[i]){
				flag = 1;
				for(j=0; j<order[m].lim; j++)
					if(order[m].after[j] == n)
						flag = 0;
				if(flag){
					if(order[n].before == -1)
						order[n].before = 1;
					else
						order[n].before++;
					order[m].after[order[m].lim++] = n;
				}
			}
			else if(order[n].before == -1){
				order[n].before = 0;
			}
			strcpy(b, in);
		}
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
