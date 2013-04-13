#include <stdio.h>
#include <time.h>
#include<string.h>

char b[2000000];

int main (int argc, char *argv[])
{
	int i, j, k, l, m, n;
	char c, a[100], d[100];
	FILE *f, *z;
	srandom (time (NULL));
	strcpy(a, "input.txt");
	if(argc < 2)
		i = 1000000;
	else
		i = atoi(argv[1]);
	f = fopen (a, "w");
	for (l = 0; l < i; l ++)
	{
		c = (random () % 62);
		if (c < 10)
			c += 48;
		else if (c < 36)
			c += 55;
		else
			c += 61;
		b[l] = c;
	}
	for (l = 0; l < i; l ++)
		fputc (b[l], f);
	fputc ('!', f);
	strcpy(d, "pattern.txt");
	j = 100;
	k = 30;
	z = fopen (d, "w");
	fprintf (z, "%d\n", j);
	freopen (a, "r", f);
	for (; j > 0; j --)
	{
		m = random () % 2;
		if (m == 0)
			for (n = 0; n < k; n ++)
			{
				c = (random () % 62);
				if (c < 10)
					c += 48;
				else if (c < 36)
					c += 55;
				else
					c += 61;
				b[n] = c;								
			}
		else
		{
			for (l = random () % (i - k); l > 0; l --)
				fgetc (f);
			for (n = 0; n < k; n ++)
				b[n] = fgetc (f);
			freopen (a, "r", f);
		}
		for (l = 0; l < k; l ++)
			fputc (b[l], z);
		fputc ('\n', z);
	}
	return 0;
}

