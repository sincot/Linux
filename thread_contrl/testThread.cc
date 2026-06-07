#include <iostream>
#include <cstdio>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <thread> // C++ 中的线程头文件


// // 调用 pthread_join 函数 —— 主线程比新线程先退出
// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         std::cout << "new thread " << name << std::endl;
//         sleep(1);
//         break;
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     sleep(5);
//     // 1. 回收等待新线程退出，如果不等待会导致类似僵尸进程的问题
//     // 2. 获取新线程的执行结果
//     void *retval = nullptr;     // 8字节大小
//     int n = pthread_join(tid, &retval); 
//     if(n == 0)
//     {
//         // retval 到底是多少
//         std::cout << "join sucess " << (long long)retval << std::endl;
//     }

//     return 0;
// }


// 新线程的返回值为类对象
// class Res
// {
// public:
//     int _code;
//     std::string _name;
//     std::string _info;
// };

// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         std::cout << "new thread " << name << std::endl;
//         sleep(1);
//         break;
//     }

//     Res *res = new Res();
//     res->_code = 10;
//     res->_name = name;
//     res->_info = "返回值是一个类对象";

//     return (void*)res;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     sleep(5);
//     // 1. 回收等待新线程退出，如果不等待会导致类似僵尸进程的问题
//     // 2. 获取新线程的执行结果
//     Res *retval = nullptr;     // 8字节大小
//     int n = pthread_join(tid, (void**)&retval); 
//     if(n == 0)
//     {
//         std::cout << "join sucess " << retval->_code << std::endl;
//         std::cout << "join sucess " << retval->_name << std::endl;
//         std::cout << "join sucess " << retval->_info << std::endl;
//     }

//     return 0;
// }


// 线程被取消时的退出信息
// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         sleep(1);
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     sleep(5);
//     std::cout << "新线程被取消了" << std::endl;

//     pthread_cancel(tid);
//     void *ret = nullptr;
//     int n = pthread_join(tid, &ret);
//     if(n == 0)
//     {
//         std::cout << "join sucess: " << (long long)ret << std::endl;
//     }

//     return 0;
// }


// 主线程分离新线程
// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout << "new thread is Running ... " << cnt << std::endl;
//         sleep(1);
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     sleep(2);   // 两秒后新线程被分离
//     std::cout << "新线程被分离了" << std::endl;
//     pthread_detach(tid);

//     void *ret = nullptr;
//     int n = pthread_join(tid, &ret);
//     if(n == 0)
//     {
//         std::cout << "join sucess: " << (long long)ret << std::endl;
//     }
//     else
//     {
//         std::cout << "join error: " << n << std::endl; 
//     }

//     return 0;
// }


// 新线程自己分离
// void* Routine(void* argc)
// {
//     pthread_detach(pthread_self());     // 自己分离自己
//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout << "new thread is Running ... " << cnt << std::endl;
//         sleep(1);
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     sleep(2);

//     void *ret = nullptr;
//     int n = pthread_join(tid, &ret);
//     if(n == 0)
//     {
//         std::cout << "join sucess: " << (long long)ret << std::endl;
//     }
//     else
//     {
//         std::cout << "join error: " << n << std::endl; 
//     }

//     return 0;
// }


// 当新线程分离后，该线程崩了
// void* Routine(void* argc)
// {
//     pthread_detach(pthread_self());     // 自己分离自己
//     int cnt = 5;
//     while(cnt--)
//     {
//         std::cout << "new thread is Running ... " << cnt << std::endl;
//         sleep(1);
        
//         // 除 0 错误
//         int a = 10;
//         a /= 0;
//     }

//     return (void*)1;
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     while(1) { sleep(1); }

//     return 0;
// }


// pid_t id = 0;       // 定义一个全部变量

// void* Routine(void* argc)
// {
//     std::string name = static_cast<const char*>(argc);
//     while(true)
//     {
//         std::cout << "new thread id: " << id << std::endl;
//         id++;
//         sleep(1);
//     }
// }

// int main()
// {
//     pthread_t tid;
//     pthread_create(&tid, nullptr, Routine, (void*)"thread_1");

//     while(true)
//     {
//         std::cout << "main thread id: " << id << std::endl;
//         id++;
//         sleep(1);
//     }

//     pthread_join(tid, nullptr);

//     return 0;
// }


// C++ 中的线程
// void Routine(int cnt)
// {
//     while(cnt)
//     {
//         std::cout << "new threaad: " << cnt << std::endl;
//         sleep(1);
//         cnt--;
//     }
// }

// int main()
// {
//     std::thread t(Routine, 5);

//     while(true)
//     {
//         std::cout << "main thread" << std::endl;
//         sleep(1);
//     }

//     t.join();

//     return 0;
// }








// pthread_create 函数的部分源码
// int __pthread_create_2_1(newthread, attr, start_routine, arg)
// pthread_t *newthread;
// const pthread_attr_t *attr;
// void *(*start_routine)(void *);
// void *arg;
// {
//     // 重点1：线程属性，虽然我们不设置，但是不妨碍我们了解
//     const struct pthread_attr *iattr = (struct pthread_attr *)attr;

//     // 重点2：传说中的原⽣线程库中的⽤来描述线程的tcb 
//     struct pthread *pd = NULL;
//     // 重点3: ALLOCATE_STACK会在先申请struct pthread对象，当然其实是申请⼀⼤块空间，
//     // struct pthread 在空间的开头，⼀会追
//     int err = ALLOCATE_STACK(iattr, &pd);

//     // 重点4：向线程tcb中设置未来要执⾏的⽅法的地址和参数
//     pd->start_routine = start_routine;
//     pd->arg = arg;

//     // 重点5：把 pd（就是线程控制块地址）作为ID，传递出去，所以上层拿到的就是⼀个虚拟地址
//     *newthread = (pthread_t)pd;

//     // 重点6: 检测线程属性是否分离，这个很好理解
//     bool is_detached = IS_DETACHED(pd);
// }


// TCB 的部分源码
// struct pthread
// {
//     /* Thread ID - which is also a 'is this thread descriptor (and
//     therefore stack) used' flag.  */
//     pid_t tid;

//     /* Process ID - thread group ID in kernel speak.  */
//     pid_t pid;

//     /* True if the user provided the stack.  */
//     bool user_stack;

//     /* The result of the thread function.  */
//     // 线程运⾏完毕，返回值就是 void*, 最后的返回值就放在 tcb 中的该变量⾥⾯
//     // 所以我们⽤ pthread_join 获取线程退出信息的时候，就是读取该结构体
//     // 另外，要能理解线程执⾏流可以退出，但是 tcb 可以暂时保留，这句话
//     void *result;

//     // ⽤⼾指定的⽅法和参数
//     void *(*start_routine)(void*);
//     void* arg;

//     // 线程⾃⼰的栈和⼤⼩
//     void *stackblock;
//     size_t stackblock_size;
// };


// 空间申请的函数，其实就是⼀个宏
// #define ALLOCATE_STACK(attr, pd) \
// allocate_stack(attr, pd, &stackaddr, &stacksize)

// static int
// allocate_stack(const struct pthread_attr *attr, struct pthread **pdp,
//                 ALLOCATE_STACK_PARMS)
// {
//     size = attr->stacksize ?: __default_stacksize; // 获取栈⼤⼩，⽤⼾没设置就默认

//     // 先尝试从 pthread 缓存中申请空间
//     reqsize = size;
//     pd = get_cached_stack(&size, &mem);

//     // 缓存申请失败，就在堆空间申请私有的匿名内存空间，这⾥ mmap 类似 malloc 
//     // 当然他也可以作为共享内存的实现，类似原理我们接触过，这个功能和当前⽆关
//     mem = mmap(NULL, size, prot, MAP_PRIVATE | MAP_ANONYMOUS | ARCH_MAP_FLAGS, -1, 0);

//     // 记录下来整个空间的地址和⼤⼩
//     pd->stackblock = mem;
//     pd->stackblock_size = size;

//     // 获取线程对应进程的pid 
//     pd->pid = THREAD_GETMEM(THREAD_SELF, pid);
// }