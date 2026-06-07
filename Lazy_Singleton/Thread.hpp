#pragma once

#include "Logger.hpp"       // 线程日志化

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <functional>
#include <pthread.h>
#include <vector>
#include <time.h>

// version2:
namespace AY
{
    static int gnumber = 1; // 线程的名字命名的序号

    using callback_t = std::function<void()>;

    // 线程的状态
    enum
    {
        THREAD_NEW, // 新建
        THREAD_RUN, // 运行
        THREAD_STOP // 取消
    };

    std::string StatusToString(int _status)
    {
        switch (_status)
        {
        case THREAD_NEW:
            return "THREAD_NEW";
        case THREAD_RUN:
            return "THREAD_RUN";
        case THREAD_STOP:
            return "THREAD_STOP";
        default:
            return "UNKOWN";
        }
    }

    std::string JoinableToString(bool _joinable)
    {
        return _joinable ? "true" : "false";
    }

    class Thread
    {
    private:
        // 新线程所要执行的任务
        // void *ThreadRoutine(Thread* this, void* argc)
        // 使用 static 修饰，表示该方法属于类，不属于对象
        static void *ThreadRoutine(void *argc)
        {
            Thread *self = static_cast<Thread *>(argc);
            self->_status = THREAD_RUN; // 修改线程的状态 THREAD_NEW -> THREAD_RUN
            // int pthread_setname_np(pthread_t thread, const char *name)
            pthread_setname_np(self->_tid, self->_name.c_str()); // 将当前线程的名字设置成 name
            // 线程要执行什么任务完全由外部决定
            self->_cb();
            // 线程的任务执行完毕后，修改线程的状态 THREAD_RUN -> THREAD_STOP
            self->_status = THREAD_STOP;

            return nullptr;
        }

    public:
        // 线程 tid 值默认设置成 -1. status 设置成新建. 默认是 joinable
        Thread(callback_t cb)
            : _tid(-1), _status(THREAD_NEW), _joinable(true), _cb(cb)
        {
            // to_string 函数将整型转成字符串类型
            _name = "Thread_" + std::to_string(gnumber++);
        }

        bool StartThread() // 创建线程
        {
            // 创建线程
            // int pthread_create(pthread_t *restrict thread,
            //            const pthread_attr_t *restrict attr,
            //            void *(*start_routine)(void *),
            //            void *restrict arg);
            // int n = pthread_create(&_tid, nullptr, ThreadRoutine, nullptr);
            int n = pthread_create(&_tid, nullptr, ThreadRoutine, this);
            if (n != 0) // 线程创建失败
            {
                return false;
            }

            // 只要线程创建好了，它的状态就设置成 THREAD_RUN
            _status = THREAD_RUN;
            return true;
        }

        void DetachThread() // 分离线程
        {
            // 线程是 THREAD_RUN 状态，并且是 joinable，才能做分离
            if (_status == THREAD_RUN && _joinable)
            {
                // 调用 pthread_detach 函数
                // int pthread_detach(pthread_t thread)
                pthread_detach(_tid);
                _joinable = false; // 已分离
            }
            else // 条件不满足，分离失败
            {
                std::cerr << "detach " << _name << " failed" << std::endl;
            }
        }

        void StopThread() // 终止线程
        {
            if (_status == THREAD_RUN)
            {
                // 调用 pthread_cancel 函数
                // int pthread_cancel(pthread_t thread)
                pthread_cancel(_tid);
                // 线程状态由 THREAD_RUN 变成 THREAD_STOP,表明线程终止了
                _status = THREAD_STOP;
            }
        }

        void JoinThread() // 等待线程
        {
            if (_joinable == true) // 线程未分离
            {
                // 调用 pthread_join 函数
                //  int pthread_join(pthread_t thread, void **retval)
                int n = pthread_join(_tid, &_result);
                if (n != 0) // 等待失败
                {
                    std::cerr << "join error " << n << std::endl;
                    exit(2);
                }

                // 等待成功，说明线程已经结束了
                _status = THREAD_STOP;
            }
            else // 线程已分离，不能被 join
            {
                std::cout << "join error, thread status: " << _joinable << std::endl;
            }
        }

        void PrintInfo() // 等待成功后，线程的信息
        {
            std::cout << "thread _name: " << _name << std::endl;
            std::cout << "thread _tid: " << _tid << std::endl;
            std::cout << "thread _status: " << StatusToString(_status) << std::endl;
            std::cout << "thread _joinable: " << JoinableToString(_joinable) << std::endl;
        }

        ~Thread() {}

    private:
        std::string _name; // 线程名
        pthread_t _tid;    // 线程ID
        int _status;       // 线程状态
        bool _joinable;    // 线程是否分离，true —— 未分离，false —— 已分离
        callback_t _cb;    // 线程自己的要执行的任务，即回调函数
        void *_result;     // 线程的退出信息
    };
}