/* @JUDGE_ID:	 10319NX 343 C */
#include<stdio.h>
#include<string.h>

int val(int n[], int len, int p)
{
	int sum=0;
	int pos=1;
	int i;

	for(i=0; i<len; i++){
		sum += n[i]*pos;
		pos *= p;
	}
	return sum;
}

int main(void)
{
	char a[100], b[100];
	int aa[100], bb[100];
	int va, vb, pa, pb, la, lb;
	int i;

	while(scanf("%s%s", a, b) != EOF){
		la = strlen(a);
		lb = strlen(b);
		pa = pb = 0;
		for(i=0; i<la; i++){
			if(a[i]>='A' && a[i]<='Z')
				aa[la-i-1] = a[i]-'A'+10;
			else
				aa[la-i-1] = a[i]-'0';		
			if(aa[la-i-1] > pa)
				pa = aa[la-i-1];
		}
		for(i=0; i<lb; i++){
			if(b[i]>='A' && b[i]<='Z')
				bb[lb-i-1] = b[i]-'A'+10;
			else
				bb[lb-i-1] = b[i]-'0';		
			if(bb[lb-i-1] > pb)
				pb = bb[lb-i-1];
		}
		pa++;
		pb++;
		if(pa < 2)
			pa = 2;
		if(pb < 2)
			pb = 2;
		va = val(aa, la, pa);
		vb = val(bb, lb, pb);
		while(pa!=37 && pb!=37){
			if(va < vb){
				pa++;
				va = val(aa, la, pa);
			}
			else if(vb < va){
				pb++;
				vb = val(bb, lb, pb);
			}
			else
				break;
		}
		if(va == vb)
			printf("%s (base %d) = %s (base %d)\n", a, pa, b, pb);
		else
			printf("%s is not equal to %s in any base 2..36\n",a,b);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
