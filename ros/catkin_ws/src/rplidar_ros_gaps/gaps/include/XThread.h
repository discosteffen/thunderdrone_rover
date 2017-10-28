/*++

  Module Name:

    XThread.h

  Abstract:

    Declaration of a simple class that wraps a system thread.

    USAGE:

      //
      // initialization.
      //
      XThread  xth;

      //
      // create a separate thread to run a function.
      //
      //      +-------------------- function to run
      //      |
      //      |                 +-- function argument
      //      |                 |
      //      V                 V
      xth.Run(MyThreadFunction, NULL);

      //
      // do other stuff in the current thread.
      //

      //
      // set the sentinel value and wait for the thread to exit.
      //
      // Note MyThreadFunction needs to check for the sentinel value
      // in order for the XThread::Stop invocation to work properly.
      //
      xth.Stop();
      xth.Join();

  History:

    11/01/2012      ChiChen           Created.

  Internal:

--*/
#ifndef __XTHREAD_H__
#define __XTHREAD_H__
#pragma once

#include <pthread.h>
#include <XCommon.h>
#include <XMutex.h>


#ifdef __cplusplus
extern "C" {
#endif


typedef struct _XTHREADARG
{
  PVOID         pv;
  volatile S32  bStop;
} XTHREADARG, *PXTHREADARG;


typedef PVOID (*XTHREADFUNC)(PVOID);  // thread function type.


#ifdef __cplusplus
}
#endif




/**
 *
 * A "condition" object.  Typically, a thread blocks on a condition
 * until it is true or signalled.
 *
 * Typical usage pattern:
 *
 * Thread 1:
 *   mutex.lock();
 *   condition.wait(&mutex); // wait for signal from thread 2
 *   mutex.unlock();
 *
 * Thread 2:
 *   mutex.lock();
 *   condition.signal();     // wake up thread 1
 *   mutex.unlock();
 *
 * Note that both threads must hold EXACTLY THE SAME mutex while
 * waiting AND signalling.
 *
 */
class XCond
{
public:
  XCond();
  ~XCond();


  //
  // Block this thread until the condition to be signalled.  This method
  // must be protected by a locked mutex, which is passed in as an argument.
  // The mutex will be unlocked while the thread is blocked (and locked
  // again when the thread unblocks).
  //
  S32
  Wait(
    XMutex*  pMutex  // IN
    );

  //
  // Block this thread until the condition to be signalled. This method
  // must be protected by a locked mutex, which is passed in as an argument.
  // The mutex will be unlocked while the thread is blocked (and locked
  // again when the thread unblocks).
  //
  // The "interval" argument specifies a timeout in milliseconds. This
  // thread will automatically unblock if this amount of time passes,
  // even if the condition is not signalled.
  //
  S32
  Wait(
    XMutex*  pMutex,        // INOUT
    U32      lIntervalMsec  // IN
    );

  //
  // Signal this condition. If a thread is blocked on this condition,
  // then it will be unblocked. At most one thread will be
  // unblocked. If no threads are blocked, then nothing will happen.
  //
  // You MUST call this method while holding the same mutex as the one
  // used in the wait method. In practice, every condition is
  // assocatiated with a specific mutex, and all operations on the
  // condition take place while holding that mutex. Sharing one mutex
  // across multiple conditions is allowed, but not vice versa.
  //
  void
  Signal(
    );

  //
  // Unblock all threads that are blocked on this condition. If no
  // threads are blocked, then nothing will happen.
  //
  // You MUST call this method while holding the same mutex as the one
  // used in the wait method. In practice, every condition is
  // assocatiated with a specific mutex, and all operations on the
  // condition take place while holding that mutex. Sharing one mutex
  // across multiple conditions is allowed, but not vice versa.
  //
  void
  Broadcast(
    );


private:
  //
  // Native implementation of conditions.
  //
  pthread_cond_t m_cond;

};  // XCond




class XThread
{
public:
  typedef XTHREADFUNC   THREADFUNC;  // thread function type;
  typedef XTHREADARG    THREADARG;   // thread argument type.


private:
  // function used to start a thread.
  friend PVOID _ThreadRunner(PVOID pv);

  pthread_t     m_thread;   // native thread handle.

  U32           m_ulID;     // system identifier of thread.
  volatile U32  m_vulID;    // system identifier of thread.
                            // this is used during thread creation.

  THREADFUNC  m_func;       // thread start-up function.
  THREADARG   m_arg;        // thread function argument.


public:
  XThread();

  virtual ~XThread();


  void
  Run(THREADFUNC func, PVOID pv);

  void
  Join();

  void
  Detach();

  void
  Stop();

  //
  // Returns the system identifier for this thread.
  //
  U32
  GetID();

  //
  // Returns the system identifier for currently running thread.
  //
  static
  pthread_t
  GetSelf();

  //
  // Blocks the current running thread for the given number of milliseconds.
  //
  static
  void
  Sleep(U32 ulMsec);


};  // XThread


#endif  // __XTHREAD_H__

