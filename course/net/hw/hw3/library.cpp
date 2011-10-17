#include"header.h"

long get_checksum(char *str, long len)
{
	long sum=0;
	int i=0;
	char *p;

	for(p=str, i=0; i<len; i++,p++)
		sum += (long)*p;
	return sum;
}

int is_corrupt(char *str, long len, long checksum)
{
	long sum=get_checksum(str, len);

	return sum!=checksum;
}
