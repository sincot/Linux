/* 异步工作器 
    管理的成员：
    1. 双缓冲区（生产-消费模型）
    2. 互斥锁（保证线程安全）
    3. 条件变量：生产者 & 消费者（生产缓冲区没有数据，处理完消费缓冲区数据后就休眠）
    4. 回调函数（针对缓冲区中数据的处理接口，外界传入一个函数，告诉异步工作器如何处理该数据）
*/
#ifndef MY_LOGLOOPER_H
#define MY_LOGLOOPER_H

#include "LogBuffer.hpp"


#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>

namespace Log
{
    using Functor = std::function<void(LogBuffer &)>;

    enum class AsyncType
    {
        ASYNC_SAFE,         // 安全状态，表示缓冲区满了则阻塞，避免资源耗尽的风险
        ASYNC_UNSAFE,       // 不安全状态，不考虑资源耗尽的问题，无限扩容，常用于测试
    };

    class AsyncLooper
    {
    public:
        using ptr = std::shared_ptr<AsyncLooper>;   // 管理异步工作器

        // 必须要等到 thread 依赖成员都初始化完毕后，最后才初始化 thread
        AsyncLooper(const Functor &cb, AsyncType loop_type = AsyncType::ASYNC_SAFE) 
            :_stop(false), _callBack(cb), _loop_type(loop_type)
        {
            _thread = (std::thread(&AsyncLooper::ThreadEntry, this));
        }  // 在构造时，传递回调函数   // 默认是安全状态

        ~AsyncLooper()  { Stop(); }

        void Stop()     // 停止异步工作器的工作
        {
            // 其它的线程也可能会调用 stop 函数 这样会出现线程安全问题 为了避免加锁导致太过复杂，因此将 _stop 成员变量设置成原子的
            _stop = true;
            _cond_consumer.notify_all();        // 唤醒所有的工作线程
            _thread.join();                     // 等待工作线程的退出
        }

        void Push(const char* data, size_t len)     // 向缓冲区插入数据
        {
            /* 1. 无限扩容 -- 不安全        2. 固定大小 -- 生产缓冲区中数据满了就阻塞 */
            std::unique_lock<std::mutex> lock(_mutex);
            // 条件变量空值，若缓冲区剩余空间大小大于数据长度，则可以添加数据
            if(_loop_type == AsyncType::ASYNC_SAFE)     // 如果是安全状态，进行安全检测
            {
                _cond_producer.wait(lock, [&](){
                    return _producer_buffer.WriteAbleLength() >= len;
                });
            }

            // 运行到这，说明满足条件，可以向缓冲区添加数据
            _producer_buffer.Push(data, len);
            // 唤醒消费者对缓冲区中的数据进行处理
            _cond_consumer.notify_one();
        }

    private:
        void ThreadEntry()      // 线程入口函数 -- 对消费缓冲区中的数据进行处理，处理完毕后，初始化缓冲区，交换缓冲区
        {
            // while(!_stop)
            while(1)
            {
                // 1. 判断生产缓冲区有没有数据，有则交换，无则阻塞
                {
                    std::unique_lock<std::mutex> lock(_mutex);       // 对互斥锁进行管理

                    // 退出标志被设置，且生产缓冲区已无数据，这时再退出，否则有可能会造成生产缓冲区中有数据，但是没有被完全处理
                    if(_stop == true && _producer_buffer.Empty() == true)   { break; } 

                    // 若当前是退出前被唤醒，或者有数据被唤醒，则返回真，继续向下运行，否则重新陷入休眠
                    _cond_consumer.wait(lock, [&](){
                        return !_producer_buffer.Empty() || _stop == true;
                    });     // producr_buffer 不能为空 

                    // 有数据进行交换
                    _consumer_buffer.Swap(_producer_buffer);

                    // 4. 唤醒生产者    如果是安全状态，线程才会被阻塞，才需要被唤醒
                    if(_loop_type == AsyncType::ASYNC_SAFE) { _cond_producer.notify_all(); }
                }   // 交换完数据之后，就可以解锁了

                // 2. 被唤醒后，对消费缓冲区进行数据处理
                _callBack(_consumer_buffer);

                // 3. 初始化消费缓冲区
                _consumer_buffer.ReSet();
            }
        }

    private:
        std::atomic<bool> _stop;                     // 工作器停止的标志
        Functor _callBack;     // 具体对缓冲区数据进行处理的回调函数，由异步工作器使用者传入
        AsyncType _loop_type;    // 决定是否进行安全的控制
        LogBuffer _producer_buffer;     // 生产缓冲区
        LogBuffer _consumer_buffer;     // 消费缓冲区
        std::mutex _mutex;              // 互斥锁
        std::condition_variable _cond_producer;     // 生产者的条件变量
        std::condition_variable _cond_consumer;     // 消费者的条件变量
        std::thread _thread;            // 异步工作器对应的工作线程
    };
}

#endif