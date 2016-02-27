#ifndef CRITICAL_FIELD_H
#define CRITICAL_FIELD_H
#include <pthread.h>
struct ThreadMutex
{
        ThreadMutex()
        {
                pthread_mutex_init(&mtx,NULL);
        }
        ~ThreadMutex()
        {
                pthread_mutex_destroy( &mtx );
        }
        inline void lock()
        {
                pthread_mutex_lock( &mtx );
        }
        inline void unlock()
        {
                pthread_mutex_unlock( &mtx );
        }
        pthread_mutex_t mtx;
};     

#define ENTER_CRITICAL  (new                                       
#endif