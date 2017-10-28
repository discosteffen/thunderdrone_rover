#ifndef __RPLIDAR_PROXY_STUFF__
#define __RPLIDAR_PROXY_STUFF__

#define _NODE_COUNT_      ( 720 )
#define _PKT_NODE_COUNT_  ( 120 )
#define _BEG_SUB_SEQ_     ( 0 )
#define _END_SUB_SEQ_     ( 5 )


//
// break one reading into ( _NODE_COUNT_ / _PKT_NODE_COUNT_ )
// separate packets.
// if one sub packet is missing, drop/ignore the entire packet.
// all sub packets are sent in same order: 0, 1, 2, 3.
//
//  _seq=1,_subseq=0
//  _seq=1,_subseq=1
//  _seq=1,_subseq=2
//  _seq=1,_subseq=3
//
//  _seq=2,_subseq=0
//  _seq=2,_subseq=1
//  _seq=2,_subseq=2
//  _seq=2,_subseq=3
//  .
//  .
//  .
//
typedef struct rplidar_reading
{
  // all timestamps in nanoseconds
  U32 _seq;
  U32 _ascend;  // sorted?
  U32 _count;
  S64 _scanBegTs;
  S64 _scanEndTs;
  U16 _agl[_NODE_COUNT_];
  U16 _dst[_NODE_COUNT_];
  U8  _qua[_NODE_COUNT_];
  
  rplidar_reading(
    ):
    _seq( 0 ),
    _ascend( 0 ),
    _count( 0 ),
    _scanBegTs( 0 ),
    _scanEndTs( 0 ),
    _agl(),
    _dst(),
    _qua()
  {
  }
} rplidar_reading_t;


typedef struct __attribute__((__packed__)) rplidar_reading_pkt
{
  // all timestamps in nanoseconds
  U32 _seq;
  U32 _subSeq;
  U32 _ascend;
  U32 _count;
  S64 _scanBegTs;
  S64 _scanEndTs;
  U16 _agl[_PKT_NODE_COUNT_];
  U16 _dst[_PKT_NODE_COUNT_];
  U8  _qua[_PKT_NODE_COUNT_];
  
  rplidar_reading_pkt(
    ):
    _seq( 0 ),
    _subSeq( 0 ),
    _ascend( 0 ),
    _count( 0 ),
    _scanBegTs( 0 ),
    _scanEndTs( 0 ),
    _agl(),
    _dst(),
    _qua()
  {
  }
} rplidar_reading_pkt_t;


#endif  // !__RPLIDAR_PROXY_STUFF__




