#include <RPLidar.h>


// returns nanoseconds
inline S64 __getsystime()
{
  timespec time;
  clock_gettime(CLOCK_REALTIME, &time);
  return (((S64)time.tv_sec) * 1000000000ULL + (S64)time.tv_nsec);
}




RPLidar::RPLidar(
  ):
  _readings(),
  _drv( NULL ),
  _expressModeOK( false )
{
}


RPLidar::~RPLidar()
{
  if ( _drv != NULL )
  {
    if ( _drv->isConnected() )
    {
      _drv->stop();
      _drv->stopMotor();
    }
    RPlidarDriver::DisposeDriver( _drv );
  }
}


S32
RPLidar::Init( const char* device )
{
  S32 ret = -1;

  _drv = RPlidarDriver::CreateDriver(
    RPlidarDriver::DRIVER_TYPE_SERIALPORT );
  if ( _drv == NULL )
  {
    fprintf( stderr, "[RPLidar::Init] out of memory.\n" );
    goto Exit;
  }

  _drv->checkExpressScanSupported( _expressModeOK );

  if ( IS_FAIL( _drv->connect( device, 115200 ) ) )
  {
    fprintf( stderr,
             "[RPLidar::Init] failed to connect %s. "
             "Did you run as sudo?\n",
             device );
    goto Exit;
  }

  ret = 1;

Exit:
  return ret;
}


S32
RPLidar::GetSDKVersion( char* buf, S32 len )
{
  return snprintf( buf, len, "%s", RPLIDAR_SDK_VERSION );
}


S32
RPLidar::GetSerialNumber( char* buf, S32 len )
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_device_info_t devinfo;
    u_result res;

    res = _drv->getDeviceInfo( devinfo );

    if ( IS_FAIL( res ) )
    {
      fprintf( stderr, "[RPLidar::GetSerialNumber] getDeviceInfo failed.\n" );
      goto Exit;
    }

    ret = snprintf(
      buf, len,
      "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
      devinfo.serialnum[0],  devinfo.serialnum[1],
      devinfo.serialnum[2],  devinfo.serialnum[3],
      devinfo.serialnum[4],  devinfo.serialnum[5],
      devinfo.serialnum[6],  devinfo.serialnum[7],
      devinfo.serialnum[8],  devinfo.serialnum[9],
      devinfo.serialnum[10], devinfo.serialnum[11],
      devinfo.serialnum[12], devinfo.serialnum[13],
      devinfo.serialnum[14], devinfo.serialnum[15]
      );

    ret = 1;
  }

Exit:
  return ret;
}  // RPLidar::GetSerialNumber


S32
RPLidar::GetFirmware( char* buf, S32 len )
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_device_info_t devinfo;
    u_result res;

    res = _drv->getDeviceInfo( devinfo );

    if ( IS_FAIL( res ) )
    {
      fprintf( stderr, "[RPLidar::GetFirmware] getDeviceInfo failed.\n" );
      goto Exit;
    }

    ret = snprintf(
      buf, len,
      "%d.%02d",
      devinfo.firmware_version>>8, devinfo.firmware_version & 0xFF
      );

    ret = 1;
  }

Exit:
  return ret;
}  // RPLidar::GetFirmware


S32
RPLidar::GetHardwareRevision( char* buf, S32 len )
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_device_info_t devinfo;
    u_result res;

    res = _drv->getDeviceInfo( devinfo );

    if ( IS_FAIL( res ) )
    {
      fprintf( stderr, "[RPLidar::GetHardwareRevision] getDeviceInfo failed.\n" );
      goto Exit;
    }

    ret = snprintf(
      buf, len,
      "%d",
      (S32)devinfo.hardware_version
      );

    ret = 1;
  }

Exit:
  return ret;
}


S32
RPLidar::IsExpressScanSupported()
{
  if ( _drv && _drv->isConnected() )
  {
    _drv->checkExpressScanSupported( _expressModeOK );
    return _expressModeOK;
  }
  return 0;
}


S32
RPLidar::CheckHealthStatus()
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_device_health_t healthinfo;
    u_result  res;

    res = _drv->getHealth( healthinfo );
    if ( !IS_OK( res ) )
    {
      goto Exit;
    }

    if ( healthinfo.status == RPLIDAR_STATUS_ERROR )
    {
      goto Exit;
    }

    ret = 1;
  }

Exit:
  return ret;
}


S32
RPLidar::GetReading( DBL* buf, S32 len )
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_measurement_node_t  nodes[_NODE_COUNT_];
    rplidar_response_measurement_node_t* node;
    U64 count = X_NUMBER_OF( nodes );
    u_result res;

    res = _drv->grabScanData( nodes, count );
    if ( !IS_OK( res ) )
    {
      goto Exit;
    }

    _drv->ascendScanData( nodes, count );
    for ( U64 pos = 0; pos < count ; ++pos )
    {
      node = &nodes[pos];

      DBL angle   = ((node->angle_q6_checkbit >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT) / 64.0f);
      DBL dist    = (node->distance_q2 / 4.0f);
      S32 quality = (node->sync_quality >> RPLIDAR_RESP_MEASUREMENT_QUALITY_SHIFT);
      S32 theta   = (S32)angle;

      //if ( quality > 0 )
      //{
      if ( dist == 0 )
      {
        dist = 10000.0;
      }
      _readings[theta] = dist;
      //}
      X_UNUSED_VAR( quality );
    }

    for ( S32 i = 0; i < len; ++i )
    {
      buf[i] = _readings[i];
    }

    ret = 1;
  }

Exit:
  return ret;
}


S32 RPLidar::GetReading( rplidar_reading_t* rdn )
{
  S32 ret = -1;

  if ( _drv && _drv->isConnected() )
  {
    rplidar_response_measurement_node_t  nodes[_NODE_COUNT_];
    rplidar_response_measurement_node_t* node;
    U64 count = _NODE_COUNT_;
    S64 begts = __getsystime();
    S64 endts = 0;
    u_result res;

    res = _drv->grabScanData( nodes, count );
    if ( !IS_OK( res ) )
    {
      goto Exit;
    }

    endts = __getsystime();

    rdn->_seq       = 0;
    rdn->_ascend    = 1;
    rdn->_scanBegTs = begts;
    rdn->_scanEndTs = endts;

    res = _drv->ascendScanData( nodes, count );
    if ( !IS_OK( res ) )
    {
      rdn->_ascend = 0;
    }

    rdn->_count = static_cast<U32>( count );

    for ( U32 idx = 0; idx < count; ++idx )
    {
      node = &nodes[idx];
      rdn->_agl[idx] = node->angle_q6_checkbit;
      rdn->_dst[idx] = node->distance_q2;
      rdn->_qua[idx] = node->sync_quality;
    }
    for ( U32 idx = count; idx < _NODE_COUNT_; ++idx )
    {
      rdn->_agl[idx] = 0;
      rdn->_dst[idx] = 0;
      rdn->_qua[idx] = 0;
    }

    ret = 1;
  }

Exit:
  return ret;
}


S32
RPLidar::Start()
{
  if ( _drv && _drv->isConnected() )
  {
    _drv->startMotor();
    _drv->startScan();
    return 1;
  }
  return -1;
}


S32
RPLidar::Stop()
{
  if ( _drv && _drv->isConnected() )
  {
    _drv->stop();
    _drv->stopMotor();
    return 1;
  }
  return -1;
}












