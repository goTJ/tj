// vim:ft=cpp:foldmethod=marker
#include <iostream>
#include "SipReq.h"
#include "RegDB.h"
#include <map>
#include <string>

using namespace std;

void initialize_description(map<int, string> &description)
{
	description[200] = "OK";
	description[404] = "Not Found";
	description[302] = "Moved Temporarily";
}

int main(int argc, char *argv[])
{
	SipReq sipReq(5060);
	RegDB regDB;
	map<int, string> description;

	initialize_description(description);
	while(1){
		sipReq.getHeader();
		switch(sipReq.getMethod()){
			case SipReq::REGISTER:
				cout << "Go register" << endl;
				regDB.insert(sipReq) ? sipReq.send(200, description[200]) : sipReq.send(404, description[404]);
				break;
			case SipReq::INVITE:
				cout << "Go invite" << endl;
				regDB.query(sipReq) ? sipReq.send(302, description[302]) : sipReq.send(404, description[404]);
				break;
			case SipReq::ACK:
				cout << "Go ack" << endl;
				break;
			default:
				cout << "Unknown" << endl;
		}
		sipReq.eraseHeader();
		cout << endl;
	}
	return 0;
}
