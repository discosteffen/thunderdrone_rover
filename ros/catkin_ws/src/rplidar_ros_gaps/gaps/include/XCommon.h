/*++

  Module Name:

    XCommon.h

  Abstract:

    Common header files.

  History:

    10/10/2012      ChiChen       Created

  Internal:

--*/
#ifndef __XCOMMON_H__
#define __XCOMMON_H__
#pragma once


#ifndef X_NUMBER_OF
#define X_NUMBER_OF(_A_)          (sizeof(_A_)/sizeof((_A_)[0]))
#endif

#ifndef X_IGNORE_RESULT
#define X_IGNORE_RESULT(_CALL_)   if (_CALL_){} 
#endif

#ifndef X_UNUSED_VAR
#define X_UNUSED_VAR(_var_)       (void)(_var_)
#endif


// for millisecond to nanosecond conversion
// NOTE - use this in only non performance critical code paths
#define X_M2NSEC( MSEC )          ( ( MSEC )*1000000LL )


// added this back so xordgwy can compile
#if defined( _DEBUG ) || defined( _DBG )
#define DEBUG_CODE( __CODE )      { __CODE; }
#else
#define DEBUG_CODE( __CODE )
#endif


// silly debug code macro that can be individually turned on and off
#define X_DEBUG_CODE( __SWITCH, __CODE )  \
if ( __SWITCH ) { __CODE; }




//
// System wide comment character.
//
#ifndef X_COMMENT_CHAR
#define X_COMMENT_CHAR  '#'
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <algorithm>
using namespace std;


#include <XType.h>
#include <XError.h>


#endif  // __XCOMMON_H__


