//### PROGRAM
/*
ID: flyherm003
PROG: clock
*/
#include<stdio.h>
#include<string.h>

char *num[] = {
	"zero", "one", "two", "three", "four", "five", "six", "seven",
       	"eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen",
       	"fifteen", "sixteen", "seventeen", "eighteen", "ninteen",
	"twenty", "twenty-one", "twenty-two", "twenty-three", 
	"twenty-four", "twenty-five", "twenty-six", "twenty-seven",
        "twenty-eight", "twenty-nine",
	"thirty", "thirty-one", "thirty-two", "thirty-three", 
	"thirty-four", "thirty-five", "thirty-six", "thirty-seven",
        "thirty-eight", "thirty-nine",
	"forty", "forty-one", "forty-two", "forty-three", 
	"forty-four"};
int circle(int n)
{
	if(n>12)
		return 1;
	else
		return n;
}

void judge(int hour, int min)
{
	char s[10];
	FILE *out = fopen("clock.out", "w");

	if(min == 0){
		strcpy(s, num[hour]);
		s[0] += 'A'-'a';
		printf("%s o'clock\n", s);
		fprintf(out, "%s o'clock\n", s);
	}
	else if(min == 15){
		printf("Quarter past %s\n", num[hour]);
		fprintf(out, "Quarter past %s\n", num[hour]);
	}
	else if(min == 45){
		printf("Quarter to %s\n", num[circle(circle(hour+1))]);
		fprintf(out, "Quarter to %s\n", num[circle(hour+1)]);
	}
	else if(min>45){
		min = 60-min;
		strcpy(s, num[min]);
		s[0] += 'A'-'a';
		printf("%s to %s\n", s, num[circle(hour+1)]);
		fprintf(out, "%s to %s\n", s, num[circle(hour+1)]);
	}
	else{
		strcpy(s, num[hour]);
		s[0] += 'A'-'a';
		printf("%s %s\n", s, num[min]);
		fprintf(out, "%s %s\n", s, num[min]);
	}
}

void input(char s[], int *hour, int *min)
{
	int i;
	for(i=0; s[i]!=':'; i++)
		*hour = 10*(*hour)+(int)(s[i]-'0');
	for(i++; s[i]!='\0'; i++)
		*min = 10*(*min)+(int)(s[i]-'0');
}

int main(void)
{
	int hour, min;
	char s[10], i;
	FILE *in = fopen("clock.in", "r");

	while(fscanf(in, "%s", s)!=EOF){
		hour = min = 0;
		input(s, &hour, &min);
		judge(hour, min);
	}
	return 0;
}
//### END
