#ifndef   _OBJECT_AINGLETON_H_
#define   _OBJECT_AINGLETON_H_
#include "common/jmutex.h"


/*********************
example :

class AAAA : public CSingleton<AAAA>
{
    friend class CSingleton<AAAA>;
private:
   AAAA();
   ~AAAA();    
}

// get Instance of class AAAA

AAAA* pAAAA = AAAA::getSingleton();

// release Instance of class AAAA
AAAA::deleteSingleton();

**********************/

template <class T>
class CSingleton
{
private:

   static T *     m_pSingleton;        //!< Singleton -> only 1 instance allowed
   static JMutex  m_oMutexSingleton;   //!< Mutex -> 'CSingleton' is thread-safe

protected:

   CSingleton() { ; }
   virtual ~CSingleton() { ; }

public:

   static T * getSingleton();
   static void deleteSingleton();

protected:

   static bool isSingletonNull() { return (m_pSingleton == NULL); }

   virtual void deleteInstance() { CSingleton<T>::deleteSingleton(); }

};


template <class T> T * CSingleton<T>::m_pSingleton = NULL;
template <class T> JMutex CSingleton<T>::m_oMutexSingleton;

template <class T>
T * CSingleton<T>::getSingleton()
{
   if (m_pSingleton)
      return m_pSingleton;

   CISAPUGuard<JMutex> locker(m_oMutexSingleton);
   if (! m_pSingleton) { m_pSingleton = new T(); }

   return m_pSingleton;
}

template <class T>
void CSingleton<T>::deleteSingleton()
{
   CISAPUGuard<JMutex> locker(m_oMutexSingleton);
   if (! m_pSingleton) { return; }
   delete m_pSingleton;
   m_pSingleton = NULL;
}

#endif

