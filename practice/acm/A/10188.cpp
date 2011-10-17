#include<cstdio>
#include<cctype>
#include<cstring>

int main()
{
	int round=1;
	char ans[100][122];
	char num[12200];
	int nl, np;
	int n, m;
	bool ac, pe;
	char buf[122];

	while(fgets(buf, 122, stdin) != NULL){
		sscanf(buf, "%d", &n);
		if(!n)
			break;
		// initialize
		ac = pe = true;
		nl = np = 0;
		// read ans data
		for(int i=0; i<n; i++){
			fgets(ans[i], 122, stdin);
			for(int j=0; ans[i][j]!='\0'; j++){
				if(isdigit(ans[i][j]))
					num[nl++] = ans[i][j];
			}
		}
		num[nl] = '\0';
		// read test data
		fgets(buf, 122, stdin);
		sscanf(buf, "%d", &m);
		for(int i=0; i<m; i++){
			if(m != n)
				ac = false;
			fgets(buf, 122, stdin);
			if(ac && strcmp(buf, ans[i]))
				ac = false;
			for(int j=0; buf[j]!='\0'; j++){
				if(isdigit(buf[j])){
					if(num[np] != buf[j])
						pe = false;
					np++;
				}
			}
		}
		if(np != nl)
			pe = false;
		// output
		if(ac){
			printf("Run #%d: Accepted\n", round);
		}else if(pe){
			printf("Run #%d: Presentation Error\n", round);
		}else{
			printf("Run #%d: Wrong Answer\n", round);
		}
		round++;
	}
	return 0;
}
