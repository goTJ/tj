/* @JUDGE_ID:   10319NX 10409 C++ "¦ó¦Ì¯S^^" */
/*
initial: 1  2  3
north:  -2  1  3
south:   2 -1  3
east:    3  2 -1
west:   -3  2  1
*/
#include<iostream>

void swap(int &a, int &b)
{
	int t=a;

	a = b;
	b = t;
}

int main(void)
{
	int n;

	while(cin >> n){
		if(!n)
			break;
		char buf[10];
		int status[3] = { 1, 2, 3};
		for(int i=0; i<n; i++){
			cin >> buf;
			switch(buf[0]){
				case 'n':
					swap(status[0], status[1]);
					status[0] = 7-status[0];
					break;
				case 's':
					swap(status[0], status[1]);
					status[1] = 7-status[1];
					break;
				case 'e':
					swap(status[0], status[2]);
					status[2] = 7-status[2];
					break;
				case 'w':
					swap(status[0], status[2]);
					status[0] = 7-status[0];
					break;
			}
		}
		cout << status[0] << endl;
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
