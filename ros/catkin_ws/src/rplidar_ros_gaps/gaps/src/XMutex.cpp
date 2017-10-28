/*++

  Module Name:

    XMutex.cpp

  Abstract:

    Definition of XMutex class.

  History:

    11/01/2012      ChiChen           Created.

  Internal:

    XMutex provides capability to lock and unlock the mutex. A thread
    will block if another thread has already locked the mutex, so that
    only one thread owns the mutex at a time.

    A thread can repeatedly lock the same mutex. The mutex will
    remained locked until the number of calls to unlock equals the
    number of calls to lock.

    Calling lock/unlock directly is risky and error prone. Using the
    XScopedMutex class instead is recommend.

--*/
#include <XMutex.h>
#include <XException.h>
#include <errno.h>


void
_ThrowException(
  const char*  pszSrcFile,  // IN
  const U32    ulSrcLine,   // IN
  const char*  pszSysCall,  // IN
  S32          iSysErr      // IN
  );


void
_ThrowException(
  const char*  pszSrcFile,  // IN
  const U32    ulSrcLine,   // IN
  const char*  pszSysCall,  // IN
  S32          iSysErr      // IN
  )
{
  throw XException(pszSrcFile, ulSrcLine, pszSysCall, iSysErr);
}




XMutex::XMutex()
{
  //
  // Linux implementation.
  //
  pthread_mutexattr_t attr;
  S32                 iError;


  iError = pthread_mutexattr_init(&attr);
  if (0 != iError)
  {
    _ThrowException(__FILE__, __LINE__, "pthread_mutexattr_init", iError);
  }

  iError = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE_NP);
  if (0 != iError)
  {
    _ThrowException(__FILE__, __LINE__, "pthread_mutexattr_settype", iError);
  }

  iError = pthread_mutex_init(&m_mutex, &attr);
  if (0 != iError)
  {
    _ThrowException(__FILE__, __LINE__, "pthread_mutex_init", iError);
  }

  iError = pthread_mutexattr_destroy(&attr);
  if (0 != iError)
  {
    _ThrowException(__FILE__, __LINE__, "pthread_mutexattr_destroy", iError);
  }

  //
  // Mutex initialization succeeded.
  //
  m_iSpinCount = 1;

  m_iCount = 0;
}


XMutex::~XMutex()
{
  pthread_mutex_destroy(&m_mutex);
}


void XMutex::lock()
{
  S32  iError;
  S32  iSpinCount = m_iSpinCount;


  do
  {
    iError = pthread_mutex_trylock(&m_mutex);
  }
  while (EBUSY == iError && --iSpinCount > 0);

  if ((0 == iSpinCount) && (EBUSY == iError))
  {
    iError = pthread_mutex_lock(&m_mutex);
    if (0 != iError)
    {
      _ThrowException(__FILE__, __LINE__, "pthread_mutex_lock", iError);
    }
  }

  m_iCount++;
}


bool XMutex::trylock()
{
  S32  iError;
  S32  iSpinCount = m_iSpinCount;


  do
  {
    iError = pthread_mutex_trylock(&m_mutex);
  }
  while (EBUSY == iError && --iSpinCount > 0);

  if (0 == iError)
  {
    m_iCount++;
    return true;
  }

  return false;
}


void XMutex::unlock()
{
  m_iCount--;

  S32 iError = pthread_mutex_unlock(&m_mutex);


  if (0 != iError)
  {
    printf("[XMutex::unlock] error=%d.\n", iError);

    _ThrowException(__FILE__, __LINE__, "pthread_mutex_unlock", iError);
  }
}

