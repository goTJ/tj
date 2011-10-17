// vim:ft=cpp:foldmethod=marker
// {{{ include library
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/wait.h>
#include<arpa/inet.h>
// }}}
#define BUFFER_SIZE 8192	// must larger than total header size

// {{{ class FdIO{
class FdIO{
public:
	FdIO(int fd);
	char *getln(char buf[], size_t size);
	bool waitin(int sec);
	void writ(char *str);
	void writ(char *str, int n);
	void flush();
// {{{ private:
private:
	FdIO();
	static const int _SIZE=8192;
	char _inbuf[_SIZE];
	int _in_pos, _in_size;
	char _outbuf[_SIZE];
	int _out_pos;
	int _fd;
// }}}
}; // }}}
// {{{ FdIO method
// {{{ FdIO::FdIO(int fd)
FdIO::FdIO(int fd)
{
	_in_pos = _in_size = 0;
	_out_pos = 0;
	_fd = fd;
} // }}}
// {{{ char *FdIO::getln(char buf[], size_t size)
char *FdIO::getln(char buf[], size_t size)
{
	int i=0;

	while(1){
		if(_in_pos == _in_size){
			_in_size = read(_fd, _inbuf, _SIZE);
			if(_in_size == 0){
				if(i){
					buf[i] = '\0';
					return buf;
				}else{
					return NULL;
				}
			}
			_in_pos = 0;
		}
		if(_inbuf[_in_pos] == '\n'){
			if(i > 0 && buf[i-1]=='\r')
				buf[i-1] = '\0';
			else
				buf[i] = '\0';
			_in_pos++;
			return buf;
		}else{
			buf[i] = _inbuf[_in_pos];
		}
		_in_pos++;
		i++;
		if(i == size){
			buf[i-1] = '\0';
			return buf;
		}
	}
} // }}}
// {{{ bool FdIO::waitin(int sec)
bool FdIO::waitin(int sec)
{
	struct timeval tv;
	fd_set readfds;

	if(_in_pos < _in_size)
		return true;
	tv.tv_sec = sec;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(_fd, &readfds);
	select(_fd+1, &readfds, NULL, NULL, &tv);
	return FD_ISSET(_fd, &readfds);
} // }}}
// {{{ void FdIO::writ(char *str)
void FdIO::writ(char *str)
{
	writ(str, strlen(str));
} // }}}
// {{{ void FdIO::writ(char *str, int size)
void FdIO::writ(char *str, int size)
{
	while(_out_pos+size >= _SIZE){
		int n=_SIZE-_out_pos;
		memmove(&_outbuf[_out_pos], str, n);
		write(_fd, _outbuf, _SIZE);
		_out_pos = 0;
		size -= n;
		str += n;
	}
	if(size){
		memmove(&_outbuf[_out_pos], str, size);
		_out_pos += size;
	}
} // }}}
// {{{ void FdIO::flush()
void FdIO::flush()
{
	write(_fd, _outbuf, _out_pos);
	_out_pos = 0;
} // }}}
// }}}

// {{{ class HttpReq{
class HttpReq{
public:
	HttpReq(FdIO *cli);
	bool parseCmd();
	void parseHeader();
	void outputHeader();
	bool isheadcmd();
	bool iskeepalive();
	void outputContent();
	int code;
	bool connect_type;	// true: Keep-Alive, false: close
	char ori_filepath[PATH_MAX];
	char filepath[PATH_MAX];
	char version[16];
	enum CmdMode{NONE, GET, PUT, HEAD};
	CmdMode cmd;
	int max_request;
// {{{ private:
private:
	FdIO *_cli;
	static const int _SIZE=8192;
	char _buf[_SIZE];
// }}}
}; // }}}
// {{{ HttpReq method
// {{{ HttpReq::HttpReq(FdIO *cli)
HttpReq::HttpReq(FdIO *cli)
{
	_cli = cli;
	max_request = 100;
} // }}}
// {{{ bool HttpReq::parseCmd()
bool HttpReq::parseCmd()
{
	if(max_request == 0)
		return false;
	code = 200;
	cmd = NONE;
	connect_type = true;
	version[0] = '\0';
	char cmd_str[_SIZE];
	char *p;
	do{
		if(!_cli->waitin(10) || _cli->getln(cmd_str, _SIZE) == NULL)
			return false;
		p = strtok(cmd_str, " ");
	}while(!p);
	if(!strcmp(p, "GET")){
		cmd = GET;
	}else if(!strcmp(p, "PUT")){
		cmd = PUT;
	}else if(!strcmp(p, "HEAD")){
		cmd = HEAD;
	}else{
		code = 501;
		return true;
	}
	// parse filename
	p = strtok(NULL, " ");
	if(p == NULL || p[0] != '/'){
		code = 501;
		return true;
	}
	strncpy(ori_filepath, p+1, PATH_MAX);
	strcpy(filepath, ori_filepath);
	if(filepath[0] == '\0'){
		filepath[0] = '.';
		filepath[1] = '\0';
	}else if(filepath[0] == '.' && filepath[1] == '.' && filepath[2] == '/'){
		filepath[1] = '/';
	}
	// parse version
	p = strtok(NULL, " ");
	if(p == NULL){
		code = 501;
		return true;
	}
	strncpy(version, p, 15);
	p[15] = '\0';
	return true;
} // }}}
// {{{ void HttpReq::parseHeader()
void HttpReq::parseHeader()
{
	if(code != 200)
		return;
	while(_cli->getln(_buf, _SIZE) != NULL){
	//	printf("get: %s\n", _buf);
		if(_buf[0] == '\0')
			break;
		char *p=strtok(_buf, ":");
		if(!strcmp(p, "Connection")){
			p = strtok(NULL, ":");
			if(p == NULL)
				continue;
			while(*p == ' ')
				p++;
			for(char *pp=p; *pp!='\0'; pp++){
				if(isupper(*pp))
					*pp = tolower(*pp);
			}
			connect_type = (!strcmp(p, "close")) ? false : true;
		}
	}
	if(max_request == 1)
		connect_type = false;
} // }}}
// {{{ void HttpReq::outputHeader()
void HttpReq::outputHeader()
{
	if(code != 200)
		return;
	// check filepath
	struct stat stbuf;
	stat(filepath, &stbuf);
	if(S_ISDIR(stbuf.st_mode&S_IFMT)){
		strcat(filepath, "/index.html");
		if(access(filepath, F_OK) == -1)
			filepath[strlen(filepath)-1] = '\0';
	}
	if(access(filepath, F_OK) == -1){
		code = 404;
	}else if(access(filepath, R_OK) == -1){
		code = 403;
	}
	// output code {{{
	_cli->writ(version);
	switch(code){
		case 200:
			_cli->writ(" 200 OK\r\n");
			break;
		case 404:
			_cli->writ(" 404 Not Found\r\n");
			break;
		default:
			sprintf(_buf, " %d\r\n", code);
			_cli->writ(_buf);
	} // }}}
	// output Connection {{{
	if(connect_type){
		sprintf(_buf, "Keep-Alive: timeout=10, max=%d\r\n", max_request);
		_cli->writ(_buf);
		_cli->writ("Connection: Keep-Alive\r\n");
	}else{
		_cli->writ("Connection: close\r\n");
	} // }}}
	if(code == 200){
		stat(filepath, &stbuf);
		{ // output Last-Modified {{{
			struct tm *tmptr=gmtime(&stbuf.st_mtime);
			strftime(_buf, _SIZE, "Last-Modified: %a, %d %b %Y %X %Z\r\n", tmptr);
			_cli->writ(_buf);
		} // }}}
		{ // output Content-Length {{{
			sprintf(_buf, "Content-Length: %d\r\n", stbuf.st_size);
			_cli->writ(_buf);
		} // }}}
	}else{
		_cli->writ("Transfer-Encoding: chunked\r\n");
	}
	{ // output Content-Type {{{
		int len=strlen(filepath);

		if(code != 200){
			_cli->writ("Content-Type: text/html\r\n");
		}else if(len >= 6 && !strcmp(filepath+len-5, ".html")){
			_cli->writ("Content-Type: text/html\r\n");
		}else if(len >= 5 && !strcmp(filepath+len-4, ".htm")){
			_cli->writ("Content-Type: text/html\r\n");
		}else if(len >= 5 && !strcmp(filepath+len-4, ".css")){
			_cli->writ("Content-Type: text/css\r\n");
		}else if(len >= 5 && !strcmp(filepath+len-4, ".css")){
			_cli->writ("Content-Type: image/icon\r\n");
		}else if(len >= 5 && !strcmp(filepath+len-4, ".gif")){
			_cli->writ("Content-Type: image/gif\r\n");
		}else if(len >= 5 && !strcmp(filepath+len-4, ".jpg")){
			_cli->writ("Content-Type: image/jpg\r\n");
		}
	} // }}}
	_cli->writ("\r\n");
	_cli->flush();
	printf("filepath: %-16s, code: %d, max: %3d, connect: %s\n", ori_filepath, code, max_request, (connect_type)?"Keep-Alive":"Close");
	max_request--;
} // }}}
// {{{ bool HttpReq::isheadcmd()
bool HttpReq::isheadcmd()
{
	return (cmd==HEAD);
} // }}}
// {{{ bool HttpReq::iskeepalive()
bool HttpReq::iskeepalive()
{
	return connect_type;
} // }}}
// {{{ void HttpReq::outputContent()
void HttpReq::outputContent()
{
	switch(code){
		case 200:{
			int fd=open(filepath, O_RDONLY);
			if(fd == -1)
				return;
			int n;
			while((n=read(fd, _buf, _SIZE)))
				_cli->writ(_buf, n);
			close(fd);
			break;
		}case 404:{
			sprintf(_buf, "%x\r\n", 198+strlen(ori_filepath));
			_cli->writ(_buf);
			_cli->writ("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
			_cli->writ("<HTML><HEAD>\n");
			_cli->writ("<TITLE>404 Not Found</TITLE>\n");
			_cli->writ("</HEAD><BODY>\n");
			_cli->writ("<H1>Not Found</H1>\n");
			sprintf(_buf, "The requested URL %s was not found on this server.<P>\n", ori_filepath);
			_cli->writ(_buf);
			_cli->writ("<HR>\n");
//			_cli->writ("<ADDRESS>Apache/1.3.33 Server at www.csie.ntu.edu.tw Port 80</ADDRESS>\n");
			_cli->writ("</BODY></HTML>\n");
			_cli->writ("\r\n0\r\n\r\n");
			break;
		}default:{
			_cli->writ("<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n");
			_cli->writ("<HTML><HEAD>\n");
			sprintf(_buf, "<TITLE>%d</TITLE>\n", code);
			_cli->writ(_buf);
			_cli->writ("</HEAD><BODY>\n");
			sprintf(_buf, "%d\n", code);
			_cli->writ(_buf);
			_cli->writ("</BODY></HTML>\n");
		}
	}
	_cli->flush();
} // }}}
// }}}

int main(int argc, char *argv[])
{
	int sfd, cfd;
	struct sockaddr_in srv, cli;
	int cli_len=sizeof(cli);

	// {{{
	if(argc < 2){
		printf("Usage: %s server_port\n", argv[0]);
		exit(1);
	}
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
	if(listen(sfd, 5) < 0){
		perror("listen error.");
		exit(1);
	}
	// }}}
	while(1){
		if((cfd=accept(sfd, (struct sockaddr *)&cli, (socklen_t *)&cli_len)) < 0){
			perror("accept error.");
			exit(1);
		}
		if(!fork()){
			printf("[[[ Client %s:%d connect. ]]]\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
			HttpReq http(new FdIO(cfd));
			char buf[BUFFER_SIZE];
			char *p;

			while(http.parseCmd()){
				http.parseHeader();
				http.outputHeader();
				if(!http.isheadcmd())
					http.outputContent();
				if(!http.iskeepalive())
					break;
			}
			close(cfd);
			printf("[[[ Client %s:%d disconnect. ]]]\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
		}
		close(cfd);
		while(waitpid(-1,NULL,WNOHANG) > 0)
			;
	}
	return 0;
}
