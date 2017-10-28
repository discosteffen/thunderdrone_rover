/*++

  Module Name:

    XSocket.h

  Abstract:

  History:

    10/12/2012    ChiChen   Created.
    05/02/2014    ChiChen   Added bind to device capability.

  Internal:

--*/
#ifndef __XSOCKET_H__
#define __XSOCKET_H__
#pragma once

#include <XCommon.h>
#include <sys/socket.h>
#include <netinet/in.h>  // sockaddr_in
#include <arpa/inet.h>  // inet_addr
#include <string.h>  // memset


// convenience macro
#ifndef __X_XSOCKADDR_DEFINED
#define __X_XSOCKADDR_DEFINED
typedef struct sockaddr_in    XSockAddr_t;

// init sockaddr with string ip address
#define XSOCKADDR_INITS( SADDR, IPS, PORT ) \
memset( &SADDR, 0, sizeof(SADDR) );         \
(SADDR).sin_family      = AF_INET;          \
(SADDR).sin_addr.s_addr = inet_addr( IPS ); \
(SADDR).sin_port        = htons( PORT );

// init sockaddr with integer ip address
#define XSOCKADDR_INITU( SADDR, IPU, PORT ) \
memset( &SADDR, 0, sizeof(SADDR) );         \
(SADDR).sin_family      = AF_INET;          \
(SADDR).sin_addr.s_addr = IPU;              \
(SADDR).sin_port        = htons( PORT );

#endif // !__X_XSOCKADDR_DEFINED


class XSocket
{
public:
  XSocket();

  // get file descriptor of socket
  INT32 GetFd() const;

  // connect by explicit ipv4 address
  bool Connect( UINT32 ipaddr, INT32 port );

  // connect to remote host/port
  bool Connect( const CHAR* ipaddr, INT32 port );

  // connect to remote host/port
  bool ConnectByName( const CHAR* hostname, INT32 port );

  // initialize Raw UDP socket
  bool InitRawUDP();

  // initialize UDP socket
  bool InitUDP();

  // initialize TCP socket
  bool InitTCP();

  // set nic interface by device name
  // can be used to set a nic interface for a TCP connection
  // example device names: eth0, eth1, etc
  // NOTE - this needs root privilege
  bool SetDevice( const CHAR* name );

  // setup as non-blocking
  bool SetNonBlock();

  // disable TCP nagle's algorithm
  bool SetNoDelay();

  // set send buffer size
  bool SetSendBufSize( INT32 bufsize );

  // set receive buffer size
  bool SetRecvBufSize( INT32 bufsize );

  // bind UDP socket to port
  bool BindUDP( INT32 port );

  // join IP socket to multicast group
  bool JoinMcastGroup( const CHAR* ipaddr, const CHAR* intrf );

  // set TTL for multicast publishing
  bool SetMcastTTL( INT32 ttl );

  // close socket
  void Close();

  // convert string ipv4 address to UINT32
  // return true and set ipaddr=0 for NULL and ""
  // return false for bogus cp values
  static bool ATON( const CHAR* cp, UINT32* ipaddr );

  // convert UINT32 to string ipv4 address
  static bool NTOA( UINT32 ipaddr, CHAR* buf, UINT32 len );

private:
  INT32  _fd;

}; // class XSocket


inline INT32 XSocket::GetFd() const
{
  return _fd;
}


#endif  // !__XSOCKET_H__

