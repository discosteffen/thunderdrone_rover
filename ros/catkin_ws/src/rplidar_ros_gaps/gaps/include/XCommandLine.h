/*++

  Module Name:

    XCommandLine.h

  Abstract:

    XCommandLine  -  Simple command line parser.

  History:

    05/16/2012    ChiChen   Created.
    11/23/2015    ChiChen   Added flag and value differentiation.

  Internal:

--*/
#ifndef __XCMDLINE_H___
#define __XCMDLINE_H___
#pragma once

#include <XCommon.h>


class XCommandLine
{
public:
  XCommandLine() {};
  ~XCommandLine() {};


  U32 Init( S32  argc, char**  argv, bool  bVerbose = false );

  bool Has( const STDSTR  strKey );

  // excluding argv[0]
  U32  GetSize() const;
  bool Get(     const STDSTR  strKey, STDSTR*  pOut );
  bool GetAsS64( const STDSTR  strKey, S64*     pOut );
  bool GetAsU64( const STDSTR  strKey, U64*     pOut );
  bool GetAsDBL( const STDSTR  strKey, DBL*     pOut );


private:
  typedef struct _Val
  {
    bool    _hasVal;
    STDSTR  _val;

    _Val():
      _hasVal( false )
    {}
  } Val_t;


private:
  typedef STDMAP<STDSTR, Val_t>   CMDMAP;

  CMDMAP  m_CmdMap;

};  // class XCommandLine


#endif  // !__XCMDLINE_H___


