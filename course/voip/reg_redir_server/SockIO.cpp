// vim:ft=cpp:foldmethod=marker
#include "SockIO.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

// {{{ SockIO::SockIO(){
SockIO::SockIO(){
	_outSd = _inSd = 0;
	_lastPort = 0;
} // }}}
// {{{ SockIO::~SockIO(){
SockIO::~SockIO(){
	if(_inSd)
		close(_inSd);
	if(_outSd)
		close(_outSd);
} // }}}
// {{{ bool SockIO::setIn(const int port){
bool SockIO::setIn(const int port){
	/* socket creation */
	if(_inSd)
		close(_inSd);
	_inSd = socket(AF_INET, SOCK_DGRAM, 0);
	if(_inSd<0) {
		cout << "Cannot open socket." << endl;
		return false;
	}
	/* bind local server port */
	_inAddr.sin_family = AF_INET;
	_inAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	_inAddr.sin_port = htons(port);
	if(bind(_inSd, (struct sockaddr *) &_inAddr,sizeof(_inAddr)) < 0){
		cout << "Cannot bind port number " << port << "." << endl;
		return false;
	}
	return true;
} // }}}
// {{{ bool SockIO::setOut(const string &ip, const int port){
bool SockIO::setOut(const string &ip, const int port){
	cout << "Set out addr: " << ip << ":" << port << endl;
	/* get server IP address (no check if input is IP address or DNS name */
	struct hostent *h=gethostbyname(ip.c_str());
	if(h==NULL) {
		cout << "Unknown host '" << ip << "'." << endl;
		return false;
	}
	_outAddr.sin_family = h->h_addrtype;
	memcpy((char *) &_outAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	_outAddr.sin_port = htons(port);
	if(_outSd == 0){
		/* socket creation */
		_outSd = socket(AF_INET, SOCK_DGRAM, 0);
		if(_outSd<0) {
			cout << "Cannot open socket." << endl;
			return false;
		}
		/* bind local server port */
		struct sockaddr_in tmpAddr;
		tmpAddr.sin_family = AF_INET;
		tmpAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		tmpAddr.sin_port = htons(0);

		if(bind(_outSd, (struct sockaddr *) &tmpAddr,sizeof(tmpAddr)) < 0){
			cout << "Cannot bind." << endl;
			return false;
		}
	}
} // }}}
// {{{ bool SockIO::setOutByLast(){
bool SockIO::setOutByLast(){
	if(_lastPort == 0)
		return false;
	return setOut(_lastIP, _lastPort);
} // }}}
// {{{ bool SockIO::write(const string &s){
bool SockIO::write(const string &s){
	if(sendto(_outSd, s.c_str(), s.size(), 0, (struct sockaddr *) &_outAddr, sizeof(_outAddr)) < 0){
		cout << "Send failed." << endl << s << endl;
		return false;
	}
	return true;
} // }}}
// {{{ string SockIO::read(){
string SockIO::read(){
	static char buf[4096];
	static struct sockaddr_in tmpAddr;
	static socklen_t tmpLen=sizeof(tmpAddr);
	static int n;

	n = recvfrom(_inSd, buf, 4096, 0, (struct sockaddr *) &tmpAddr, &tmpLen); 
	buf[n] = '\0';
	_lastPort = ntohs(tmpAddr.sin_port);
	_lastIP = inet_ntoa(tmpAddr.sin_addr);
	return buf;
} // }}}
