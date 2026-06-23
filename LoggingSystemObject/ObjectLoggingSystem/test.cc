#include "Util.hpp"
#include "LogLevel.hpp"
#include "Format.hpp"

/************************ 测试 Util.hpp ************************/
// int main()
// {
//     std::cout << Log::util::Date::GetTime() << std::endl;       // 获取当前时间戳
//     std::string pathname = "./abc/bcd/a.txt";
//     // 调用 File 类中的 GetPath 函数，获取当前文件路径
//     // 调用 File 类中的 CreateDirectory 函数，根据获取到的路径创建目录
//     Log::util::File::CreateDirectory(Log::util::File::GetPath(pathname));

//     return 0;
// }
/************************ 测试 Util.hpp ************************/

/************************ 测试 LogLevel.hpp ************************/
// int main()
// {
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::DEBUG) << std::endl;
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::UNKNOW) << std::endl;
//     std::cout << Log::LogLevel::LevelToString(Log::LogLevel::value::INFO) << std::endl;
// }
/************************ 测试 Util.hpp ************************/

/************************ 测试 Format.hpp ************************/
int main()
{
    /*
        LogLevel::value level, 
        std::string file, 
        size_t line,  
        std::string logger,
        std::string payload
    */
    Log::LogMessage msg(Log::LogLevel::value::INFO, __FILE__, __LINE__, "root", "格式化 Format 测试");
    Log::Formatter fmt("abc%%de[%d{%H:%M:%S}]%m%n");
    std::string str = fmt.format(msg);  // 返回字符串
    std::cout << str << std::endl;

    return 0;
}
/************************ 测试 Format.hpp ************************/
