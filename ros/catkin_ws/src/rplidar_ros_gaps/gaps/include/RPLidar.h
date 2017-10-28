#ifndef __RPLIDAR_H__
#define __RPLIDAR_H__
#pragma once

#include <XCommon.h>
#include <rplidar.h>  //RPLIDAR standard sdk all-in-one header
#include <RPLidarProxyStuff.h>


using namespace rp::standalone::rplidar;






class RPLidar
{
public:
  RPLidar();
  ~RPLidar();

  S32 Init( const char* device );

  // buf/len should be at least 64 to be safe
  S32 GetSDKVersion( char* buf, S32 len );
  S32 GetSerialNumber( char* buf, S32 len );
  S32 GetFirmware( char* buf, S32 len );
  S32 GetHardwareRevision( char* buf, S32 len );
  S32 IsExpressScanSupported();

  S32 CheckHealthStatus();
  S32 GetFrequency(  );
  // TODO - retire this function
  S32 GetReading( DBL* buf, S32 len );
  S32 GetReading( rplidar_reading_t* rdn );
  S32 Start();
  S32 Stop();


private:
  // TODO - retire this and corresponding GetReading function
  DBL _readings[_NODE_COUNT_];

  RPlidarDriver* _drv;

  bool _expressModeOK;

};





#endif // __RPLIDAR_H__




