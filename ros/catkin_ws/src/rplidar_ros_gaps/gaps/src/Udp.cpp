/*++

  Module Name:

    Udp.cpp

  Abstract:

    Definition of commonly used UDP functionalities.

    USAGE:

      Channel         xchanl;
      UDPSend         xudpsend;
      UDPRecv         xudprecv;
      MyEventSinkImpl  mes;


      // MULTICAST SENDER:
      //
      //          +------------------- multicast group
      //          |
      //          |                +-- destination port
      //          |                |
      //          V                V
      xchanl.Init("239.110.88.88", 8800);

      // set ttl
      //
      //            +----------------- 0: unicast, 16: multicast
      //            |
      //            V
      xudpsend.Init(16);
      xudpsend.AddChannel(xchanl);

      while (bRun)
      {
        char    szBuf[512] = {0};
        U32 ulCbSent   = 0L;


        GenerateStuff(szBuf, sizeof(szBuf));

        xudpsend.Send(szBuf, sizeof(szBuf), &ulCbSent);
      }


      // UNICAST RECEIVER:
      //
      //                 +-------------- unicast
      //                 |
      //                 |     +-------- receiving port
      //                 |     |
      //                 |     |     +-- no port reuse
      //                 |     |     |
      //                 V     V     V
      xr = xudprecv.Init(NULL, 9800, false);
      if (X_FAILURE(xr))
      {
        //
        // do error handling
        //
      }

      xudprecv.SetCallback(&mes);

      xr = xudprecv.StartListen();
      if (X_FAILURE(xr))
      {
        //
        // do error handling
        //
      }

      //
      // do other stuff
      //

      xr = xudprecv.StopListen();
      if (X_FAILURE(xr))
      {
        //
        // do error handling
        //
      }

  History:

    12/30/2009      ChiChen       Created.
    08/22/2013      ChiChen       Re-purposed.

  Internal:

--*/
#include <Udp.h>
#include <XStrSafe.h>
#include <unistd.h>
#include <errno.h>




//---------------------------------------------------------------------------
// HELPER FUNCTIONS
//---------------------------------------------------------------------------
extern "C"
{

static
PVOID
InvokeLoopFunction(
  PVOID  pv
  );

}


static
PVOID
InvokeLoopFunction(
  PVOID  pv
  )
{
  PXTHREADARG  pxarg   =  (PXTHREADARG)pv;
  UDPRecv*    pxrecv  =  (UDPRecv*)pxarg->pv;

  pxrecv->LoopAndReceiveMessage();

  return NULL;
}






//---------------------------------------------------------------------------
// XCHANNEL DEFINITIONS
//---------------------------------------------------------------------------
Channel::Channel(
  )
{
  _Clear();
}


Channel::Channel(
  const Channel&  channel
  )
{
  _Clear();
  _Copy( channel.m_sockaddr );
}


Channel::~Channel(
  )
{
  DeInit();
}


XRESULT
Channel::Init(
  const char* pszIpAddr,  // IN
  const U32   uPort       // IN
  )
{
  DeInit();

  m_sockaddr.sin_family      = AF_INET;
  m_sockaddr.sin_addr.s_addr = inet_addr(pszIpAddr);
  m_sockaddr.sin_port        = htons(uPort);

  if ( INADDR_NONE == m_sockaddr.sin_addr.s_addr )
  {
    return RESULT_FAILED;
  }

  return RESULT_SUCCESS;
}


XRESULT
Channel::DeInit(
  )
{
  _Clear();
  return RESULT_SUCCESS;
}


Channel&
Channel::operator=(
  const Channel&  rhs
  )
{
  if (this != &rhs)
  {
    _Clear();
    _Copy( rhs.m_sockaddr );
  }
  return (*this);
}






//---------------------------------------------------------------------------
// XUDSEND DEFINITIONS
//---------------------------------------------------------------------------
UDPSend::UDPSend(
  ):
  m_iFD(-1)
{
}


UDPSend::~UDPSend(
  )
{
  DeInit();
}


XRESULT
UDPSend::Init(
  S32  iTTL  // IN_OPT
  )
{
  XRESULT  xr  =  RESULT_FAILED;


  m_iFD = socket(AF_INET, SOCK_DGRAM, 0);
  if ( 0 > m_iFD )
  {
    printf( "UDPSend|SocketFailed|.\n" );
    goto Exit;
  }

  printf( "UDPSend|socket|%d|.\n", m_iFD );

  //
  // Set TTL.
  //
  if ( 0 < iTTL )
  {
    S32  iRet  =  setsockopt(m_iFD,
                             IPPROTO_IP,
                             IP_MULTICAST_TTL,
                             (const void*)&iTTL,
                             sizeof(iTTL));
    if ( 0 > iRet )
    {
      printf( "UDPSend|FailedToSetTTL|.\n");
      goto Exit;
    }

    printf( "UDPSend|SetTTL|%d|.\n", iTTL );
  }

  xr = RESULT_SUCCESS;


Exit:
  return xr;
}


XRESULT
UDPSend::DeInit(
  )
{
  if ( 0 < m_iFD )
  {
    close( m_iFD );
    m_iFD = -1;
  }

  m_xchanlvec.clear();

  return RESULT_SUCCESS;
}


XRESULT
UDPSend::AddChannel(
  const Channel&  xchanl
  )
{
  XRESULT xr = RESULT_SUCCESS;

  try
  {
    m_xchanlvec.push_back( xchanl );
  }
  catch ( std::bad_alloc& ex )
  {
    printf( "UDPSend|AddChannel|OutOfMemory|.\n" );
    xr = RESULT_NO_MEMORY;
  }

  return xr;
}


U32
UDPSend::GetChannelCount(
  )
{
  return (U32)m_xchanlvec.size();
}


XRESULT
UDPSend::Send(
  const PVOID  pMsg,      // IN
  U32          ulCbMsg,   // IN
  U32*         pulCbSent  // IN_OPT
  )
{
  if ( 0 <= m_iFD )
  {
    const U32 uChanlCnt     = GetChannelCount();
    U32    ulCbSentTotal = 0L;


    for ( U32 uIdx = 0; uIdx < uChanlCnt; ++uIdx )
    {
      const Channel& xchanl   = m_xchanlvec[uIdx];
      U32             ulCbSent = 0L;


      ulCbSent = sendto (m_iFD,
                         pMsg,
                         ulCbMsg,
                         0,
                         xchanl.C_SOCKADDR(),
                         xchanl.C_SIZEOFSOCKADDR() );
      if ( (U32)(-1) != ulCbSent )
      {
        ulCbSentTotal += ulCbSent;
      }
    }

    if ( NULL != pulCbSent )
    {
      (*pulCbSent) = ulCbSentTotal;
    }

    return RESULT_SUCCESS;
  }

  return RESULT_FAILED;
}  // UDPSend::Send






//---------------------------------------------------------------------------
// XUDPRECV DEFINITIONS
//---------------------------------------------------------------------------
UDPRecv::UDPRecv(
  ):
  m_pxth( NULL ),
  m_pxes( NULL ),
  m_iFD( -1 )
{
  m_pipefd[0] = ( -1 );
  m_pipefd[1] = ( -1 );
}


UDPRecv::~UDPRecv(
  )
{
  DeInit();
}




XRESULT
UDPRecv::Init(
  const char*  pszGrpAddr,  // IN_OPT
  S32          iPort,       // IN
  bool         bPortReuse   // IN
  )
{
  XRESULT    xr = RESULT_FAILED;
  XSockAddr_t  localsock;
  S32        iRet;


  m_iFD = _CreateSocket( bPortReuse );
  if (0 > m_iFD)
  {
    printf( "UDPRecv|FailedToCreateSocket|.\n");
    goto Exit;
  }

  //
  // Bind to specified port.
  //
  memset( &localsock, 0, sizeof( localsock ) );

  localsock.sin_family      = AF_INET;
  localsock.sin_addr.s_addr = INADDR_ANY;
  localsock.sin_port        = htons( iPort );

  iRet = ::bind( m_iFD, (struct sockaddr *) &localsock, sizeof( localsock ) );
  if (0 > iRet)
  {
    if ( EADDRINUSE == errno ) { xr = RESULT_BUSY; }

    printf(
      "UDPRecv|FailedToBind|errno=%d|.\n",
      errno );
    goto Exit;
  }

  //
  // Join multicast group.
  //
  if ( NULL != pszGrpAddr )
  {
    struct ip_mreq  mreq;


    memset( &mreq, 0, sizeof( mreq ) );
    mreq.imr_multiaddr.s_addr = inet_addr( pszGrpAddr );
    mreq.imr_interface.s_addr = htonl( INADDR_ANY );

    iRet = setsockopt( m_iFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq) );
    if ( 0 > iRet )
    {
      printf(
        "UDPRecv|FailedToJoinMcastMembership|errno=%d|.\n",
        errno);
      goto Exit;
    }
    else
    {
      printf(
        "UDPRecv|JoinedMembership|%s|.\n",
        pszGrpAddr);
    }
  }

  xr = RESULT_SUCCESS;


Exit:
  if ( X_FAILURE(xr) && 0 < m_iFD )
  {
    close(m_iFD);
    m_iFD = -1;
  }

  return xr;
}  // UDPRecv::Init


XRESULT
UDPRecv::Init(
  const char*  pszIntrf,    // IN_OPT
  const char*  pszGrpAddr,  // IN_OPT
  S32          iPort,       // IN
  bool         bPortReuse   // IN
  )
{
  XRESULT    xr = RESULT_FAILED;
  XSockAddr_t  localsock;
  S32        iRet;


  m_iFD = _CreateSocket( bPortReuse );
  if (0 > m_iFD)
  {
    printf( "UDPRecv|FailedToCreateSocket|.\n");
    goto Exit;
  }

  //
  // Bind to specified port.
  //
  memset( &localsock, 0, sizeof( localsock ) );

  localsock.sin_family      = AF_INET;
  localsock.sin_addr.s_addr = INADDR_ANY;
  localsock.sin_port        = htons( iPort );

  iRet = ::bind( m_iFD, (struct sockaddr *) &localsock, sizeof( localsock ) );
  if (0 > iRet)
  {
    if ( EADDRINUSE == errno ) { xr = RESULT_BUSY; }

    printf(
      "UDPRecv|FailedToBind|errno=%d|.\n",
      errno );
    goto Exit;
  }

  //
  // Join multicast group.
  //
  if ( NULL != pszGrpAddr )
  {
    struct ip_mreq  mreq;


    memset( &mreq, 0, sizeof( mreq ) );
    mreq.imr_multiaddr.s_addr = inet_addr( pszGrpAddr );
    mreq.imr_interface.s_addr = htonl( INADDR_ANY );
    if ( NULL != pszIntrf )
    {
      mreq.imr_interface.s_addr = inet_addr( pszIntrf );
    }

    iRet = setsockopt( m_iFD, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq) );
    if ( 0 > iRet )
    {
      printf(
        "UDPRecv|FailedToJoinMcastMembership|errno=%d|.\n",
        errno);
      goto Exit;
    }
    else
    {
      printf(
        "UDPRecv|JoinedMembership|%s|.\n",
        pszGrpAddr);
    }
  }

  xr = RESULT_SUCCESS;


Exit:
  if ( X_FAILURE(xr) && 0 < m_iFD )
  {
    close(m_iFD);
    m_iFD = -1;
  }

  return xr;
}  // UDPRecv::Init




XRESULT
UDPRecv::DeInit(
  )
{
  StopListen();

  if ( 0 < m_iFD )
  {
    close( m_iFD );
    m_iFD = -1;
  }

  m_pxes = NULL;

  return RESULT_SUCCESS;
}


void
UDPRecv::SetCallback(
  EventSinkPure*  pxes  // IN
  )
{
  m_pxes = pxes;
}


XRESULT
UDPRecv::StartListen(
  )
{
  XRESULT xr = RESULT_FAILED;


  if ( 0 > m_iFD )
  {
    printf(
      "UDPRecv|StartListen|UninitializedSocket|.\n");
    goto Exit;
  }

  StopListen();

  X_IGNORE_RESULT( pipe( m_pipefd ) );

  m_pxth = new XThread();
  if ( NULL == m_pxth )
  {
    xr = RESULT_NO_MEMORY;
    goto Exit;
  }

  m_pxth->Run( InvokeLoopFunction, this );

  xr = RESULT_SUCCESS;


Exit:
  return xr;
}


XRESULT
UDPRecv::StopListen(
  )
{
  //
  // Write one null byte to pipe to break up the loop in
  // the LoopAndReceiveMessage thread.
  //
  if ( NULL != m_pxth )
  {
    X_IGNORE_RESULT( write( m_pipefd[1], "", 1 ) );

    m_pxth->Join();
    delete m_pxth;
    m_pxth = NULL;

    close( m_pipefd[0] );
    close( m_pipefd[1] );
  }

  return RESULT_SUCCESS;
}


void
UDPRecv::LoopAndReceiveMessage(
  bool  bVerbose
  )
{
  fd_set  rset;
  S32     iMaxFdCnt;


  iMaxFdCnt = ( max( m_iFD, m_pipefd[0] ) + 1 );

  printf( "UDPRecv|LoopAndRecvMsg|Begin|.\n" );

  if ( NULL == m_pxes )
  {
    printf( "UDPRecv|LoopAndRecvMsg|NoEventSink|.\n" );
  }

  while ( true )
  {
    char         szBuf[4096]  =  {0};
    XSockAddr_t  srcaddr;
    UDPMSG       xudpmsg;
    char*        pszSrcIP;
    U32          uSrcAddrCb;
    U32          uMsgCb;
    S32          iReadyCnt;


    uSrcAddrCb = sizeof( srcaddr );
    memset( &srcaddr, 0, uSrcAddrCb );

    FD_ZERO( &rset );
    FD_SET( m_iFD,       &rset );
    FD_SET( m_pipefd[0], &rset );

    iReadyCnt = select( iMaxFdCnt, &rset, NULL, NULL, NULL );
    if ( 0 > iReadyCnt )
    {
      if ( EINTR == errno )
      {
        continue;
      }
      else
      {
        printf( "UDPRecv|LoopAndRecvMsg|SelectFailed|.\n" );
      }
    }

    if (FD_ISSET(m_iFD, &rset))
    {
      uMsgCb = recvfrom( m_iFD,
                         szBuf,
                         X_NUMBER_OF( szBuf ),
                         0,
                         (struct sockaddr*)&srcaddr,
                         (socklen_t*)&uSrcAddrCb );

      pszSrcIP = inet_ntoa(srcaddr.sin_addr);

      if ( 0UL < uMsgCb && NULL != m_pxes )
      {
        XStrCopyA( xudpmsg.szSrcIP, XIPADDRSTR_CCH, pszSrcIP );

        xudpmsg.pMsg    = szBuf;
        xudpmsg.ulCbMsg = uMsgCb;

        m_pxes->ReceiveMessage( &xudpmsg );
      }

      if (bVerbose)
      {
        printf(
          "UDPRecv|LoopAndRecvMsg|src=%s|msgcb=%d|.\n",
          pszSrcIP,
          uMsgCb );
      }
    }  // socket ready

    if ( FD_ISSET(m_pipefd[0], &rset) )
    {
      printf( "UDPRecv|LoopAndRecvMsg|RecvPipe|.\n" );
      X_IGNORE_RESULT( read( m_pipefd[0], &iReadyCnt, 1 ) );
      break;
    }  // pipe ready

  }  // forever loop

  printf( "UDPRecv|LoopAndRecvMsg|End|.\n" );

}  // UDPRecv::LoopAndReceiveMessage


//static
S32
UDPRecv::_CreateSocket(
  bool  bReuse  // IN
  )
{
  S32 iFD    = -1;


  iFD = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  if ( 0 > iFD )
  {
    printf(
      "UDPRecv|SocketFailed|errno=%d|.\n",
      errno );
    goto Exit;
  }

  if ( bReuse )
  {
    S32 iRet   = -1;
    S32 iReuse = 1;


    iRet = setsockopt( iFD, SOL_SOCKET, 
                       SO_REUSEADDR, &iReuse, sizeof( iReuse ) );
    if ( 0 > iRet )
    {
      printf(
        "UDPRecv|SetsockoptFailed|errno=%d|.\n",
        errno);
      close( iFD );
      iFD = -1;
      goto Exit;
    }
  }

Exit:
  return iFD;
}


