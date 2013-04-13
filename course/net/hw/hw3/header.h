#ifndef	HEADER_H
#define HEADER_H

#define WINDOW_SIZE 65536
#define PKT_HEADER_SIZE 20	// bytes;
#define PKT_DATA_SIZE 1024	// bytes;
#define PKT_TOTAL_SIZE (PKT_HEADER_SIZE+PKT_DATA_SIZE)	// bytes;
#define MAX_FILENAME_LEN 128

#define EMPTY_FLAG(flag)	flag=0
#define SET_SYN(flag)	flag|=128
#define SET_FIN(flag)	flag|=64
#define IS_SYN(flag)	flag&128
#define IS_FIN(flag)	flag&64

long get_checksum(char *str, long len);	// return checksum
int is_corrupt(char *str, long len, long checksum);	// 0: false, not 0: true

#endif
