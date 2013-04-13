#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#define PROG_NAME "b92902106_c"
#define BUFFER_SIZE 8192

int main(int argc, char *argv[])
{
	int sfd;
	struct sockaddr_in srv;
	char buf[BUFFER_SIZE];
	struct hostent *h;

	if(argc < 4){
		printf("Usage: ./%s server_ip server_port message\n", PROG_NAME);
		exit(1);
	}
	if((h=gethostbyname(argv[1])) == NULL){
		herror("gethostbyname failed");
		exit(1);
	}
	if((sfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error");
		exit(1);
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(atoi(argv[2]));
	srv.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr)));
	if(connect(sfd, (struct sockaddr *)&srv, sizeof(srv)) < 0){
		perror("connect error");
		exit(1);
	}

	if(write(sfd, argv[3], strlen(argv[3])+1) < 0){
		perror("write error");
		exit(1);
	}
	if(read(sfd, buf, BUFFER_SIZE) < 0){
		perror("read error");
		exit(1);
	}
	close(sfd);
	return 0;
}
