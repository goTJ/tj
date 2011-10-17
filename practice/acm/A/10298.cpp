#include<stdio.h>
#include<string.h>

int main()
{
	char s[1000006];
	int len;
	int i, j, k;
	bool flag;

	while(scanf("%s", s) != EOF && s[0]!='.'){
		len = strlen(s);
		for(i=1; i<=len; i++){
			if(!(len%i)){
				flag = true;
				for(j=0; j<i; j++){
					for(k=i+j; k<len; k+=i){
						if(s[k] != s[j])
							flag = false;
					}
				}
				if(flag)
					break;
			}
		}
		printf("%d\n", len/i);
	}
	return 0;
}
