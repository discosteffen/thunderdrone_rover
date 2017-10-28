/*++

  Module Name:

    XCommandLine.cpp

  Abstract:

    XCommandLine  -  Simple command line parser.

  History:

    05/16/2012    ChiChen   Created.
    11/23/2015    ChiChen   Added flag and value differentiation.

  Internal:

--*/
#include <XCommandLine.h>
#include <limits.h>
#include <errno.h>


U32
XCommandLine::Init(
  S32  argc,      // IN
  char**  argv,   // IN
  bool  bVerbose  // IN
  )
{
  U32  uRet  =  0;

  for (S32 i = 1; i < argc; ++i)
  {
    if ('-' == argv[i][0])
    {
      char*  psz  =  &argv[i][1];

      if ('-' == (*psz))
      {
        ++psz;
      }

      // parameter?
      if ((i + 1) < argc && '-' != argv[i + 1][0])
      {
        m_CmdMap[psz]._hasVal = true;
        m_CmdMap[psz]._val    = argv[i + 1];
      }
      else
      {
        m_CmdMap[psz]._hasVal = false;
      }

      ++uRet;
    }
  }

  if (bVerbose)
  {
    printf("[CMDLINE MAP BEG]\n");
    for (CMDMAP::iterator i = m_CmdMap.begin();
         m_CmdMap.end() != i;
         ++i)
    {
      printf("[%s -> %d|%s]\n",
        (*i).first.c_str(), (*i).second._hasVal, (*i).second._val.c_str());
    }
    printf("[CMDLINE MAP END]\n");
    printf("\n");
  }

  return uRet;
}  // XCommandLine::Init


U32
XCommandLine::GetSize(
  ) const
{
  return (U32)m_CmdMap.size();
}


bool
XCommandLine::Has(
  const STDSTR  strKey  // IN
  )
{
  return (m_CmdMap.end() != m_CmdMap.find(strKey));
}


bool
XCommandLine::Get(
  STDSTR   strKey,  // IN
  STDSTR*  pOut     // OUT
  )
{
  if (NULL != pOut)
  {
    CMDMAP::iterator  i  =  m_CmdMap.find(strKey);
    if (m_CmdMap.end() != i)
    {
      if ((*i).second._hasVal)
      {
        (*pOut) = (*i).second._val;
        return true;
      }
    }
  }
  return false;
}


bool
XCommandLine::GetAsS64(
  STDSTR  strKey,  // IN
  S64*    pOut     // OUT
  )
{
  if (NULL != pOut)
  {
    STDSTR  str;

    if (Get(strKey, &str))
    {
      char*  pszEnd;
      S64    iiRet;
      bool   bRet;

      errno = 0;
      iiRet = (S64)strtoll(str.c_str(), &pszEnd, 10);
      if ( ( errno == ERANGE &&
             (iiRet == LLONG_MAX || iiRet == LLONG_MIN) ) ||
           ( errno != 0 && iiRet == 0 ) )
      {
        bRet = false;
      }
      else if ( str.c_str() == pszEnd || '\0' != *pszEnd )
      {
        bRet = false;
      }
      else
      {
        (*pOut) = iiRet;
        bRet = true;
      }
      return bRet;
    }
  }
  return false;
}


bool
XCommandLine::GetAsU64(
  STDSTR  strKey,  // IN
  U64*    pOut     // OUT
  )
{
  if (NULL != pOut)
  {
    STDSTR  str;

    if (Get(strKey, &str))
    {
      char*  pszEnd;
      U64    uiiRet;
      bool   bRet;

      uiiRet = (U64)strtoll(str.c_str(), &pszEnd, 10);
      if ( ( errno == ERANGE && (uiiRet == ULLONG_MAX) ) ||
           ( errno != 0 && uiiRet == 0 ) )
      {
        bRet = false;
      }
      else if ( str.c_str() == pszEnd || '\0' != *pszEnd )
      {
        bRet = false;
      }
      else
      {
        (*pOut) = uiiRet;
        bRet = true;
      }
      return bRet;
    }
  }
  return false;
}


bool
XCommandLine::GetAsDBL(
  STDSTR  strKey,  // IN
  DBL*  pOut  // OUT
  )
{
  if (NULL != pOut)
  {
    STDSTR  str;

    if (Get(strKey, &str))
    {
      char*  pszEnd;
      DBL    dRet;
      bool   bRet;

      dRet = strtod(str.c_str(), &pszEnd);
      if ( ( errno == ERANGE &&
             (dRet == +HUGE_VAL || dRet == -HUGE_VAL) ) ||
           ( errno != 0 && dRet == 0.0 ) )
      {
        bRet = false;
      }
      else if ( str.c_str() == pszEnd || '\0' != *pszEnd )
      {
        bRet = false;
      }
      else
      {
        (*pOut) = dRet;
        bRet = true;
      }
      return bRet;
    }
  }
  return false;
}




