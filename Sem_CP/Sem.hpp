#pragma once

#include <semaphore.h>
#include <iostream>

class Sem
{
public:
    Sem(int init_value) // 信号量的初始值由外部决定
    {
        // 对传入的参数进行检测
        if (init_value >= 0)
        {
            // 调用 sem_init 函数，初始化信号量 sem
            // int sem_init(sem_t *sem, int pshared, unsigned int value);
            sem_init(&_sem, 0, init_value);
        }
    }

    void P()    // P 操作 —— 申请信号量
    {
        // 调用 sem_wait 函数，申请信号量 sem
        // int sem_wait(sem_t *sem)
        sem_wait(&_sem);
    }

    void V()    // V 操作 —— 释放信号量
    {
        // 调用 sem_post 函数，释放/归还信号量 sem
        // int sem_post(sem_t *sem)
        sem_post(&_sem);
    }

    ~Sem()
    {
        // 调用 sem_destroy 函数，销毁信号量 sem
        // int sem_destroy(sem_t *sem)
        sem_destroy(&_sem);
    }

private:
    sem_t _sem;
};


