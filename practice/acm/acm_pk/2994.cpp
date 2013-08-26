#include <stdio.h>
#define MAX 200002

char stack[MAX];
int top;
struct ele {
	int x;
	int y;
};
struct ele u2[MAX/2];
int u2_top;

int main()
{
	char input[MAX];
	while ( fgets(input, MAX, stdin) != NULL )	
	{
		int a = -1, b=-1;
		int i=0,j;
		top=0, u2_top = 0;
		for (i=0 ; input[i]!=0 ; i++ )
		{
			if ( input[i] == 'C')
			{
				stack[top] = 'C';
				top++;
			}
			else if ( input[i] == 'U')
			{
				stack[top] = 'U';
				stack[top+1] = 'P';
				top+=2;
			}
			else if ( input[i] == 'V')
			{
				a = 1, b=1;
				while (top > 0)
				{
					if ( stack[top-1] == 'C')
					{
						int tmp = a;
						a=b;
						b=tmp;
						top--;
					}
					else if ( stack[top-1] == 'P' )
					{
						u2[u2_top].x = a;
						u2[u2_top].y = b;
						u2_top++;
						a = -1, b=-1;
						top--;
						break;
					}
					else if ( stack[top-1] == 'U')
					{
						int a1 = u2[u2_top-1].x;
						int b1 = u2[u2_top-1].y;
						u2_top--;

						a +=a1;
						if ( b1 > b )
							b = b1;
						top--;
					}
				} 
			}
		}
		printf("%d\n", a);
	}
	return 0;
}
