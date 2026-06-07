#include "ThreadPool.hpp"
#include "Logger.hpp"

#include <memory> // 智能指针
#include <functional>
#include <ctime>

using namespace THREAD_POOL;
using namespace LOGMOUDLE;
using namespace AY;

using task_t = std::function<void()>;

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

    int cnt = 5;
    while (cnt--)
    {
        int x = rand() % 10 + 1;
        usleep(21);
        int y = rand() % 10 + 1;

        // 不能创建线程池对象了
        Task t(x, y);
        ThreadPool<Task>::LazySingle()->PushQueue(t);
        sleep(1);
    }

    return 0;
}
