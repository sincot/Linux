#pragma once

#include "Logger.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"
#include "Cond.hpp"

#include <iostream>
#include <vector>
#include <queue>

namespace THREAD_POOL
{
    using namespace LOGMOUDLE;
    using namespace AY;

    const int defaultnum = 5; // 默认线程池中线程的个数
    // 线程池需要对多个任务线程做管理 —— "先描述，再组织"

    // 线程池
    template <class T>
    class ThreadPool
    {
    private:
        void ThreadPoolTask()
        {
            char name[64];
            pthread_getname_np(pthread_self(), name, sizeof(name));
            while (true)
            {
                // 保护任务队列
                _mutex.LockMutex();

                // 检测是否存在任务:
                // 线程不休眠的条件：任务队列不为空，并且线程池退出
                // 线程休眠的条件：任务队列为空，并且线程池正在运行
                while (_tasks.empty() && _running == true) // 没有任务,就一直休眠
                {
                    // 休眠，线程休眠的个数加1
                    _work_sleep_count++;

                    // 没有任务，休眠，使用条件变量
                    _cond.WaitCond(_mutex);

                    // 唤醒，线程休眠的个数减1
                    _work_sleep_count--;
                }

                // 线程池退出了，不代表 while 循环就要退出
                // 必须要处理完任务队列中的所有任务
                if (_tasks.empty() && !_running)
                {
                    // 解锁
                    _mutex.UnlockMutex();
                    break;
                }

                // 有任务，取任务(队头)，本质是把任务由公共变成私有
                T task = _tasks.front();
                // 对取走的任务 pop 掉
                _tasks.pop();

                _mutex.UnlockMutex();

                LOG(LogLevel::INFO) << name << " hander task: ";

                LOG(LogLevel::INFO) << "hander task";
                // 处理任务，不需在临界区内处理
                task();
                LOG(LogLevel::DEBUG) << task.GetResult();
            }

            LOG(LogLevel::INFO) << name << " exit";
        }

    public:
        // 线程池刚创建，还未运行
        ThreadPool(int work_num = defaultnum)
            : _running(false), _work_num(work_num), _work_sleep_count(0)
        {
            // 此时 ThreadPool 对象已经存在

            // 创建线程对象
            // 线程 tid 值默认设置成 -1. status 设置成新建. 默认是 joinable
            // Thread(callback_t cb)
            //    : _tid(-1), _status(THREAD_NEW), _joinable(true), _cb(cb)
            //{
            //    // to_string 函数将整型转成字符串类型
            //    _name = "Thread_" + std::to_string(gnumber++);
            //}
            for (int i = 0; i < _work_num; i++)
            {
                // 目前不清楚任务线程要执行的任务
                // _works.emplace_back(Test);

                // 参数绑定
                // auto bindtest = std::bind(ThreadPool::ThreadPoolTest, this, nullptr);
                // _works.emplace_back(bindtest);

                // lambda 表达式
                _works.emplace_back([this]()
                                    { this->ThreadPoolTask(); });
            }
        }

        void StartThreadPool() // 启动线程池
        {
            if (_running) // 线程池已经启动
            {
                LOG(LogLevel::WARNING) << "ThreadPool already running";
                return;
            }

            // 线程池未启动，更新线程池的运行状态
            _running = true;
            for (auto &work : _works)
            {
                work.StartThread(); // 让线程启动
            }
        }

        void WaitThreadPool()
        {
            for (auto &work : _works)
            {
                work.JoinThread(); // 等待线程
            }
        }

        void StopThreadPool() // 暂停线程池
        {
            // version 1
            // if (!_running) // 若线程池没有运行，就不必暂停了
            // {
            //     LOG(LogLevel::WARNING) << "ThreadPool is not Running";
            //     return;
            // }

            // // 让所有的线程池终止
            // for (auto &work : _works)
            // {
            //     work.StopThread(); // 让线程终止
            // }

            // // 更新线程池的运行状态
            // _running = false;

            // version 2
            // 线程池终止需要的条件
            // 1. 线程池处于非运行状态
            // 2. tasks 任务队列中所有任务全部处理完成
            // 3. 唤醒所有的线程去执行任务
            _mutex.LockMutex();

            // 将线程池处于非运行状态
            _running = false;
            if (_work_sleep_count > 0)
            {
                _cond.BroadcastCond();
            } // 唤醒所有的线程

            _mutex.UnlockMutex();
        }

        void PushQueue(T task) // 向任务队列放任务
        {
            // 必然需要加锁
            _mutex.LockMutex();
            _tasks.push(task);

            // 有线程在休眠，就唤醒一个
            if (_work_sleep_count > 0)
            {
                _cond.SignalCond();
            }

            _mutex.UnlockMutex();
        }

        ~ThreadPool() {}

    private:
        bool _running;              // 线程池是否在运行，便于停止线程池
        int _work_num;              // 线程池中任务线程的个数
        std::vector<Thread> _works; // 管理任务线程
        std::queue<T> _tasks;       // 任务队列
        Mutex _mutex;               // 使用锁保护任务队列
        Cond _cond;                 // 条件变量，没有获取到任务的线程休眠

        int _work_sleep_count; // 有多少个任务队列在休眠，便于使用条件变量
    };
}
