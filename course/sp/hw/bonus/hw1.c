// vim:fileencodings=big5:ft=c:foldmethod=marker
/***********************************************************************
 * This program reads a number of words from a file in argv[1],        *
 * looks for words with an initial 'A', sort them in the alphabetic    *
 * order, and prints sorted words out at stdout.                       *
 *                                                                     *
 * Author: B92902106 資工二 莊典融                                     *
 * Date: 03/04/05                                                      *
 *                                                                     *
 ***********************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define PROC_NAME "hw1.c"
#define Bool char

enum Boolean{False, True};

// {{{ void *malloc_with_error(size_t size)
// Similar with malloc, but with error when memory allocation failed.
// [PARAM] size: the number of bytes to allocate
void *malloc_with_error(size_t size)
{
	void *t=malloc(size);
	if(t == NULL){
		fprintf(stderr, "memory allocation failed.\n");
		exit(1);
	}
	return t;
} // }}}

// {{{ int count_weight(char c)
// Count weight of char, for now, I define 'A' < 'a' < 'B' < 'b' < ... < 'Z' < 'z' < others.
// [PARAM] c: the char to be counted.
int count_weight(char c)
{
	if('a' <= c && c <= 'z')
		return (c-'a')*2+1;
	if('A' <= c && c <= 'Z')
		return (c-'A')*2;
	return 999;	// a very large number
}
// }}}

// {{{ int comp_by_alphabetic_order(const void *p, const void *q)
// Compare two word by alphabetic order.
// [PARAM]: two void pointer storing two words' address
int comp_by_alphabetic_order(const void *p, const void *q)
{
	const char *pp=*(char **)p;
	const char *qq=*(char **)q;
	int wp, wq;

	while(*pp != '\0' && *qq != '\0'){
		wp = count_weight(*pp);
		wq = count_weight(*qq);
		if(wp > wq)
			return 1;
		else if(wp < wq)
			return -1;
		pp++;
		qq++;
	}
	if(*qq == '\0')
		return 1;
	return -1;
} // }}}

// {{{ int count_word_initial_A(FILE *fin)
// Count the number of words with initial 'A' in the file.
// [PARAM] fin: the file handle
int count_word_initial_A(FILE *fin)
{
	int c;
	int n=0;
	Bool f_reading_word=False;

	while((c=fgetc(fin)) != EOF){
		if(isspace(c) && f_reading_word == True)
			f_reading_word = False;
		if(!isspace(c) && f_reading_word == False){
			if(c == 'A')
				n++;
			f_reading_word = True;
		}
	}
	return n;
} // }}}

// {{{ char **get_word_initial_A(FILE *fin, int *word_num)
// Get the word set which stores all the words with initial 'A' from file.
// [PARAM] fin: the file handle
//         *word_num: points where to store the number of words
char **get_word_initial_A(FILE *fin, int *word_num)
{
	char **buf;
	int c;
	Bool f_reading_word=False;
	int i, j;
	int l;

	// Count the number of words with initial 'A' in the file.
	*word_num = count_word_initial_A(fin);
	// Create space to store the word set.
	buf = (char **)malloc_with_error(sizeof(char *)**word_num);

	// Reset file handle.
	fseek(fin, 0, SEEK_SET);
	// Store all words with initial 'A' in the file to word set "buf"
	i = 0;
	while((c=fgetc(fin)) != EOF){
		if(isspace(c) && f_reading_word == True)
			f_reading_word = False;
		if(!isspace(c) && f_reading_word == False){
			if(c == 'A'){
				l = 1;
				while((c=fgetc(fin)) != EOF && !isspace(c))
					l++;
				fseek(fin, (c==EOF)?-l:-(l+1), SEEK_CUR);
				buf[i] = (char *)malloc_with_error(sizeof(char)*l);
				for(j=0,c=fgetc(fin); c!=EOF && !isspace(c); j++,c=fgetc(fin))
					buf[i][j] = c;
				buf[i][j] = '\0';
				i++;
			}else{
				f_reading_word = True;
			}
		}
	}
	return buf;
} // }}}

// {{{ void output_word_set(char **set, int n)
// Output all words in the set.
// [Param] set: the pointed array which stores all words
//         n: the number of words in the set
void output_word_set(char **set, int n)
{
	int i;

	for(i=0; i<n; i++)
		printf("%s\n", set[i]);
} // }}}

// {{{ int main(int argc, char *argv[])
int main(int argc, char *argv[])
{
	FILE *fin;
	char **word_set;
	int word_num;

	// Check if the number of parameters are enough.
	if(argc < 2){
		printf("Usage: ./%s FILENAME\n", PROC_NAME);
		exit(0);
	}
	// Open input file or die if any error occurs
	if((fin=fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "Please check if file \"%s\" is readable.\n", argv[1]);
		exit(1);
	}

	// Get the word set which stores all the words with initial 'A'.
	word_set = get_word_initial_A(fin, &word_num);
	// Sort all words in the set by alphabetic order with Quick Sort Method.
	qsort(word_set, word_num, sizeof(char *), comp_by_alphabetic_order);
	// Output all words in the set.
	output_word_set(word_set, word_num);

	// Close input file
	fclose(fin);
	return 0;
} // }}}
