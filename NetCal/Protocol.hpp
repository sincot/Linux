#pragma once

#include "Logger.hpp"
#include <iostream>
#include <jsoncpp/json/json.h> // 序列化和反序列化
#include <functional>

using namespace LOGMOUDLE;
// 自定义协议
// 1. 自己实现 (不建议)
// 2. 使用别人实现的 —— json protobuf xml
//    平常使用最多的是 json protobuf

// 无论是 request 还是 response，都要有序列化和反序列化
// 客户端 -> 服务器 => request 请求
class Request
{
public:
    Request() : _dataleft(0), _dataright(0), _oper(0) {} // 无参构造
    Request(int x, int y, char oper) : _dataleft(x), _dataright(y), _oper(oper) {}
    ~Request() {}

    bool Serialize(std::string *out) // 序列化
    {
        // 结构化 -> 字符串
        Json::Value root;
        root["left"] = _dataleft;
        root["right"] = _dataright;
        root["oper"] = _oper;

        Json::FastWriter writer;
        *out = writer.write(root);

        return true;
    }

    bool DeSerialize(std::string &in) // 反序列化
    {
        // 字符串 -> 结构化
        Json::Value root;
        Json::Reader reader;
        bool parsesuccess = reader.parse(in, root); // 将解析的结果解析到 in 中
        if (!parsesuccess)
        {
            return false;
        } // 解析

        _dataleft = root["left"].asInt();
        _dataright = root["right"].asInt();
        _oper = root["oper"].asInt();

        return true;
    }

    // 获取类中的成员变量
    int GetDataLeft() { return _dataleft; }
    int GetDadaRight() { return _dataright; }
    char GetOper() { return _oper; }

public:
    // 约定表达式的结构：_dataleft _oper _dataright
    int _dataleft;  // 左操作数
    int _dataright; // 右操作数
    char _oper;     // 操作符
};

// 服务端 -> 客户端 => response 应答
class Response
{
public:
    Response() : _result(0), _status(0) {}
    Response(int result, int status) : _result(result), _status(status) {}
    ~Response() {}

    bool Serialize(std::string *out) // 序列化
    {
        // 结构化 -> 字符串
        Json::Value root;
        root["result"] = _result;

        Json::FastWriter writer;
        *out = writer.write(root);

        return true;
    }

    bool DeSerialize(std::string &in) // 反序列化
    {
        // 字符串 -> 结构化
        Json::Value root;
        Json::Reader reader;
        bool parsesuccess = reader.parse(in, root); // 将解析的结果解析到 in 中
        if (!parsesuccess)
        {
            return false;
        } // 解析

        _result = root["result"].asInt();

        return true;
    }

    // 获取类中的成员变量
    int GetResult() { return _result; }
    int GetStatus() { return _status; }

public:
    int _result;
    int _status; // 状态码 区分是否计算结果是否正确 0->正确 其它->错误
};

static std::string gsep = "\r\n";
// 给你 request 类对象，给我 response 类对象
using HandlerRequest_t = std::function<Response(Request &)>;
// 给你 response 类对象，处理应答
using HandlerResponse_t = std::function<void (Response &)>;

// 定制协议
class Protocol
{
public:
    Protocol(HandlerRequest_t handler_request) : _version("1.0"), _handler_request(handler_request) {}
    Protocol(HandlerResponse_t handler_response) : _version("1.0"), _handler_response(handler_response) {}
    ~Protocol() {}

    // 请求和应答后的字符串添加报头，有效载荷长度
    // 字符串的格式为：{"left": xx, "right":xx, "oper":xx}
    // 封包: len/r/n{"left": xx, "right":xx, "oper":xx}\r\n
    std::string Packet(const std::string &json_string) // 封包
    {
        return std::to_string(json_string.size()) + gsep + json_string + gsep; // 封包
    }

    // 收到的请求可能有多种：
    // len/r/n{"left": xx, "right":xx, "oper":xx}\r\n
    // len/r/n{"left": xx, "right":xx, "oper":xx}\r\nlen/r/n{"left": xx, "right":xx, "oper":xx}\r\n
    // len/r/n{"left": xx, "right":xx, "oper":xx}\r\nlen/r/n{"left": xx,
    // len/r/n{"left": xx, "right":xx
    // len
    // 返回值规定：
    // ret < 0: error
    // ret == 0: no error json_string != nullptr
    // ret > 0: no error json_string == nullptr
    int Unpack(std::string &packet, std::string *json_string) // 解包
    {
        if (packet.empty())
        {
            return 1;
        }
        if (json_string == nullptr)
        {
            return -1;
        }

        // 分析报文
        auto pos = packet.find(gsep); // 搜索分隔符
        if (pos == std::string::npos)
        {
            return -2;
        } // 找不到分隔符

        std::string lenstr = packet.substr(0, pos);            // 获取到整个报文的长度字符串
        int len = std::stoi(lenstr);                           // 将字符串转化成长度
        int total = lenstr.length() + len + 2 * gsep.length(); // 整个报文的长度
        if (packet.length() < total)
        {
            return -3;
        }

        // 提取报文
        *json_string = packet.substr(pos + gsep.length(), len); // 跳过分隔符,截取len长度
        packet.erase(0, total);                                 // 将整条报文删除

        return 0;
    }

    // 如果读到半个报文，什么都不做；如果读到一个报文及以上，循环处理，把所有合法的报文都进行统一处理
    std::string ParseRequest(std::string &inbuffer)     // 解析请求报文
    {
        std::string result;
        while (true)
        {
            // 1. 解包
            std::string json_string;
            int n = Unpack(inbuffer, &json_string); // 解包
            if (n < 0)                              // 解析出错
            {
                LOG(LogLevel::DEBUG) << "parse error";
                return std::string(); // 返回空串
            }
            if (n > 0)
            {
                LOG(LogLevel::WARNING) << inbuffer << " parse done";
                return result;
            }

            LOG(LogLevel::DEBUG) << "json:string\n" << json_string;
            LOG(LogLevel::DEBUG) << "unpack done, inbuffer\n" << inbuffer;


            // 2. 请求 request 反序列化
            // 得到一个完整的报文 json_string, 就是请求方发来的字符串
            Request req;
            if (!req.DeSerialize(json_string)) // 对解析出来的字符串做反序列化
            {
                return std::string(); // 反序列化失败，返回空串
            }

            // 3. 业务计算
            Response resp;
            if (_handler_request)
                resp = _handler_request(req);

            // 4. 应答 response 序列化
            std::string resp_json_string;
            resp.Serialize(&resp_json_string);

            // 5. 封包
            result += Packet(resp_json_string);
        }
    }

    std::string ParseResponse(std::string &inbuffer)     // 解析应答报文
    {
        std::string result;
        while (true)
        {
            // 1. 解包
            std::string json_string;
            int n = Unpack(inbuffer, &json_string); // 解包
            if (n < 0)                              // 解析出错
            {
                LOG(LogLevel::DEBUG) << "parse error";
                return std::string(); // 返回空串
            }
            if (n > 0)
            {
                LOG(LogLevel::WARNING) << inbuffer << " parse done";
                return result;
            }

            // 2. 应答 Response 反序列化
            // 得到一个完整的报文 json_string, 就是请求方发来的字符串
            Response resp;
            if (!resp.DeSerialize(json_string)) // 对解析出来的字符串做反序列化
            {
                return std::string(); // 反序列化失败，返回空串
            }

            // 3. 回调处理
            if (_handler_response)
                _handler_response(resp);
        }
    }

private:
    std::string _version;
    HandlerRequest_t _handler_request;
    HandlerResponse_t _handler_response;
};