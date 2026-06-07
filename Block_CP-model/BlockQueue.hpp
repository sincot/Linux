#pragma once

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <queue>

// const int defalutcapacity = 16;
const int defalutcapacity = 15;

// 使用模板类
template<class T>
class BlockQueue
{
public:
    BlockQueue(int capacity = defalutcapacity) :_capacity(capacity)
    {
        pthread_mutex_init(&_mutex, nullptr);           // 初始化锁
        pthread_cond_init(&_consumer_cond, nullptr);    // 初始化消费者的条件变量
        pthread_cond_init(&_productor_cond, nullptr);   // 初始化生产者的条件变量

        // 方法2：水位线
        // _blockqueue_up_water = _capacity * (2/3);       // 高水位线 —— capacity 的 2/3
        // _blockqueue_low_water = _capacity * (1/3);      // 低水位线 —— capacity 的 1/3

        // 方法3：生产者和消费者各个线程休眠的个数
        int _sleep_productor_num = 0;        // 生产者休眠的个数
        int _sleep_consumer_num = 0;         // 消费者休眠的个数
    }

    // 放数据
    void PushQueue(T& indata)
    {
        pthread_mutex_lock(&_mutex);       // 加锁

        // 判断队列是否为满，为满，生产者在对应的条件变量下等待
        // if(_bq.size() == _capacity)
        while(_bq.size() == _capacity)  // 防止pthread_cond_wait函数调用失败，增强代码的健壮性
        {
            _sleep_productor_num++;      // 生产者休眠个数 ++
            // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
            pthread_cond_wait(&_productor_cond, &_mutex);
            _sleep_productor_num--;      // 生产者休眠个数 --
        }
        // 在队尾添加数据
        _bq.push(indata);

        // 方法1：pthread_cond_signal(&_consumer_cond);   // 唤醒消费者

        // 方法2：水位线
        // if(_bq.size() > _blockqueue_up_water)    // 到达高水位线，唤醒消费者
        //     pthread_cond_signal(&_consumer_cond);   // 唤醒消费者

        // 方法3：生产者和消费者各个线程休眠的个数 —— 是否有消费者在休眠,唤醒消费者
        if(_sleep_consumer_num > 0) { pthread_cond_signal(&_consumer_cond); }

        pthread_mutex_unlock(&_mutex);     // 解锁
    }

    // 取数据
    void PopQueue(T* outdata)
    {
        pthread_mutex_lock(&_mutex);       // 加锁

        // 判断队列是否为空,为空,消费者在对应条件变量下等待
        // if(_bq.empty())
        while(_bq.empty())  // 防止pthread_cond_wait函数调用失败，增强代码的健壮性
        { 
            _sleep_consumer_num++;       // 消费者休眠个数 ++
            // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
            pthread_cond_wait(&_consumer_cond, &_mutex);
            _sleep_consumer_num--;       // 消费者休眠个数 --
        }
        // 获取队列头部的数据
        *outdata = _bq.front();
        _bq.pop();

        // 方法1：pthread_cond_signal(&_productor_cond);   // 唤醒生产者

        // 方法2：水位线
        // if(_bq.size() < _blockqueue_low_water)    // 到达低水位线，唤醒生产者
        //     pthread_cond_signal(&_productor_cond);   // 唤醒生产者

        // 方法3：生产者和消费者各个线程休眠的个数 —— 是否有生产者在休眠,唤醒生产者
        if(_sleep_productor_num > 0) { pthread_cond_signal(&_productor_cond); }

        pthread_mutex_unlock(&_mutex);     // 解锁
    }



    // // 放数据
    // void PushQueue(T& indata)
    // {
    //     pthread_mutex_lock(&_mutex);       // 加锁

    //     // 判断队列是否为满，为满，生产者在对应的条件变量下等待
    //     if(_bq.size() == _capacity)
    //     {
    //         // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
    //         pthread_cond_wait(&_productor_cond, &_mutex);
    //     }
    //     // 在队尾添加数据
    //     _bq.push(indata);

    //     pthread_mutex_unlock(&_mutex);     // 解锁
    // }

    // // 取数据
    // void PopQueue(T* outdata)
    // {
    //     pthread_mutex_lock(&_mutex);       // 加锁

    //     // 判断队列是否为空,为空,消费者在对应条件变量下等待
    //     if(_bq.empty()) 
    //     { 
    //         // int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex)
    //         pthread_cond_wait(&_consumer_cond, &_mutex);
    //     }
    //     // 获取队列头部的数据
    //     *outdata = _bq.front();
    //     _bq.pop();

    //     pthread_mutex_unlock(&_mutex);     // 解锁
    // }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);           // 释放锁
        pthread_cond_destroy(&_consumer_cond);    // 释放消费者的条件变量
        pthread_cond_destroy(&_productor_cond);   // 释放生产者的条件变量
    }

private:
    std::queue<T> _bq;
    int _capacity;

    pthread_mutex_t _mutex;         // 保护临界资源的锁
    pthread_cond_t _consumer_cond;  // 消费者的条件变量
    pthread_cond_t _productor_cond; // 生产者的条件变量

    // 方法2：水位线
    // int _blockqueue_low_water;      // 低水位线
    // int _blockqueue_up_water;       // 高水位线

    // 方法3：生产者和消费者各个线程休眠的个数
    int _sleep_productor_num;        // 生产者休眠的个数
    int _sleep_consumer_num;        // 消费者休眠的个数
};

