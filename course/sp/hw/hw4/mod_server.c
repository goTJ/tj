// vim:ft=c:foldmethod=marker
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#define CONFIG_FILE	"config.ini"
#define MAX_CMD_LEN	1024
#define MAX_SIZE        2048

int main()
{
	char buf[MAX_CMD_LEN+1], buf2[MAX_SIZE+1];
	int n;
	char *cmd, *arg, *p;
	FILE *fin;
	int tmp=3;

	write(STDOUT_FILENO, "I'm working.", strlen("I'm working."));
	while((n=read(STDIN_FILENO, buf, MAX_CMD_LEN)) != -1){
		buf[n] = '\0';
		cmd = strtok(buf, " ");
		if(cmd == NULL){
			write(STDOUT_FILENO, "", 0);
			continue;
		}
		// {{{ judge login
		if(!strcmp(cmd, "login")){
			int isFound=0;
			arg = strtok(NULL, " ");
			if(arg == NULL){
				sprintf(buf2, "Please enter user name.");
				write(STDOUT_FILENO, buf2, strlen(buf2));
				continue;
			}
			if((fin=fopen(CONFIG_FILE, "r")) == NULL){
				sprintf(buf2, "user %s is not listed.", arg);
				write(STDOUT_FILENO, buf2, strlen(buf2));
				continue;
			}
			while(fgets(buf2, MAX_SIZE, fin)){
				if(buf2[0] == '\n')
					break;
			}
			while(fgets(buf2, MAX_SIZE, fin)){
				if(buf2[0] == '#')
					continue;
				buf2[strlen(buf2)-1] = '\0';
				p = strtok(buf2, ",");
				if(p == NULL)
					continue;
				if(!strcmp(arg, p)){
					isFound = 1;
					p = buf2+strlen(p)+2;
					sprintf(buf2, p, arg);
					write(STDOUT_FILENO, buf2, strlen(buf2));
					break;
				}
			}
			if(!isFound){
				printf("arg: (%s)\n", arg);
				sprintf(buf2, "user %s is not listed.", arg);
				write(STDOUT_FILENO, buf2, strlen(buf2));
			}
			fclose(fin);
		// }}}
		// {{{ judge mod
		}else if(!strcmp(cmd, "mod")){
			int num=0, random=123;
			if((fin=fopen(CONFIG_FILE, "r")) == NULL){
				sprintf(buf2, "No welcome message.");
				write(STDOUT_FILENO, buf2, strlen(buf2));
				continue;
			}
			while(fgets(buf2, MAX_SIZE, fin)){
				if(buf2[0] == '#')
					continue;
				if(buf2[0] == '\n')
					break;
				num++;
			}
			fclose(fin);
			srand(time(NULL));
			random = tmp = abs(rand()*(tmp+7)+tmp+3)%num;
			if((fin=fopen(CONFIG_FILE, "r")) == NULL){
				sprintf(buf2, "No welcome message.");
				write(STDOUT_FILENO, buf2, strlen(buf2));
				continue;
			}
			while(fgets(buf2, MAX_SIZE, fin)){
				if(buf2[0] == '#')
					continue;
				if(!random){
					buf2[strlen(buf2)-1] = '\0';
					write(STDOUT_FILENO, buf2, strlen(buf2));
					break;
				}
				random--;
			}
			fclose(fin);
		// }}}
		}else if(!strcmp(cmd, "disconnect")){
			sprintf(buf2, "disconnect");
			write(STDOUT_FILENO, buf2, strlen(buf2));
			return 0;
		}else{
			sprintf(buf2, "Unknow cmd.");
			write(STDOUT_FILENO, buf2, strlen(buf2));
		}
	}
	return 0;
}
