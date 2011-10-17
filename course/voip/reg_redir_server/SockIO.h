#ifndef SOCKIO_H
#define SOCKIO_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>

using namespace std;

class SockIO{
public:
	SockIO();
	~SockIO();
	bool setIn(const int port);
	bool setOut(const string &ip, const int port);
	bool setOutByLast();
	bool write(const string &);
	string read();
//	string &getLastIP(){ return _lastIP; };
//	string getLastPortStr(){ char buf[10]; sprintf(buf, "%d", _lastPort); return buf; };
private:
	int _inSd, _outSd;
	struct sockaddr_in _inAddr, _outAddr;
	int _lastPort;
	string _lastIP;
};

#endif
