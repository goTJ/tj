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
#define PROG_NAME "receiver"

int main(int argc, char *argv[])
{
	int sfd, cfd;
	struct sockaddr_in srv, cli;
	int cli_len=sizeof(cli);
	double E0=10000.0, halfD;

	if(argc <= 3){
		printf("Usage: ./%s filename qamfile server_port [E0=%.0lf]\n", PROG_NAME, E0);
		exit(1);
	}else if(argc >= 5){
		sscanf(argv[4], "%lf", &E0);
	}
	halfD = sqrt(E0);
	// {{{ bind server socket(sfd)
	if((sfd=socket(PF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket error.");
		exit(1);
	}
	srv.sin_family = AF_INET;
	srv.sin_port = htons(atoi(argv[3]));
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
	Qam qam(argv[2], halfD);
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
			unsigned char c;
			Coor coor;
			Sample sample;
			bool flag=false;
			FILE *fout=fopen(argv[1], "wb");
			while((n=read(cfd, &sample, sizeof(Sample))) > 0){
				if(n != sizeof(Sample))
					printf("Wrong data.\n");
				coor = phy.decode(sample);
	//			printf("%.2lf, %.2lf: %d\n", coor.x, coor.y, qam.decode(coor));
				switch(bits){
					case 4:
						if(flag){
							c += qam.decode(coor);
							fwrite(&c, sizeof(char), 1, fout);
						}else{
							c = qam.decode(coor)<<4;
						}
						flag = !flag;
						break;
					case 8:
						c = qam.decode(coor);
						fwrite(&c, sizeof(char), 1, fout);
						break;
				}
			}
			close(cfd);
			fclose(fout);
			printf("Finished.\n");
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
