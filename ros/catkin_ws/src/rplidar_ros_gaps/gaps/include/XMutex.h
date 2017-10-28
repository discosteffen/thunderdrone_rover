/*++

  Module Name:

    XMutex.h

  Abstract:

    Declaration of XMutex class.

  History:

    11/01/2012      ChiChen           Created.

  Internal:

--*/
#ifndef __XMUTEX_H__
#define __XMUTEX_H__
#pragma once

#include <XCommon.h>




class XMutex
{
friend class XCond;


public:
  pthread_mutex_t  m_mutex;
  S32              m_iSpinCount;

  //
  // Current recursion level of this mutex. This is the number of
  // calls to "lock" without corresonding calls to "unlock".
  //
  S32 m_iCount;


  XMutex();
  ~XMutex();


  //
  // Lock the mutex. This thread will block if another thread has
  // already locked the mutex, so that only one thread owns the mutex
  // at a time.
  //
  // A thread can repeatedly lock the same mutex. The mutex will
  // remained locked until the number of calls to unlock equals the
  // number of calls to lock.
  //
  // Calling lock/unlock directly is risky and error prone. Using the
  // XScopedMutex class instead is strongly recommend.
  //
  void lock();

  //
  // Try to lock the mutex.  Return true if successful.
  // Return false only if another thread already holds the lock.
  //
  bool trylock();

  //
  // Unlock the the mutex. If another thread is waiting for the mutex,
  // then it will be able to proceed.
  //
  void unlock();
};




//
// This class provides exception-safe locking and unlocking. It is
// generally a more robust and convenient way of using mutexes
// than calling lock/unlock directly.
//
class XScopedMutex
{
private:
  XMutex*  m_pMutex;


public:
  //
  // This constructor locks the given mutex. The unlock happens
  // automatically when the XScopedMutex falls out of scope, or an
  // exception is thrown.
  //
  inline
  XScopedMutex(
    XMutex*    pm,
    bool       locked = false
    )
  {
    m_pMutex = pm;

    if (false == locked)
    {
      m_pMutex->lock();
    }
  }

  //
  // This destructor unlocks the mutex given in the constructor.
  //
  inline
  ~XScopedMutex()
  {
    m_pMutex->unlock();
  }
};




//
// This class provides exception-safe try locking and unlocking. It is
// generally a more robust and convenient way of using mutexes
// than calling lock/unlock directly.
//
class XScopedTryMutex
{
private:
  XMutex*  m_pMutex;
  bool     m_bIsLocked;


public:
  //
  // This constructor tries to lock the given mutex. The unlock happens
  // automatically when the XScopedTryMutex falls out of scope, or an
  // exception is thrown.
  //
  inline
  XScopedTryMutex(
    XMutex*    pm,
    bool       bLock = true
    )
  {
    m_pMutex    = pm;
    m_bIsLocked = false;

    if (bLock && m_pMutex->trylock())
    {
      m_bIsLocked = true;
    }
  }

  //
  // This destructor unlocks the mutex given in the constructor.
  //
  inline
  ~XScopedTryMutex()
  {
    if (m_bIsLocked)
    {
      m_pMutex->unlock();
    }
  }

  inline
  bool TryLock()
  {
    if (!m_bIsLocked)
    {
      if (m_pMutex->trylock())
      {
        m_bIsLocked = true;
      }
    }

    return m_bIsLocked;
  }

  inline
  bool IsLocked() const
  {
    return m_bIsLocked;
  }
};


#endif  // __XMUTEX_H__


