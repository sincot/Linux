#pragma once

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <string>

class Mutex
{
public:
    Mutex() 
    {
        pthread_mutex_init(&_lock, nullptr);    // 初始化锁
    }

    void LockMutex()    // 加锁
    {
        pthread_mutex_lock(&_lock);
    }

    void UnlockMutex()  // 解锁
    {
        pthread_mutex_unlock(&_lock);
    }

    pthread_mutex_t *GetMutexAddr()      // 返回锁的地址
    {
        return &_lock;
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&_lock);      // 释放锁
    }

private:
    pthread_mutex_t _lock;  // 锁
};

class LockGuard
{
public:
    LockGuard(Mutex &lock) :_lockref(lock)
    {
        // _lockref 已经定义好了
        _lockref.LockMutex();    // 加锁
    }

    ~LockGuard()
    {
        _lockref.UnlockMutex();     // 解锁
    }
private:
    Mutex &_lockref;
};