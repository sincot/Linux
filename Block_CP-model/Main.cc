#include "BlockQueue.hpp"


int gnum = 0;
pthread_mutex_t glock = PTHREAD_MUTEX_INITIALIZER;     // 保护 gnum

int GetNumber()
{
    pthread_mutex_lock(&glock);
    int number = gnum++;
    pthread_mutex_unlock(&glock);

    return number;
}

void *ConsumerRoutine(void *argc)   // 消费者
{
    std::string con_name = "Consumer_" + std::to_string(GetNumber());   // 设置名字
    // int pthread_setname_np(pthread_t thread, const char *name);
    // 设置消费者线程的名称，长度不能超过16
    pthread_setname_np(pthread_self(), con_name.c_str());
    
    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(argc);

    // 消费者不断消费
    while(true)
    {
        // sleep(5);
        int outdata;
        bq->PopQueue(&outdata);
        // std::cout << "consumer: " << outdata << std::endl;
        std::cout << con_name << " 消费: " << outdata << std::endl;
    }
}

void *ProductorRoutine(void *argc)
{
    std::string pro_name = "Productor_" + std::to_string(GetNumber());   // 设置名字
    // int pthread_setname_np(pthread_t thread, const char *name);
    // 设置消费者线程的名称，长度不能超过16
    pthread_setname_np(pthread_self(), pro_name.c_str());

    BlockQueue<int> *bq = static_cast<BlockQueue<int> *>(argc);

    int indata = 1;     // 投入的数据

    // 生产者不断的生产
    while(true)
    {
        // sleep(5);
        bq->PushQueue(indata);
        // std::cout << "productor: " << indata << std::endl;
        std::cout << pro_name <<  " 生产: "<< indata << std::endl;
        indata++;
    }
}


// 多生产者多消费者
int main()
{
    // 创建阻塞队列
    BlockQueue<int> *bq = new BlockQueue<int>();
    pthread_t consumer[3], productor[2];    // 3个生产者，2个消费者
    // int pthread_create(pthread_t *restrict thread,
    //                    const pthread_attr_t *restrict attr,
    //                    void *(*start_routine)(void *),
    //                    void *restrict arg);
    pthread_create(consumer, nullptr, ConsumerRoutine, bq);
    pthread_create(consumer+1, nullptr, ConsumerRoutine, bq);
    pthread_create(consumer+2, nullptr, ConsumerRoutine, bq);
    pthread_create(productor, nullptr, ProductorRoutine, bq);
    pthread_create(productor+1, nullptr, ProductorRoutine, bq);

    // int pthread_join(pthread_t thread, void **retval);
    pthread_join(consumer[0], nullptr);
    pthread_join(consumer[1], nullptr);
    pthread_join(consumer[2], nullptr);
    pthread_join(productor[0], nullptr);
    pthread_join(productor[1], nullptr);


    return 0;
}



// 单生产者单消费者
// int main()
// {
//     // 创建阻塞队列
//     BlockQueue<int> *bq = new BlockQueue<int>();
//     pthread_t consumer, productor;
//     // int pthread_create(pthread_t *restrict thread,
//     //                    const pthread_attr_t *restrict attr,
//     //                    void *(*start_routine)(void *),
//     //                    void *restrict arg);
//     pthread_create(&consumer, nullptr, ConsumerRoutine, bq);
//     pthread_create(&productor, nullptr, ProductorRoutine, bq);

//     // int pthread_join(pthread_t thread, void **retval);
//     pthread_join(consumer, nullptr);
//     pthread_join(productor, nullptr);


//     return 0;
// }