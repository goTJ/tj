#include<stdio.h>

const int move[9][9] = { {1, 8, 6, 7, 4, 3, 2, 5, 0}
		 , {8, 0, 6, 5, 4, 7, 2, 3, 1}
		 , {8, 6, 1, 5, 4, 7, 2, 3, 0}
		 , {8, 7, 6, 0, 4, 1, 2, 5, 3}
		 , {7, 5, 6, 1, 8, 3, 2, 4, 0}
		 , {8, 7, 6, 1, 4, 2, 5, 3, 0}
		 , {8, 7, 6, 2, 4, 1, 3, 5, 0}
		 , {7, 5, 6, 1, 4, 3, 2, 8, 0}
		 , {8, 7, 6, 1, 4, 0, 2, 3, 5} };

const char *act[9] = { "DDRRUULDDRUULDLDRRULLURRDLLDRUU"
		     , "DDRUULLDDRURULLDDRURULLDDRRUULL"
		     , "DDLLUURDDLUURDRDLLURRULLDRRDLUU"
		     , "DRURULDLURDDRUULDLDRRULLDRRULUL"
		     , "RULDLURDRDLLURRULDDRULDLUURRDD"
		     , "DLURULDLURDDRUULDLDRRULLDRRULUR"
		     , "RRULLURDDRUULDDLUURDRDLLURRDLLU"
		     , "RULLDRRUULDLURDLDRRULDRUULLDDRR"
		     , "LLURRULDDLUURDDRUULDLDRRULLDRRU" };

int main()
{
	int round, times;
	int state[9], ans[9];
	int zero;
	int i;

	scanf("%d", &times);
	for(round=0; round<times; round++){
		if(round)
			printf("\n");
		printf("Puzzle #%d\n", round+1);
		for(i=0; i<9; i++){
			scanf("%d", &state[i]);
			if(!state[i])
				zero = i;
		}
		for(i=0; i<9; i++)
			ans[move[zero][i]] = state[i];
		for(i=0; i<9; i++){
			printf("%d", ans[i]);
			if(i%3 == 2)
				printf("\n");
			else
				printf(" ");
		}
		printf("%s\n", act[zero]);
	}
	return 0;
}
