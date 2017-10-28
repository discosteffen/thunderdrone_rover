/*++

  Module Name:

    XThread.h

  Abstract:

    Declaration of a simple class that wraps a system thread..

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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/file.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <XThread.h>
#include <XException.h>
#include <time.h>  // for sleep




//---------------------------------------------------------------------------
// HELPER FUNCTIONS
//---------------------------------------------------------------------------
void
_ThrowThreadException(
  const char*    pszSrcFile,
  U32        ulSrcLine,
  const char*    pszSysCall,
  S32            iErrorNo
  );

void
_ThrowThreadException(
  const char*    pszSrcFile,
  U32        ulSrcLine,
  const char*    pszSysCall,
  S32            iErrorNo
  )
{
  throw XException(pszSrcFile, ulSrcLine, pszSysCall, iErrorNo);
}


PVOID
_ThreadRunner(
  PVOID    pv  // IN
  )
{
  XThread*  pxt  =  (XThread*)pv;
  pxt->m_vulID = (U32)syscall(SYS_gettid);  // gettid()
  (*(pxt->m_func))(&pxt->m_arg);
  return NULL;
}




//---------------------------------------------------------------------------
// XTHREAD IMPLEMENTATION
//---------------------------------------------------------------------------


XThread::XThread(
  )
{
  m_thread    = 0;
  m_ulID      = 0;
  m_vulID     = 0;
  m_func      = NULL;
  m_arg.pv    = NULL;
  m_arg.bStop = false;
}


XThread::~XThread()
{
  Detach();
}




void
XThread::Run(
  THREADFUNC    func,  // IN
  PVOID         pv     // IN
  )
{
  m_arg.pv    = pv;
  m_arg.bStop = false;

  pthread_attr_t  attr;
  S32             iError;


  m_func = func;

  iError = pthread_attr_init(&attr);
  if (0 != iError)
  {
    m_thread = 0;
    _ThrowThreadException(__FILE__, __LINE__, "pthread_attr_init", iError);
  }

  iError = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  if (0 != iError)
  {
    m_thread = 0;
    _ThrowThreadException(__FILE__, __LINE__, "pthread_attr_setdetachstate", iError);
  }

  iError = pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
  if (0 != iError)
  {
    m_thread = 0;
    _ThrowThreadException(__FILE__, __LINE__, "pthread_attr_setinheritsched", iError);
  }

  iError = pthread_create(&m_thread, &attr, _ThreadRunner, this);
  if (0 != iError)
  {
    m_thread = 0;
    _ThrowThreadException(__FILE__, __LINE__, "pthread_create", iError);
  }

  iError = pthread_attr_destroy(&attr);
  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_attr_destroy", iError);
  }

  while (0 == m_vulID);  // wait until id is assigned in helper function

  m_ulID = m_vulID;      // then assign it to a non volatile variable
}  // XThread::Run




void
XThread::Join(
  )
{
  if (0 != m_thread)
  {
    S32 iReturn = pthread_join(m_thread, 0);

    m_thread = 0;

    if (0 != iReturn)
    {
      _ThrowThreadException(__FILE__, __LINE__, "pthread_join", iReturn);
    }
  }
}  // XThread::Join




U32
XThread::GetID(
  )
{
  return m_ulID;
}


//static
pthread_t
XThread::GetSelf(
  )
{
  return (U32)syscall(SYS_gettid);
}




void
XThread::Detach(
  )
{
  if (0 != m_thread)
  {
    pthread_detach(m_thread);
    m_thread = 0;
  }
}


void
XThread::Stop(
  )
{
  m_arg.bStop = true;
}


void
XThread::Sleep(
  U32    ulMsec  // IN
  )
{
  struct timespec  ts;
  ts.tv_sec  = (ulMsec / 1000);
  ts.tv_nsec = (ulMsec % 1000) * 1000000;
  nanosleep(&ts, NULL);
}




//---------------------------------------------------------------------------
// XCOND IMPLEMENTATION
//---------------------------------------------------------------------------

XCond::XCond(
  )
{
  pthread_condattr_t  attr;
  S32                 iError;


  iError = pthread_condattr_init(&attr);
  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_condattr_init", iError);
  }

  iError = pthread_cond_init(&m_cond, &attr);
  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_init", iError);
  }

  iError = pthread_condattr_destroy(&attr);
  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_condattr_destroy", iError);
  }
}


XCond::~XCond(
  )
{
  S32 iError;


  iError = pthread_cond_destroy(&m_cond);
  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_destroy", iError);
  }
}


S32
XCond::Wait(
  XMutex*  pMutex  // INOUT
  )
{
  return Wait(pMutex, -1);
}


S32
XCond::Wait(
  XMutex*    pMutex,      // INOUT
  U32     lIntervalMs  // IN
  )
{
  S32  iError;


  if (0 < lIntervalMs)
  {
    timeval  tv;
    gettimeofday(&tv, NULL);
    timespec tt = { tv.tv_sec, tv.tv_usec };


#ifndef _MILLISEC
#define _MILLISEC 1000
#endif

#ifndef _MICROSEC
#define _MICROSEC 1000000
#endif

#ifndef _NANOSEC
#define _NANOSEC  1000000000
#endif

    tt.tv_nsec *= _MILLISEC;
    tt.tv_sec  += (lIntervalMs / _MILLISEC);
    tt.tv_nsec += (lIntervalMs % _MILLISEC) * _MICROSEC;

    if(tt.tv_nsec > _NANOSEC)
    {
      tt.tv_nsec -= _NANOSEC;
      tt.tv_sec  += 1;
    }

    S32 iCnt = pMutex->m_iCount;


    pMutex->m_iCount = 0;

    for (S32 i = 1; i < iCnt; ++i)
    {
      pthread_mutex_unlock(&pMutex->m_mutex);
    }

    do
    {
      iError = pthread_cond_timedwait(&m_cond, &pMutex->m_mutex, &tt);
    } while (EINTR == iError);

    for (S32 i = 1; i < iCnt; ++i)
    {
      pthread_mutex_lock(&pMutex->m_mutex);
    }

#ifdef _DEBUG
    assert(pMutex->m_iCount == 0);
#endif

    pMutex->m_iCount = iCnt;

    if (0 != iError)
    {
      if (ETIME == iError || ETIMEDOUT == iError || EINVAL == iError)
      {
        return -2;
      }

      _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_timedwait", iError);
    }

    return 0;

  }
  else
  {
    S32  iCnt  =  pMutex->m_iCount;


    pMutex->m_iCount = 0;

    for (S32 i = 1; i < iCnt; ++i)
    {
      pthread_mutex_unlock(&pMutex->m_mutex);
    }

    do
    {
      iError = pthread_cond_wait(&m_cond, &pMutex->m_mutex);
    } while (EINTR == iError);

    for (S32 i = 1; i < iCnt; ++i)
    {
      pthread_mutex_lock(&pMutex->m_mutex);
    }

#ifdef _DEBUG
    assert(pMutex->m_iCount == 0);
#endif

    pMutex->m_iCount = iCnt;

    if (0 != iError)
    {
      if (ETIME == iError || ETIMEDOUT == iError)
      {
        return -2;
      }

      _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_wait", iError);
    }

    return 0;
  }
}


void
XCond::Signal(
  )
{
  S32  iError  =  pthread_cond_signal(&m_cond);


  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_signal", iError);
  }
}


void
XCond::Broadcast(
  )
{
  S32  iError  =  pthread_cond_broadcast(&m_cond);


  if (0 != iError)
  {
    _ThrowThreadException(__FILE__, __LINE__, "pthread_cond_broadcast", iError);
  }
}



