/**
 * \file BrainPET7TSocketComunication.h (/client_lauda/src/BrainPET7TSocketComunication.h)
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
#include "BrainPET7TSocketGlobal.h"

#include <string.h>			//memset()
#include <string>			//string
#include <poll.h>			//poll()
#include <arpa/inet.h>		//sockaddr_in



#ifndef BRAINPET7TSOCKETCOMUNICATION_H_
#define BRAINPET7TSOCKETCOMUNICATION_H_



class BrainPET7TSocketCommunication {
public:
	BrainPET7TSocketCommunication();
	virtual ~BrainPET7TSocketCommunication();

	/// Connect to the host via hostname or plain ip
	void connect_to_host(std::string);
	/// Disconnect from the host
	void disconnect();
	/// Reconnect to the same host (specified in host_name)
	void reconnect();
	/// Send message
	void send_msg(std::string);
	/// Receive message
	std::string recv_msg();

private:
	int sock;						//!< file descriptor
	struct sockaddr_in address;		//!< structure that stores address of host (in resolved form)
	struct pollfd pfd;				//!< structure for poll requests
	char buffer [SOCKET_BUFLEN];	//!< stores a message
	std::string host_name;			//!< host name

};

#endif /* BRAINPET7TSOCKETCOMUNICATION_H_ */
