// vim:ft=c:foldmethod=marker
// {{{ include library
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<fcntl.h>
// }}}
// {{{ define value
#define MAX_ARGS 500
#define MAX_CMD_LENGTH 2048
#define MAX_BG_PROC 256
// }}}
// {{{ structs
struct BgInfo{
	int pid;
	FILE *fout;
	int used;
}; // }}}

// {{{ int getcmd(char **pathname, char **in_file, char **out_file, int *isbg, char *args[])
int getcmd(char **pathname, char **in_file, char **out_file, int *isbg, char *args[])
{
	static char buf[MAX_CMD_LENGTH+2];
	char *p;
	int i;

	printf("chuang> ");
	if(fgets(buf, 2050, stdin) == NULL)
		return 0;
	buf[strlen(buf)-1] = '\0';
	if((p=strtok(buf, " ")) == NULL){
		*pathname = NULL;
		return 1;
	}
	if(p[0] == '#'){
		*pathname = NULL;
		return 1;
	}
	// parse [bg:]
	if(!strcmp(p, "bg:")){
		*isbg = 1;
		p = strtok(NULL, " ");
		if(p == NULL) return -1;
		if(p[0] == '#') return -1;
	}else{
		*isbg = 0;
	}
	// parse [in: in_file]
	if(!strcmp(p, "in:")){
		p = strtok(NULL, " ");
		if(p == NULL) return -1;
		if(p[0] == '#') return -1;
		*in_file = p;
		p = strtok(NULL, " ");
		if(p == NULL) return -1;
		if(p[0] == '#') return -1;
	}else{
		*in_file = NULL;
	}
	// parse [out: out_file]
	if(!strcmp(p, "out:")){
		p = strtok(NULL, " ");
		if(p == NULL) return -1;
		if(p[0] == '#') return -1;
		*out_file = p;
		p = strtok(NULL, " ");
		if(p == NULL) return -1;
		if(p[0] == '#') return -1;
	}else{
		*out_file = NULL;
	}
	// parse pathname
	*pathname = p;
	args[0] = p;
	// parse args
	for(p=strtok(NULL, " "),i=1; p!=NULL&&i<=MAX_ARGS; p=strtok(NULL, " "), i++)
		args[i] = p;
	args[i] = NULL;
	if(i == MAX_ARGS+1)
		return -2;
	return 1;
} // }}}
// {{{ int redirect(char *filename, int tofd, int flags, mode_t mode)
int redirect(char *filename, int tofd, int flags, mode_t mode)
{
	int fromfd;
	
	if((fromfd=open(filename, flags, mode)) == -1){
		perror("open error");
		return -1;
	}
	if(dup2(fromfd, tofd) == -1){
		perror("dup error");
		return -1;
	}
	return 0;
} // }}}
// {{{ BgInfo class
struct BgInfo bginfo[MAX_BG_PROC]={0};
int bg_num=0;
int add_bg_proc(pid_t pid, char *out_file)
{
	int i;

	if(bg_num == MAX_BG_PROC)
		return 1;
	for(i=0; i<MAX_BG_PROC; i++){
		if(!bginfo[i].used)
			break;
	}
	bginfo[i].used = 1;
	bginfo[i].pid = pid;
	bg_num++;
	if(out_file)
		bginfo[i].fout = fopen(out_file, "a");
	else
		bginfo[i].fout = stdout;
	if(bginfo[i].fout == NULL)
		return 2;
	return 0;
}
struct BgInfo *get_bg_info(pid_t pid)
{
	int i;

	for(i=0; i<MAX_BG_PROC; i++){
		if(bginfo[i].used && bginfo[i].pid==pid)
			break;
	}
	return &bginfo[i];
}
void del_bg_proc(struct BgInfo *bg)
{
	if(bg->fout != stdout)
		fclose(bg->fout);
	bg->used = 0;
	bg_num--;
}
// }}}

int main()
{
	char *pathname, *in_file, *out_file, *args[MAX_ARGS+2];
	int isbg;
	int status;
	int i;
	int pid;
	int last_pid=-1, last_status;
	char PATH[4096];

	strcpy(PATH, getenv("PATH"));
////	printf("%s\n", getenv("PATH"));
	while(status=getcmd(&pathname, &in_file, &out_file, &isbg, args)){
		// {{{ check getcmd status
		switch(status){
			case -1:
				printf("parse error.\n");
				break;
			case -2:
				printf("too many args.\n");
		} // }}}
		if(status == 1 && pathname){
////			printf("pathname: %s %s\n", pathname, (isbg)?"&":"");
////			for(i=0; args[i]; i++){
////				printf("arg %d: %s\n", i, args[i]);
////			}
			// {{{ judge cd
			if(!strcmp(pathname, "cd")){
				if(args[1] == NULL){
					printf("Usage: cd pathname\n");
				}else{
					if(chdir(args[1]) == -1)
						perror("chdir error");
				} // }}}
			// {{{ judge status
			}else if(!strcmp(pathname, "status")){
				FILE *fout;
				if(out_file){
					if((fout=fopen(out_file, "a")) == NULL)
						continue;
				}else{
					fout = stdout;
				}
				if(last_pid == -1){
					fprintf(fout, "No exit status.\n");
				}else{
					fprintf(fout, "Process %6d terminates with status %4d.\n", last_pid, (WEXITSTATUS(last_status)&128)?(WEXITSTATUS(last_status)|0xffffff00):WEXITSTATUS(last_status));
				}
				if(out_file)
					fclose(fout);
				// }}}
			// {{{ judge exit
			}else if(!strcmp(pathname, "exit")){
				exit(0);
			// }}}
			// {{{ judge addpath
			}else if(!strcmp(pathname, "addpath")){
				for(i=1; args[i]; i++)
					strcat(strcat(PATH, ":"), args[i]);	// PATH .= ":".args[i];
				setenv("PATH", PATH, 1);
	////			printf("PATH: %s\n", getenv("PATH"));
				// }}}
			// {{{ judge not built-in cmd
			}else{
				fflush(stdout);
				if((pid=fork()) == -1){
					fprintf(stderr, "fork error.\n");
					continue;
				}else if(pid == 0){
					// {{{ file redirect
					if(isbg){
						if(freopen("/dev/null", "r", stdin) == NULL){
							perror("freopen error");
							exit(-1);
						}
					}
					if(in_file){
////						printf("in_file: %s\n", in_file);
						if(freopen(in_file, "r", stdin) == NULL){
							perror("freopen error");
							exit(-1);
						}
					}
					if(out_file){
////						printf("out_file: %s\n", out_file);
						if(freopen(out_file, "a", stdout) == NULL){
							perror("freopen error");
							exit(-1);
						}
					} // }}}
					if(isbg){
						printf("Process %6d.\n", getpid());
						fflush(stdout);
					}
					if(execvp(pathname, args) == -1){
						perror("exec error");
						exit(-1);
					}
					exit(0);
				}
				if(isbg){
					if((status=add_bg_proc(pid, out_file)) != 0){
						printf("add bg status: status: %d\n", status);
						// {{{ check add_bg_proc status
						switch(status){
							case 1:
								fprintf(stderr, "too many bg process.\n");
								exit(-2);
								break;
							case 2:
								fprintf(stderr, "Can't open file.\n");
								exit(-1);
								break;
						} // }}}
					}
				}else{
					if(waitpid(pid, &status, 0) != pid)
						perror("waitpid error");
					last_pid = pid;
					last_status = status;
				}
			} // }}}
		}
		// {{{ check if any bg proc ended
		while((pid=waitpid(-1, &status, WNOHANG)) > 0){
			struct BgInfo *bg=get_bg_info(pid);
			fprintf(bg->fout, "Process %6d terminates with status %4d.\n", pid, (WEXITSTATUS(status)&128)?(WEXITSTATUS(status)|0xffffff00):WEXITSTATUS(status));
			del_bg_proc(bg);
		} // }}}
	}
	return 0;
}
