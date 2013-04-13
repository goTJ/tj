#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#define PROG_NAME "b92902106_s"
#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
	int sfd, cfd;
	struct sockaddr_in srv, cli;
	char buf[BUFFER_SIZE];
	int cli_len=sizeof(cli);

	if(argc < 2){
		printf("Usage: ./%s server_port\n", PROG_NAME);
		exit(1);
	}
	if((sfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error.");
		exit(1);
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(atoi(argv[1]));
	srv.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sfd, (struct sockaddr *)&srv, sizeof(srv)) < 0){
		perror("bind error.");
		exit(1);
	}
	if(listen(sfd, 0) < 0){
		perror("listen error.");
		exit(1);
	}
	while(1){
		if((cfd=accept(sfd, (struct sockaddr *)&cli, (socklen_t *)&cli_len)) < 0){
			perror("accept error.");
			exit(1);
		}
		printf("Client_IP[%s]\n", inet_ntoa(cli.sin_addr));
		printf("Client_Port[%d]\n", ntohs(cli.sin_port));
		if(!fork()){
			int n;
			while((n=read(cfd, buf, BUFFER_SIZE)) > 0){
				printf("Message_Received[%s]\n", buf);
				if(write(cfd, buf, n) < 0){
					perror("write error");
					exit(1);
				}
				printf("Message_Sendback[%s]\n", buf);
			}
			close(cfd);
			exit(0);
		}
		close(cfd);
		while(waitpid(-1,NULL,WNOHANG) > 0)
			;
	}
	return 0;
}
