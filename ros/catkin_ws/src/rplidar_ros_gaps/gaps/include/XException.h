/*++

  Module Name:

    XException.h

  Abstract:

    Simple exception class used for XLib.

  History:

    11/01/2012      ChiChen           Created.

  Internal:

--*/
#ifndef __XEXCEPTION_H__
#define __XEXCEPTION_H__
#pragma once


#include <XCommon.h>




class XException
{
private:
  STDSTR   m_strDescription;  // exception description
  STDSTR   m_strFile;         // exception source file
  U32      m_ulLine;          // exception source line number
  S32      m_iErrorNo;        // error code


public:
  XException(
    const char*    pszDescription
    ):
    m_ulLine(0L),
    m_iErrorNo(0)
  {
    if (NULL != pszDescription)   { m_strDescription = pszDescription; }
  }

  XException(
    const char*    pszFile,
    U32            ulLine,
    const char*    pszDescription
    ):
    m_ulLine(ulLine),
    m_iErrorNo(0)
  {
    if (NULL != pszFile)          { m_strFile        = pszFile; }
    if (NULL != pszDescription)   { m_strDescription = pszDescription; }
  }

  XException(
    const char*    pszFile,
    U32        ulLine,
    const char*    pszDescription,
    S32            iErrorNo
    ):
    m_ulLine(ulLine),
    m_iErrorNo(iErrorNo)
  {
    if (NULL != pszFile)          { m_strFile        = pszFile; }
    if (NULL != pszDescription)   { m_strDescription = pszDescription; }
  }

  virtual
  ~XException(
    ) throw()
  {};


  virtual
  const char*
  GetDescription() const throw()  { return m_strDescription.c_str(); }

  const char*
  GetFile() const throw()         { return m_strFile.c_str(); }

  U32
  GetLine() const throw()         { return m_ulLine; }

  S32
  GetErrorNumber() const throw()  { return m_iErrorNo; }


};  // XException


#endif  // __XEXCEPTION_H__

