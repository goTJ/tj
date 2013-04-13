#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/wait.h>
#include<netdb.h>
#include<netinet/in.h>
#include "creator_s3c2410_lcd.h"
#include "creator_s3c2410_cmos.h"
#include "creator_lib.h"
#include "inc.h"

unsigned char *ctrl;
unsigned char *mask;
unsigned char *masklist;
//{{{int tcp_server(int port)
// build up a tcp server which listening the "port"
int tcp_server(int port){
	int fd;
	static struct sockaddr_in srv;

	// open server socket
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("SOCKET Create error");
		exit(1);
	}

	// bind to the port
	srv.sin_family = AF_INET;
	srv.sin_port = htons(port);
	srv.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(fd, (struct sockaddr*) &srv, sizeof(srv))< 0){
		perror("BIND error");
		exit(1);
	}

	// listen to the port
	if(listen(fd, 5) < 0) {
		perror("LISTEN error");
		exit(1);
	}
	return fd;
}
//}}}
//tcp_socket(char hostname[],int port){{{
int tcp_socket(char hostname[],int port){
	struct hostent *hp;
	static struct sockaddr_in srv_c;
	int fd_c;

	// translate hostname to host ip
	if((hp = gethostbyname(hostname)) == NULL){
		fprintf(stderr,"no host");
		exit(1);
	}

	// open a client socket
	if((fd_c = socket(AF_INET, SOCK_STREAM, 0))<0){
		perror("Clinet socket");
		exit(1);
	}

	srv_c.sin_family = AF_INET;
	srv_c.sin_port = htons(port);
	srv_c.sin_addr.s_addr = ((struct in_addr* ) (hp->h_addr))->s_addr;

	// connect to web server
	if(connect(fd_c, (struct sockaddr*) &srv_c, sizeof(srv_c))<0){
		perror("connect Error"); exit(1);
	}
	fprintf(stderr,"socket already connect!!");
	return fd_c;
}
//}}}
//void read_raw(char host[], int port){{{
void read_raw(char host[], int port){

	int terminate = 1, nRead;
	int dwTotalReadSize, dwImageSize, count;
	int fd_cmos;
	unsigned char pImageBuffer[2][WIDTH * HEIGHT];
	unsigned char *buffer[2][WIDTH];
	int i, j;
	int recode;
	int data_socket;
	int page = 0;
	int different_times=-3;
	
	unsigned char **buf;
	int numbytes;
	FILE *fout;
	int fd;
	char filename[20];
	int threshold = 0;

	unsigned int temp;
	cmos_info_t CMOSInfo;
	// do CMOSInfo
	fd_cmos = open("/dev/cmos", O_RDWR);
	
	CMOSInfo.ImageSizeMode = CMOS_SIZE_MODE_320_240;
	CMOSInfo.HighRef = 200;
	if(ioctl(fd_cmos, CMOS_ON, &CMOSInfo) <0)
		fprintf(stderr, "error");
	dwImageSize = WIDTH * HEIGHT;

	for(i = 0; i < HEIGHT; i++){
		buffer[0][i] = pImageBuffer[0] + i * WIDTH;
		buffer[1][i] = pImageBuffer[1] + i * WIDTH;
	}
	data_socket = tcp_socket(host, port);
	while(terminate){	

		if (ioctl(fd_cmos, CMOS_GET_STATUS, &CMOSInfo) >= 0){
			if (CMOSInfo.Status == CMOS_IMG_READY){

				dwTotalReadSize = 0; count = dwImageSize; 		
				// fetch the image from cmos
				if(different_times==0){
					page=0;
					printf("change the base!\n");
				}
				else
					page=1;

				do {


					if (count + dwTotalReadSize > dwImageSize)
						count = dwImageSize - dwTotalReadSize ;

					nRead = read(fd_cmos, pImageBuffer[page] + dwTotalReadSize, count);	

					if (nRead > 0 ){
						dwTotalReadSize += nRead;
					}
					else if (nRead == 0){
						break;	
					}
					else{
						//break;
					}
				} while (dwTotalReadSize < dwImageSize); 

				if(ctrl[0]){
					

					threshold = (ctrl[2]*256+ctrl[3])*1000;
					// deterimeted record or not	
					recode = (ctrl[1])?1:imagecompare(pImageBuffer[page], pImageBuffer[(page+1)%2],threshold, mask);
					printf("%d %d %d %d\n",page,buffer[0][0][0],buffer[1][0][0],recode);
					if(recode){
						buf = buffer[page];


						for(i = 0; i < 240;i++ )
							for(j = 0; j< 320;j++){
								temp = ((255 - buf[i][j])>60)?60:(255-buf[i][j]);
								buf[i][j]  = buf[i][j]+ temp*(1-mask[i*WIDTH+j]);
							}
						if (write(data_socket, buffer[page], HEIGHT * WIDTH) <0){
							perror("sendto error");
							exit(1);
						}	

						write(data_socket, "\r\n", 2);
						write(data_socket, "--myboundary\r\n", 14);
					}
					if(recode==1)
						different_times++;
					else
						different_times=1;
					if(different_times==5)
						different_times=0;

				}
				//				page = (page +1)%2;



			}
		}
		if(CMOSInfo.Status == CMOS_IMG_EMPTY){
			Delay(333);
		}
	}

}
//}}}
//{{{int get_data_socket_port(char host[], int init_port){
int get_data_socket_port(char host[], int init_port){
	int init_socket;
	int port;
	char buffer[MAX_LENG], temp[MAX_LENG];
	init_socket = tcp_socket(host,init_port);
	fprintf(stderr,"start write!!\n");
	write(init_socket, "HELO FJB\r\n", 10);
	sync();
	fprintf(stderr,"start read!!\n");
	read(init_socket, buffer, MAX_LENG);
	sscanf(buffer, "%s",temp);
	//	fprintf(stderr,"%s : %s",buffer,temp);
	if(strcmp(temp,"PINKY") == 0){
		sscanf(buffer, "%s %d",&temp,&port);
	}
	else{
		fprintf(stderr,"get data socket port error");
	}
	fprintf(stderr,"%d",port);
	return port;
}
//}}}
//void build_mask(){{{
void build_mask(){
	int i, j, index,r;
	
	for(i = 0; i < HEIGHT; i++){
		for(j= 0; j< WIDTH; j++)
			mask[i*WIDTH + j] = 1;
	}
	
	for(index = 0; index <  masklist[0]; index++ ){

		r = (double)masklist[4*index +4]*320.0/250.0;
			printf("==================%d %d %d %d=============",masklist[4*index+1], masklist[4*index+2],masklist[4*index+3],r);
		for(i = masklist[4*index +1]; i < masklist[4*index +2];i++){

			
			for(j = masklist[4*index +3]; j < r;j++)
				mask[i * WIDTH +j] = 0;
		}
	}
}
//}}}
//{{{int remove_mask(int index){
int remove_mask(int index){
	int i,j;
	int next;
	printf("**********%d********** ",index);
	if(index < masklist[0]){
		for(i = index; i < masklist[0]; i++){
			next = i+1;
			for(j = 1; j<=4; j++)
				masklist[ i*4 +j] = masklist[next *4 +j];
		}
		masklist[0]--;
		build_mask();
		return 1;
	}
	else
		return 0;
}
//}}}
//{{{int parse_command(char buffer[], int ctrl_socket){
//parse camara command 
int parse_command(char buffer[], int ctrl_socket){
	char command[MAX_LENG];
	char temp[MAX_LENG];
	int index;
	int i,j;
	int a,b,c,d;
	if(sscanf(buffer,"%s", command) == 1){
		if(strcmp(command,"stop") == 0){
			ctrl[0] = 0;
			return 1;
		}
		else if(strcmp(command, "start") ==0){
			ctrl[0] = 1;
			return 1;
		}
		else if(strcmp(command, "allrecord") == 0){
			ctrl[1] = 1;
			return 1;
		}
		else if(strcmp(command, "smartrecord") ==0){
			ctrl[1] = 0;
			return 1;
		}
		else if(strcmp(command, "rect_add") == 0){
			index = masklist[0];
			sscanf(buffer, "%s %u %u %u %u", command, &a,&b,&c,&d);
			printf("===%d %d %d %d===",a,b,c,d);
			
			masklist[4*index +1] = (a>255)?255:a; masklist[4*index +2] = (b>255)?255:b; masklist[4*index +3] = c; masklist[4*index +4] = (unsigned char)((int)((double)d/320.0*250.0));	
			

			printf("%d %s %u %u %u %u",masklist[0], command, masklist[4*index +1], masklist[4*index +2], masklist[4*index +3],(int) masklist[4*index +4] * 320/250);
			
			masklist[0]++;
			build_mask();
			printf("%d", masklist[0]);
			return 1;
		}
		else if(strcmp(command, "rect_del") == 0){
			sscanf(buffer,"%s %d", command, &index);
			return remove_mask(index);
		}
		else if(strcmp(command, "list") == 0){
			for(i = 0 ; i < masklist[0]; i++){
				sprintf(temp, "rect %d %d %d %d\r\n", masklist[i*4+1],masklist[i*4+2],masklist[i*4+3], (int)((double)masklist[i*4+4]*320.0/250.0));	
				write(ctrl_socket,temp, strlen(temp));
			}
			return 1;
		}
		else if(strcmp(command, "setthreshold") == 0){
			sscanf(buffer, "%s %d", command, &a);
			ctrl[2] = a /256;
			ctrl[3] = a % 256;
			return 1;
		}
		else
			return 0;
	}
}
//}}}
//{{{ initial setting
int init(){
	int i;
	ctrl[0] = 1, ctrl[1] = 1;
	ctrl[2] = 39, ctrl[3] = 16;
	for(i = 0; i < SHLEN_MASK;i++)
		mask[i] = 1;
	for(i = 0; i <SHLEN_MASKLIST;i++)
		masklist[i] = 0;

}
//}}}
int main(int argc, char *argv[]){
	int init_port;
	int data_port;
	int ctrl_socket;
	int init_socket;
	pid_t p;
	int shmid_ctrl, shmid_mask, shmid_masklist; 

	int fd_s;
	struct sockaddr_in cli;
	char buffer[MAX_LENG];
	socklen_t cli_len = sizeof(cli);
	if(argc < 3){
		fprintf(stderr, "get_raw hostname, port");
		exit(1);
	}
	sscanf(argv[2],"%d", &init_port);

	shmid_ctrl = shmget(SHMKEY_CTRL, SHLEN_CTRL, PERMS|IPC_CREAT);  //share memory
	shmid_mask = shmget(SHMKEY_MASK, SHLEN_MASK, PERMS|IPC_CREAT);  //share memory
	shmid_masklist = shmget(SHMKEY_MASKLIST, SHLEN_MASKLIST, PERMS|IPC_CREAT);  //share memory

	ctrl = (char *) shmat(shmid_ctrl, NULL, 0);	
	mask = (char *) shmat(shmid_mask, NULL, 0);	
	masklist = (char *) shmat(shmid_masklist, NULL, 0);	

	init();
	data_port = get_data_socket_port(argv[1], init_port);
	ctrl_socket = tcp_server(CTRL_PORT);
	p = fork();

	if(p <0){
		fprintf(stderr, "fork error");
		exit(1);
	}
	if(p == 0){
		char buffer[MAX_LENG];	
		nice(3);
		read_raw(argv[1], data_port);
		exit(0);
	}
	nice(-1);
	while(1){
		if((fd_s = accept(ctrl_socket, (struct sockaddr*)&cli, &cli_len))<0){perror("ACCEPT error"); exit(1);}
		if (!fork()){
			read(fd_s, buffer, MAX_LENG);
			if(parse_command(buffer, fd_s) ==1){
				write(fd_s, "ok\r\n",4);
			}
			else
				write(fd_s, "error\r\n",7);
			exit(0);
			close(fd_s);	
		}
		close(fd_s);
		while(waitpid(-1,NULL, WNOHANG)>0);
	}
	close(ctrl_socket);


}
