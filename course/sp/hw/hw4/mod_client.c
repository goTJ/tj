// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<signal.h>

#define SERVER_NAME	"./mod_server"
#define MAX_CMD_LEN     1024
#define MAX_SIZE	2048

// {{{ int getcmd(char **cmd, char **out_file)
int getcmd(char **cmd, char **out_file, char **args_str)
{
	static char buf[MAX_CMD_LEN+2];
	char *p;
	char *buf_end;

	printf("> ");
	if(fgets(buf, 2050, stdin) == NULL)
		return -1;
	buf[strlen(buf)-1] = '\0';
	buf_end = buf+strlen(buf);
	if((p=strtok(buf, " ")) == NULL){
		*cmd = NULL;
		return 0;
	}
	if(p[0] == '#'){
		*cmd = NULL;
		return 0;
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
	// parse cmd
	*cmd = p;
	// parse args_str
	*args_str = p+strlen(p);
	if(*args_str < buf_end)
		*args_str += 1;
	return 0;
} // }}}

int main()
{
	char *cmd, *out_file, *args_str;
	int ctop[2], ptoc[2];
	int pid=0;
	int status;
	char buf[MAX_SIZE];
	int n;
	FILE *fout;
	struct timeval tv;
	fd_set readfds;

	while(getcmd(&cmd, &out_file, &args_str) == 0){
//		printf("cmd: %s, out_file: %s, args_str: %s\n", cmd, out_file, args_str);
		if(cmd == NULL)
			continue;
		// {{{ set fout
		fout = out_file ? fopen(out_file, "a") : stdout;
		if(fout == NULL){
			perror("fopen error");
			continue;
		}
		setvbuf(fout, NULL, _IOLBF, 0);
		// }}}
		if(!strcmp(cmd, "exit")){
			break;
		}
		// {{{ for connect
		if(!strcmp(cmd, "connect")){
			if(pid){
				fprintf(fout, "Message server creation fails.\n");
				continue;
			}
			if(pipe(ctop) == -1){
				perror("pipe error");
				fprintf(fout, "Message server creation fails.\n");
				continue;
			}
			if(pipe(ptoc) == -1){
				perror("pipe error");
				fprintf(fout, "Message server creation fails.\n");
				continue;
			}
			if((pid=fork()) < 0){
				fprintf(fout, "Message server creation fails.\n");
				continue;
			}else if(pid == 0){
				// {{{ create server
				close(ctop[0]);
				close(ptoc[1]);
				if(ptoc[0] != STDIN_FILENO){
					if(dup2(ptoc[0], STDIN_FILENO) == -1){
						fprintf(fout, "Message server creation fails.\n");
						exit(-1);
					}
				}
				if(ctop[1] != STDERR_FILENO){
					if(dup2(ctop[1], STDOUT_FILENO) == -1){
						fprintf(fout, "Message server creation fails.\n");
						exit(-1);
					}
				}
				if(execl(SERVER_NAME, strrchr(SERVER_NAME, '/')?strrchr(SERVER_NAME, '/')+1:SERVER_NAME, NULL) == -1){
					exit(-1);
				}
				exit(-1);
				// }}}
			}
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			FD_ZERO(&readfds);
			FD_SET(ctop[0], &readfds);
			select(ctop[0]+1, &readfds, NULL, NULL, &tv);
			if(!FD_ISSET(ctop[0], &readfds)){
				fprintf(fout, "Message server creation fails.\n");
				close(ctop[0]);
				close(ctop[1]);
				close(ptoc[0]);
				close(ptoc[1]);
				pid = 0;
				continue;
			}else{
				close(ctop[1]);
				close(ptoc[0]);
				read(ctop[0], buf, MAX_SIZE);
				strcpy(cmd, "mod");
			}
		}
		// }}}
		// {{{ check if existing server
		if(!pid){
			fprintf(fout, "No message server.\n");
			continue;
		} // }}}
		// {{{ for disconnect
		if(!strcmp(cmd, "disconnect")){
			if(write(ptoc[1], cmd, strlen(cmd)) == -1){
				perror("write error");
				continue;
			}
			close(ptoc[1]);
			close(ctop[0]);
			kill(pid, SIGINT);
			if(waitpid(pid, &status, 0) == -1){
				perror("waitpid error");
				continue;
			}
			fprintf(fout, "Message Server %d terminates with status %d.\n", pid, (WEXITSTATUS(status)&128)?(WEXITSTATUS(status)|0xffffff00):WEXITSTATUS(status));
			pid = 0;
		// }}}
		// {{{ judge cmd
		}else{
			sprintf(buf, "%s %s", cmd, args_str);
			if(write(ptoc[1], buf, strlen(buf)) == -1){
				perror("write error");
				continue;
			}
			if((n=read(ctop[0], buf, MAX_SIZE)) == -1){
				perror("read error");
				continue;
			}
			buf[n] = '\0';
			fprintf(fout, "[Message Server %d]: %s\n", pid, buf);
		} // }}}
	}
	// {{{ kill server
	if(pid){
		kill(pid, SIGINT);
		if(waitpid(pid, &status, 0) == -1)
			perror("waitpid error");
	}
	// }}}
	return 0;
}
