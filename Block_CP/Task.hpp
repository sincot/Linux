#include <iostream>

class Task
{
public:
    Task(int x, int y) :_x(x), _y(y) {}

    void Add() { _result = _x + _y; }

    void Print() 
    { 
        std::cout << _x << " + " <<  _y << " = " << _result << std::endl; 
    }

    
    ~Task() {}

private:
    int _x;
    int _y;
    int _result;
};