// vim:ft=cpp:foldmethod=marker
// {{{ include library
#include<cstdio>
#include<ctime>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include "header.h"
// }}}
#define DATA_ENTRY_NUM (WINDOW_SIZE/PKT_DATA_SIZE)

// {{{ struct Data{
struct Data{
	char buf[PKT_DATA_SIZE+1];
	char isgot;
	int len;
}; // }}}

// header: CHECKSUM(4)  SEQ(4)  ACK(4)  LENGTH(4)  SYN+FIN(1) OPIONAL(3)} 20bytes
// {{{ void parse_pkt(char *buf, int32_t *checksum, int32_t *seq_num, int32_t *ack_num, int32_t *pkt_len, int8_t *flag)
int parse_pkt(char *buf, int32_t buf_size, int32_t *checksum, int32_t *seq_num, int32_t *ack_num, int32_t *pkt_len, int8_t *flag, char **data_str)
{
	if(buf_size < PKT_HEADER_SIZE){
		printf("incorrect: nbytes(%d) is less than PKT_HEADER_SIZE(%d).\n", buf_size, PKT_HEADER_SIZE);
		return -1;
	}
	memcpy(checksum, buf, 4);
	memcpy(seq_num, buf+4, 4);
	memcpy(ack_num, buf+8, 4);
	memcpy(pkt_len, buf+12, 4);
	memcpy(flag, buf+16, 1);
	*data_str = buf+PKT_HEADER_SIZE;
	return 0;
} // }}}
// {{{ void set_header(char *buf, int32_t seq_num, int32_t ack_num, int32_t pkt_len, int8_t flag)
void set_header(char *buf, int32_t seq_num, int32_t ack_num, int32_t pkt_len, int8_t flag)
{
	int32_t checksum;

	memcpy(buf+4, &seq_num, 4);
	memcpy(buf+8, &ack_num, 4);
	memcpy(buf+12, &pkt_len, 4);
	memcpy(buf+16, &flag, 1);

	checksum = get_checksum(buf+4, 13+pkt_len);
	memcpy(buf, &checksum, 4);
} // }}}
// {{{ int wait_pkt(int fd, int sec)
int wait_pkt(int fd, int sec)
{
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec = sec;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(fd, &readfds);
	select(fd+1, &readfds, NULL, NULL, &tv);
	return FD_ISSET(fd, &readfds);
} // }}}
// {{{ void send_ACK_pkt(int sockfd, struct sockaddr_in to_addr, int ack_num)
void send_ACK_pkt(int sockfd, struct sockaddr_in to_addr, int ack_num)
{
	char buf[PKT_HEADER_SIZE+1];
	int8_t zero_flag;
	EMPTY_FLAG(zero_flag);

////	printf("send ACK(%d) to %s:%d\n", ack_num, inet_ntoa(to_addr.sin_addr), (int32_t)to_addr.sin_port);
	set_header(buf, 0, ack_num, 0, zero_flag);
	if(sendto(sockfd, buf, PKT_HEADER_SIZE, 0, (struct sockaddr *)&to_addr, sizeof(struct sockaddr)) == -1)
		perror("send");
} // }}}
// {{{ void send_SYN_pkt(int sockfd, struct sockaddr_in to_addr)
void send_SYN_pkt(int sockfd, struct sockaddr_in to_addr)
{
	char buf[PKT_HEADER_SIZE+1];
	int8_t syn_flag;
	EMPTY_FLAG(syn_flag);
	SET_SYN(syn_flag);

////	printf("send SYN to %s:%d\n", inet_ntoa(to_addr.sin_addr), (int32_t)to_addr.sin_port);
	set_header(buf, 0, 0, 0, syn_flag);
	if(sendto(sockfd, buf, PKT_HEADER_SIZE, 0, (struct sockaddr *)&to_addr, sizeof(struct sockaddr)) == -1)
		perror("send");
} // }}}
// {{{ void send_FIN_pkt(int sockfd, struct sockaddr_in to_addr)
void send_FIN_pkt(int sockfd, struct sockaddr_in to_addr)
{
	char buf[PKT_HEADER_SIZE+1];
	int8_t fin_flag;
	EMPTY_FLAG(fin_flag);
	SET_FIN(fin_flag);

////	printf("send FIN to %s:%d\n", inet_ntoa(to_addr.sin_addr), (int32_t)to_addr.sin_port);
	set_header(buf, 0, 0, 0, fin_flag);
	if(sendto(sockfd, buf, PKT_HEADER_SIZE, 0, (struct sockaddr *)&to_addr, sizeof(struct sockaddr)) == -1)
		perror("send");
} // }}}
// {{{ int addr_cmp(struct sockaddr_in a, struct sockaddr_in b)
int addr_cmp(struct sockaddr_in a, struct sockaddr_in b)
{
	return a.sin_addr.s_addr != b.sin_addr.s_addr;
} // }}}
int main(int argc, char *argv[])
{
	// {{{ declare variable
	int sockfd;
	struct sockaddr_in my_addr; /* my address information */
	struct sockaddr_in sender_addr; /* connector's address information */
	struct sockaddr_in client_addr; /* connector's address information */
	char read_buf[PKT_TOTAL_SIZE+1];
	int32_t nbytes, sockaddr_len;
	Data data[DATA_ENTRY_NUM];
	int out_bound_num;
	int8_t zero_flag;
	EMPTY_FLAG(zero_flag);
	// }}}
	long long recv_pkt_num, dup_pkt_num, filesize;
	double elapsed_time;
	struct timeval begin_tv, end_tv;
	struct timezone tz;

	// {{{ initialize socket
	if(argc != 2){
		printf("Usage: %s port\n", argv[0]);
		exit(0);
	}
	if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	my_addr.sin_family = AF_INET;	/* host byte order */
	my_addr.sin_port = htons(atoi(argv[1]));	/* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY;	/* auto-fill with my IP */
	bzero(&(my_addr.sin_zero), 8);	/* zero the rest of the struct */
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(1);
	} // }}}
	sockaddr_len = sizeof(client_addr);
	while((nbytes=recvfrom(sockfd, read_buf, PKT_TOTAL_SIZE, 0, (sockaddr *)&client_addr, (socklen_t *)&sockaddr_len)) != -1){
		read_buf[nbytes] = '\0';
		printf("======\n");
		printf("get %d bytes from %s:%d. [%s]\n", nbytes, inet_ntoa(client_addr.sin_addr), (int32_t)client_addr.sin_port, read_buf+PKT_HEADER_SIZE);
		// {{{ declare variable
		int32_t checksum, seq_num, ack_num, pkt_len;
		int8_t flag;
		char *data_str=read_buf+PKT_HEADER_SIZE;
		char filename[MAX_FILENAME_LEN];
		FILE *fout;
		int32_t base_seq_num=1;
		// }}}
		// {{{ initialize connection
		gettimeofday(&begin_tv, &tz);
		if(parse_pkt(read_buf, nbytes, &checksum, &seq_num, &ack_num, &pkt_len, &flag, &data_str) == -1)
			continue;
		printf("header: cs(%d) sn(%d) an(%d) pl(%d) f(%x)\n", checksum, seq_num, ack_num, pkt_len, flag);
		// error detection
		if(get_checksum(read_buf+4, nbytes-4) != checksum){
			printf("incorrect: wrong checksum.\n");
			continue;
		}
		if(!IS_SYN(flag)){
			printf("incorrect: I need a SYN pkt.\n");
			continue;
		}else{
			if(pkt_len < 5){
				printf("incorrect: pkt_len is less than 5.\n");
				continue;
			}
			// initialize connection
			int32_t port;
			memcpy(&port, data_str, 4);
			memcpy(filename, data_str+4, pkt_len-4);
			filename[pkt_len-4] = '\0';
			printf("port: %d filename: %s\n", port, filename);
			if((fout=fopen(filename, "w")) == NULL){
				perror("fopen");
				continue;
			}
			memset(data, 0, DATA_ENTRY_NUM*sizeof(Data));
			out_bound_num = 0;
			client_addr.sin_port = htons(port);
			client_addr.sin_family = AF_INET;
			// send SYN
			send_SYN_pkt(sockfd, client_addr);
		} // }}}
		// {{{ while(receive data){
		recv_pkt_num = 1;
		dup_pkt_num = filesize = 0;
		sockaddr_len = sizeof(sender_addr);
		while((nbytes=recvfrom(sockfd, read_buf, PKT_TOTAL_SIZE, 0, (sockaddr *)&sender_addr, (socklen_t *)&sockaddr_len)) != -1){
			recv_pkt_num++;
			read_buf[nbytes] = '\0';
////			printf("===\n");
			/*
			for(int i=0; i<nbytes; i++)
				printf("%d: %c, %d\n", i, read_buf[i], read_buf[i]);
			printf("\n");
			*/
			if(addr_cmp(sender_addr, client_addr)){
////				printf("incorrect: wrong ip address.\n");
				continue;
			}
////			printf("get %d bytes from %s:%d. [%p]\n", nbytes, inet_ntoa(client_addr.sin_addr), client_addr.sin_port, read_buf+PKT_HEADER_SIZE);
			// parse package
			if(parse_pkt(read_buf, nbytes, &checksum, &seq_num, &ack_num, &pkt_len, &flag, &data_str) == -1){
				send_ACK_pkt(sockfd, client_addr, base_seq_num);
				continue;
			}
////			printf("header: cs(%d) sn(%d) an(%d) pl(%d) f(%x)\n", checksum, seq_num, ack_num, pkt_len, flag);
////			printf("base_seq_num: %d\n", base_seq_num);
			// }}}
			// {{{ .   error or end detection
			if(get_checksum(read_buf+4, nbytes-4) != checksum){
////				printf("incorrect: wrong checksum.\n");
				send_ACK_pkt(sockfd, client_addr, base_seq_num);
				continue;
			}
			if(IS_SYN(flag)){
////				printf("incorrect: get syn pkt.\n");
				send_SYN_pkt(sockfd, client_addr);
				continue;
			}
			if(IS_FIN(flag)){
////				printf("end: get fin pkt.\n");
				break;
			}
			/*
			if(pkt_len != nbytes-PKT_HEADER_SIZE){
				printf("incorrect: pkt_len(%d) is unequal to nbytes(%d)-PKT_HEADER_SIZE(%d)\n", pkt_len, nbytes, PKT_HEADER_SIZE);
				send_ACK_pkt(sockfd, client_addr, base_seq_num);
				continue;
			}
			*/
			if(seq_num < base_seq_num || seq_num >= base_seq_num+WINDOW_SIZE){
////				printf("incorrect: seq_num(%d) is out of bound(%d~%d)\n", seq_num, base_seq_num, base_seq_num+WINDOW_SIZE);
				out_bound_num++;
				if(out_bound_num >= 3)
					send_ACK_pkt(sockfd, client_addr, base_seq_num);
				dup_pkt_num++;
				continue;
			}
////			printf("correct pkt\n");
			// }}}
			// {{{ .   put data into data_buf
			int idx=(seq_num/PKT_DATA_SIZE)&(DATA_ENTRY_NUM-1);
			memmove(data[idx].buf, data_str, pkt_len);
			data[idx].isgot = 1;
			data[idx].len = pkt_len;
			// change base sequence number & write to file if necesaary
			if(seq_num == base_seq_num){
				out_bound_num = 0;
				for(int i=idx; data[i].isgot; i=(i+1)&(DATA_ENTRY_NUM-1)){
////					printf("write data(%d) to file\n", base_seq_num);
					fwrite(data[i].buf, sizeof(char), data[i].len, fout);
					data[i].isgot = 0;
					base_seq_num += data[i].len;
				}
				filesize += pkt_len;
			}else{
////				printf("must have pkt miss!!\n");
			}
			// }}}
			// {{{ .   send ACK
			send_ACK_pkt(sockfd, client_addr, base_seq_num);
			sockaddr_len = sizeof(sender_addr);
			// sleep(5);
			// }}}
		// {{{ }
		}
		if(nbytes == -1){
			perror("recv error");
			continue;
		}
		// }}}
		// {{{ close connection
		if(IS_FIN(flag)){
			fclose(fout);
			printf("===\nnow FIN\n");
			// send FIN
			do{
				printf("wait FIN_ACK.\n");
				do{
					send_FIN_pkt(sockfd, client_addr);
				}while(!wait_pkt(sockfd, 5));
				if((nbytes=recvfrom(sockfd, read_buf, PKT_TOTAL_SIZE, 0, (sockaddr *)&sender_addr, (socklen_t *)&sockaddr_len)) == -1){
					perror("recv error");
					continue;
				}
				recv_pkt_num++;
				parse_pkt(read_buf, nbytes, &checksum, &seq_num, &ack_num, &pkt_len, &flag, &data_str);
				if(get_checksum(read_buf+4, nbytes-4) != checksum){
					printf("incorrect: wrong checksum.\n");
					continue;
				}
				if(ack_num == -1)
					break;
			}while(1);
		}
		gettimeofday(&end_tv, &tz);
		elapsed_time = (double)(end_tv.tv_sec-begin_tv.tv_sec)+(end_tv.tv_usec-begin_tv.tv_usec)*0.000001;
		printf("end\n===\n");
		// }}}
		printf("recv pkt: %lld, dup pkt: %lld\n", recv_pkt_num, dup_pkt_num);
		printf("filesize: %lld bytes, elapsed time: %.3lf sec, trans rate: %.3lf KBs/sec\n",
				filesize, elapsed_time, filesize/elapsed_time/1024);
		printf("======\n");
	}
	return 0;
}
