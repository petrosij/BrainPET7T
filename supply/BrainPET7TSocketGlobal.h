/**
 * \file BrainPET7TSocketGlobal.h (/client_lauda/src/BrainPET7TSocketGlobal.h)
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

/// number of file descriptors used by func poll()
#define SOCKET_FDN 1
///	Time that function poll() waits [ms]
#define SOCKET_POLLTIMEOUT 100

#define SOCKET_BUFLEN 256


///Errors that can occur during program executing
#define SOCKET_ERRSEND 200			///Could not send a message
#define SOCKET_ERRRECV 201 		///Could not read a message
#define SOCKET_ERRUNEXPECT 202		///Unexpected message received
#define SOCKET_ERRNOREPLY 204 		///No reply
#define SOCKET_ERRHOSTRESOLVE 205 	///Failed to resolve host name
#define SOCKET_ERRCON 206 			///Could not connect to specified address
#define SOCKET_ERRSOCKMAKE 207 	///Could not create socket

/*
 * If in future exception handler will be implemented:
 *
 * For mistakes 200,201,204 recommended reconnect to the lauda cooling station.
 * If it is not helping try to read "errno". Could be helpful. Include <sys/errno.h>.
 *
 * For mistake 205: probably a mistake in the host name spelling, or host does not exist. Check the eixistig host list.
 * If it didn't solve a problem, look for "h_errno" value (include <netdb.h>). It can take following values:
 *		HOST_NOT_FOUND	The specified host is unknown.
 *		NO_ADDRESS or NO_DATA	The requested name is valid but does not have an IP address.
 *		NO_RECOVERY	A nonrecoverable name server error occurred.
 *		TRY_AGAIN	A temporary error occurred on an authoritative name server. Try again later.
 *
 * For mistake 206: (to find out which particular error you have check "errno" it will be assigned to one
 * of values listed below. Include <sys/errno.h> to get access to the "errno")
 * 		EBADF 	 The socket socket is not a valid file descriptor.
 *		ENOTSOCK File descriptor socket is not a socket.
 *		EADDRNOTAVAIL The specified address is not available on the remote machine.
 *		EAFNOSUPPORT The namespace of the addr is not supported by this socket.
 *		EISCONN The socket socket is already connected.
 *		ETIMEDOUT The attempt to establish the connection timed out.
 *		ECONNREFUSED The server has actively refused to establish the connection.
 *		ENETUNREACH The network of the given addr isn't reachable from this host.
 *		EADDRINUSE The socket address of the given addr is already in use.
 *		EINPROGRESS The socket socket is non-blocking and the connection could not be established
 *	 	 	 immediately. You can determine when the connection is completely established with select; . Another
 *	 	 	 connect call on the same socket, before the connection is completely established, will fail with
 *	 	 	 EALREADY.
 *		EALREADY The socket socket is non-blocking and already has a pending connection in progress
 *	 	 	 (see EINPROGRESS above).
 *
 * For mistake 207:(to find out which particular error you have check "errno" it will be assigned to one
 * of values listed below. Include <sys/errno.h> to get access to the "errno")
 * 		EMFILE The process already has too many file descriptors open.
 *		ENFILE The system already has too many file descriptors open.
 *		EACCES The process does not have the privilege to create a socket of the specified style or protocol.
 *		ENOBUFS The system ran out of internal buffer space.
 */
