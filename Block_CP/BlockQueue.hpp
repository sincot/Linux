#pragma once

#include <iostream>
#include <string>
#include <unistd.h>
#include <queue>
#include "Cond.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"
#include <memory>
#include "Task.hpp"


// const int defalutcapacity = 16;
const int defalutcapacity = 15;

// 使用模板类
template <class T>
class BlockQueue
{
public:
    BlockQueue(int capacity = defalutcapacity) : _capacity(capacity)
    {
        // 方法3：生产者和消费者各个线程休眠的个数
        int _sleep_productor_num = 0; // 生产者休眠的个数
        int _sleep_consumer_num = 0;  // 消费者休眠的个数
    }

    // 放数据
    void PushQueue(T &indata)
    {
        {
            LockGuard lockguard(_mutex); // 自动加锁与解锁

            // 判断队列是否为满，为满，生产者在对应的条件变量下等待
            while (_bq.size() == _capacity) // 防止pthread_cond_wait函数调用失败，增强代码的健壮性
            {
                _sleep_productor_num++; // 生产者休眠个数 ++
                _productor_cond.WaitCond(_mutex);
                _sleep_productor_num--; // 生产者休眠个数 --
            }
            // 在队尾添加数据
            _bq.push(indata);

            // 方法3：生产者和消费者各个线程休眠的个数 —— 是否有消费者在休眠,唤醒消费者
            if (_sleep_consumer_num > 0)
            {
                _consumer_cond.SignalCond();
            }
        }
    }

    // 取数据
    void PopQueue(T *outdata)
    {
        {
            LockGuard lockguard(_mutex); // 加锁

            while (_bq.empty()) // 防止pthread_cond_wait函数调用失败，增强代码的健壮性
            {
                _sleep_consumer_num++; // 消费者休眠个数 ++
                _consumer_cond.WaitCond(_mutex);
                _sleep_consumer_num--; // 消费者休眠个数 --
            }
            // 获取队列头部的数据
            *outdata = _bq.front();
            _bq.pop();

            // 方法3：生产者和消费者各个线程休眠的个数 —— 是否有生产者在休眠,唤醒生产者
            if (_sleep_productor_num > 0) { _productor_cond.SignalCond(); }
        }
    }

    ~BlockQueue()
    {}

private:
    std::queue<T> _bq;
    int _capacity;

    Mutex _mutex;
    Cond _consumer_cond;
    Cond _productor_cond;

    // 方法3：生产者和消费者各个线程休眠的个数
    int _sleep_productor_num; // 生产者休眠的个数
    int _sleep_consumer_num;  // 消费者休眠的个数
};
