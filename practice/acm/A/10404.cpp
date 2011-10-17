/* @JUDGE_ID:   10319NX 10404 C++ "¦ó¦Ì¯S^^" */
#include<iostream.h>

int main(void)
{
	int n;
	int tab[10], m;

	while(cin >> n){
		int a[1000006]={0};
		cin >> m;
		for(int i=0; i<m; i++)
			cin >> tab[i];
		a[0] = -1;
		for(int i=0; i<=n; i++){
			if(a[i]){
				int flag=a[i]*-1;
				for(int j=0; j<m; j++){
					if(i+tab[j]<=n && (flag==1 || a[i+tab[j]]!=1))
						a[i+tab[j]] = flag;
				}
			}
		}
		cout << ((a[n]==1) ? "Stan" : "Ollie") << " wins" << endl;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
