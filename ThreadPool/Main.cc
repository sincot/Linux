#include "ThreadPool.hpp"
#include "Logger.hpp"

#include <memory> // 智能指针
#include <functional>
#include <ctime>

using namespace THREAD_POOL;
using namespace LOGMOUDLE;
using namespace AY;

using task_t = std::function<void()>;

// void task()
// {
//     LOG(LogLevel::DEBUG) << "I am a task, is handlering";
// }

class Task
{
public:
    Task(int x, int y) : _x(x), _y(y) {}

    // 重载 () 操作符 —— task()
    void operator()()
    {
        _result = _x + _y;
    }

    std::string GetResult()
    {
        return std::to_string(_x) + " + " + std::to_string(_y) + " = " + std::to_string(_result);
    }

    ~Task() {}

private:
    int _x;
    int _y;
    int _result;
};

int main()
{
    ENABLE_CONSOLE_LOG_STRATEGY(); // 输出采用控制台策略

    srand((unsigned int)time(nullptr));

    // std::unique_ptr<ThreadPool<int>> tp = std::make_unique<ThreadPool<int>>();
    // std::unique_ptr<ThreadPool<task_t>> tp = std::make_unique<ThreadPool<task_t>>();
    std::unique_ptr<ThreadPool<Task>> tp = std::make_unique<ThreadPool<Task>>();

    tp->StartThreadPool();

    int cnt = 5;
    while (cnt--)
    {
        int x = rand() % 10 + 1;
        usleep(21);
        int y = rand() % 10 + 1;

        Task t(x, y);
        tp->PushQueue(t);
        sleep(1);
    }

    tp->StopThreadPool();
    tp->WaitThreadPool();

    // tp->StartThreadPool();  // 启动线程池
    // sleep(2);
    // tp->StopThreadPool();
    // tp->WaitThreadPool();   // 等待线程池
    // sleep(2);

    return 0;
}
