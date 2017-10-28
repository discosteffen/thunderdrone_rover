/*++

  Module Name:

    XStrSafe.h

  Abstract:

    A set of safe string functions.

  History:

    10/10/2012      ChiChen       Re-purposed.

  Internal:

--*/
#ifndef __XSTRSAFE_H_INCLUDED__
#define __XSTRSAFE_H_INCLUDED__
#pragma once

#include <XType.h>


#ifndef RESULT_SUCCESS
#define RESULT_SUCCESS                    ((XRESULT)0x00000000L)
#endif

#ifndef X_SUCCESS
#define X_SUCCESS(__result)               ((XRESULT)(__result) >= 0)
#endif

#ifndef X_FAILURE
#define X_FAILURE(__result)               ((XRESULT)(__result) <  0)
#endif


//
// XSTRSAFE error return codes
//
#define XSTR_RESULT_BUFFER_INSUFFICIENT   ((XRESULT)0x80070001L)  // 0x1L - same as RESULT_BUFFER_INSUFFICIENT
#define XSTR_RESULT_INVALID_PARAMETER     ((XRESULT)0xC0070005L)  // 0x5L - same as RESULT_INVALID_PARAMETER




#define XSTRSAFEAPI                       __inline
#define XSTRSAFE_INLINE


#define XSTR_MAX_CCH                      2147483647      // max # of characters supported
#define XSTR_INVALID_IDX                  ((UINT32)(-1))






/*++

    XStrCopyA

    Routine Description:
      This routine is a safer version of the C built-in function 'strcpy'.
      The size of the destination buffer (in characters) is a parameter and
      this function will not write past the end of this buffer and it will
      ALWAYS null terminate the destination buffer, unless it is zero
      length.

    Parameters:
      pszDest  -  Destination string.

      uCch    -  Size of destination buffer in characters.
                  Length must be = (strlen(src) + 1) to hold all of the
                  source including the null terminator.

      pszSrc   -  Source string that must be null terminated.

    Return:
      RESULT_SUCCESS  -  If there was source data and it was all copied
                         and the resultant dest string was null terminated.

      XSTR_RESULT_BUFFER_INSUFFICIENT -
                         This return value is an indication that the copy
                         operation failed due to insufficient space. When
                         this error occurs, the destination buffer is
                         modified to contain a truncated version of the
                         ideal result and is null terminated. This is useful
                         for situations where truncation is OK.

      XSTR_RESULT_INVALID_PARAMETER  -
                         The buffer size is greater than XSTR_MAX_CCH or is
                         zero, in which the buffer cannot be null terminated.

--*/
XSTRSAFEAPI
XRESULT
XStrCopyA(
  char*          pszDest,  // OUT
  UINT32         uCch,     // IN
  const char*    pszSrc    // IN
  );


XSTRSAFEAPI
XRESULT
XStrCopyA(
  char*          pszDest,  // OUT
  UINT32         uCch,    // IN
  const char*    pszSrc    // IN
  )
{
  if ((XSTR_MAX_CCH >= uCch) && (0 < uCch))
  {
    XRESULT xr = RESULT_SUCCESS;


    while (uCch && (*pszSrc != '\0'))
    {
      *pszDest++ = *pszSrc++;
      uCch--;
    }

    if (uCch == 0)
    {
      //
      // Need to truncate pszDest.
      //
      pszDest--;
      xr = XSTR_RESULT_BUFFER_INSUFFICIENT;
    }

    *pszDest = '\0';

    return xr;
  }

  return XSTR_RESULT_INVALID_PARAMETER;
}




/*++

    XStrCchLenA

    Routine Description:
      Ensures that a string is not larger than XSTR_MAX_CCH, in
      characters. If that condition is met, XStrCchLenA returns the
      length of the string in characters, not including the terminating
      null character.

    Parameters:
      psz       -  String to check the length of.

      ulCchMax  -  Maximum number of characters including the null terminator
                   that psz is allowed to contain.

      pulCch    -  if the function succeeds and pcch is non-null, the current
                   length in characters of psz excluding the null terminator
                   will be returned. This out parameter is equivalent to the
                   return value of strlen(psz).

    Return:
        true      If successful
        false     If psz is NULL or is longer than ulCchMax or
                  XSTR_MAX_CCH.

--*/
XSTRSAFEAPI
bool
XStrCchLenA(
  const char*    psz,       // IN
  UINT32         ulCchMax,  // IN
  UINT32*        pulCch     // OUT_OPT
  );


XSTRSAFEAPI
bool
XStrCchLenA(
  const char*    psz,       // IN
  UINT32         ulCchMax,  // IN
  UINT32*        pulCch     // OUT_OPT
  )
{
  if (NULL != psz && (XSTR_MAX_CCH >= ulCchMax))
  {
    UINT32 ulRemaining = ulCchMax;
    bool    bValid      = true;


    while (ulRemaining && '\0' != (*psz))
    {
      ++psz;
      --ulRemaining;
    }

    if (0 == ulRemaining)  // longer than ulCchMax
    {
      bValid = false;
    }

    if (NULL != pulCch)
    {
      (*pulCch) = (bValid) ? (ulCchMax - ulRemaining) : 0;
    }

    return bValid;
  }

  return false;
}




XSTRSAFEAPI
bool
XTrimStartA(
  char*      psz,             // INOUT
  UINT32     uCch,            // IN
  char       ch,              // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  );


XSTRSAFEAPI
bool
XTrimStartA(
  char*      psz,             // INOUT
  UINT32     uCch,            // IN
  char       ch,              // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  )
{
  if (NULL != psz && (XSTR_MAX_CCH >= uCch))
  {
    UINT32 ulIdxBeg = 0L;


    while (ulIdxBeg < uCch && ch == psz[ulIdxBeg])
    {
      ++ulIdxBeg;
    }

    if (0 < ulIdxBeg)
    {
      if (ulIdxBeg == uCch)
      {
        psz[0] = '\0';
      }
      else
      {
        UINT32 ulOffset = ulIdxBeg;


        for (UINT32 uIdx = ulIdxBeg; uIdx < uCch; ++uIdx)
        {
          psz[uIdx - ulOffset] = psz[uIdx];
        }
        psz[uCch - ulIdxBeg] = '\0';
      }
    }

    if (NULL != pulCchRemaining)
    {
      (*pulCchRemaining) = (uCch - ulIdxBeg);
    }

    return true;
  }

  return false;
}




XSTRSAFEAPI
bool
XTrimEndA(
  char*       psz,            // INOUT
  UINT32     uCch,            // IN
  char        ch,             // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  );


XSTRSAFEAPI
bool
XTrimEndA(
  char*      psz,             // INOUT
  UINT32     uCch,            // IN
  char       ch,              // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  )
{
  if (NULL != psz && (XSTR_MAX_CCH >= uCch))
  {
    while (0 < uCch && ch == psz[uCch - 1])
    {
      psz[uCch - 1] = '\0';
      --uCch;
    }

    if (NULL != pulCchRemaining) { (*pulCchRemaining) = uCch; }

    return true;
  }

  return false;
}




XSTRSAFEAPI
bool
XTrimA(
  char*      psz,             // INOUT
  UINT32     uCch,            // IN
  char       ch,              // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  );


XSTRSAFEAPI
bool
XTrimA(
  char*      psz,             // INOUT
  UINT32     uCch,            // IN
  char       ch,              // IN
  UINT32*    pulCchRemaining  // OUT_OPT
  )
{
  UINT32 ulCchRemaining = 0L;


  if (XTrimEndA(psz, uCch, ch, &ulCchRemaining))
  {
    return XTrimStartA(psz, ulCchRemaining, ch, pulCchRemaining);
  }

  return false;
}




XSTRSAFEAPI
UINT32
XSplitA(
  const char*   psz,        // IN
  UINT32        ulCchMax,   // IN
  char          chSpliter,  // IN
  char*         pszBuf1,    // OUT
  UINT32        ulCchBuf1,  // IN
  char*         pszBuf2,    // OUT
  UINT32        ulCchBuf2   // IN
  );


XSTRSAFEAPI
UINT32
XSplitA(
  const char*   psz,        // IN
  UINT32        ulCchMax,   // IN
  char          chSpliter,  // IN
  char*         pszBuf1,    // OUT
  UINT32        ulCchBuf1,  // IN
  char*         pszBuf2,    // OUT
  UINT32        ulCchBuf2   // IN
  )
{
  UINT32 uIdxSpliter = XSTR_INVALID_IDX;
  UINT32 uIdx        = 0;


  for (uIdx = 0; uIdx < ulCchMax; ++uIdx)
  {
    if (chSpliter == psz[uIdx])
    {
      uIdxSpliter = uIdx;
      break;
    }
  }

  if (uIdxSpliter == XSTR_INVALID_IDX)
  {
    if (0 < ulCchBuf1) { pszBuf1[0] = '\0'; }
    if (0 < ulCchBuf2) { pszBuf2[0] = '\0'; }
  }
  else
  {
    for (uIdx = 0;
         uIdx < ulCchBuf1 && uIdx < uIdxSpliter;
         ++uIdx)
    {
      pszBuf1[uIdx] = psz[uIdx];
    }
    pszBuf1[uIdx] = '\0';

    ++uIdxSpliter;

    for (uIdx = 0;
         uIdx < ulCchBuf2 && uIdxSpliter < ulCchMax;
         ++uIdx)
    {
      pszBuf2[uIdx] = psz[uIdxSpliter++];
    }
    pszBuf2[uIdx] = '\0';
  }


  return uIdxSpliter;
}




/*++

    XStrFindA

    Routine Description:
      Searches for a character in a given string, beginning at a specified
      character index.  If not found, (*puIdx) will be assigned to
      XSTR_INVALID_IDX.

    Parameters:
      psz          -  String to search.

      uCch         -  Length of the string in characters.

      chTarget     -  Character to search for.

      uIdxBegSrch  -  Index of character to begin search.

      puIdx        -  Pointer to a variable of type DWORD containing
                       the index.

    Return:
        true      If successful.
        false     If one or more input arguments is invalid.

--*/
XSTRSAFEAPI
bool
XStrFindA(
  char*       psz,          // IN
  UINT32      uCch,         // IN
  char        chTarget,     // IN
  UINT32      uIdxBegSrch,  // IN
  PUINT32     puIdx         // OUT
  );


XSTRSAFEAPI
bool
XStrFindA(
  char*      psz,          // IN
  UINT32     uCch,         // IN
  char       chTarget,     // IN
  UINT32     uIdxBegSrch,  // IN
  PUINT32    puIdx         // OUT
  )
{
  if (
      NULL != psz                    &&
      NULL != puIdx                  &&
      (XSTR_MAX_CCH >= uCch)         &&
      (XSTR_MAX_CCH >= uIdxBegSrch)
     )
  {
    (*puIdx) = XSTR_INVALID_IDX;

    for (UINT32 uIdx = uIdxBegSrch; uIdx < uCch; ++uIdx)
    {
      if (chTarget == psz[uIdx])
      {
        (*puIdx) = uIdx;
        break;
      }
    }

    return true;
  }

  return false;
}


#endif  // __XSTRSAFE_H_INCLUDED__


