#ifndef __RPLIDARPROXY_H__
#define __RPLIDARPROXY_H__
#pragma once
#include <XCommon.h>
#include <Udp.h>
#include <RPLidarProxyStuff.h>






#define _BUF_SIZE_  ( 100 )




class RPLidarProxy: public EventSinkPure
{
public:
  typedef struct BufferEntry
  {
    volatile U32       _rcnt;  // read  counter
    volatile U32       _wcnt;  // write counter
    volatile S64       _ts;
    rplidar_reading_t  _rdn;

    BufferEntry(
      ):
      _rcnt( 0 ),
      _wcnt( 0 ),
      _ts( 0 ),
      _rdn()
    {
    }
  } BufferEntry_t;


public:
  RPLidarProxy();
  ~RPLidarProxy();

  S32  Init( S32 port );

  void SetVerbose( S32 verbose );

  void Start();
  void Stop();

  // udp callback
  void ReceiveMessage( PUDPMSG pMsg );

  // return 1 if successful
  // return 0 if no data is available
  S32  GetReading( rplidar_reading* rdn );

  U32  GetMsgCnt();


private:
  UDPRecv  _udprecv;

  S32      _verbose;

  S32      _port;
  U32      _msgCnt;

  U32      _expSubSeq;
  U32      _expSeq;

  // circular buffer management
  BufferEntry_t   _entVec[_BUF_SIZE_];
  BufferEntry_t*  _curEntR;
  BufferEntry_t*  _curEntW;
  volatile U32    _curridx;  // index to next entry to read
  volatile U32    _curwidx;  // index to next entry to write

};  // class RPLidarProxy




#endif // __RPLIDARPROXY_H__






