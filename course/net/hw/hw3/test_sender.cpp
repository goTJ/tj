#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include<unistd.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include<arpa/inet.h>
#include"header.h"

#define MYPORT 8888 /* the port users will be connecting to */

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in their_addr; /* connector's address information */
	struct hostent *he;
	int numbytes;

	printf("%ld\n", get_checksum("aaaaa", 4));
	if (argc != 2) {
		fprintf(stderr,"usage: talker hostname\n");
		exit(1);
	}
	if ((he=gethostbyname(argv[1])) == NULL) { /* get the host info */
		herror("gethostbyname");
		exit(1);
	}
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	their_addr.sin_family = AF_INET; /* host byte order */
	their_addr.sin_port = htons(MYPORT); /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8); /* zero the rest of the struct */
	char buf[PKT_TOTAL_SIZE];
	char *filename="filename.txt";
	long checksum;
	long seq_num=1;
	long ack_num=0;
	long pkt_len=4+strlen(filename);
	long port=9999;
	char flag=128;
	memcpy(buf+4, &seq_num, 4);
	memcpy(buf+8, &ack_num, 4);
	memcpy(buf+12, &pkt_len, 4);
	memcpy(buf+16, &flag, 1);
	memcpy(buf+20, &port, 4);
	memcpy(buf+24, filename, strlen(filename));
	checksum = get_checksum(buf+4, 20+strlen(filename));
	memcpy(buf, &checksum, 4);
	if ((numbytes=sendto(sockfd, buf, 20+4+strlen(filename), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
		perror("sendto");
		exit(1);
	}
	printf("sent %d bytes to %s\n",numbytes,inet_ntoa(their_addr.sin_addr));
	flag = 0;
	while(fgets(buf+20, PKT_DATA_SIZE, stdin) != NULL){
		buf[20+strlen(buf+20)-1] = '\0';
		pkt_len = strlen(buf+20);
		memcpy(buf+4, &seq_num, 4);
		memcpy(buf+8, &ack_num, 4);
		memcpy(buf+12, &pkt_len, 4);
		memcpy(buf+16, &flag, 1);
		checksum = get_checksum(buf+4, 16+strlen(buf+20));
		memcpy(buf, &checksum, 4);
		if ((numbytes=sendto(sockfd, buf, 20+strlen(buf+20), 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
			perror("sendto");
			exit(1);
		}
		seq_num += pkt_len;
	}
	flag = 64;
	pkt_len = 0;
	memcpy(buf+4, &seq_num, 4);
	memcpy(buf+8, &ack_num, 4);
	memcpy(buf+12, &pkt_len, 4);
	memcpy(buf+16, &flag, 1);
	checksum = get_checksum(buf+4, 16);
	memcpy(buf, &checksum, 4);
	if ((numbytes=sendto(sockfd, buf, 20, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
		perror("sendto");
		exit(1);
	}
	flag = 0;
	pkt_len = 0;
	ack_num = -1;
	memcpy(buf+4, &seq_num, 4);
	memcpy(buf+8, &ack_num, 4);
	memcpy(buf+12, &pkt_len, 4);
	memcpy(buf+16, &flag, 1);
	checksum = get_checksum(buf+4, 16);
	memcpy(buf, &checksum, 4);
	if ((numbytes=sendto(sockfd, buf, 20, 0, (struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
		perror("sendto");
		exit(1);
	}
	close(sockfd);
	return 0;
}
