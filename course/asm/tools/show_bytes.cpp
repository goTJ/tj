#include<stdio.h>

void show_bytes(unsigned char *start, int len)
{
	for(int i=0; i<len; i++)
		printf("%.2x ", start[i]);
	printf("\n");
	for(int i=0; i<len; i++){
		for(int j=0; j<8; j++){
			if(start[i] & -128)
				putchar('1');
			else
				putchar('0');
			start[i] <<= 1;
		}
		putchar(' ');
	}
	printf("\n");
}

void show_int(int n)
{
	printf("%3d:\n", n);
	show_bytes((unsigned char *)&n, sizeof(int));
}

void show_float(float n)
{
	show_bytes((unsigned char *)&n, sizeof(float));
}

int main()
{
	float n;

	while(scanf("%f", &n) == 1){
		show_float(n);
	}
	return 0;
}
