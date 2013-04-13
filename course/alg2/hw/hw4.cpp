// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<string.h>
#include<time.h>

#define MAX(a, b)	(a>b)?a:b
#define MIN(a, b)	(a<b)?a:b

#define MAX_CHAR_NUM 256
#define MAX_PATTERN_LEN 36
#define MAX_TEXT_SIZE 1050000
#define MAX_PATTERN_NUM 106

// {{{ void initializeBmBc(char ptn[], int len, int bmBc[])
void initializeBmBc(char ptn[], int len, int bmBc[])
{
	int i;

	for(i=0; i<MAX_CHAR_NUM; i++)
		bmBc[i] = len;
	for(i=0; i<len-1; i++)
		bmBc[ptn[i]] = len-i-1;
} // }}}
// {{{ void suffixes(char ptn[], int len, int suff[])
void suffixes(char ptn[], int len, int suff[])
{
	int last_match, match_h, i;

	suff[len-1] = len;
	match_h = len-1;
	for(i=len-2; i>=0; --i) {
		if (i > match_h && suff[i+len-1-last_match] < i-match_h){
			suff[i] = suff[i+len-1-last_match];
		}else{
			if (i < match_h)
				match_h = i;
			last_match = i;
			while (match_h >= 0 && ptn[match_h] == ptn[match_h+len-1-last_match])
				match_h--;
			suff[i] = last_match-match_h;
		}
	}
} // }}}
// {{{ void initializeBmGs(char ptn[], int len, int bmGs[])
void initializeBmGs(char ptn[], int len, int bmGs[])
{
	int i, j, suff[MAX_PATTERN_LEN];
	int k;

	suffixes(ptn, len, suff);
	/*
	printf("suff:\n");
	for(i=0; i<len; i++)
		printf("%2d ", i);
	printf("\n");
	for(i=0; i<len; i++)
		printf(" %c ", ptn[i]);
	printf("\n");
	for(i=0; i<len; i++)
		printf("%2d ", suff[i]);
	printf("\n");
	*/

	for(i=0; i<len; i++)
		bmGs[i] = len;
	for(i=len-1, j=0; i>=-1; i--){
		if (i == -1 || suff[i] == i+1){
			for (; j<len-1-i; j++){
				if (bmGs[j] == len)
					bmGs[j] = len-1-i;
			}
		}
		/*
		printf("%d\n", i);
		for(k=0; k<len; k++)
			printf("%2d ", bmGs[k]);
		printf("\n");
		*/
	}
	for(i=0; i<=len-2; i++)
		bmGs[len-1-suff[i]] = len-1-i;
	/*
	for(k=0; k<len; k++)
		printf("%2d ", bmGs[k]);
	printf("\n");
	*/
} // }}}
// {{{ void turboBM(char ptn[], int pl, char str[], int sl)
int turboBM(char ptn[], int pl, char str[], int sl)
{
	int bcShift, i, j, shift, u, v, turboShift;
	int bmGs[MAX_PATTERN_LEN], bmBc[MAX_CHAR_NUM];

	/* Preprocessing */
	initializeBmGs(ptn, pl, bmGs);
	initializeBmBc(ptn, pl, bmBc);

	/* Searching */
	j = u = 0;
	shift = pl;
	while(j <= sl-pl){
		i = pl-1;
		while(i >= 0 && ptn[i] == str[i+j]){
			i--;
			if (u != 0 && i == pl-1-shift)
				i -= u;
		}
		if(i < 0) {
			return 1;
		}else{
			v = pl-1-i;
			turboShift = u-v;
			bcShift = bmBc[str[i+j]]-pl+1+i;
			shift = MAX(turboShift, bcShift);
			shift = MAX(shift, bmGs[i]);
			if(shift == bmGs[i])
				u = MIN(pl-shift, v);
			else{
				if(turboShift < bcShift)
					shift = MAX(shift, u+1);
				u = 0;
			}
		}
		j += shift;
	}
	return 0;
} // }}}
// {{{ int tunedBM(char ptn[], int pl, char str[], int sl)
int tunedBM(char ptn[], int pl, char str[], int sl)
{
	int j, k, shift, bmBc[MAX_CHAR_NUM];

	initializeBmBc(ptn, pl, bmBc);
	shift = bmBc[ptn[pl-1]];
	bmBc[ptn[pl-1]] = 0;
	memset(str+sl, ptn[pl-1], pl);

	j = 0;
	while(j < sl){
		k = bmBc[str[j+pl-1]];
		while(k !=  0){
			j += k;
			j += bmBc[str[j+pl-1]];
			j += bmBc[str[j+pl-1]];
			j += bmBc[str[j+pl-1]];
			j += bmBc[str[j+pl-1]];
			k = bmBc[str[j+pl-1]];
		}
		if(memcmp(ptn, str+j, pl-1) == 0 && j < sl)
			return 1;
		j += shift;
	}
	return 0;
}
// }}}

int main()
{
	char str[MAX_TEXT_SIZE];
	char ptn[MAX_PATTERN_NUM][MAX_PATTERN_LEN];
	int is_match[MAX_PATTERN_NUM];
	int i, n, sl;
	clock_t begin, end;

	freopen("input.txt", "r", stdin);
	scanf("%s", str);
	for(i=0; str[i]!='!'; i++)
		;
	str[i] = '\0';
	sl = strlen(str);

	freopen("pattern.txt", "r", stdin);
	scanf("%d", &n);
	for(i=0; i<n; i++)
		scanf("%s", ptn[i]);
	begin = clock();
	for(i=0; i<n; i++){
		is_match[i] = tunedBM(ptn[i], strlen(ptn[i]), str, sl);
	}
	end = clock();
	for(i=0; i<n; i++){
		if(is_match[i])
			printf("%s\n", ptn[i]);
	}
	printf("time(tunedBM): %.3lf sec\n", ((double)end-begin)/CLOCKS_PER_SEC);
	begin = clock();
	for(i=0; i<n; i++){
		is_match[i] = turboBM(ptn[i], strlen(ptn[i]), str, sl);
	}
	end = clock();
	printf("time(turboBM): %.3lf sec\n", ((double)end-begin)/CLOCKS_PER_SEC);
	return 0;
}
