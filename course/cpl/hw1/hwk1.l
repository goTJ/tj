%{
// vim:ft=lex:foldmethod=marker

#define YY_NO_UNPUT

int sum=0;
void ator(int);
%}

%x ROMAN

Separ [,.?!]
RI (V?I{0,3}|I[VX])
RX (L?X{0,3}|X[LC])
RC (D?C{0,3}|C[DM])
RM M{0,4}

%%

<ROMAN>[^IVXLCDM] { printf("%d%s", sum, yytext); sum=0; BEGIN 0; }
<ROMAN>I/[VX] { sum--; }
<ROMAN>I { sum++; }
<ROMAN>V { sum += 5; }
<ROMAN>X/[LC] { sum -= 10; }
<ROMAN>X { sum += 10; }
<ROMAN>L { sum += 50; }
<ROMAN>C/[DM] { sum -= 100; }
<ROMAN>C { sum += 100; }
<ROMAN>D { sum += 500; }
<ROMAN>M { sum += 1000; }

{RM}{RC}{RX}{RI} { yyless(0); BEGIN ROMAN; }
[0-9]+ { ator(atoi(yytext)); }
{Separ}*[a-zA-Z0-9]* { printf("%s", yytext); }

%%
// {{{ void ator(int num)
void ator(int num)
{
	static char one[]={'I', 'X', 'C', 'M'};
	static char five[]={'V', 'L', 'D'};
	int n;
	int i, j;
	int v;

	if(num >= 5000){
		printf("(too big number)");
		return;
	}
	n = num/1000;
	for(i=0; i<n; i++)
		printf("M");
	num %= 1000;
	for(v=100, i=2; v; v/=10, i--){
		n = num/v;
		switch(n){
			case 1:
			case 2:
			case 3:
				for(j=0; j<n; j++)
					printf("%c", one[i]);
				break;
			case 4:
				printf("%c%c", one[i], five[i]);
				break;
			case 5:
			case 6:
			case 7:
			case 8:
				printf("%c", five[i]);
				for(j=5; j<n; j++)
					printf("%c", one[i]);
				break;
			case 9:
				printf("%c%c", one[i], one[i+1]);
		}
		num %= v;
	}
} // }}}
