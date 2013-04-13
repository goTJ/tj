#include<stdio.h>
#include<string.h>
#define MAX 10005
char str[MAX];
char* findx(char *temp, char *target){
	int i;
	char *te, *pe;
	char tem[3];
	te =temp;
	for(i=0; target[i]; i++){
		tem[0] = target[i];
		tem[1] = 0;
		if((pe = strstr(te,tem))==NULL)
			return NULL;
		te = pe+1;
	}
	return te;
}
int main(){
	while(fgets(str,10005, stdin)!=NULL){
		int ans = 0;
		char list[] = "dccclxxxviii";
		int alpha[256];
		alpha['d'] = 500;
		alpha['c'] = 100;
		alpha['l'] = 50;
		alpha['x'] = 10;
		alpha['v']  =5;
		alpha['i'] = 1;
		char *temp, *p;
		temp = str;
		while((p = strstr(temp,"m")) != NULL){
			temp  = p+1;
			ans += 1000;
		}
		for(int i=0; i< 4;i++){
			char te[3];
			te[0] = list[i];
			te[1] = 0;
			if( (p = strstr(temp,te)) !=NULL){
				temp = p+1;
				ans += alpha[list[i]];
			}

		}
		if((p = findx(temp, "xc"))!=NULL){
			temp = p;
			ans += 90;
		}
		else {
			for(int i=4; i< 8;i++){
				char te[3];
				te[0] = list[i];
				te[1] = 0;
				if( (p = strstr(temp,te)) !=NULL){
					temp = p+1;
					ans += alpha[list[i]];
				}
			}
		}
		if((p=findx(temp,"ix"))!=NULL){
			temp = p;
			ans +=9;
		}
		else{

			for(int i=8; i< 12;i++){
				char te[3];
				te[0] = list[i];
				te[1] = 0;
				if( (p = strstr(temp,te)) !=NULL){
					temp = p+1;
					ans += alpha[list[i]];
				}
			}
		}
		printf("%d\n",ans);
	}
}
