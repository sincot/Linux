#pragma once

#include "Logger.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace LOGMOUDLE;

std::string sep = ": ";

static const std::string default_dict_path = "./dict.txt";

class Dict
{
public:
    Dict(const std::string &dict_path = default_dict_path) :_dict_path(dict_path)
    {
        // 在构造字典对象时，字典就字典加载
        LoadDict();
    }

    ~Dict() {}

    void LoadDict()     // 加载字典
    {
        std::ifstream in(_dict_path);   // 打开字典文件流
        if(!in.is_open())       // 打开文件失败
        {
            LOG(LogLevel::FATAL) << "open " << _dict_path << " error";
            exit(1);
        }

        std::string line;
        while(std::getline(in, line))     // 将读取到的数据存储在 _dict 对象中
        {
            LOG(LogLevel::FATAL) << "load: " << line << " error";
            // apple: 苹果 根据 ': ' 分割单词和翻译
            auto pos = line.find(sep);      // 找到了冒号,即找到了开始分割的地方
            if(pos == std::string::npos)    // 没找到分割符
            {
                LOG(LogLevel::WARNING) << "Format: " << line << " error";
                continue;   // 继续读取下一行
            }

            // 开始分割
            std::string k = line.substr(0, pos);    // 前闭后开区间，获取单词
            std::string v = line.substr(pos+sep.size());    // 跳过分割字符串，获取词义

            // 存储在 _dict 对象中
            _dict.insert(std::make_pair(k, v));
        }

        in.close();     // 关闭打开的文件流
        LOG(LogLevel::INFO) << "dict load done...";
    }

    std::string Translate(std::string& word)       // 翻译
    {
        auto it = _dict.find(word);       // 在字典里找单词
        if(it != _dict.end())   // 单词找到了
        {
            return it->second;     // 返回单词的词义
        }
        else    // 单词不存在字典中
        {
            return "None";
        }
    }
private:
    std::string _dict_path;
    std::unordered_map<std::string, std::string> _dict;
};