#ifndef IN_OUT_H
#define IN_OUT_H
#include <pthread.h>
#include <iostream>
struct ThreadMutex
{
        ThreadMutex();
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

int _kbhit();  
void PString(const std::string& str);
void PInt(const int var);
#endif