/* @JUDGE_ID:   10319NX 10262 C++ "¶ã....¬y²\¤FQ_Q" */
#include<stdio.h>
#include<string.h>

int getline(char s[])
{
	char c;
	int l=0;

	while((c=getchar()) != EOF){
		if(c == '\n'){
			s[l] = '\0';
			return l;
		}
		s[l++] = c;
	}
	return -1;
}

int fstrcmp(char a[], int la, char b[], int lb)
{
	if(la > lb)
		return 1;
	else if(la < lb)
		return -1;
	else{
		for(la--; la>=0; la--){
			if(a[la] > b[la])
				return 1;
			else if(a[la] < b[la])
				return -1;
		}
		return 0;
	}
}

void gogo(int *m, int *f, int lim, int val)
{
	*f += val;
	if(*m > lim){
		*f = 0;
		return;
	}
	if(*f == 2){
		*m += 1;
		*f = 0;
	}
	else if(*f == -1){
		*m -= 1;
		*f = 1;
	}
}

void reverse(char s[], int len, char t[])
{
	int i;

	for(i=0; i*2<=len; i++){
		t[i] = s[len-i-1];
		t[len-i-1] = s[i];
	}
}

int next(char s[], int len, char p[], int *lp, int m, int flag)
{
	int pre, last;

////	printf("go next: %s  m: %d  flag: %d   lp: %d   len: %d\n", s, m, flag, *lp, len);
	pre = m-1+flag;
	if(m == len){
////		printf("new: %d\n", *lp);
		if(*lp == len){
			p[*lp] = 'a';
			*lp += 1;
		}
		else if(p[*lp-1] == 'z'){
			p[*lp-1] = 'a';
			p[*lp] = 'a';
			*lp += 1;
			p[*lp] = '\0';
		}
		else
			p[*lp-1]++;
	}
	else{
////		printf("add: %d\n", pre);
		for(last = m+1; s[last]; last++){
			if(pre==-1 || s[pre]!=s[last])
				return 0;
			pre--;
		}
////		printf("pre: %d\n", pre);
		for(; *lp<=pre; (*lp)++)
			p[*lp] = s[*lp];
		p[*lp] = '\0';
	}
////	printf("%s\n", p);
	return 1;
}

int main(void)
{
	char a[1001], b[1001];
	char pa[1001], pb[1001];
	int la, lb, lpa, lpb;
	int aa, bb, fa, fb, ra, rb;
	int t;

	while(getline(a)!=-1 && getline(b)!=-1 != EOF){
	////	printf("%s %s\n", a, b);
		la = strlen(a);
		lb = strlen(b);
		lpa = lpb = 0; pa[0] = pb[0] = '\0';
		aa = la/2; bb = lb/2; fa = la%2; fb = lb%2;
		gogo(&aa, &fa, la, -1); gogo(&bb, &fb, lb, -1);
		while(!next(a, la, pa, &lpa, aa, fa))
			gogo(&aa, &fa, la, 1);
		while(!next(b, lb, pb, &lpb, bb, fb))
			gogo(&bb, &fb, lb, 1);
		ra = rb = t = fstrcmp(pa, lpa, pb, lpb);
////		printf("ra: %d   rb: %d\n", ra, rb);
////		printf("%s: %d %d    %s: %d %d\n", pa, aa, fa, pb, bb, fb);
////		printf("t: %d     %d  %d ----- %d  %d\n", t, lpa, lpb, ra, rb);
////		printf("\n");
		while(1){
////			printf("[%s]: %d %d    [%s]: %d %d\n", pa, aa, fa, pb, bb, fb);
			t = fstrcmp(pa, lpa, pb, lpb);
////			printf("t: %d     %d  %d ----- %d  %d\n", t, lpa, lpb, ra, rb);
			if(t<0 && ra)
				break;
			if(t>0 && rb)
				break;
			if(lpa>la && lpb>lb && !t)
				break;
			ra = rb = t;
			if(t < 0){
////				printf("a gogo\n");
				gogo(&aa, &fa, la, 1);
				while(!next(a, la, pa, &lpa, aa, fa))
					gogo(&aa, &fa, la, 1);
			}
			else{
////				printf("b gogo\n");
				gogo(&bb, &fb, lb, 1);
				while(!next(b, lb, pb, &lpb, bb, fb))
					gogo(&bb, &fb, lb, 1);
			}
		}
////		printf("\t%s %s\n", pa, pb);
		if(t<0 && ra)
			for(lpa--; lpa>=0; lpa--)
				printf("%c", pa[lpa]);
		else if(t>0 && rb)
			for(lpb--; lpb>=0; lpb--)
				printf("%c", pb[lpb]);
		else if(lpa>la && lpb>lb && !t)
			printf("No solution.");
		printf("\n");
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
