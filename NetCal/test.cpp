#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>
#include <sstream>

int main()
{
    std::string json_string =  "{\"name\":\"张三\", \"age\":30, \"city\":\"北京\"}";
    std::cout << json_string << std::endl;

    Json::Reader reader;
    Json::Value root;
    bool parseok = reader.parse(json_string, root);     // 将文件流解释成Json::Value对象

    // 反序列化成功，使用结构化数据接收
    std::string name = root["name"].asString(); // 提取字符串
    int age = root["age"].asInt();  // 提取整型
    std::string city = root["city"].asString();

    std::cout << name << " " << age << " " << city << std::endl;

    return 0;
}

