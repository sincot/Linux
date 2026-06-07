#include <pthread.h>
#include "Mutex.hpp"    // 使用之前封装实现的 Mutex 锁


// 封装实现条件变量 —— cond
class Cond
{
public:
    Cond()
    {
        // 调用 pthread_cond_init
        // int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr)
        pthread_cond_init(&_cond, nullptr);
    }

    void WaitCond(Mutex &mutex)     // 在当前条件变量下等待
    {
        // 调用 pthread_cond_wait 函数
        // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
        int n = pthread_cond_wait(&_cond, mutex.GetMutexAddr());
    }

    void SignalCond()   // 唤醒在当前条件变量下等待的线程
    {
        // 调用 pthread_cond_signal 函数
        // int pthread_cond_signal(pthread_cond_t *cond)
        int n = pthread_cond_signal(&_cond);
    }

    void BroadcastCond()    // 唤醒在当前条件变量下等待的所有线程
    {
        // 调用 pthread_cond_broadcast 函数
        // int pthread_cond_broadcast(pthread_cond_t *cond)
        int n = pthread_cond_broadcast(&_cond);
    }

    ~Cond()
    {
        // 调用 pthread_cond_destroy
        // int pthread_cond_destroy(pthread_cond_t *cond)
        pthread_cond_destroy(&_cond);
    }

private:
    pthread_cond_t _cond;
};