#include "RingQueue.hpp"

#include <unistd.h>

void *ProductorRoutine(void *argc)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);

    int indata = 1;   // 生产的数据
    while(true)
    {
        sleep(2);   // 生产慢一点

        rq->PushQueue(indata);
        std::cout << "生产: " << indata << std::endl;
        indata++;
    }
}

void *ConsumerRoutine(void *argc)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(argc);
    
    int outdata = 0;   // 消费数据
    while(true)
    {
        // sleep(2);   // 消费慢一点

        rq->PopQueue(&outdata);
        std::cout << "消费: " << outdata << std::endl;
    }
}

// 单生产者-单消费者 模型
int main()
{
    RingQueue<int> *rq = new RingQueue<int>();    // 基于环形队列实现的 PC 模型
    pthread_t productor, consumer;
    pthread_create(&productor, nullptr, ProductorRoutine, rq);     // 初始化 productor
    pthread_create(&consumer, nullptr, ConsumerRoutine, rq);       // 初始化 consumer




    pthread_join(productor, nullptr);  
    pthread_join(consumer, nullptr);

    return 0;
}