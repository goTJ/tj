#include<cstdio>
#include<cstring>
#define MAX_WIDTH 10

inline void lcdPartFont(int n, int w, int r, char t[])
{
	static const bool digit[10][7] = { { true,  true,  true, false,  true,  true,  true}
					  ,{false, false,  true, false, false,  true, false}	//    0
					  ,{ true, false,  true,  true,  true, false,  true}	//  1   2
					  ,{ true, false,  true,  true, false,  true,  true}	//    3
					  ,{false,  true,  true,  true, false,  true, false}	//  4   5
					  ,{ true,  true, false,  true, false,  true,  true}	//    6
					  ,{ true,  true, false,  true,  true,  true,  true}
					  ,{ true, false,  true, false, false,  true, false}
					  ,{ true,  true,  true,  true,  true,  true,  true}
					  ,{ true,  true,  true,  true, false,  true,  true} };
	if(r == 0){
		t[0] = ' ';
		char pattern=(digit[(int)n][0]) ? '-' : ' ';
		for(int i=1; i<=w; i++)
			t[i] = pattern;
		t[w+1] = ' ';
		t[w+2] = '\0';
	}
	else if(r >= 1 && r <= w){
		t[0]=(digit[(int)n][1]) ? '|' : ' ';
		for(int i=1; i<=w; i++)
			t[i] = ' ';
		t[w+1]=(digit[(int)n][2]) ? '|' : ' ';
		t[w+2] = '\0';
	}
	else if(r == w+1){
		t[0] = ' ';
		char pattern=(digit[(int)n][3]) ? '-' : ' ';
		for(int i=1; i<=w; i++)
			t[i] = pattern;
		t[w+1] = ' ';
		t[w+2] = '\0';
	}
	else if(r >= w+2 && r <= 2*w+1){
		t[0]=(digit[(int)n][4]) ? '|' : ' ';
		for(int i=1; i<=w; i++)
			t[i] = ' ';
		t[w+1]=(digit[(int)n][5]) ? '|' : ' ';
		t[w+2] = '\0';
	}
	else{
		t[0] = ' ';
		char pattern=(digit[(int)n][6]) ? '-' : ' ';
		for(int i=1; i<=w; i++)
			t[i] = pattern;
		t[w+1] = ' ';
		t[w+2] = '\0';
	}
}

int main()
{
	int width;
	char num[10];
	int times=0;
	while(scanf("%d%s", &width, num) == 2 && width){
		printf(times?"\n":"");
		int nLen=strlen(num);
		for(int r=0; r<2*width+3; r++){
			for(int i=0; i<nLen; i++){
				char tmp[MAX_WIDTH+5];
				lcdPartFont((int)num[i]-'0', width, r, tmp);
				printf(i?" %s":"%s", tmp);
			}
			printf("\n");
		}
		times++;
	}
	return 0;
}
