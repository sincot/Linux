/* 实用工具类实现：
    1. 获取系统时间
    2. 判断文件是否存在
    3. 获取文件所在路径
    4. 创建目录
*/

#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <iostream>
#include <ctime>
#include <string>

/* 实现跨平台性  之所以使用 if defined 是为了良好的扩展性 */
#if defined(_WIN32)
    #include <direct.h>
    #include <sys/stat.h>
    #define PLATFORM_STAT(path, st) _stat64(path, st)
    using PlatformStat = struct __stat64;   // __stat64 st 在x86/x64 通用
    #define PLATFORM_MKDIR(path) _mkdir(path)
#elif defined(__linux__)
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #define PLATFORM_STAT(path, st) stat(path, st)
    using PlatformStat = struct stat;
    #define PLATFORM_MKDIR(path) mkdir(path, 0777)
#else
    #error "Unsupported platform: only Windows and Linux are supported"
#endif


namespace Log
{
    namespace util
    {
        // 全部声明成 static 静态的，这样就可以直接使用类名来访问，不需要实例化对象
        class Date      // 与日期有关的
        {
        public:
            static size_t GetTime() { return static_cast<size_t>(time(nullptr)); } // 1. 获取系统时间
        };

        class File      // 与文件有关的 
        {
        public:     
            static bool IsExists(const std::string &pathname) // 2. 判断文件是否存在
            {
                // 判断一个文件是否存在，可以使用 OS 提供的接口 access
                // 头文件：unistd.h     函数原型： int access(const char *pathname, int mode);
                // mode 为 F_OK 用于测试 pathname 是否存在
                // 返回值：若 mode 为 F_OK，文件存在返回 0，文件不存在返回 -1
                // 需要注意的是 access 是系统调用接口，这意味着它的跨平台性不好，无法移植到 windows 下使用
                // return access(pathname.c_str(), F_OK) == 0;

                PlatformStat st;

                return PLATFORM_STAT(pathname.c_str(), &st) == 0;
            }

            static std::string GetPath(const std::string &pathname) // 3. 获取文件所在路径
            {
                size_t pos = pathname.find_last_of("/\\"); // 查找最后的 '/' 或者 '\'
                if (pos == std::string::npos)   { return "."; }                     // 没找到，没有其它的路径，就是当前目录
                return pathname.substr(0, pos + 1); // pos + 1 包含 / 在内
            }

            static void CreateDirectory(const std::string &pathname) // 4. 创建目录
            {
                // 从外向内层层创建
                size_t pos = 0, idx = 0; // pos：用于查找 分隔符 的位置   idx：用于标记查找的起始位置

                // 示例路径：./abc/bcd/cde
                while (idx < pathname.length())
                {
                    pos = pathname.find_first_of("/\\", idx); // 从 idx 位置开始寻找
                    if (pos == std::string::npos)             // 没找到，直接创建目录
                    {
                        PLATFORM_MKDIR(pathname.c_str());
                        break;      // 直接跳出循环
                    }

                    // 找到了，先创建父级目录
                    // 从查找的位置开始，获取 pos 长度的字符串，+1 将 / 也截取出来
                    std::string parent_dir = pathname.substr(0, pos + 1);     // 创建文件是在文件的内部创建
                    if (IsExists(parent_dir) == true)   // 如果 parent_dir 文件已经存在，就不需要再创建了
                    { 
                        idx = pos + 1;  // 跳过 '/' 或者 '\' 字符 
                        continue;   // 如果它已经存在，继续寻找
                    }
                    // parent_dir 不存在 则创建
                    PLATFORM_MKDIR(parent_dir.c_str());
                    idx = pos + 1;  // 继续向后寻找
                }
            }
        };
    }
}
#endif