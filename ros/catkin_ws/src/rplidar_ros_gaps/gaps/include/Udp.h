/*++

  Module Name:

    Udp.h

  Abstract:

    Declaration of commonly used UDP functionalities.

    USAGE:

      Channel         xchanl;
      UDPSend         xudpsend;
      UDPRecv         xudprecv;
      MyEventSinkImpl  mes;


      // multicast sender:
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
        char  szBuf[512] = {0};
        U32   ulCbSent   = 0L;


        GenerateStuff(szBuf, sizeof(szBuf));

        xudpsend.Send(szBuf, sizeof(szBuf), &ulCbSent);
      }


      // unicast receiver:
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
#ifndef __UDP_H__
#define __UDP_H__
#pragma once

#include <XCommon.h>
#include <XSocket.h>
#include <XThread.h>


class Channel;
typedef STDVEC<Channel>    ChannelVec;




//
// IPv4: XXX.XXX.XXX.XXX
// IPv6: XXXX.XXXX.XXXX.XXXX.XXXX.XXXX.XXXX.XXXX
//
#ifndef XIPADDRSTR_CCH
#define XIPADDRSTR_CCH      (40)
#endif  // !XIPADDRSTR_CCH


typedef struct UDPMSG
{
  UDPMSG(
    ):
    pMsg(NULL),
    ulCbMsg(0L)
  {
    szSrcIP[0] = '\0';
  }

  ~UDPMSG() {}


  char     szSrcIP[XIPADDRSTR_CCH];
  PVOID    pMsg;
  U32      ulCbMsg;

} *PUDPMSG;




/**
 *
 * Module client implement this class to receive the messages.
 *
 * Interface used to receive messages received via UDPRecv class.
 * Upon receiving a message, the ReceiveMessage will be invoked to
 * process the message.
 *
 */
class EventSinkPure
{
public:
  virtual
  void
  ReceiveMessage(
    PUDPMSG  pMsg  // IN
    ) = 0;
};




class Channel
{
public:
  Channel();
  Channel(const Channel&);
  virtual ~Channel();


  XRESULT
  Init(
    const char*  pszIpAddr,  // IN
    const U32    uPort       // IN
    );

  XRESULT
  DeInit(
    );

  Channel&
  operator=(
    const  Channel&
    );


  inline
  const sockaddr*
  C_SOCKADDR(
    ) const
  {
    return (const sockaddr*)&m_sockaddr;
  }

  inline
  U32
  C_SIZEOFSOCKADDR(
    ) const
  {
    return (U32)sizeof(m_sockaddr);
  }


private:
  inline
  void
  _Clear(
    )
  {
    memset(&m_sockaddr, 0, sizeof(XSockAddr_t));
  }

  inline
  void
  _Copy(
    const XSockAddr_t&  xsockaddr
    )
  {
    m_sockaddr.sin_family      = xsockaddr.sin_family;
    m_sockaddr.sin_addr.s_addr = xsockaddr.sin_addr.s_addr;
    m_sockaddr.sin_port        = xsockaddr.sin_port;
  }


  XSockAddr_t  m_sockaddr;

};  // Channel




class UDPSend
{
public:
  UDPSend();
  virtual ~UDPSend();


  //
  // Use 16 if multicast.  Otherwise use 0 for default value.
  //
  XRESULT
  Init(
    S32  iTTL = 0  // IN_OPT
    );

  XRESULT
  DeInit(
    );

  XRESULT
  AddChannel(
    const Channel&  xchanl  // IN
    );

  U32
  GetChannelCount(
    );

  XRESULT
  Send(
    const PVOID  pMsg,      // IN
    U32      ulCbMsg,   // IN
    U32*     pulCbSent  // IN_OPT
    );


private:
  ChannelVec  m_xchanlvec;
  S32         m_iFD;


};  // UDPSend




class UDPRecv
{
public:
  UDPRecv();
  virtual ~UDPRecv();


  //
  // pszGrpAddr  -  Multicast group IP address.  Use NULL for receiving
  //                unicast messages.
  //
  // iPort       -  Port from which to receive messages.
  //
  // bPortReuse  -  Use "true" if allow port reuse, otherwise use "false".
  //
  // Returns:
  //  RESULT_SUCCESS  -  If successful.
  //  RESULT_BUSY     -  If IP address and port are already in use.
  //  RESULT_FAILED   -  If otherwise.
  //
  XRESULT
  Init(
    const char*  pszGrpAddr,  // IN_OPT
    S32          iPort,       // IN
    bool         bPortReuse   // IN
    );

  XRESULT
  Init(
    const char*  pszIntrf,    // IN_OPT
    const char*  pszGrpAddr,  // IN_OPT
    S32          iPort,       // IN
    bool         bPortReuse   // IN
    );

  XRESULT
  DeInit(
    );

  void
  SetCallback(
    EventSinkPure*  pxes  // IN
    );

  XRESULT
  StartListen(
    );

  XRESULT
  StopListen(
    );

  void
  LoopAndReceiveMessage(
    bool  bVerbose = false  // IN
    );


private:
  //
  // Creates a socket.
  //
  // Returns:
  //  non negative integer  -  If successful.
  //  -1                    -  If otherwise.
  //
  static
  S32
  _CreateSocket(
    bool  bReuse = false  // IN
    );


private:
  XThread*         m_pxth;
  EventSinkPure*  m_pxes;

  S32   m_pipefd[2];
  S32   m_iFD;


};  // UDPRecv




#endif  // !__UDP_H__

