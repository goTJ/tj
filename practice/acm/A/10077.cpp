#include<cstdio>

class Fraction{
public:
	Fraction(){
		m=0, n=1;
	}
	Fraction(int tm, int tn){
		int tmp=gcd(tm,tn);
		m=tm/tmp, n=tn/tmp;
	}
	bool operator==(Fraction& a){
		return (m == a.m && n == a.n) ? true : false;
	}
	bool operator!=(Fraction& a){
		return (*this == a) ? false : true;
	}
	bool operator<(Fraction& a){
		int tmp=gcd(n,a.n);
		return (m*(a.n/tmp) < a.m*(n/tmp)) ? true : false;
	}
	void operator+=(Fraction& a){
		m += a.m;
		n += a.n;
	}
	void print(){
		printf("%d %d\n", m, n);
	}
private:
	int m, n;
	inline int gcd(int a, int b){
		int tmp;
		while(b){
			tmp = b;
			b = a%b;
			a = tmp;
		}
		return a;
	}
};

int main()
{
	int m, n;
	while(scanf("%d%d", &m, &n)==2 && (m!=1 || n!=1)){
		Fraction left(0,1), right(1,0), mid(1,1);
		Fraction n(m, n);
		while(mid != n){
			if(mid < n){
				printf("R");
				left = mid;
				mid += right;
			}else{
				printf("L");
				right = mid;
				mid += left;
			}
		}
		printf("\n");
	}
	return 0;
}
