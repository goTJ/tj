// vim:ft=c:foldmethod=marker
// {{{ include library
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<dirent.h>
// }}}

// {{{ define CONST
#define PROGNAME "FindAndReplace"
#define MAX_PATH_LENGTH 1024
#define bool char
#define true 1
#define false 0
#define HASH_NUM 65535
#define MAX_PATTERN_LENGTH
// }}}

// {{{ Global variable
typedef struct List{
	ino_t ino;
	dev_t dev;
	struct List *next;
}List;	// i-node list for hash table
List *hash[HASH_NUM];	// hash table for storing i-node
char *pattern;		// 要比對的字串
bool needreplace;	// 1: 要置換, 0: 不要置換
char *match_str, *replace_str;	// match_str: 被置換的字串, replace_str: 取代的字型
int *pi;
// }}}

// {{{ int *compute_prefix_function(char *str)
int *compute_prefix_function(char *str)
{
	int len=strlen(str);
	int *tmp=(int *)malloc(sizeof(str)*len);
	int k=0;
	int i;

	tmp[0] = 0;
	for(i=1; i<len; i++){
		while(k>0 && str[k]!=str[i])
			k = tmp[k-1];
		if(str[k] == str[i])
			k++;
		tmp[i] = k;
	}
	return tmp;
} // }}}

// {{{ void output_stat(const char *filename, const struct stat fst)
void output_stat(const char *filename, const struct stat *fst)
{
	char buf[MAX_PATH_LENGTH];
	int len;

	if(getcwd(buf, MAX_PATH_LENGTH) == NULL){
		perror("getcwd");
		return;
	}
	len = strlen(buf);
	buf[len] = '/';
	buf[len+1] = '\0';
	/*
	printf("path: %s ", strcat(buf, filename));
	if(S_ISREG(fst->st_mode))
		printf("(regular)");
	if(S_ISDIR(fst->st_mode))
		printf("(directory)");
	if(S_ISLNK(fst->st_mode))
		printf("(link)");
	printf("\n");
	*/
	fputs(buf, stdout);
	puts(filename);	// absolute file name
	fputs(ctime(&(fst->st_mtime)), stdout);	// last modification time
	fputs(ctime(&(fst->st_ctime)), stdout);	// last status change time
	fputs(ctime(&(fst->st_atime)), stdout);	// last access time
	printf("\n%10d (%d/%d)\n", (int)fst->st_size, // file size
		major(fst->st_dev), // major device number
		minor(fst->st_dev) // minor device number
	);
} // }}}

// {{{ bool isvisit(ino_t ino)
// NOTE: by hash table method
bool isvisit(ino_t ino, dev_t dev)
{
	int idx=ino%HASH_NUM;
	List *p=hash[idx];

	while(p){
		if(ino == p->ino && dev == p->dev)
			return true;
		p = p->next;
	}
	return false;
} // }}}

// {{{ void insert(ino_t ino)
// NOTE: by hash table method
void insert(ino_t ino, dev_t dev)
{
	int idx=ino%HASH_NUM;

	if(hash[idx] == NULL){
		hash[idx] = malloc(sizeof(List));
		hash[idx]->next = NULL;
		hash[idx]->ino = ino;
		hash[idx]->dev = dev;
	}else{
		List *p=malloc(sizeof(List));
		p->ino = ino;
		p->dev = dev;
		p->next = hash[idx]->next;
		hash[idx]->next = p;
	}
} // }}}

// {{{ void replace_file(const char *filename)
// NOTE: by KMP method
void replace_file(const char *filename)
{
	FILE *fin=fopen(filename, "r+");
	int c;
	int len=strlen(match_str);
	int q=0;

	if(fin == NULL){
		fprintf(stderr, "Can't open file %s: ", filename);
		perror(NULL);
		return;
	}
	// if the length of match_str is equal to replace_str's
	if(strlen(match_str) == strlen(replace_str)){
		while((c=fgetc(fin)) != EOF){
			while(q>0 && match_str[q] != c)
				q = pi[q-1];
			if(match_str[q] == c){
				q++;
			}
			if(q == len){
				fseek(fin, -len, SEEK_CUR);
				fputs(replace_str, fin);
				q = pi[q-1];
			}
		}
	}else{
		FILE *ftmp=tmpfile();
		bool is_change=false;
		while((c=fgetc(fin)) != EOF){
			while(q>0 && match_str[q] != c)
				q = pi[q-1];
			if(match_str[q] == c){
				q++;
			}
			if(q == len){
				is_change = true;
				fseek(ftmp, -(len-1), SEEK_CUR);
				fputs(replace_str, ftmp);
				q = pi[q-1];
			}else{
				fputc(c, ftmp);
			}
		}
		if(is_change){
			fclose(fin);
			if((fin=fopen(filename, "w")) == NULL){
				perror("fopen");
				fclose(ftmp);
				return;
			}
			rewind(ftmp);
			while((c=fgetc(ftmp)) != EOF)
				fputc(c, fin);
		}
		fclose(ftmp);
	}
	fclose(fin);
} // }}}

// {{{ void gogodir()
void gogodir()
{
	DIR *dp;
	struct dirent *dirp;
	struct stat fst;
	char buf[MAX_PATH_LENGTH];
	
	if(!(dp=opendir("."))){
		getcwd(buf, MAX_PATH_LENGTH);
		fprintf(stderr, "Can't open directory %s: ", buf);
		perror(NULL);
		return;
	}
	// surf file 
	while((dirp=readdir(dp)) != NULL){
		// skip "." and ".."
		if(!strcmp(dirp->d_name, "..") || !strcmp(dirp->d_name, "."))
			continue;
		// get lstat
		if(lstat(dirp->d_name, &fst) == -1){
			getcwd(buf, MAX_PATH_LENGTH);
			fprintf(stderr, "Can't get lstat of %s: ", strcat(strcat(buf, "/"), dirp->d_name));
			perror(NULL);
			continue;
		}
		// check if filename contain pattern
		// and get the stat of the target of linkfile if the file is a symbolic link file
		if(strstr(dirp->d_name, pattern) != NULL){
			output_stat(dirp->d_name, &fst);
			if(S_ISLNK(fst.st_mode)){
				if(stat(dirp->d_name, &fst) == -1){
					getcwd(buf, MAX_PATH_LENGTH);
					fprintf(stderr, "Can't get stat of %s: ", strcat(strcat(buf, "/"), dirp->d_name));
					perror(NULL);
					continue;
				}
			}
			// replace file if necessary and possible
			if(needreplace && S_ISREG(fst.st_mode)){
				replace_file(dirp->d_name);
			}
		}else{
			if(S_ISLNK(fst.st_mode)){
				if(stat(dirp->d_name, &fst) == -1){
					getcwd(buf, MAX_PATH_LENGTH);
					fprintf(stderr, "Can't get stat of %s: ", strcat(strcat(buf, "/"), dirp->d_name));
					perror(NULL);
					continue;
				}
			}
		}
		// chdir if the file is a directory
		if(S_ISDIR(fst.st_mode)){
			if(!isvisit(fst.st_ino, fst.st_dev)){
				insert(fst.st_ino, fst.st_dev);
				getcwd(buf, MAX_PATH_LENGTH);
				if(chdir(dirp->d_name) != -1){
					gogodir();
					chdir(buf);
				}else{
					fprintf(stderr, "Can't chdir to %s: ", strcat(strcat(buf, "/"), dirp->d_name));
					perror(NULL);
				}
			}
		}
	}
	closedir(dp);
} // }}}

// {{{ int main(int argc, char *argv[])
int main(int argc, char *argv[])
{
	struct stat fst;

	// check parameter
	if(argc != 3 && argc != 5){
		printf("Usage: ./%s DIR PATTERN [MATCH REPLACE]\n", PROGNAME);
		exit(0);
	}else if(argc == 5){
		needreplace = true;
		match_str = argv[3];
		replace_str = argv[4];
		pi = compute_prefix_function(match_str);
	}else{
		needreplace = false;
	}
	// chdir to argv[1]
	if(chdir(argv[1]) == -1){
		fprintf(stderr, "Can't chdir to %s: ", argv[1]);
		perror(NULL);
		exit(1);
	}
	pattern = argv[2];
	// insert i-node of argv[1] into hash table
	if(stat(".", &fst) == -1){
			fprintf(stderr, "Can't get stat of %s: ", argv[1]);
			perror(NULL);
	}else{
		insert(fst.st_ino, fst.st_dev);
	}
	// trace directory
	gogodir();
	return 0;
} // }}}
