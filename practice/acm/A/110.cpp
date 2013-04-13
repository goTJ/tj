#include<cstdio>

void swap(int &a, int &b)
{
	int t=a;
	a = b;
	b = t;
}

void sort(int data[], int depth, int n)
{
	if(depth == n){
		for(int j=0; j<depth; j++)
			printf("  ");
		printf("writeln(");
		for(int i=0; i<n; i++)
			printf(i?",%c":"%c", 'a'+data[i]);
		printf(")\n");
	}
	else{
		for(int i=depth-1; i>=0; i--){
			for(int j=0; j<depth; j++)
				printf("  ");
			if(i == depth-1)
				printf("if %c > %c then\n", 'a'+depth, 'a'+data[i]);
			else
				printf("else if %c > %c then\n", 'a'+depth, 'a'+data[i]);
			sort(data, depth+1, n);
			swap(data[i], data[i+1]);
		}
		for(int j=0; j<depth; j++)
			printf("  ");
		printf("else\n");
		sort(data, depth+1, n);
		for(int i=0; i<depth; i++)
			swap(data[i], data[i+1]);
	}
}

int main()
{
	int round;
	scanf("%d", &round);
	for(int times=0; times<round; times++){
		printf(times?"\n":"");
		int data[]={0, 1, 2, 3, 4, 5, 6, 7, 8};
		int n;
		scanf("%d", &n);
		printf("program sort(input,output);\n");
		printf("var\n");
		for(int i=0; i<n; i++)
			printf(i?",%c":"%c", 'a'+i);
		printf(" : integer;\n");
		printf("begin\n");
		printf("  readln(");
		for(int i=0; i<n; i++)
			printf(i?",%c":"%c", 'a'+i);
		printf(");\n");
		sort(data, 1, n);
		printf("end.\n");
	}
	return 0;
}
