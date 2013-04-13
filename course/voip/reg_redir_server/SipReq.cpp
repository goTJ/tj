// vim:ft=cpp:foldmethod=marker
#include "SipReq.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cctype>

using namespace std;

// {{{ SipReq::SipReq(const int port){
SipReq::SipReq(const int port){
	_sockIO.setIn(port);
} // }}}
// {{{ void SipReq::getHeader(){
void SipReq::getHeader(){
	string header, s;
	int from, to;
	while(1){
		header = _sockIO.read();
		// Check if it's SIP header
		if((to=header.find("\r\n")) == string::npos)
			continue;
		s = header.substr(0, to);
		if(s.find("SIP/") == string::npos)
			continue;
		// Get Method
		s = s.substr(0, s.find(" "));
		if(s == "REGISTER"){
			_method = REGISTER;
		}else if(s == "INVITE"){
			_method = INVITE;
		}else if(s == "ACK"){
			_method = ACK;
		}else{
			_method = UNKNOWN;
		}
		// Read field
		from = to+2;
		while((to=header.find("\r\n", from)) != string::npos){
			s = header.substr(from, to-from);
			int pos=s.find(":");
			if(pos != string::npos){
				string key=s.substr(0, pos), val=s.substr(pos+1);
				if(key == "Contact"){
					_contact = val;
				}else if(key == "To"){
					_to = val;
				}else if(key == "Expires"){
					_expires = val;
				}else if(key == "Content-Length"){
					int n=atoi(val.c_str());
					header.erase(header.size()-n-2);
					_header[key] = "0";
				}else if(isupper(key[0])){
					_header[key] = val;
				}else{
					;
				}
			}
			from = to+2;
		}
		break;
	}
	_sockIO.setOutByLast();
} // }}}
// {{{ void SipReq::eraseHeader(){
void SipReq::eraseHeader(){
	eraseData();
	_header.clear();
} // }}}
// {{{ void SipReq::send(const int code, const string &description){
void SipReq::send(const int code, const string &description){
	char buf[8];
	string msg="SIP/2.0 ";

	cout << "Send " << code << " " << description << endl;
	sprintf(buf, "%d", code);
	msg += buf;
	msg += " "+description+"\r\n";
	if(_to != "")
		_header["To"] = _to;
	if(_contact != "")
		_header["Contact"] = _contact;
	if(_expires != "")
		_header["Expires"] = _expires;
	for(map<string, string>::iterator iter=_header.begin(); iter!=_header.end(); iter++)
		msg += iter->first + ":" + iter->second + "\r\n";
	msg += "\r\n";
	_sockIO.write(msg);
} // }}}
