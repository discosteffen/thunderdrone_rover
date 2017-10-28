#include <RPLidarProxy.h>




RPLidarProxy::RPLidarProxy(
  ):
  _udprecv(),
  _verbose( 0 ),
  _port( -1 ),
  _msgCnt( 0 ),
  _expSubSeq( 0 ),
  _expSeq( 0 ),
  _entVec(),
  _curEntR( NULL ),
  _curEntW( NULL ),
  _curridx( 0 ),
  _curwidx( 0 )
{
}


RPLidarProxy::~RPLidarProxy(
  )
{
}




S32 RPLidarProxy::Init( S32 port )
{
  S32      ret = -1;
  XRESULT  xr;

  // use NULL for unicast
  xr = _udprecv.Init( NULL, port, true );
  if ( X_FAILURE( xr ) )
  {
    if ( _verbose > 0 )
    {
      printf( "[RPLidarProxy::Init] Init failed.\n");
    }
    goto Exit;
  }

  _udprecv.SetCallback( this );

  _curEntR = _entVec;
  _curEntW = _entVec;

  ret = 1;

Exit:
  return ret;
}




void RPLidarProxy::SetVerbose( S32 verbose )
{
  _verbose = verbose;
}




void RPLidarProxy::Start()
{
  _udprecv.StartListen();
}

void RPLidarProxy::Stop()
{
  _udprecv.StopListen();
}




void RPLidarProxy::ReceiveMessage( PUDPMSG pMsg )
{
  // write received packets into buffer

  U32 cbmsg = pMsg->ulCbMsg;

  ++_msgCnt;

  if ( sizeof( rplidar_reading_pkt ) == cbmsg )
  {
    rplidar_reading_pkt* rdn = (rplidar_reading_pkt*)pMsg->pMsg;
    bool accept = false;
    
    if ( _expSeq == rdn->_seq && _expSubSeq == rdn->_subSeq )
    {
      accept = true;
      
      if ( rdn->_subSeq == 5 )
      {
        _expSeq    = ( rdn->_seq + 1 );
        _expSubSeq = 0;
      }
      else
      {
        _expSubSeq = ( rdn->_subSeq + 1 );
      }
    }
    else
    {
      if ( rdn->_subSeq == 0 )
      {
        accept = true;

        _expSeq    = rdn->_seq;
        _expSubSeq = ( rdn->_subSeq + 1 );
      }
    }

    if ( accept )
    {
      if ( _verbose > 0 )
      {
        printf(
          "[RPLidarProxy::ReceiveMessage] Received %u|%u  %u bytes.\n",
          rdn->_seq, rdn->_subSeq, cbmsg
          );
      }

      bool cpyEnt = false;

      if ( rdn->_subSeq == _BEG_SUB_SEQ_ )
      {
        // sub packet 0
        // check that write count is even and has been read
        if ( !( _curEntW->_wcnt & 0x01 ) && 
              ( _curEntW->_wcnt == _curEntW->_rcnt ) )
        {
          // set write counter to odd to indicate entry is now being writen
          ++_curEntW->_wcnt;
          cpyEnt = true;
        }
        else
        {
          if ( _verbose > 1 )
          {
            printf( "!!!!!!!! buffer full !!!!!!!!\n" );
          }
        }
      }
      else
      {
        //  sub packets 1-5
        // write count is odd and has been read
        if ( ( _curEntW->_wcnt & 0x01 ) && 
             ( 1 == ( _curEntW->_wcnt - _curEntW->_rcnt ) ) )
        {
          cpyEnt = true;
        }
        else
        {
          if ( _verbose > 1 )
          {
            printf( "!!!!!!!! bogus write entry !!!!!!!!"
                    " ( wcnt=%u,rcnt=%u )\n",
                    _curEntW->_wcnt, _curEntW->_rcnt );
          }
        }
      }

      if ( cpyEnt )
      {
        U32 idx    = ( rdn->_subSeq * _PKT_NODE_COUNT_ );
        U32 endIdx = ( idx          + _PKT_NODE_COUNT_ );
        U32 subIdx = 0;

        _curEntW->_rdn._seq       = rdn->_seq;
        _curEntW->_rdn._ascend    = rdn->_ascend;
        _curEntW->_rdn._count     = rdn->_count;
        _curEntW->_rdn._scanBegTs = rdn->_scanBegTs;
        _curEntW->_rdn._scanEndTs = rdn->_scanEndTs;

        for ( ; idx < endIdx; ++idx )
        {
          _curEntW->_rdn._agl[idx] = rdn->_agl[subIdx];
          _curEntW->_rdn._dst[idx] = rdn->_dst[subIdx];
          _curEntW->_rdn._qua[idx] = rdn->_qua[subIdx];
          ++subIdx;
        }
      }

      if ( rdn->_subSeq == _END_SUB_SEQ_ )
      {
        // last sub packet
        // set the write counter to even to indicate entry is now
        // ready for consumption
        // also advance the current write entry pointer
        ++_curEntW->_wcnt;

        if ( _curEntW == &_entVec[_BUF_SIZE_-1] )
        {
          _curEntW = &_entVec[0];
          _curwidx = 0;
        }
        else
        {
          ++_curEntW;
          ++_curwidx;
        }
      }
    }
    else
    {
      if ( _verbose > 1 )
      {
        printf( "======== sync... ========\n" );
      }
    }
  }
  else
  {
    if ( _verbose > 1 )
    {
      printf( "******** incomplete packet received ********\n" );
    }
  }
}




S32 RPLidarProxy::GetReading( rplidar_reading* rdn )
{
  S32 ret = 0;

  // check if the entry is not current being written
  // and has not been read since last write
  if ( !( _curEntR->_wcnt & 0x01 ) &&
        ( 2 == ( _curEntR->_wcnt - _curEntR->_rcnt ) ) )
  {
    const rplidar_reading* src = &_curEntR->_rdn;
  
    // set read counter to odd to indicate entry is now being read
    ++_curEntR->_rcnt;

    //-------------------------------------------------------------
    rdn->_seq       = src->_seq;
    rdn->_ascend    = src->_ascend;
    rdn->_count     = src->_count;
    rdn->_scanBegTs = src->_scanBegTs;
    rdn->_scanEndTs = src->_scanEndTs;
    
    for ( U32 i = 0; i < _NODE_COUNT_; ++i )
    {
      rdn->_agl[i] = src->_agl[i];
      rdn->_dst[i] = src->_dst[i];
      rdn->_qua[i] = src->_qua[i];
    }
    //-------------------------------------------------------------

    // set the read counter to even to indicate entry has been read
    ++_curEntR->_rcnt;

    // advance pointer
    if ( _curEntR == &_entVec[_BUF_SIZE_-1] )
    {
      _curEntR = &_entVec[0];
      _curridx = 0;
    }
    else
    {
      ++_curEntR;
      ++_curridx;
    }

    ret = 1;
  }

  return ret;
}




U32 RPLidarProxy::GetMsgCnt()
{
  return _msgCnt;
}






