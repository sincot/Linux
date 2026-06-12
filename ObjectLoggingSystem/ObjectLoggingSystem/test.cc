#include "Util.hpp"
#include "LogLevel.hpp"
#include "LogMessage.hpp"

int main()
{
    // std::cout << Log::util::Date::GetTime() << std::endl;       // 获取当前时间戳
    // std::string pathname = "./abc/bcd/a.txt";
    // // 调用 File 类中的 GetPath 函数，获取当前文件路径
    // // 调用 File 类中的 CreateDirectoy 函数，根据获取到的路径创建目录
    // Log::util::File::CreateDirectoy(Log::util::File::GetPath(pathname));

    std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::DEBUG) << std::endl;
    std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::UNKNOW) << std::endl;
    std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::INFO) << std::endl;


    return 0;
}
