#pragma once 

#include "Protocol.hpp"
#include <iostream>
#include <string>

class Calculator
{
public:
    Response Execute(Request &req)
    {
        Response resp;
        switch(req.GetOper())
        {
        case '+':
            resp._result = req._dataleft + req._dataright;
            break;
        case '-':
            resp._result = req._dataleft - req._dataright;
            break;
        case '*':
            resp._result = req._dataleft * req._dataright;
            break;
        case '/':
            {
                if(req._dataright == 0)
                {
                    resp._status = 1;   // 除0错误
                }
                else
                {
                    resp._result = req._dataleft / req._dataright;
                }
            }
            break;
        case '%':
            {
                if(req._dataright == 0)
                {
                    resp._status = 2;   // 模错误
                }
                else
                {
                    resp._result = req._dataleft % req._dataright;
                }
            }
            break;
        default:
            resp._status = 3;   // 非法操作
            break;
        }
        
        return resp;
    }
private:

};