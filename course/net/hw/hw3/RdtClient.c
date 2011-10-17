#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/select.h>

#include "header.h"

#define alpha 0.125
#define beta 0.25

#define BUFFER_ENTRY ((int)(WINDOW_SIZE/PKT_TOTAL_SIZE)+5)

#define SLOWSTART 0
#define CONGAVOID 1

struct head{
	int32_t checksum : 32;
	int32_t SEQ : 32;
	int32_t ACK : 32;
	int32_t length : 32;
	int8_t flag : 8;	// FIN=128, SYN=64
};

struct packet{
	struct head header;
	char data[PKT_DATA_SIZE];
};

struct packetbuf{
	int32_t ACK;
	struct packet pkt;
};


int mkpkt(struct packet **pkt, int fd);
void myconnect(int socket_fd, struct sockaddr_in sendaddr, int myport, char *filename);
void disconnect(int socket_fd, struct sockaddr_in sendaddr);
uint32_t double2int(double);
void parse_header(char *buf);
void print_pkt(struct packet *pkt);

int main(int argc, char *argv[]) {/*{{{*/

	printf("sizeof(struct packet) = %d, sizeof(struct head) = %d\n", sizeof(struct packet), sizeof(struct head));
	struct stat filestat;
	struct packet *pkt;
	struct packetbuf buf[BUFFER_ENTRY];
	struct head header;
	struct sockaddr_in sender, sendaddr;
	struct timeval tv;
	int myport;				// sender's port number
	int filesize;
	int NextSEQ;
	int socket_fd;				// sender's udp socket
	int file_fd;
	int buf_base;				// point to buffer window base
	int buf_index;				// point to next available space in buffer
	int CongState;
	int Threshold;
	int i;
	int ACKnum;
	int32_t ExpectedACK, lastACK;
	int32_t base;
	int32_t checksum;
	int32_t CongWin;
	double EstimatedRTT, SampleRTT, DevRTT;
	char *file;
	char *desaddr;
	char *desport;
	fd_set fdset;

	// get argv[]/*{{{*/
	if (argc != 4) {
		fprintf(stderr, "argc error\n");
		return -1;
	}
	desaddr = argv[1];	// destination ip
	desport = argv[2];	// destination port
	file = argv[3];		// filename
	myport =  1026;		// my sending port/*}}}*/
			
	// open file/*{{{*/
	if ( (file_fd = open(file, O_RDONLY)) < 0 )
		perror("open file error");
	stat(file, &filestat);
	filesize = filestat.st_size;/*}}}*/

	// initialize sender socket/*{{{*/
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
	sender.sin_family = AF_INET;
	sender.sin_port = htons( (short)myport );
	sender.sin_addr.s_addr = htonl(INADDR_ANY);
	while (myport <= 65535) {
		if ( bind(socket_fd, (struct sockaddr *)&sender, sizeof(sender)) < 0 ) {
			fprintf(stderr, "bind error, myport(%ld): used\n", myport);
			myport++;
			sender.sin_port = htons( (short)myport );
		} else
			break;
	}/*}}}*/

	// initialize sender sendaddr/*{{{*/
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_port = htons( (short)atoi(desport) );
	sendaddr.sin_addr.s_addr = inet_addr( desaddr );/*}}}*/

	// iniialize others/*{{{*/
	base = 1;			// TCP base number
	NextSEQ = 1;
	
	ExpectedACK = 1;		// this value is a bit strang....

	ACKnum = 0;
	lastACK = 0;
	CongWin = PKT_TOTAL_SIZE;	// congestion window size
	CongState = SLOWSTART;		// congestion control state
	Threshold = WINDOW_SIZE;	// congestoin control threshold
	buf_base = 1;
	buf_index = 1;		
	buf[1].ACK = -1;		
	EstimatedRTT = 1;		// congestion control variables
	SampleRTT = 0;
	DevRTT = 1;/*}}}*/

	// initialize select()/*{{{*/
	tv.tv_sec = (int32_t) EstimatedRTT+1;
	tv.tv_usec = 0;
	FD_ZERO(&fdset);
	FD_SET(socket_fd, &fdset);	/*}}}*/

	
	// three-way hand shake/*{{{*/
	printf("before 3-way handshake\n");
	myconnect(socket_fd, sendaddr, myport, file);/*}}}*/

	while (NextSEQ <= filesize || buf_base != buf_index) { /*{{{*/

		// CongWin have to be greater than 2, Threshold have to be greater than 1/*{{{*/
		if (CongWin < 2*PKT_TOTAL_SIZE)
			CongWin = 2*PKT_TOTAL_SIZE;
		if (Threshold < PKT_TOTAL_SIZE)
			Threshold = PKT_TOTAL_SIZE;/*}}}*/
		
		// send packet/*{{{*/
		while ( (NextSEQ+PKT_TOTAL_SIZE) <= (base+CongWin) ) {
			
			if (NextSEQ > filesize)
				break;
			
			NextSEQ = NextSEQ + mkpkt(&pkt, file_fd);
			//printf("=.==.=..=.=.=.=.=.=.==.=.=.=.=\n");
			//printf("buf_index = %d, BUFFER_ENTRY = %d\n", buf_index,  BUFFER_ENTRY);
			//printf("buf_index % BUFFER_ENTRY = %d\n", buf_index % BUFFER_ENTRY);
			//printf("buf_index = %d, buf[%d]\n", buf_index, buf_index % BUFFER_ENTRY);
			//printf("ordinary send packet: ACK = %ld\n",  NextSEQ);
			memcpy(&(buf[buf_index % BUFFER_ENTRY].pkt), pkt, sizeof(struct packet));
			buf[buf_index % BUFFER_ENTRY].ACK = NextSEQ;
			buf[(buf_index+1) % BUFFER_ENTRY].ACK = -1;
			buf_index++;
			sendto(socket_fd, pkt, PKT_TOTAL_SIZE/*sizeof(struct packet)*/, 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));
		}	/*}}}*/

		// Timeout Setup (Congestion Control)/*{{{*/
		if (SampleRTT - EstimatedRTT < 0)
			DevRTT = (1-beta)*DevRTT + beta*( SampleRTT - EstimatedRTT )*(-1);
		else
			DevRTT = (1-beta)*DevRTT + beta*( SampleRTT - EstimatedRTT );
		EstimatedRTT = (1-alpha)*EstimatedRTT + alpha*SampleRTT;
		tv.tv_sec = lrint(EstimatedRTT + 4*DevRTT);
		if (EstimatedRTT + 4*DevRTT >= lrint(EstimatedRTT + 4*DevRTT))
				tv.tv_usec = lrint( 1000000 * (EstimatedRTT + 4*DevRTT - lrint(EstimatedRTT + 4*DevRTT)) );
		else 
				tv.tv_usec = lrint( 1000000 * (lrint(EstimatedRTT + 4*DevRTT) - (EstimatedRTT + 4*DevRTT) ));
		
		if (SampleRTT < 0 || DevRTT < 0 || EstimatedRTT < 0) {
			printf("SampleRTT = %lf, DevRTT = %lf, EstimatedRTT = %lf\n", SampleRTT, DevRTT, EstimatedRTT);
			printf("lrint(EstimatedRTT + 4*DevRTT) = tv.tv_sec = %d, tv.tv_usec = %d\n", tv.tv_sec, tv.tv_usec);
			return 0;
		}
		
		//printf("SampleRTT = %lf, DevRTT = %lf, EstimatedRTT = %lf\n", SampleRTT, DevRTT, EstimatedRTT);
		//printf("BEFORE SELECT tv.tv_sec = %d, tv.tv_usec = %d\n", tv.tv_sec, tv.tv_usec);/*}}}*/
		
		// wait for response.../*{{{*/
		FD_ZERO(&fdset);
		FD_SET(socket_fd, &fdset);	
		select(socket_fd+1, &fdset, NULL, NULL, &tv);
		if (tv.tv_usec > 10000)
			tv.tv_usec = tv.tv_usec - 10000;
		//printf("AFTER SELECT tv.tv_sec = %d, tv.tv_usec = %d\n", tv.tv_sec, tv.tv_usec);
		//printf("BEFORE UPDATE SAMPLERTT  SampleRTT = %lf, DevRTT = %lf, EstimatedRTT = %lf\n", SampleRTT, DevRTT, EstimatedRTT);
		//printf("lrint(EstimatedRTT + 4*DevRTT) = %d\n", lrint(EstimatedRTT + 4*DevRTT));
		SampleRTT =  (double) (lrint(EstimatedRTT + 4*DevRTT) - tv.tv_sec) - tv.tv_usec*0.000001; 
		//printf("AFTER UPDATE SAMPLERTT  SampleRTT =  lrint(EstimatedRTT + 4*DevRTT) - tv.tv_sec - tv.tv_usec*0.000001 = %lf\n", SampleRTT);

		if (SampleRTT < 0)
			SampleRTT *= -1;/*}}}*/
		
		if (!FD_ISSET(socket_fd, &fdset)) {/*{{{*/
			// timeout event (action: resend smallest sequence num pkt)
			//printf("timeout event send packet: ACK = %ld, SEQ = %ld\n", buf[buf_base % BUFFER_ENTRY].ACK, buf[buf_base % BUFFER_ENTRY].pkt.header.SEQ);
			sendto(socket_fd, &(buf[buf_base % BUFFER_ENTRY].pkt), sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));

			// congestion control
			Threshold = CongWin / 2;
			CongWin = PKT_TOTAL_SIZE;
			CongState = SLOWSTART;

			// CongWin have to be greater than 2, Threshold have to be greater than 1
			if (CongWin < 2*PKT_TOTAL_SIZE)
				CongWin = 2*PKT_TOTAL_SIZE;
			if (Threshold < PKT_TOTAL_SIZE)
				Threshold = PKT_TOTAL_SIZE;

			// timeout, continue...
			continue;
		}	/*}}}*/

		// receive ACK/*{{{*/
		recv(socket_fd, &header, sizeof(header), 0);
		
		checksum = header.checksum;
		header.checksum = 0;
/*}}}*/
		if (!is_corrupt((char *) &header, PKT_HEADER_SIZE, checksum)) {/*{{{*/
			// not corrupted

			//printf("received ACK = %ld\n", header.ACK);
			//printf("base is now %ld\n", base);
			if (header.ACK > base/*ExpectedACK*/) {
				//printf("good ACK = %ld\n", header.ACK);
				// good ACK
				base = header.ACK;
				
				// move Sender's buffer window
				for (i = buf_base; i < buf_index; i++) {
					if (buf[i % BUFFER_ENTRY].ACK == header.ACK) {
						//printf("==== updata buffer base ====\n");
						//printf("buf_base(%d) < i(%d) < buf_index(%d), BUFFER_ENTRY = %d\n", buf_base, i, buf_index, BUFFER_ENTRY);
						//printf("buf[i % BUFFER_ENTRY].ACK = header.ACK = %ld\n", header.ACK);
						//printf("new buf_base = %ld\n", i + 1);
						buf_base = i+1;	
						break;
					}
				}

				// update ExpectedACK
				ExpectedACK = buf[buf_base % BUFFER_ENTRY].ACK;
				
				// congestion contrl
				if (CongState == SLOWSTART) {
					if ( (CongWin + PKT_TOTAL_SIZE) <= WINDOW_SIZE ) {
						CongWin += PKT_TOTAL_SIZE;
						if (CongWin > Threshold)
							CongState = CONGAVOID;
					}
				} else if (CongState == CONGAVOID) {

					// CongWin have to be greater than 2, Threshold have to be greater than 1
					if (CongWin < 2*PKT_TOTAL_SIZE)
						CongWin = 2*PKT_TOTAL_SIZE;
					if (Threshold < PKT_TOTAL_SIZE)
						Threshold = PKT_TOTAL_SIZE;

					if ( (CongWin + PKT_TOTAL_SIZE*(PKT_TOTAL_SIZE/CongWin)) <= WINDOW_SIZE ) {
						CongWin += PKT_TOTAL_SIZE * (PKT_TOTAL_SIZE/CongWin);
					}
				}

			} else {
				printf("bad ACK = %ld, base now %ld\n", header.ACK, base);
				// duplicate ACK
				if (header.ACK == lastACK)
					ACKnum++;
				else {
					lastACK = header.ACK; 
					ACKnum = 0;
				}

				printf("ACKnum = %d\n", ACKnum);
				if (ACKnum >= 3) {
					// Fast Retransmission	
					printf("fast retrans send packet: ACK = %ld, SEQ = %ld\n", buf[buf_base % BUFFER_ENTRY].ACK, buf[buf_base % BUFFER_ENTRY].pkt.header.SEQ);
					sendto(socket_fd, &(buf[buf_base % BUFFER_ENTRY].pkt), sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));
					sendto(socket_fd, &(buf[buf_base % BUFFER_ENTRY].pkt), sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));
					sendto(socket_fd, &(buf[buf_base % BUFFER_ENTRY].pkt), sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));
					
					//printf("3 times transover\n");
					
					// reset ACKnum
					ACKnum = 0;

					// congestion control
					Threshold = CongWin / 2;
					CongWin = CongWin / 2; // Reno version
					CongState = CONGAVOID;

					// CongWin have to be greater than 2, Threshold have to be greater than 1
					if (CongWin < 2*PKT_TOTAL_SIZE)
						CongWin = 2*PKT_TOTAL_SIZE;
					if (Threshold < PKT_TOTAL_SIZE)
						Threshold = PKT_TOTAL_SIZE;

				}
			}	/*}}}*/
		} else {
			// corrupted (action: discard)
		}
	}/*}}}*/
 
	// drain all packets in buffer/*{{{*/
	while (1) {
		tv.tv_sec = 0;
		tv.tv_usec = 1;
		FD_ZERO(&fdset);
		FD_SET(socket_fd, &fdset);
		select(socket_fd+1, &fdset, NULL, NULL, &tv);
		
		if (!FD_ISSET(socket_fd, &fdset)) {
			break;
		} else {
			recv(socket_fd, &header, sizeof(header), 0);
		}
	}/*}}}*/
	
	// two-way disconnect/*{{{*/
	printf("before disconnect\n");
	disconnect(socket_fd, sendaddr);/*}}}*/

	return 0;

}/*}}}*/


int mkpkt(struct packet **pkt_ptr, int fd) {/*{{{*/

	off_t offset;
	int n;	
	struct packet *pkt;

	*pkt_ptr = (struct packet *) malloc( sizeof(struct packet) );
	pkt = *pkt_ptr;
	offset = lseek(fd, 0, SEEK_CUR);

	// initialize to 0
	memset(pkt, 0, sizeof(struct packet));

	// data
	n = read(fd, pkt->data, PKT_DATA_SIZE);

	// SEQ
	(pkt->header).SEQ = offset+1;

	// ACK
	(pkt->header).ACK = 0;

	// length
	(pkt->header).length = n;

	// flag
	(pkt->header).flag = (int8_t) 0;

	// checksum
	(pkt->header).checksum = 0;
	(pkt->header).checksum = get_checksum((char *)pkt+4, sizeof(struct packet)-4);

	return n;
}
/*}}}*/
void myconnect(int socket_fd, struct sockaddr_in sendaddr, int myport, char *filename) { /*{{{*/
	// 3-way handshake

	struct packet pkt;
	struct timeval tv;
	struct head header;
	int32_t length;
	int32_t checksum;
	fd_set fdset;

	// initialize select()
	tv.tv_sec = (int32_t) 1;
	tv.tv_usec = 0;
	FD_ZERO(&fdset);
	FD_SET(socket_fd, &fdset);	

	// initialize pkt to 0
	memset(&pkt, 0, sizeof(struct packet));

	// flag
	SET_SYN( (pkt.header).flag );

	// port number
	memcpy( &(pkt.data), &myport, 4);

	// file name
	memcpy( pkt.data+4, filename, strlen(filename));

	// length
	length =  4+strlen(filename);	// 4 is the size of "port number"
	pkt.header.length = length;

	// checksum
	checksum = get_checksum(((char *)&pkt)+4, sizeof(struct packet)-4);
	pkt.header.checksum = checksum;

	// send SYN packet and wait for confirmation
	while (1) {
		sendto(socket_fd, &pkt, sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));

		// wait for receiver to reply...
		tv.tv_sec = (int32_t) 5;
		FD_ZERO(&fdset);
		FD_SET(socket_fd, &fdset);
		select(socket_fd+1, &fdset, NULL, NULL, &tv);

		if (!FD_ISSET(socket_fd, &fdset)) {
			// not responding
			printf("SYN-Ack not received\n");
			continue;
		}

		// client respond with SYN-ACK, connection setupped!
		recv(socket_fd, &header, sizeof(header), 0);
		break;
	}

	return;
}
/*}}}*/
void disconnect(int socket_fd, struct sockaddr_in sendaddr) {/*{{{*/
	// two-way disconnection

	struct packet pkt;
	struct timeval tv;
	struct head header;
	int32_t checksum;
	fd_set fdset;

	// initialize select()
	tv.tv_sec = (int32_t) 1;
	tv.tv_usec = 0;
	FD_ZERO(&fdset);
	FD_SET(socket_fd, &fdset);

	// initialize pkt to 0	
	memset(&pkt, 0, sizeof(struct packet));

	// flag
	SET_FIN( (pkt.header).flag );

	// checksum
	checksum = get_checksum(((char *)&pkt)+4, sizeof(struct packet)-4);
	pkt.header.checksum = checksum;

	// now pkt is ready...

	// send FIN packet and wait for confirmation
	while (1) {
printf("send packet FIN\n");
		sendto(socket_fd, &pkt, sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));

		// wait for receiver to reply...
		tv.tv_sec = (int32_t) 1;
		FD_ZERO(&fdset);
		FD_SET(socket_fd, &fdset);
		select(socket_fd+1, &fdset, NULL, NULL, &tv);

		if (!FD_ISSET(socket_fd, &fdset)) {
			// not responding...
			continue;
		}

		// client respond with FIN-ACK, closing connection...
		recv(socket_fd, &header, sizeof(header), 0);

		if ( !IS_FIN(header.flag) ) 
			continue;
		else
			break;				
	}

	(pkt.header).flag = (int8_t) 0;
	(pkt.header).ACK |= 0xffffffff;

	// recompute check sum for this new ACK
	checksum = get_checksum(((char *)&pkt)+4, sizeof(struct packet)-4);
	pkt.header.checksum = checksum;


	while (1) {
printf("send FIN-ACK\n");
		sendto(socket_fd, &pkt, sizeof(struct packet), 0, (struct sockaddr *) &sendaddr, sizeof(sendaddr));

		// wait... if ACK loss, Receiver will resend FIN
		tv.tv_sec = (int32_t) 2;
		tv.tv_usec = 0;
		FD_ZERO(&fdset);
		FD_SET(socket_fd, &fdset);
		select(socket_fd+1, &fdset, NULL, NULL, &tv);

		if ( !FD_ISSET(socket_fd, &fdset) ) {
			// receiver successfully received ACK, close connection
			break;
		} else {
printf("received packet!!\n");
parse_header( (char *) &header );
			recv(socket_fd, &header, sizeof(header), 0);
			continue;
		}
	}

	return;

}/*}}}*/
void parse_header(char *buf)/*{{{*/
{
	int32_t checksum;
	int32_t seq_num;
	int32_t ack_num;
	int32_t pkt_len;
	int8_t flag;

	memcpy(&checksum, buf, 4);
	memcpy(&seq_num, buf+4, 4);
	memcpy(&ack_num, buf+8, 4);
	memcpy(&pkt_len, buf+12, 4);
	memcpy(&flag, buf+16, 1);

	printf("header: cs(%ld) sn(%ld) an(%ld) pl(%ld) f(%x)\n", checksum, seq_num, ack_num, pkt_len, flag);

	return;
}/*}}}*/
void print_pkt(struct packet *pkt) {/*{{{*/

	int i;
	char *c;

	c = (char *) pkt;

	for (i = 0; i < PKT_TOTAL_SIZE; i++) {
		printf("pkt[%ld] = %x %c %ld\n", i, *c, *c, *c);
		c++;
	}

	return;
}/*}}}*/


