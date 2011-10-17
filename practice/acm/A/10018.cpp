#include<stdio.h>
#include<string.h>

inline void reverse(char s[], int len)
{
	char tmp;
	for(int i=0; i<len/2; i++){
		tmp = s[i];
		s[i] = s[len-i-1];
		s[len-i-1] = tmp;
	}
}

inline void swap(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}

bool isPalindrome(char s[], int len)
{
	for(int i=0; i<=len/2; i++){
		if(s[i] != s[len-i-1])
			return false;
	}
	return true;
}

int main()
{
	int round;
	scanf("%d", &round);
	for(int times=0; times<round; times++){
		char s[2][12];
		int now=0, next=1;
		scanf("%s", s[now]);
		int len=strlen(s[now]);;
		reverse(s[now], len);
		for(int i=0; i<len; i++)
			s[now][i] -= '0';
		int count=0;
		do{
			for(int i=0; i<len; i++){
				s[next][i] = s[now][i]+s[now][len-i-1];
			}
			for(int i=0; i<len-1; i++){
				if(s[next][i] >= 10){
					s[next][i+1] += s[next][i]/10;
					s[next][i] %= 10;
				}
			}
			if(s[next][len-1] >= 10){
				s[next][len] = s[next][len-1]/10;
				s[next][len-1] %= 10;
				len++;
			}
			swap(now, next);
			count++;
		}while(!isPalindrome(s[now], len));
		printf("%d ", count);
		for(int i=0; i<len; i++)
			printf("%d", s[now][i]);
		printf("\n");
	}
	return 0;
}
