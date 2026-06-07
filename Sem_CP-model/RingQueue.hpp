#pragma once


#include "Sem.hpp"

#include <iostream>
#include <string>
#include <pthread.h>
#include <vector>

const int defaultcapacity = 15;

template<class T>
class RingQueue
{
public:
    RingQueue(int capacity = defaultcapacity)
        : _capacity(capacity) , _rq(capacity)
        , _consumer_position(0) , _productor_position(0)
        , _data_sem(0) , _blank_sem(capacity)
    {}

    void PushQueue(T &indata)   // 放数据
    {
        // 1. 申请资源，才能访问环形队列 —— P 操作
        _blank_sem.P();

        // 2. 申请成功后，找位置进行生产, productor_position
        _rq[_productor_position++] = indata;
        _productor_position %= _capacity;   // 体现环形这一特点

        // 3. 释放资源，释放 _data_sem —— V 操作
        _data_sem.V();
    }

    void PopQueue(T *outdata)   // 拿数据
    {
        // 1. 申请资源，才能访问环形队列 —— P 操作
        _data_sem.P();

        // 2. 申请成功后，找位置取数据，_consumer_position
        *outdata = _rq[_consumer_position++];
        _consumer_position %= _capacity;    // 体现环形这一特点

        // 3. 释放资源，释放 _blank_sem —— V 操作
        _blank_sem.V();
    }

    ~RingQueue()
    {}

private:
    int _capacity;          // 环形队列的容量
    std::vector<T> _rq;     // 使用数组模拟环形队列

    int _consumer_position;     // 消费位置
    int _productor_position;    // 生产位置

    Sem _data_sem;          // 数据资源，消费者关心
    Sem _blank_sem;         // 格子资源，生产者关心
};

