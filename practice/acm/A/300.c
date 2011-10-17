/* @JUDGE_ID:   10319NX 300 C */
#include<stdio.h>
#include<string.h>

char Hname[19][9] = {"pop", "no", "zip", "zotz", "tzec", "xul", "yoxkin", "mol", "chen", "yax", "zac", "ceh", "mac", "kankin", "muan", "pax", "koyab", "cumhu", "uayet" };
int Hday[19] = { 20, 20, 20, 20, 20, 20, 20, 20, 20 , 20 , 20 , 20, 20, 20, 20, 20, 20, 20, 5 };
char Tname[20][9] = { "imix", "ik", "akbal", "kan", "chicchan", "cimi", "manik", "lamat", "muluk", "ok", "chuen", "eb", "ben", "ix", "mem", "cib", "caban", "eznab", "canac", "ahau" };

int main(void)
{
	int round, times;
	char name[7];
	int day, year, month;
	int i, days;
	int Tyear, Tday;

	scanf("%d", &round);
	printf("%d\n", round);
	for(times=0; times<round; times++){
		scanf("%d. %s %d\n", &day, name, &year);
		days = year*365+day;
		for(i=0; i<19; i++){
			if(!strcmp(name, Hname[i])){
				month = i;
				break;
			}
			days += Hday[i];
		}
		printf("%d %s %d\n", (days%13)+1, Tname[days%20], days/260);
	}
	return 0;
}
/* @END_OF_SOURCE_CODE */
