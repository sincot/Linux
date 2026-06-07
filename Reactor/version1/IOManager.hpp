#pragma once

#include "Connection.hpp"
#include "Exitcode.hpp"

// IO 处理器
class IOManager : public BaseConnection
{
public:
    IOManager() {}
    ~IOManager() {}

    int Recver() override
    {
        return 0;
    }

    int Sender() override
    {
        return 0;
    }

    int Exceptioner() override
    {
        return 0;
    }

private:

};