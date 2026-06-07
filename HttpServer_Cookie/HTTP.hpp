#pragma once

#include "Logger.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <cstdio>

const std::string linesep = "\r\n";       // 行分隔符
const std::string spacesep = " ";         // 空格分隔符
const std::string headersep = ": ";      // 请求报头分隔符
const std::string suffixsep = ".";        // 后缀分隔符
const std::string g_version = "HTTP/1.0";    // HTTP 版本
const std::string g_status_code = "200";   // 状态码
const std::string g_reason_phrase = "OK";  // 状态描述
const std::string g_first_page = "index.html";    // 首页
const std::string g_root_dir = "wwwroot";    // 网站根目录
const std::string page_404 = "wwwroot/404.html";    // 404 页面
const std::string argsep = "?";        // URI 参数分隔符

using namespace LOGMOUDLE;

class Util
{
public:
    // 读取存在三种情况：
    // 1. 空：没有完整行
    // 2. \r\n：读取到了空行
    // 3. 其他：读取到了正常行
    static std::string ReadLine(std::string &req)   // 以行读取
    {
        size_t pos = req.find(linesep);        // 读取行
        if (pos == std::string::npos)   // 没有读到返回空字符串
            return "";

        std::string line = req.substr(0, pos);    // 读到返回请求行
        req.erase(0, pos + linesep.size());    // 从请求字符串中删除已经读取的部分
        if(line.empty())    return linesep;    // 读到的是空行，直接返回空行

        return line;
    }

    // 如果要读取二进制文件，推荐采用 vector<char> 存储
    static std::string ReadFile(const std::string &filename)   // 以文件读取
    {
        // 以二进制的方式读取 HTTP 请求字符串，对文本没有影响
        std::ifstream ifs(filename, std::ios::binary);
        if(!ifs.is_open())     // 默认以文本的方式打开文件，如果打开失败，返回空字符串
        {
            LOG(LogLevel::ERROR) << "Failed to open file: " << filename;
            return "";
        }

        // 获取指定文件大小  —— 调用 seekg 函数
        // seekg: 设置文件指针位置，函数原型为 std::istream& seekg(std::streampos pos)
        // 其中 pos 是一个表示文件指针位置的 std::streampos 对象。该函数将文件指针移动到指定的位置。
        // end: 文件末尾，函数原型为 std::ios_base::seekdir end = std::ios_base::end
        ifs.seekg(0, ifs.end);
        // tellg: 获取当前文件指针位置，函数原型为 std::streampos tellg() const
        // 该函数返回一个 std::streampos 对象，表示当前文件指针的位置。通过调用 tellg 函数，可以获取文件的大小。
        size_t file_size = ifs.tellg();
        // beg: 文件开头，函数原型为 std::ios_base::seekdir beg = std::ios_base::beg
        ifs.seekg(0, ifs.beg);

        // 根据文件大小，将缓冲区设置成 file_size大小
        std::string content;
        content.resize(file_size);
        ifs.read((char *)content.c_str(), file_size);

        ifs.close();

        return content;

        // this is a bug
        // std::ifstream ifs(filename, std::ios::binary);
        // if (!ifs.is_open())     // 默认以文本的方式打开文件，如果打开失败，返回空字符串
        // {
        //     LOG(LogLevel::ERROR) << "Failed to open file: " << filename;
        //     return "";
        // }

        // std::string inbuffer;
        // std::string content;
        // while (std::getline(ifs, inbuffer))    // 以行读取文件内容，直到读到文件末尾
        // {
        //     content += inbuffer + linesep;    // 将读取到的内容拼接成一个完整的字符串，行与行之间用行分隔符分隔
        // }
        // ifs.close();   // 关闭文件
        
        // return content;
    }
    
};

// 问题1：怎么知道读取到的是一个完整的报文？ 根据 HTTP 协议，报文以 \r\n\r\n 结尾
// 问题2：怎么处理粘包？ 通过 Content-Length 字段，指定请求正文的长度，读取指定长度的数据
class HttpRequest       // HTTP 请求
{
private:
    bool ParseRequestLine(std::string &req)     // 解析请求行
    {
        // 请求行 = 请求方法 + 请求资源路径 + HTTP 版本
        std::string req_line = Util::ReadLine(req);    // 读取请求行
        if(req_line.empty() || req_line == linesep)    return false;          // 没有读到完整请求行，报文不完整

        std::stringstream ss(req_line);
        ss >> _method >> _uri >> _version;

        if(_uri == "/")   _uri += g_first_page;    // 加上首页 index.html

        // 将请求资源路径加上网站根目录，得到完整的资源路径
        _uri = g_root_dir + _uri;  // wwwroot/index.html 这就是 web 根目录  

        // 拿到了文件的完整格式
        _suffix = GetSuffix();    // 获取请求资源路径的后缀

        return true;
    }

    bool ParseHanderkv(std::string &req)     // 解析请求报头
    {
        // 请求报头 —— 请求报头 = 字段名 + 字段值
        std::string header_line = Util::ReadLine(req);   // 读取请求报头
        // 请求报头有多行，直到读到空行 / 报头读完
        while(!header_line.empty() && header_line != linesep)   // 
        {
            size_t pos = header_line.find(headersep);      // 查找分隔符位置
            if(pos == std::string::npos)    
            {
                LOG(LogLevel::WARNING) << "invalid header line: " << header_line;
                return false;  // 没有找到分隔符，报文不完整
            }
            std::string key = header_line.substr(0, pos);     // 获取字段名
            std::string value = header_line.substr(pos + headersep.size());   // 获取字段值
            _headers[key] = value;     // 存储到请求报头中

            header_line = Util::ReadLine(req);   // 继续读取下一行请求报头
        }
        return true;
    }

    bool ParseText(std::string &req)    // 解析正文部分
    {
        if(strcasecmp(_method.c_str(), "GET") == 0)    // 判断是 Get 方法还是 POST 方法
        {
            // 怎么确定 GET 是否带参？根据 URI 中是否存在 ? 来判断
            size_t pos = _uri.find(argsep);     // 查找 URI 中是否存在 ?，获取 ? 的位置
            if(pos != std::string::npos)     // 存在 ?，说明带参了
            {
                // _body 和 _uri 的提取不能交换位置
                // 因为 _body 的提取需要使用到 _uri，如果先提取了 _uri，就会把原 _uri 覆盖掉，导致无法提取到正确的参数部分
                _body = _uri.substr(pos + argsep.length());    // 获取 ? 后面的参数
                _uri = _uri.substr(0, pos);      // 获取 ? 前面的 URI
            }
            else    // 不存在 ?，说明没有带参
            {
                _body = "";    // 没有带参，正文部分 body 设置为空字符串
                return false;   // 表示没有正文部分
            }
        }
        else
        {
            // 判断是否存在正文部分，根据是否设置了 Content-Length 字段
            if(_headers.find("Content-Length") == _headers.end())
            {
                _body = "";    // 没有正文部分，正文部分 body 设置为空字符串
                return false;   // 表示没有正文部分
            }
  
            // 存在正文部分,正文多大？从哪开始？根据 Content-Length 字段，读取指定长度的正文部分
            // 这部分是给 POST 方法准备的，GET 方法没有正文部分
            int content_length = std::stoi(_headers["Content-Length"]);    // 获取正文部分的长度
            _body = req.substr(0, content_length);   // 根据 Content-Length 字段，读取指定长度的正文部分
            req.erase(0, content_length);    // 从请求字符串中删除已经读取的正文部分
        }

        return true;
    }

public:
    HttpRequest() {}
    ~HttpRequest() {}

    // 反序列化
    bool Deserialize(std::string &req)
    {
        // 1. 解析请求行 —— 请求行 = 请求方法 + 请求资源路径 + HTTP 版本
        ParseRequestLine(req);
        LOG(LogLevel::DEBUG) << "method: " << _method << " uri: " << _uri << " version: " << _version;
        // 2. 解析请求报头 —— 请求报头 = 字段名 + 字段值
        ParseHanderkv(req);
        // 3. 设置空行
        _blank_line = Util::ReadLine(req);
        // 4. 解析请求正文 —— 根据 Content-Length 字段，读取指定长度
        ParseText(req);
    }

    std::string GetUri() { return _uri; }    // 获取请求资源路径
    std::string RequestContent() 
    { 
        return Util::ReadFile(_uri);    // 根据请求资源路径，读取文件内容，作为请求正文
    }

    std::string GetSuffix()    // 获取请求资源路径的后缀
    {
        size_t pos = _uri.rfind(suffixsep);     // 逆向查找最后一个 . 的位置
        if(pos == std::string::npos)     return ".html";   // 没有找到，默认后缀为 .html

        return _uri.substr(pos);     // 返回后缀部分
    }

    std::string GetBody() { return _body; }    // 获取请求正文

    void DebugPrint()    // 调试函数，打印 HTTP 请求对象的内容
    {
        std::cout << "Method: " << _method << std::endl;
        std::cout << "URI: " << _uri << std::endl;
        std::cout << "Version: " << _version << std::endl;
        std::cout << "Headers: " << std::endl;
        for(const auto &header : _headers)
        {
            std::cout << header.first << " : " << header.second << std::endl;
        }
        std::cout << _blank_line << std::endl;
        std::cout << "Body: " << _body << std::endl;
    }


private:
    // 结构化字段
    std::string _method;   // 请求方法
    std::string _uri;      // 请求资源路径
    std::string _version;  // HTTP 版本
    std::unordered_map<std::string, std::string> _headers;   // 请求报头
    std::string _blank_line;   // 空行
    std::string _body;     // 请求正文

    // 私有数据和协议无关
    std::string _suffix;   // 请求资源路径的后缀
};



class HttpResponse      // HTTP 响应
{
public:
    HttpResponse() 
        :_version(g_version), _status_code(g_status_code), _reason_phrase(g_reason_phrase)
    {}

    ~HttpResponse() {}

    void SetCode(int code)    // 设置状态码和状态码描述
    {
        _status_code = std::to_string(code);
        switch(code)
        {
            case 200:
                _reason_phrase = "OK";
                break;
            case 302:
                _reason_phrase = "Found";
                break;
            case 403:
                _reason_phrase = "Forbidden";
                break;
            case 404:
                _reason_phrase = "Not Found";
                break;
            case 504:
                _reason_phrase = "Gateway Timeout";
                break;
            default:
                _reason_phrase = "Unknown";
        }
    }

    void AddHeader(const std::string &key, const std::string &value)    // 设置响应报头
    {
        // 如果属性存在，就覆盖；如果不存咋，就添加
        _headers[key] = value;
    }

    void SetBody(const std::string &body) { _body = body; }    // 设置响应正文

    int GetBodyLength() { return _body.length(); }    // 获取响应正文长度   
    
    // 序列化
    std::string Serialize(std::string &req)
    {
        std::string resp_string;
        // 构建状态行
        // 状态行 = HTTP 版本 + 状态码 + 状态描述
        _status_line = _version + spacesep + _status_code + spacesep + _reason_phrase;    // 构造状态行
        resp_string += _status_line + linesep;    // 状态行 + 行分隔符

        // 构建响应报头
        // 响应报头 = 字段名 + 字段值
        for(const auto &header : _headers)
        {
            resp_string += header.first + headersep + header.second + linesep;    // 字段名 + 字段值 + 行分隔符
        }

        // 构建空行和响应正文
        resp_string += linesep;    // 响应报头后跟一个空行
        resp_string += _body;      // 响应正文

        return resp_string;     // 返回完整的 HTTP 响应字符串
    }

private:
    // 结构化字段
    std::string _version;  // HTTP 版本
    std::string _status_code;  // 状态码
    std::string _reason_phrase;    // 状态描述
    std::string _status_line;  // 状态行
    std::unordered_map<std::string, std::string> _headers;   // 响应报头
    std::string _blank_line;   // 空行
    std::string _body;     // 响应正文
};

class MineType 
{
public:
    MineType() {}
    ~MineType() {}

    static std::string SuffixToMineType(const std::string &suffix)    // 根据后缀获取 MIME 类型
    {
        auto it = _mime_types.find(suffix);
        if(it != _mime_types.end())     return it->second;  // 找到，返回 MIME 类型
        else    return "text/html";    // 没有找到，返回默认 MIME 类型   
    }

private:
    static std::unordered_map<std::string, std::string> _mime_types;   // 存储 MIME 类型的映射关系
};

std::unordered_map<std::string, std::string> MineType::_mime_types = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".ico", "image/x-icon"}    // 初始化 MIME 类型映射关系
};


// 定义服务函数类型，参数为 HTTP 请求对象和 HTTP 响应对象
using service_t = std::function<void (HttpRequest &req, HttpResponse &resp)>;

class HttpProtocol
{
public:
    HttpProtocol() {}
    ~HttpProtocol() {}

    void RegisterService(const std::string &uri, service_t service)    // 注册服务函数
    {
        _http_service[uri] = service;   // 将 URI 和服务函数的映射关系存储到 _http_service 中
    }

    bool IsReqService(std::string uri)    // 判断用户是否在请求某个服务
    {
        uri = g_root_dir + uri;  // 将 URI 加上网站根目录，得到完整的资源路径
        // 找到了 uri 就说明用户请求的是服务，否则就是请求静态资源
        return _http_service.find(uri) != _http_service.end();   // 查找请求 URI 是否在 _http_service 中
    }

    std::string HandlerStaticResource(std::string &req)     // 处理静态资源
    {
        // 1. 我们收到了 HTTP 请求，首先需要保证报文的完整性，分析报文是否有空行 —— IGN

        // 2. 知道已经是一个完整的报文，接下来需要对报文进行反序列化，得到结构化的 HTTP 请求对象
        HttpRequest http_req;
        http_req.Deserialize(req);  // 将传进来的字符串反序列化成 HTTP 请求对象

        // 3. 根据不同的请求方法和请求URI，给用户返回不同的报文
        HttpResponse http_resp;
        
        std::string content = http_req.RequestContent();    // 根据请求资源路径，读取文件内容，作为请求正文
        if(content.empty())    // 读取文件失败，返回 404 Not Found
        {
            // http_resp.SetCode(404);      // 同时设置了状态码和状态描述
            // // 设置响应报头 Content-Length，指定响应正文的长度
            // http_resp.AddHeader("Content-Length", std::to_string(Util::ReadFile(page_404).size()));
            // http_resp.SetBody(Util::ReadFile(page_404));    // 读取 404 页面内容，作为响应正文

            // 如果访问的资源不存在，重定向到 CSDN 首页
            http_resp.SetCode(302);      // 同时设置了状态码和状态描述
            http_resp.AddHeader("Location", "./404.html");    // 设置响应报头 Location，指定重定向的 URI
            http_resp.SetBody("");    // 设置响应正文为空字符串
        }
        else
        {
            http_resp.SetCode(200);       // 同时设置了状态码和状态描述
            // 设置响应报头 Content-Length，指定响应正文的长度
            http_resp.AddHeader("Content-Length", std::to_string(content.size()));
            // 设置响应报头 Content-Type，指定响应正文的类型
            http_resp.AddHeader("Content-Type", MineType::SuffixToMineType(http_req.GetSuffix()));
            http_resp.AddHeader("Connection", "close");    // 设置响应报头 Connection，关闭长连接
            // 设置响应报头 Set-Cookie，设置 cookie
            http_resp.AddHeader("Set-Cookie", "name=zhangsan; password=123456");
            http_resp.SetBody(content);   // 设置响应正文
        }
 
        return http_resp.Serialize(req);  // 将 HTTP 响应对象序列化成字符串，返回给用户
    }

    std::string HandlerHttpRequest(std::string &req)     // 处理 HTTP 请求
    {
        HttpRequest http_req;
        http_req.Deserialize(req);  // 将传进来的字符串反序列化成 HTTP 请求对象
        HttpResponse http_resp;
        // 如果用户请求的是某个服务，就执行服务处理函数，否则就按照之前的逻辑处理静态资源
        if(IsReqService(http_req.GetUri()))      // 判断用户是否在请求某个服务
        {
            // 怎么判定用户请求的是服务还是静态资源？ 通过请求 URI 是否在 _http_service 中来判断
            // 功能路由 —— 根据请求 URI，找到对应的服务函数，执行服务处理函数
            _http_service[http_req.GetUri()](http_req, http_resp);
        }
        else
        {
            return HandlerStaticResource(req);    // 处理静态资源
        }
    }

private:
    // 根据 uri(服务路径) 进行功能路由
    std::unordered_map<std::string, service_t> _http_service;    // 存储 URI 和服务函数的映射关系
};