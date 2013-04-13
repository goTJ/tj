// vim:ft=cpp:foldmethod=marker
#include <cstdio>
#include <climits>
#include <string>
#include <cctype>
#include "quantize.h"
#include "bitio.h"

using std::string;

int main(int argv, char *argc[])
{
	if(sizeof(short) != 2){
		printf("Must run on a 32-bit machine.\n");
		return 0;
	}
	int codeBit=4;
	Quan *quan;
	char quanType='m';
	int fileIdx=-1;
	{ // {{{ initialize quan, codeBit
		for(int i=1; i<argv; i++){
			if(argc[i][0] == '-'){
				switch(argc[i][1]){
					case 'U':
					case 'u':
						quanType='u';
						break;
					case 'A':
					case 'a':
						quanType='a';
						break;
					default:
						if(isdigit(argc[i][1]))
							codeBit = atoi(&argc[i][1]);
				}
			}else{
				fileIdx = i;
			}
		}
		switch(quanType){
			case 'u':
				quan = new QuanByUni(sizeof(short)*8, codeBit);
				break;
			case 'a':
				quan = new QuanByA(sizeof(short)*8, codeBit);
				break;
			default:
				quan = new QuanByMu(sizeof(short)*8, codeBit);
		}
	} // }}}
	if(fileIdx == -1){
		printf("Usage: %s [-m|-a|-u] [-number] pcm_filename\n", argc[0]);
		printf("-u: quantizate by uniform\n");
		printf("-m: quantizate by mu-law\n");
		printf("-a: quantizate by mu-law\n");
		printf("-number: number of bit per sample (suggestion: 4~8)\n");
		return 0;
	}
	// {{{ initialize string outFileName(argc[1]); // $argc[1]_$CODE_BIT.dpcm
	string outFileName(argc[fileIdx]); // $argc[1]_4.dpcm
	{
		char buf[20];
		outFileName.erase(outFileName.length()-4, 4);
		outFileName += "_D_";
		sprintf(buf, "%c", quanType);
		outFileName += buf;
		outFileName += "_";
		sprintf(buf, "%d", codeBit);
		outFileName += buf;
		outFileName += ".dpcm";
		printf("Generating %s\n", outFileName.c_str());
	} // }}}
	FILE *fout=fopen(outFileName.c_str(), "wb");
	FILE *fin=fopen(argc[fileIdx], "rb");
	short ir=0;
	while(!feof(fin)){
		short signal;
		fread(&signal, sizeof(short), 1, fin);
		short d=signal-ir;
		unsigned int code=quan->encode(d);
		output(code, codeBit, fout);
		short dd=quan->decode(code);
		ir += dd;
	}
	output(0, -1, fout);
	fclose(fin);
	fclose(fout);
	return 0;
}
