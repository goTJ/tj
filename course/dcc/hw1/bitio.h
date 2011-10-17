// vim:ft=c:foldmethod=marker
#ifndef BITIO_H
#define BITIO_H

// {{{ int input(int bit, FILE *fin)
int input(int bit, FILE *fin)
{
	static int buf=0;
	static int remain=0;
	int data=0;

//	printf("input: %d %d %d\n", bit, remain, buf);
	if(feof(fin)){
		if(remain >= bit){
			remain -= bit;
			data = (data<<bit)+(buf>>remain);
			buf &= (1<<remain)-1;
			return data;
		}else{
			return -1;
		}
	}
	while(!feof(fin) && bit > 0){
		if(remain >= bit){
			remain -= bit;
			data = (data<<bit)+(buf>>remain);
			buf &= (1<<remain)-1;
			bit = 0;
		}else{
			data = (data<<remain)+buf;
			bit -= remain;
			buf = fgetc(fin);
//			printf("get: %d\n", buf);
			remain = 8;
		}
	}
	return data;
} // }}}
// {{{ void output(int data, int bit, FILE *fout)
void output(int data, int bit, FILE *fout)
{
	static int buf=0;
	static int remain=8;

//	printf("output: %d %d %d %d\n", data, bit, remain, buf);
	if(bit == -1){  // flush
		if(remain < 8){
			buf <<= remain;
			fputc(buf, fout);
		}
		return;
	}
	while(bit > 0){
		if(bit < remain){
			buf = (buf<<bit)+data;
			remain -= bit;
			bit = 0;
		}else{
			buf = (buf<<remain)+(data>>(bit-remain));
			fputc(buf, fout);
			bit -= remain;
			buf = data&((1<<bit)-1);
			remain = 8;
		}
	}
} // }}}

#endif
