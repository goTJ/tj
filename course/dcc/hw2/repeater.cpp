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
#include "qam.h"
#include "physical.h"
// }}}
#define PROG_NAME "repeater"

int main(int argc, char *argv[])
{
	int sfd, cfd, tfd;
	struct sockaddr_in srv, cli, tar;
	int cli_len=sizeof(cli);
	struct hostent *h;
	double E0=10000.0, halfD;

	if(argc <= 4){
		printf("Usage: ./%s qamfile server_port target_ip target_port [E0=%.0lf]\n", PROG_NAME, E0);
		exit(1);
	}else if(argc >= 6){
		sscanf(argv[5], "%lf", &E0);
	}
	halfD = sqrt(E0);
	// {{{ bind server socket(sfd)
	if((sfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error.");
		exit(1);
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(atoi(argv[2]));
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
	Qam qam(argv[1], halfD);
	Physical phy;
	int bits=qam.getBits();
	if(bits != 4 && bits != 8){
		printf("Transmitter can't support this type qam.\n");
		return 0;
	}
	printf("d/2=%.2lf\n", halfD);
	while(1){
		if((cfd=accept(sfd, (struct sockaddr *)&cli, (socklen_t *)&cli_len)) < 0){
			perror("accept error.");
			exit(1);
		}
		printf("Client_IP[%s]\n", inet_ntoa(cli.sin_addr));
		printf("Client_Port[%d]\n", ntohs(cli.sin_port));
		if(!fork()){
			int n;
			Coor coor;
			Sample sample;
			unsigned c;
			// {{{ bind target socket(tfd)
			if((h=gethostbyname(argv[3])) == NULL){
				herror("gethostbyname failed");
				exit(1);
			}
			if((tfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
				perror("socket error");
				exit(1);
			}
			tar.sin_family = AF_INET;
			tar.sin_port = htons(atoi(argv[4]));
			tar.sin_addr.s_addr = inet_addr(inet_ntoa(*((struct in_addr *)h->h_addr)));
			if(connect(tfd, (struct sockaddr *)&tar, sizeof(tar)) < 0){
				perror("connect error");
				exit(1);
			}
			// }}}
			while((n=read(cfd, &sample, sizeof(Sample))) > 0){
				if(n != sizeof(Sample))
					printf("Wrong data.\n");
				coor = phy.decode(sample);
				printf("%.2lf, %.2lf: %d\n", coor.x, coor.y, qam.decode(coor));
				c = qam.decode(coor);
				coor = qam.encode(c);
//				printf("%.2lf, %.2lf: %d\n", coor.x, coor.y, c);
				sample = phy.encode(coor);
				write(tfd, &sample, sizeof(Sample));
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
