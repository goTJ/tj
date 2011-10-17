#ifndef SIPREQ_H
#define SIPREQ_H

#include <map>
#include <string>
#include "RegData.h"
#include "SockIO.h"

class SipReq: public RegData{
public:
	enum Method{UNKNOWN, ACK, REGISTER, INVITE};

	SipReq(const int port);
	void getHeader();
	void eraseHeader();
	Method getMethod(){return _method;};
	void send(const int code, const string &description);
private:
	SockIO _sockIO;
	Method _method;
	map<string, string> _header;
};

#endif
