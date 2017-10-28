/*++

  Module Name:

    XSocket.h

  Abstract:

  History:

    2012/10/12      ChiChen       Created.
    2012/11/25      ChiChen       Added TCP.

  Internal:

--*/
#include <XSocket.h>
#include <XStrSafe.h>
#include <unistd.h>  // for read / write / close
#include <fcntl.h>
#include <net/if.h>       // SO_BINDTODEVICE
#include <netinet/tcp.h>  // TCP_NODELAY
#include <sys/ioctl.h>    // ioctl
#include <errno.h> // EINPROGRESS, etc


// socket() returns -1 on failure
#define X_IS_SOCK_VALID  (-1 < _fd)


// helper function
bool _Connect( INT32 fd, XSockAddr_t* saddr, U32 size );
inline bool _Connect( INT32 fd, XSockAddr_t* saddr, U32 size )
{
  INT32 res = connect( fd, (struct sockaddr*)saddr, size );
  if ( -1 == res && EINPROGRESS == errno )
  {
    // non-blocking socket
    fd_set wfds;
    INT32 optval = 1;
    FD_ZERO(&wfds);
    FD_SET(fd, &wfds);
    if ( select( ( fd + 1 ), NULL, &wfds, NULL, NULL ) )
    {
      socklen_t len = sizeof( optval );
      getsockopt( fd, SOL_SOCKET, SO_ERROR, &optval, &len );
    }
    return ( 0 == optval );
  }
  return ( 0 == res );
}




XSocket::XSocket(
  ):
  _fd(-1)
{
}


// connect by explicit ipv4 address
bool XSocket::Connect( UINT32 ipaddr, INT32 port )
{
  XSockAddr_t  hostaddr;
  memset( &hostaddr, 0, sizeof( hostaddr ) );
  hostaddr.sin_addr.s_addr = ipaddr;
  hostaddr.sin_family      = AF_INET;
  hostaddr.sin_port        = htons(port);
  return _Connect( _fd, &hostaddr, sizeof( hostaddr ) );
}


// connect to remote host/port
bool XSocket::Connect( const CHAR* ipaddr, INT32 port )
{
  XSockAddr_t  hostaddr;
  memset( &hostaddr, 0, sizeof( hostaddr ) );
  if ( 1 == inet_pton( AF_INET, ipaddr, &hostaddr.sin_addr ) )
  {
    hostaddr.sin_family = AF_INET;
    hostaddr.sin_port   = htons(port);
    return _Connect( _fd, &hostaddr, sizeof( hostaddr ) );
  }
  return false;
}


// connect to remote host/port
bool XSocket::ConnectByName( const CHAR* hostname, INT32 port )
{
  return false;
}


// initialize Raw UDP socket
bool
XSocket::InitRawUDP()
{
  return false;
}


// initialize UDP socket
bool
XSocket::InitUDP()
{
  Close();
  _fd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  return ( X_IS_SOCK_VALID );
}


// initialize TCP socket
bool
XSocket::InitTCP()
{
  Close();
  _fd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  return ( X_IS_SOCK_VALID );
}


// set nic interface by device name
bool XSocket::SetDevice( const CHAR* name )
{
  if ( X_IS_SOCK_VALID )
  {
    UINT32 namelen = 0;
    if ( XStrCchLenA( name, XSTR_MAX_CCH, &namelen ) )
    {
      struct ifreq ifr;
      INT32 ret;
      memset( &ifr, 0, sizeof( ifr ) );
      XStrCopyA( ifr.ifr_name, sizeof( ifr.ifr_name ), name );
      ioctl( _fd, SIOCGIFINDEX, &ifr );
      ret = setsockopt( _fd, SOL_SOCKET, 
                        SO_BINDTODEVICE, &ifr, sizeof( ifr ) );
      return ( 0 == ret );
    }
  }
  return false;
}


// setup as non-blocking
bool
XSocket::SetNonBlock()
{
  if ( X_IS_SOCK_VALID )
  {
    INT32 val;
    val = fcntl(_fd, F_GETFL);
    fcntl(_fd, F_SETFL, val | O_NONBLOCK);
    return true;
  }
  return false;
}


// disable TCP nagle's algorithm
bool
XSocket::SetNoDelay(
  )
{
  if ( X_IS_SOCK_VALID )
  {
    INT32 optval = 1;
    socklen_t len = sizeof( optval );
    setsockopt( _fd, IPPROTO_TCP, TCP_NODELAY, &optval, len );
    getsockopt( _fd, IPPROTO_TCP, TCP_NODELAY, &optval, &len );
    return ( 1 == optval );
  }
  return false;
}


// set send buffer size
bool
XSocket::SetSendBufSize( INT32 bufsize )
{
  if ( X_IS_SOCK_VALID )
  {
    INT32 ret;
    socklen_t len = sizeof( bufsize );
    ret = setsockopt( _fd, SOL_SOCKET, SO_SNDBUF, &bufsize, len );
    return ( 0 == ret );
  }
  return false;
}


// set receive buffer size
bool
XSocket::SetRecvBufSize(
  INT32 bufsize
  )
{
  if ( X_IS_SOCK_VALID )
  {
    INT32 ret;
    socklen_t len = sizeof( bufsize );
    ret = setsockopt( _fd, SOL_SOCKET, SO_RCVBUF, &bufsize, len );
    return ( 0 == ret );
  }
  return false;
}


// bind UDP socket to port
bool
XSocket::BindUDP(
  INT32 port
  )
{
  if ( X_IS_SOCK_VALID )
  {
    XSockAddr_t  localsockaddr;
    INT32 iRet;

    memset( &localsockaddr, 0, sizeof( localsockaddr ) );
    localsockaddr.sin_family      = AF_INET;
    localsockaddr.sin_addr.s_addr = INADDR_ANY;
    localsockaddr.sin_port        = htons(port);
    iRet = ::bind( _fd,
                   (struct sockaddr*)&localsockaddr,
                   sizeof( localsockaddr ) );
    return ( 0 == iRet );
  }
  return false;
}


// join IP socket to multicast group
bool
XSocket::JoinMcastGroup(
  const CHAR*  ipaddr,
  const CHAR*  intrf
  )
{
  if ( X_IS_SOCK_VALID )
  {
    struct ip_mreq mreq;
    INT32 iRet;

    memset( &mreq, 0, sizeof(mreq) );
    mreq.imr_multiaddr.s_addr = inet_addr( ipaddr );
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if ( NULL != intrf )
    {
      mreq.imr_interface.s_addr = inet_addr(intrf);
    }
    iRet = setsockopt(_fd,
                      IPPROTO_IP,
                      IP_ADD_MEMBERSHIP,
                      &mreq,
                      sizeof(mreq) );
    return ( 0 == iRet );
  }
  return false;
}


// set TTL for multicast publishing
bool
XSocket::SetMcastTTL(
  INT32 ttl
  )
{
  if ( X_IS_SOCK_VALID )
  {
    INT32 iRet = setsockopt(_fd,
                            IPPROTO_IP,
                            IP_MULTICAST_TTL,
                            (const void*)&ttl,
                            sizeof(ttl));
    return ( 0 == iRet );
  }
  return false;
}


void
XSocket::Close(
  )
{
  if (X_IS_SOCK_VALID)
  {
    close(_fd);
    _fd = -1;
  }
}


// convert string ipv4 address to UINT32
//static
bool
XSocket::ATON(
  const CHAR*  cp,
  UINT32*      ipaddr
  )
{
  UINT32 addrlen = 0;

  // return true  for NULL, "", any valid address
  // return false for all other things
  if ( NULL == cp || XStrCchLenA( cp, XSTR_MAX_CCH, &addrlen ) )
  {
    (*ipaddr) = 0;
    if ( 0 < addrlen )
    {
      struct in_addr  srcip;
      if ( 1 != inet_pton( AF_INET, cp, &srcip ) )
      {
        goto Exit;
      }
      (*ipaddr) = srcip.s_addr;
    }
    return true;
  }
Exit:
  return false;
}


// convert UINT32 to string ipv4 address
//static
bool
XSocket::NTOA(
  UINT32  ipaddr,
  CHAR*   buf,
  UINT32  len
  )
{
  if ( NULL != buf && INET_ADDRSTRLEN <= len )
  {
    struct in_addr  srcip;
    srcip.s_addr = ipaddr;
    if ( NULL == inet_ntop( AF_INET, &srcip, buf, len ) )
    {
      buf[0] = '\0';
      goto Exit;
    }
    return true;
  }
Exit:
  return false;
}


