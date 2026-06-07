#include "RingQueue.hpp"

#include <unistd.h>

/*********************** 线程的名字 *************************/
int gnum = 0;
pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER; // 保护 gnum

int GetNumber()
{
    pthread_mutex_lock(&glock);
    int number = gnum++;
    pthread_mutex_unlock(&glock);

    return number;
}
/*********************** 线程的名字 *************************/

/*********************** 优化 *************************/
Mutex count_lock;
Mutex screen_lock;

int indata = 1;   // 生产的数据
int GetData()
{
    count_lock.LockMutex();

    int result = indata++;

    count_lock.UnlockMutex();

    return result;
}


void Print(const std::string &name, const std::string &info)
{
    screen_lock.LockMutex();

    std::cout << name << " : " << info << std::endl;

    screen_lock.UnlockMutex();
}
/************************* 优化 ***************************/

/************************* OLD ***************************/
// void *ProductorRoutine(void *argc)
// {
//     RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);

//     std::string name = "Productor_" + std::to_string(GetNumber());

//     while(true)
//     {
//         // sleep(2);   // 生产慢一点

//         rq->PushQueue(indata);
//         std::cout << name << " 生产: " << indata << std::endl;
//         indata++;
//     }
// }

// void *ConsumerRoutine(void *argc)
// {
//     RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);
    
//     std::string name = "Consumer_" + std::to_string(GetNumber());

//     int outdata = 0;   // 消费数据
//     while(true)
//     {
//         sleep(2);   // 消费慢一点

//         rq->PopQueue(&outdata);
//         std::cout << name << " 消费: " << outdata << std::endl;
//     }
// }
/************************* OLD ***************************/


/************************* NEW ***************************/
void *ProductorRoutine(void *argc)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);

    std::string name = "Productor_" + std::to_string(GetNumber());

    while(true)
    {
        // sleep(2);   // 生产慢一点

        int data = GetData();
        rq->PushQueue(data);
        Print(name, " 生产数据：" + std::to_string(data));
    }
}

void *ConsumerRoutine(void *argc)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);
    
    std::string name = "Consumer_" + std::to_string(GetNumber());

    int outdata = 0;   // 消费数据
    while(true)
    {
        sleep(2);   // 消费慢一点

        rq->PopQueue(&outdata);
        Print(name, " 消费数据：" + std::to_string(outdata));
    }
}
/************************* NEW ***************************/

// 多生产者-多消费者 模型
int main()
{
    RingQueue<int> *rq = new RingQueue<int>();    // 基于环形队列实现的 PC 模型
    pthread_t productor[3], consumer[2];
    pthread_create(productor, nullptr, ProductorRoutine, rq);       // 初始化 productor
    pthread_create(productor+1, nullptr, ProductorRoutine, rq);     // 初始化 productor
    pthread_create(productor+2, nullptr, ProductorRoutine, rq);     // 初始化 productor
    pthread_create(consumer, nullptr, ConsumerRoutine, rq);         // 初始化 consumer
    pthread_create(consumer+1, nullptr, ConsumerRoutine, rq);       // 初始化 consumer




    pthread_join(productor[0], nullptr);  
    pthread_join(productor[1], nullptr);  
    pthread_join(productor[2], nullptr);  
    pthread_join(consumer[0], nullptr);
    pthread_join(consumer[1], nullptr);

    return 0;
}