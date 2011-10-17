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
#define PROG_NAME "transmitter"

int main(int argc, char *argv[])
{
	int tfd;
	struct sockaddr_in tar;
	struct hostent *h;
	double E0=10000.0, halfD;

	if(argc <= 4){
		printf("Usage: ./%s filename qamfile target_ip target_port [E0=%.0lf]\n", PROG_NAME, E0);
		exit(1);
	}else if(argc >= 6){
		sscanf(argv[5], "%lf", &E0);
	}
	halfD = sqrt(E0);
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
	Qam qam(argv[2], halfD);
	Physical phy;
	FILE *fin=fopen(argv[1], "rb");
	int bits=qam.getBits();
	if(bits != 4 && bits != 8){
		printf("Transmitter can't support this type qam.\n");
		return 0;
	}
	printf("Eb=%.2lf, d/2=%.2lf\n", qam.getEb(), halfD);
	while(!feof(fin)){
		unsigned char c;
		Coor coor;
		Sample sample;
		if(fread(&c, sizeof(char), 1, fin) <= 0)
			break;
		switch(bits){
			case 4:
				coor = qam.encode(c>>4);
//				printf("%.2lf, %.2lf => %d\n", coor.x, coor.y, c>>4);
				sample = phy.encode(coor);
				write(tfd, &sample, sizeof(sample));
				coor = qam.encode(c&0xf);
//				printf("%.2lf, %.2lf => %d\n", coor.x, coor.y, c&0xf);
				sample = phy.encode(coor);
				write(tfd, &sample, sizeof(sample));
				break;
			case 8:
				coor = qam.encode(c);
				sample = phy.encode(coor);
				write(tfd, &sample, sizeof(sample));
				break;
		}
	}
	close(tfd);
	fclose(fin);
	return 0;
}
