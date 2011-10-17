// vim:ft=cpp:foldmethod=marker
// {{{ includes
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "noiseGen.h"
#include "physical.h"
// }}}
#define PROG_NAME "noiseGenServer"

int main(int argc, char *argv[])
{
	int sfd, cfd, tfd;
	struct sockaddr_in srv, cli, tar;
	int cli_len=sizeof(cli);
	struct hostent *h;
	double N0=100.0, d;

	if(argc <= 3){
		printf("Usage: ./%s server_port target_ip target_port [N0=%.0lf]\n", PROG_NAME, N0);
		exit(1);
	}else if(argc >= 5){
		sscanf(argv[4], "%lf", &N0);
	}
	d = sqrt(N0/2);
	// {{{ bind server socket(sfd)
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
	// }}}
	while(1){
		if((cfd=accept(sfd, (struct sockaddr *)&cli, (socklen_t *)&cli_len)) < 0){
			perror("accept error.");
			exit(1);
		}
		printf("Client_IP[%s]\n", inet_ntoa(cli.sin_addr));
		printf("Client_Port[%d]\n", ntohs(cli.sin_port));
		if(!fork()){
			int n;
			NoiseGen noiseGen(d);
			Physical phy;
			Coor coor;
			Sample sample, noiseSample;
			// {{{ bind target socket(tfd)
			if((h=gethostbyname(argv[2])) == NULL){
				herror("gethostbyname failed");
				exit(1);
			}
			if((tfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
				perror("socket error");
				exit(1);
			}
			tar.sin_family = AF_INET;
			tar.sin_port = htons(atoi(argv[3]));
			tar.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr)));
			if(connect(tfd, (struct sockaddr *)&tar, sizeof(tar)) < 0){
				perror("connect error");
				exit(1);
			}
			// }}}
			while((n=read(cfd, &sample, sizeof(Sample))) > 0){
				if(n != sizeof(Sample))
					printf("Wrong data.\n");
				coor = noiseGen.getNoise();
				noiseSample = phy.encode(coor);
				for(int i=0; i<8; i++)
					sample.t[i] += noiseSample.t[i];
				if(write(tfd, &sample, sizeof(Sample)) < 0){
					perror("write error");
					exit(1);
				}
			}
			printf("Finished\n");
			close(cfd);
			close(tfd);
			exit(0);
		}
		close(cfd);
		while(waitpid(-1,NULL,WNOHANG) > 0)
			;
		break;
	}
	close(sfd);
	return 0;
}
