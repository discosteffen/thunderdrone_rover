/*++

  Module Name:

      XResult.h

  Abstract:

      Constant definitions for the XRESULT values.

  Author:

      ChiChen

  Revision History:

  Notes:

      Please add new error values to the end of the file. To do otherwise
      will jumble the error values.

--*/
#ifndef __XRESULT_H__
#define __XRESULT_H__
#pragma once




//
// Generic test for success on any result value (non-negative numbers
// indicate success).
//
#define X_SUCCESS(__result)                   ((XRESULT)(__result) >= 0)

#define X_FAILURE(__result)                   ((XRESULT)(__result) <  0)

//
// Generic test for information on any result value.
//
#define X_INFORMATION(__result)               ((ULONG)(__result) >> 30 == 1)

//
// Generic test for warning on any result value.
//
#define X_WARNING(__result)                   ((ULONG)(__result) >> 30 == 2)

//
// Generic test for error on any result value.
//
#define X_ERROR(__result)                     ((ULONG)(__result) >> 30 == 3)




//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|R|R|     Component         |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success (0x0~)
//          01 - Informational (0x4~)
//          10 - Warning (0x8~)
//          11 - Error (0xC~)
//
//      R - is a reserved bit
//
//      Component - is the component code
//
//      Code - is the component's result code
//


/////////////////////////////////////////////////////////////////////////////
//
// Success values
//
/////////////////////////////////////////////////////////////////////////////
#define RESULT_SUCCESS                        ((XRESULT)0x00000000L)




/////////////////////////////////////////////////////////////////////////////
//
// Information values
//
/////////////////////////////////////////////////////////////////////////////

//
// MessageId: RESULT_WAS_LOCKED
//
// MessageText:
//
//  The resource to lock was already locked.
//
#define RESULT_WAS_LOCKED                     ((XRESULT)0x40000001L)

//
// MessageId: RESULT_WAS_UNLOCKED
//
// MessageText:
//
//  The protection of a locked resource was unlocked.
//
#define RESULT_WAS_UNLOCKED                   ((XRESULT)0x40000002L)




/////////////////////////////////////////////////////////////////////////////
//
//  Warning values
//
/////////////////////////////////////////////////////////////////////////////

//
// MessageId: RESULT_BUFFER_INSUFFICIENT
//
// MessageText:
//
//  The data was too large to fit into the specified buffer entirely.
//
#define RESULT_BUFFER_INSUFFICIENT            ((XRESULT)0x80000001L)

//
// MessageId: RESULT_BUSY
//
// MessageText:
//
//  The resource necessary to complete this operation is being used.
//
#define RESULT_BUSY                           ((XRESULT)0x80000002L)

//
// MessageId: RESULT_NO_MORE_ENTRIES
//
// MessageText:
//
//  No more entries are available from an enumeration operation.
//
#define RESULT_NO_MORE_ENTRIES                ((XRESULT)0x80000003L)

//
// MessageId: RESULT_DUPLICATE_ENTRY
//
// MessageText:
//
//  A duplicate entry found during an insertion operation.
//
#define RESULT_DUPLICATE_ENTRY                ((XRESULT)0x80000004L)




/////////////////////////////////////////////////////////////////////////////
//
//  Error values
//
/////////////////////////////////////////////////////////////////////////////

//
// MessageId: RESULT_FAILED
//
// MessageText:
//
//  The requested operation has failed.
//
#define RESULT_FAILED                         ((XRESULT)0xC0000001L)

//
// MessageId: RESULT_NOT_IMPLEMENTED
//
// MessageText:
//
//  The requested operation is not implemented.
//
#define RESULT_NOT_IMPLEMENTED                ((XRESULT)0xC0000002L)

//
// MessageId: RESULT_PATH_NOT_FOUND
//
// MessageText:
//
//  Cannot find the path specified.
//
#define RESULT_PATH_NOT_FOUND                 ((XRESULT)0xC0000003L)

//
// MessageId: RESULT_NO_MEMORY
//
// MessageText:
//
//  Ran out of memory.
//
#define RESULT_NO_MEMORY                      ((XRESULT)0xC0000004L)

//
// MessageId: RESULT_INVALID_PARAMETER
//
// MessageText:
//
//  One or more parameters are invalid.
//
#define RESULT_INVALID_PARAMETER              ((XRESULT)0xC0000005L)

//
// MessageId: RESULT_ABORTED
//
// MessageText:
//
//  Operation aborted.
//
#define RESULT_ABORTED                        ((XRESULT)0xC0000006L)

//
// MessageId: RESULT_ACCESS_DENIED
//
// MessageText:
//
//  A requested access right has not been granted.
//
#define RESULT_ACCESS_DENIED                  ((XRESULT)0xC0000007L)

//
// MessageId: RESULT_PENDING
//
// MessageText:
//
//  The data necessary to complete this operation is not yet available.
//
#define RESULT_PENDING                        ((XRESULT)0xC0000008L)

//
// MessageId: RESULT_BUFFER_TOO_SMALL
//
// MessageText:
//
//  The buffer is too small to contain the data.
//
#define RESULT_BUFFER_TOO_SMALL               ((XRESULT)0xC0000009L)

//
// MessageId: RESULT_NOT_FOUND
//
// MessageText:
//
//  Element not found.
//
#define RESULT_NOT_FOUND                      ((XRESULT)0xC000000AL)




#endif  // __XRESULT_H__



