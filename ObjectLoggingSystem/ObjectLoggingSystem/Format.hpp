#ifndef MY_FORMAT_H
#define MY_FORMAT_H

#include "Util.hpp"
#include "LogMessage.hpp"

#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>
#include <memory>

/*
        %d： 表示日期，包含子格式 {%H:%M:%S}
        %t： 表示线程 ID
        %c： 表示日志器名称
        %f： 表示源文件名称
        %l： 表示源码行号
        %p： 表示日志级别
        %T： 表示制表符缩进
        %m： 表示主体消息
        %n： 表示换行符
*/
const std::string LOG_OUTPUT_FORMAT = "[%d{%H:%M:%S}][%t][%c][%f:%l][%p]%T%m%n";

namespace Log
{
    // 抽象格式化子项基类
    class FormatItem
    {
    public:
        virtual void Format(std::ostream &out, const LogMessage &msg) = 0;    // 纯虚函数
        using ptr = std::shared_ptr<FormatItem>;    // 对基类的管理
    };

    // 派生类格式化子项：主体消息，日志等级，时间子项，文件名，行号，日志器名称，线程ID，制表符缩进，换行，其它
    class MsgFormatItem : public FormatItem     // 主体消息
    {
    public:
        void Format(std::ostream &out, const LogMessage &msg) override { out << msg._payload; }   // 将数据放进去
    };

    class LogLevelFormatItem : public FormatItem    // 日志等级
    {
    public:
        void Format(std::ostream &out, const LogMessage &msg) override { out << LogLevel::LevelToString(msg._level); }
    };

    class TimeFormatItem : public FormatItem        // 时间子项
    {
    public:
        TimeFormatItem(const std::string &fmt = "%H:%M:%S"):_time_fmt(fmt) {}       // 构造函数
        void Format(std::ostream& out, const LogMessage &msg) override
        { 
            // 将时间戳转化为特定的结构 localtime_r 原型：struct tm *localtime_r(const time_t *timep, struct tm *result);
            struct tm result;
            localtime_r(&msg._ctime, &result);
            // 使用 strftime 函数   size_t strftime(char *s, size_t max, const char *format, const struct tm *tm);
            char tmp[32] = { 0 };
            strftime(tmp, 31, _time_fmt.c_str(), &result);
            out << tmp;     // 将时间子项放入到 out 中
        }
    
    private:
        std::string _time_fmt;      // 时间格式，默认是 %H%M%S（时分秒）
    };

    class FileFormatItem : public FormatItem        // 文件名
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << msg._file; }
    };

    class LineFormatItem : public FormatItem        // 行号
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << msg._line; }
    };

    class ThreadIdFormatItem : public FormatItem        // 线程 ID
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << msg._tid; }
    };

    class LoggerFormatItem : public FormatItem        // 日志器名称
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << msg._logger; }
    };

    class TableFormatItem : public FormatItem        // 制表符缩进
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << "\t"; }
    };

    class NewLineFormatItem : public FormatItem        // 换行
    {
    public:
        void Format(std::ostream& out, const LogMessage &msg) override    { out << "\n"; }
    };

    class OtherFormatItem : public FormatItem        // 其它非格式化子项
    {
    public:
        OtherFormatItem(const std::string &str):_str(str)   { }
        void Format(std::ostream& out, const LogMessage &msg) override    { out << _str; }

    private:
        std::string _str;
    };



    // 格式化模块类的设计
    class Formatter
    {
    public:
        Formatter(const std::string &pattern = LOG_OUTPUT_FORMAT): _pattern(pattern)
        {
            assert(ParsePattern());       // 解析必须成功
        }

        // 对 msg 进行格式化
        std::string format(const LogMessage &msg)
        {
            std::stringstream ss;
            format(ss, msg);
            return ss.str();
        }
        void format(std::ostream &out, const LogMessage &msg)
        {
            for(auto& item : _items)
            {
                item->Format(out, msg);
            }
        }
        
        using ptr = std::shared_ptr<Formatter>;
    private:
        // 对格式化字符串进行解析   items 数组的内容从格式化字符串中解析出来
        bool ParsePattern()
        {
            // 例子： abcde[%d{%H:%M:%S}][%p]%T%m%n
            /* 没有以 % 字符为起始的字符串都是原始字符      处理思想：不是 % 则一直向后走，直到遇到 % 则是原始字符串的结束 */
            /* 遇到 % 字符，看看紧随其后的这个字符是不是 %  如果是就是 % 字符 如果不是则代表紧随其后的这个字符是格格式化字符 */
            /* 紧随格式化字符之后 如果有 '{' 则 '{' 之后 '}' 之前的数据是格式化字符的子格式 */
            /* 循环上述的步骤 规则字符串的处理过程是一个循环的过程 */
            /* 在处理过程中，需要将处理得到的信息保存下来 */

            // 例子： abcde[%d{%H:%M:%S}][%p]%T%m%n
            /* key = nullptr    val = abcde[  */
            /* key = d          val = %H%M%S  */
            /* key = nullptr    val = ][      */
            /* key = p          val = nullptr */
            /* key = nullptr    val = ]       */
            /* key = T          val = null    */
            /* key = m          val = null    */
            /* key = n          val = null    */
            // 得到了数组后，根据数组的内容，创建对应的格式化子项对象，添加到 items 成员数组中

            // 1. 对格式化规则字符串进行解析
            std::vector<std::pair<std::string, std::string>> fmt_order;
            size_t pos = 0;
            std::string key, val;
            while(pos < _pattern.length())
            {
                // 处理原始字符串 —— 判断是否是 %，不是就是原始字符
                if(_pattern[pos] != '%')
                {
                    val.push_back(_pattern[pos++]);
                    continue;
                }

                // 运行到这里 说明 pos 位置是 % 接下来判断是否是 双百分号
                if(pos + 1 < _pattern.length() && _pattern[pos + 1] == '%')
                {
                    val.push_back('%');     // 将 %% 处理成 一个原始 % 字符
                    pos += 2;   // 直接向后走两步 跳过双百分号
                    continue;
                }
                // 运行到这里，说明 % 字符后面是个格式化字符，代表原始字符串处理完毕
                if(val.empty() == false)
                { 
                    fmt_order.push_back(std::make_pair("", val)); 
                    val.clear();    // 清空 val 的内容
                }
            

                /*********** 格式化字符的处理 ***********/
                pos += 1;   // pos+1 后指向格式化字符
                if(pos == _pattern.length())    // pos 走到了末尾   
                {
                    std::cout << "%字符之后，没有对应的格式化字符" << std::endl;
                    return false;
                }
                // 运行到这里 说明 pos 位置不是原始 % 字符，而是格式化字符
                key = _pattern[pos];
                pos += 1;   // 这时候 pos 指向格式化字符后的位置
                if(pos < _pattern.length() && _pattern[pos] == '{')    
                { 
                    pos += 1;   // 这时候 pos 指向子串的起始位置
                    
                    while(pos < _pattern.length() && _pattern[pos] != '}')
                    {
                        val.push_back(_pattern[pos++]);
                    }

                    // 如果找不到 '}' 字符，这该怎么办？ 若没有找到 '}' 字符，说明走到了末尾 代表格式是错误的
                    if(pos == _pattern.length())    
                    { 
                        std::cout << "子规则 {} 匹配出错" << std::endl;
                        return false; 
                    }
                    pos += 1;   // 这时 pos 指向的是 } 位置，向后走一步，走到了下次处理的位置
                }
                fmt_order.push_back(std::make_pair(key, val));
                key.clear(), val.clear();
            }
            // 2. 根据解析到的数据初始化格式化子项数组成员
            for(auto& it : fmt_order)
            {
                _items.push_back(CreateItem(it.first, it.second));
            }

            return true;
        }

        // 根据不同的格式化字符创建出不同的格式化子项对象
        FormatItem::ptr CreateItem(const std::string &key, const std::string &val)
        {
            if(key == "d")  { return std::make_shared<TimeFormatItem>(val); }       // 日期子项
            if(key == "t")  { return std::make_shared<ThreadIdFormatItem>(); }      // 线程 ID
            if(key == "c")  { return std::make_shared<LoggerFormatItem>(); }        // 日志器名称
            if(key == "f")  { return std::make_shared<FileFormatItem>(); }          // 文件名称
            if(key == "l")  { return std::make_shared<LineFormatItem>(); }           // 行号
            if(key == "p")  { return std::make_shared<LogLevelFormatItem>(); }      // 日志等级
            if(key == "T")  { return std::make_shared<TableFormatItem>(); }         // 制表符缩进
            if(key == "m")  { return std::make_shared<MsgFormatItem>(); }           // 消息主体
            if(key == "n")  { return std::make_shared<NewLineFormatItem>(); }       // 换行
            if(key.empty())  { return std::make_shared<OtherFormatItem>(val); }     // 空 key 表示是普通文本
            
            std::cout << "没有对应的格式化字符： %" << key << std::endl;
            abort();    
        }

    private:
        std::string _pattern;       // 格式化规则字符串
        std::vector<FormatItem::ptr> _items;    
    };
}
#endif