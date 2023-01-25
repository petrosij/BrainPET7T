/**
 * \file BrainPET7TSocketComunication.cpp (/client_lauda/src/BrainPET7TSocketComunication.cpp)
 * \ingroup client_lauda
 * \brief
 *
 * \date:   Created on: Jan 30, 2020
 * \author: Skudarnov Petr (pskudarnov)
 * \version: 0.0
 *
 * \remarks 
 *
 * \todo TODO
 */
#include "BrainPET7TCoolingUnitGlobal.h"
#include "BrainPET7TSocketCommunication.h"
#include "BrainPET7TSocketCommunicationException.h"

#include <poll.h>			//poll()
#include <iomanip>			//setprecision()
#include <sstream>			//stringstream
#include <sys/socket.h>		//send()
#include <string.h>			//memset()
#include <unistd.h>			//close(),read()
#include <netdb.h>			//gethostbyname()
#include <arpa/inet.h>		//inet_pton(), inet_addr, sockaddr_in


BrainPET7TSocketCommunication::BrainPET7TSocketCommunication() {
	// TODO Auto-generated constructor stub
	memset(buffer,0,SOCKET_BUFLEN);
    address.sin_family = AF_INET;
    address.sin_port = htons(LAUDA_PORT);
    sock = -1;
    host_name.clear();
}

BrainPET7TSocketCommunication::~BrainPET7TSocketCommunication() {
	// TODO Auto-generated destructor stub
	disconnect();
}

void BrainPET7TSocketCommunication::set_host_name(std::string host){
	host_name = host;
}

std::string BrainPET7TSocketCommunication::get_host_name(){
	return host_name;
}

void BrainPET7TSocketCommunication::connect_to_host(){
	//Create socket

	if (sock == -1){
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1){
			throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_SOCKET_MAKE_ERROR_STRING,
					BrainPET7TSocketCommunicationException::SCError::SocketNotCreated);
		}

	}

	pfd.fd = sock;
	pfd.events = POLLIN; // There is data to read
	//Address setup
	if(inet_addr(host_name.c_str()) == INADDR_NONE){
		struct hostent *he;
		struct in_addr **addr_list;
		//resolve the hostname, its not an ip address
		if ((he = gethostbyname(host_name.c_str())) == NULL){
			throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_HOST_RESOLVE_ERROR_STRING,
					BrainPET7TSocketCommunicationException::SCError::HostNameNotResolved);
		}
			addr_list = (struct in_addr **) he->h_addr_list;
		for(int i = 0; addr_list[i] != NULL; i++){
			address.sin_addr = *addr_list[i];
			break;
		}
	}

	//plain ip address
	else{
		address.sin_addr.s_addr = inet_addr(host_name.c_str());
	}

	//Connect to remote server
	if (connect(sock, (struct sockaddr *)&address , sizeof(address)) < 0){
		throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_CONNECTION_ERROR_STRING,
					BrainPET7TSocketCommunicationException::SCError::ConectionFailed);
	}
}

void BrainPET7TSocketCommunication::disconnect(){
	if (sock!=-1){
		close(sock);
	}
	sock = -1;
	pfd.fd = -1;
}

void BrainPET7TSocketCommunication::reconnect(){
	disconnect();
	if (sock == -1){
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1){
			throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_SOCKET_MAKE_ERROR_STRING,
						BrainPET7TSocketCommunicationException::SCError::SocketNotCreated);
		}
	}
	pfd.fd = sock;
	if (connect(sock , (struct sockaddr *)&address , sizeof(address)) < 0){
		throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_CONNECTION_ERROR_STRING,
					BrainPET7TSocketCommunicationException::SCError::ConectionFailed);
	}
}

void BrainPET7TSocketCommunication::send_msg(std::string msg){


	if (send(sock,msg.c_str(),strlen( msg.c_str()),0) <= 0){
		throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_SEND_ERROR_STRING,
					BrainPET7TSocketCommunicationException::SCError::MessageNorSent);
	}
}

std::string BrainPET7TSocketCommunication::recv_msg(){

	std::string msgrecv;
	memset(buffer,0,SOCKET_BUFLEN);
	if (poll(&pfd,SOCKET_FDN,SOCKET_POLLTIMEOUT) > 0){
		if( recv( sock , buffer, SOCKET_BUFLEN, 0) <= 0){
			throw BrainPET7TSocketCommunicationException(BRAINPET7T_SOCKET_COMMUNICATION_RECV_ERROR_STRING,
						BrainPET7TSocketCommunicationException::SCError::MessageNotRead);
		}
	}

	msgrecv = buffer;
	return msgrecv;
}

