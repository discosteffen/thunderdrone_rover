#ifndef __XTYPE_H__
#define __XTYPE_H__
#pragma once

#include <stdint.h>
#include <map>
#include <set>
#include <list>
#include <string>
#include <vector>
using namespace std;


#ifdef __cplusplus
extern "C" {
#endif


#ifndef __X_XRESULT_DEFINED
#define __X_XRESULT_DEFINED
typedef int32_t                XRESULT, *PXRESULT;
#endif // !__X_XRESULT_DEFINED




//---------------------------------------------------------------------------
// BASIC GENERIC TYPES
//---------------------------------------------------------------------------

#ifndef __X_GENERIC_TYPES_DEFINED
#define __X_GENERIC_TYPES_DEFINED

//
// NOTE:
//  No long variant here to avoid precision confusion.
//
typedef int8_t            INT8,    *PINT8;
typedef int16_t           INT16,   *PINT16;
typedef int32_t           INT32,   *PINT32;
typedef int64_t           INT64,   *PINT64;
typedef uint8_t           UINT8,   *PUINT8;
typedef uint16_t          UINT16,  *PUINT16;
typedef uint32_t          UINT32,  *PUINT32;
typedef uint64_t          UINT64,  *PUINT64;
typedef size_t            SIZE_T,  *PSIZE_T;
typedef ssize_t           SSIZE_T, *PSSIZE_T;

typedef int8_t            S8,   *PS8;
typedef int16_t           S16,  *PS16;
typedef int32_t           S32,  *PS32;
typedef int64_t           S64,  *PS64;
typedef uint8_t           U8,   *PU8;
typedef uint16_t          U16,  *PU16;
typedef uint32_t          U32,  *PU32;
typedef uint64_t          U64,  *PU64;

typedef float             FLT, *PFLT;
typedef double            DBL, *PDBL;

typedef void              VOID,  *PVOID;

typedef char              CHAR,  *PCHAR;
typedef unsigned char     UCHAR, *PUCHAR;
typedef UCHAR             BYTE,  *PBYTE;
typedef bool              BOOL,  *PBOOL;

#endif // !__X_GENERIC_TYPES_DEFINED




//---------------------------------------------------------------------------
// STD TYPES
//---------------------------------------------------------------------------

#ifndef STDSET
#define STDSET      std::set
#endif  // !STDSET

#ifndef STDSTR
#define STDSTR      std::string
#endif  // !STDSTR

#ifndef STDVEC
#define STDVEC      std::vector
#endif  // !STDVEC

#ifndef STDMAP
#define STDMAP      std::map
#endif  // !STDMAP

#ifndef STDLST
#define STDLST      std::list
#endif  // !STDLST

#ifndef STDQUE
#define STDQUE      std::queue
#endif  // !STDQUE

#ifndef __X_STD_TYPES_DEFINED
#define __X_STD_TYPES_DEFINED

typedef  STDVEC<STDSTR>   STRVEC;
typedef  STDVEC<S32>      S32VEC;
typedef  STDVEC<U32>      U32VEC;
typedef  STDVEC<S64>      S64VEC;
typedef  STDVEC<U64>      U64VEC;
typedef  STDVEC<DBL>      DBLVEC;
typedef  STDSET<S64>      S64SET;
typedef  STDSET<U64>      U64SET;

#endif // !__X_STD_TYPES_DEFINED


#ifdef __cplusplus
}
#endif


#endif  // __XTYPE_H__
