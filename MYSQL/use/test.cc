#include <iostream>
#include <mysql/mysql.h>
#include <string>

const std::string DB_HOST = "localhost";    // 数据库主机地址
const std::string DB_USER = "root";          // 数据库用户名
const std::string DB_PASSWORD = "";      // 数据库密码
const std::string DB_NAME = "test";         // 数据库名称
const unsigned int DB_PORT = 3306;          // 数据库端口


int main()
{
    MYSQL *conn = mysql_init(nullptr);
    if(conn == nullptr)
    {
        std::cerr << "mysql_init() failed\n";
        return 1;
    }

    if(mysql_real_connect(conn, DB_HOST.c_str(), DB_USER.c_str(), DB_PASSWORD.c_str(), DB_NAME.c_str(), DB_PORT, nullptr, 0) == nullptr)
    {
        std::cerr << "mysql_real_connect() failed\n";
        return 2;
    }
    std::cout << "mysql_real_connect() success" << std::endl;
    // 连接成功后，设置默认字符集 utf8mb4
    // mysql_set_character_set(conn, "utf8mb4");
    
    // std::string sql = "insert into account values(5, '田七', 500)";
    // std::string sql = "insert into account values(6, '南宫', 600)";
    std::string sql = "select * from account";
    int n = mysql_query(conn, sql.c_str());
    if(n == 0)  std::cout << "mysql_query success" << std::endl;
    else { std::cerr << "mysql_query fails\n"; return 3; }

    MYSQL_RES *res = mysql_store_result(conn);      // 获取 mysql_query 的执行结果
    if(res == nullptr) { std::cerr << "mysql_store_result" << std::endl; return 4; }

    my_ulonglong rows = mysql_num_rows(res);
    my_ulonglong fields = mysql_num_fields(res);

    std::cout << "行：" << rows << std::endl;
    std::cout << "列：" << fields << std::endl; 

    // 打印表的字段
    MYSQL_FIELD *fields_array = mysql_fetch_fields(res);
    for(my_ulonglong i = 0; i < fields; i++)    std::cout << fields_array[i].name << "\t";
    std::cout << std::endl;

    // 打印表的内容
    for(my_ulonglong i = 0; i < rows; i++)
    {
        MYSQL_ROW data = mysql_fetch_row(res);
        for(my_ulonglong j = 0; j < fields; j++)
        {
            std::cout << data[j] << "\t"; 
        }
        std::cout << std::endl;
    }

    // std::string sql;
    // while(true)
    // {
    //     std::cout << "MYSQL>>> "; 
    //     if(!std::getline(std::cin, sql)) break;
    //     if(sql == "quit")   break;
        
    //     int n = mysql_query(conn, sql.c_str());
    //     if(n == 0)  std::cout << "mysql_query success" << std::endl;
    //     else std::cerr << "mysql_query fails\n";
    // }
    
    // mysql_store_result(conn);   // 获取执行结果
    free(res);  // 释放掉获取的数据库信息
    mysql_close(conn);      // 关闭打开的数据库

    return 0;
}