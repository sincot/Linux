#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <memory>
#include <string>

/********************** 不定参宏函数 ***********************/
// __FILE__ 显示当前文件的文件名，__LINE__ 当前的行数，__VA_ARGS__ 使用 ... 不定参数
// #define LOG(fmt, ...)   printf("[%s:%d] " fmt, __FILE__, __LINE__, __VA_ARGS__)

// 解决方案：在 __VA_ARGS__ 前加上两个#号，表示若不定参函数为空，则取消 __VA_ARGS__ 前面的逗号
// #define LOG(fmt, ...)   printf("[%s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

// int main()
// {
//     // LOG("%s-%d\n", "xay", 123);

//     // 在进行宏替换的时候，不定参数为空，__VA_ARGS__ 为空，如此会存在逗号
//     // LOG("xay");      // 不定参为空，编译时会报错
//     // LOG("xay");

//     return 0;
// }
/********************** 不定参宏函数 ***********************/



/********************** C语言不定参函数 ***********************/
// count：传入的数字的个数，...：传入的具体数字
// void printNum(int count, ...)
// {
//     // 使用 va_start 宏获取到第一个不定参参数的地址     如何获取？定义一个 va_list     
//     // va_start：void va_start(va_list ap, last);
//     va_list ap;
//     va_start(ap, count);    // 获取 count 参数之后第一个不定参参数的地址

//     for(int i = 0; i < count; i++)
//     {
//         // va_arg：type va_arg(va_list ap, type);
//         int num = va_arg(ap, int);  // 要获取 int 类型的数据 ap会向后偏移type类型大小
//         printf("param[%d]:%d\n", i, num);
//     }
//     va_end(ap);     // 将 ap 置空
    
// }

// void myPrintf(const char* fmt, ...)
// {
//     va_list ap;
//     va_start(ap, fmt);
//     char* strp;
//     // 专门用来使用不定参参数的函数: int vasprintf(char **strp, const char *fmt, va_list ap);
//     int ret = vasprintf(&strp, fmt, ap);    // 必须使用 free 释放 strp    成功返回组织后的字符串长度失败返回 -1
//     if(ret != -1)
//     {
//         std::cout << strp << std::endl;
//         free(strp);
//     }
//     va_end(ap);     // 将 ap 置空
// }

// int main()
// {
//     printNum(5, 4, 7, 4, 6, 8);
//     // printNum(5, 4, 7, 4, 6);   // 若不定参参数的个数为5个，但是只传递了4个，剩下的值为很大的随机值 
//     // myPrintf("%s-%d", "xay", 123);
//     // myPrintf("xay");    

//     return 0;
// }
/********************** C语言不定参函数 ***********************/


/********************** C++不定参函数 ***********************/
// template<class T, class ...Args>
// void myPrintf(T &v, Args &&...args)     // args 参数包
// {
//     std::cout << std::forward<T>(v) << std::endl;
//     // 获取不定参参数的个数
//     if((sizeof ...(args)) > 0)
//     {
//         // 对剩下的参数做完美转发 std::forward<Args>(args) 左值依旧是左值 右值依旧是右值
//         myPrintf(std::forward<Args>(args)...);     // 对参数包使用 ... 展开
//     }
//     else
//     {
//          std::cout << std::endl;    
//     }
// }

// 基础终止函数 - 处理没有参数的情况
// void myPrintf() {
//     // 什么都不做，终止递归
// }

// template<class T, class ...Args>
// void myPrintf(const T &v, Args &&...args)     // args 参数包
// {
//     std::cout << v << std::endl;
//     // 获取不定参参数的个数
//     if((sizeof ...(args)) > 0)
//     {
//         // 对剩下的参数做完美转发 std::forward<Args>(args) 左值依旧是左值 右值依旧是右值
//         myPrintf(std::forward<Args>(args)...);     // 对参数包使用 ... 展开
//     }
//     else
//     {
//         myPrintf();
//     }
// }

// int main()
// {
//     myPrintf("xay");    // "xay" 是 const 值
//     myPrintf("xay", 123, 345);

//     return 0;
// }
/********************** C++不定参函数 ***********************/


/********************** 单例饿汉模式 ***********************/
// class Singleton
// {
// public:
//     static Singleton& GetInstance() { return _eton; }
//     int GetData()   { return _data; }
// private:
//     Singleton():_data(1) { std::cout << "单例对象构造成功" << std::endl; }                 // 构造函数私有化
//     Singleton(const Singleton&) = delete;   // 删除拷贝构造函数
//     ~Singleton() {}     // 析构函数

// private:
//     static Singleton _eton;     // 单例模式的声明
//     int _data;
// };
// Singleton Singleton::_eton;      // 单例模式的定义（在类外定义）

// int main()
// {
//     // std::cout << Singleton::GetInstance().GetData() << std::endl;

//     return 0;
// }
/********************** 单例饿汉模式 ***********************/


/********************** 单例懒汉模式 ***********************/
// class Singleton
// {
// private:
//     Singleton():_data(1) { std::cout << "单例对象构造成功" << std::endl; }                 // 构造函数私有化
//     Singleton(const Singleton&) = delete;   // 删除拷贝构造函数
//     ~Singleton() {}     // 析构函数

// private:
//     int _data;

// public:
//     static Singleton &GetInstance()
//     {
//         static Singleton _eton;     // 在成员函数中实例化局部单例对象
//         return _eton;
//     }

//     int GetData()   { return _data; }
// };

// int main()
// {
//     std::cout << Singleton::GetInstance().GetData() << std::endl;

//     return 0;
// }
/********************** 单例懒汉模式 ***********************/


/********************** 简单工厂模式 ***********************/
// class Fruit
// {
// public:
//     virtual void name() = 0;     // 纯虚函数
// };

// class Apple : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个苹果" << std::endl;
//     }
// };

// class Banana : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个香蕉" << std::endl;
//     }
// };

// // 若直接通过 new 创建对象，当类对象的构造函数发生改变，会比较麻烦。通过工厂来实例化对象
// class FruitFactory
// {
// public:
//     static std::shared_ptr<Fruit> Create(const std::string& name)
//     {
//         if(name == "苹果")  return std::make_shared<Apple>();   // 实例化 Apple 对象
//         if(name == "香蕉")  return std::make_shared<Banana>();   // 实例化 Banana 对象

//         return nullptr;
//     }
// };

// int main()
// {
//     std::shared_ptr<Fruit> apple = FruitFactory::Create("苹果");
//     apple->name();
//     std::shared_ptr<Fruit> banana = FruitFactory::Create("香蕉");
//     banana->name();

//     return 0;
// }

// 简单工厂模式：通过参数控制可以生成任意产品
// 简单工厂模式的优点：简单且易于理解，什么类型就创建什么类型的对象
// 简单工厂模式的缺点：
// 1. 所有东西生产在依次，产品太多会导致代码量庞大
// 2. 简单工厂模式违背了开闭原则（对扩展开放，对修改关闭），若又新增了一种产品，需要修改工厂方法
/********************** 简答工厂模式 ***********************/


/********************** 工厂方法模式 ***********************/
// class Fruit
// {
// public:
//     virtual void name() = 0;     // 纯虚函数
// };

// class Apple : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个苹果" << std::endl;
//     }
// };

// class Banana : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个香蕉" << std::endl;
//     }
// };

// class FruitFactory
// {
// public:
//     virtual std::shared_ptr<Fruit> Create() = 0;    // 纯虚函数
// };

// class AppleFactory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> Create() override    { return std::make_shared<Apple>(); }   // 构造 Apple 对象
// };

// class BananaFactory : public FruitFactory
// {
// public:
//     std::shared_ptr<Fruit> Create() override    { return std::make_shared<Banana>(); }   // 构造 Apple 对象
// };

// int main()
// {
//     // std::shared_ptr<FruitFactory> ff(new AppleFactory());
//     // std::shared_ptr<Fruit> fruit = ff->Create();
//     // fruit->name();
//     // ff.reset(new BananaFactory());   // 重新设置资源的管理者
//     // fruit = ff->Create();
//     // fruit->name();

//     std::shared_ptr<FruitFactory> ff1(new AppleFactory());
//     std::shared_ptr<Fruit> apple = ff1->Create();
//     apple->name();
//     std::shared_ptr<FruitFactory> ff2(new BananaFactory());
//     std::shared_ptr<Fruit> banana = ff2->Create();
//     banana->name();

//     return 0;
// }

// 工厂方法模式：定义⼀个创建对象的接口，但是由子类来决定创建哪种对象，使用多个工厂分别生产指定的固定产品
// 工厂方法模式的优点：
//      1. 减轻了工厂类的负担，将某类产品的生产交给指定的工厂来进行
//      2. 开闭原则遵循较好，添加新产品只需要新增产品的工厂即可，不需要修改原先的工厂类
// 工厂方法模式的缺点：对于某种可以形成⼀组产品族的情况处理较为复杂，需要创建大量的工厂类
/********************** 工厂方法模式 ***********************/


/********************** 抽象工厂模式 ***********************/
// class Fruit
// {
// public:
//     virtual void name() = 0;     // 纯虚函数
// };

// class Apple : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个苹果" << std::endl;
//     }
// };

// class Banana : public Fruit
// {
// public:
//     void name() override    // 对父类虚函数的重写
//     {
//         std::cout << "这是一个香蕉" << std::endl;
//     }
// };


// class Animal
// {
// public:
//     virtual void name() = 0;
// };

// class Cat : public Animal
// {
// public:
//     void name() override    { std::cout << "这是一只小猫" << std::endl; }
// };

// class Dog : public Animal
// {
// public:
//     void name() override    { std::cout << "这是一只小狗" << std::endl; }
// };


// class Factory
// {
// public:
//     virtual std::shared_ptr<Fruit> GetFruit(const std::string& name) = 0;   // 根据 name 生产不同的水果产品
//     virtual std::shared_ptr<Animal> GetAnimal(const std::string& name) = 0;   // 根据 name 生产不同的动物产品
// };

// class FruitFactory : public Factory
// {
// public:
//     std::shared_ptr<Animal> GetAnimal(const std::string& name) override  { return std::shared_ptr<Animal>(); }
//     std::shared_ptr<Fruit> GetFruit(const std::string& name) override
//     { 
//         if(name == "苹果") return std::make_shared<Apple>();
//         if(name == "香蕉") return std::make_shared<Banana>();

//         return nullptr;
//     }
// };

// class AnimalFactory : public Factory
// {
// public:
//     std::shared_ptr<Fruit> GetFruit(const std::string& name)  override  { return std::shared_ptr<Fruit>(); }
//     std::shared_ptr<Animal> GetAnimal(const std::string& name) override
//     { 
//         if(name == "小猫") return std::make_shared<Cat>();
//         if(name == "小狗") return std::make_shared<Dog>();

//         return nullptr;
//     }
// };

// class FactoryProducer   // 生产工厂的类
// {
// public:
//     static std::shared_ptr<Factory> Create(const std::string& name)
//     {
//         if(name == "水果")  return std::make_shared<FruitFactory>();
//         if(name == "动物")  return std::make_shared<AnimalFactory>();

//         return nullptr;
//     }
// };

// int main()
// {
//     std::shared_ptr<Factory> ff1 = FactoryProducer::Create("水果");     // 创建水果工厂
//     std::shared_ptr<Fruit> apple = ff1->GetFruit("苹果");
//     apple->name();
//     std::shared_ptr<Fruit> banana = ff1->GetFruit("香蕉");
//     banana->name();

//     std::shared_ptr<Factory> ff2 = FactoryProducer::Create("动物");
//     std::shared_ptr<Animal> cat = ff2->GetAnimal("小猫");
//     cat->name();
//     std::shared_ptr<Animal> dog = ff2->GetAnimal("小狗");
//     dog->name();
    
//     return 0;
// }
/********************** 抽象工厂模式 ***********************/


/********************** 建造者模式 ***********************/
/* 通过对苹果笔记本电脑的构造来理解建造者模式 */
// class Computer      // 1. 抽象产品类
// {
// public:
//     Computer()  {}
//     void SetBoard(const std::string& board) { _board = board; }
//     void SetDisplay(const std::string& display) { _display = display; }
//     virtual void SetOS() = 0;

//     void Print()
//     {
//         std::string param = "Computer Param: \n";
//         param += "\tBoard: " + _board + "\n";
//         param += + "\tDisplay: " + _display + "\n";
//         param += "\tOS: " + _os + "\n";

//         std::cout << param << std::endl;
//     }

// protected:      // 子类可以访问父类
//     std::string _board;     // 主板
//     std::string _display;   // 显示器
//     std::string _os;        // 操作系统
// };

// class MacBook : public Computer     // 2. 具体产品类
// {
// public:
//     void SetOS() override { _os = "Mac OS x12"; }
// };

// class Builder       // 3. 抽象 builder 类
// {
// public:
//     virtual void BuildBoard(const std::string& board) = 0;
//     virtual void BuildDisplay(const std::string& display) = 0;
//     virtual void BuildOS() = 0;
//     virtual std::shared_ptr<Computer> GetBuilder() = 0;
// };

// class MacBookBuilder : public Builder       // 4. 具体产品的 builder 类
// {
// public:
//     MacBookBuilder():_computer(new MacBook()) {}

//     // 构造零件
//     void BuildBoard(const std::string& board)   { _computer->SetBoard(board); }
//     void BuildDisplay(const std::string& display)   { _computer->SetDisplay(display); }
//     void BuildOS()  { _computer->SetOS(); }

//     // 返回 Computer 对象
//     std::shared_ptr<Computer> GetBuilder()   { return _computer; }

// private:
//     std::shared_ptr<Computer> _computer;
// };

// class Director      // 5. 指挥者 Director 类
// {
// public:
//     Director(Builder *builder):_builder(builder){}
//     void Construct(const std::string& board, const std::string& display)
//     {
//         _builder->BuildBoard(board);            // 构造主板
//         _builder->BuildDisplay(display);        // 构造显示器
//         _builder->BuildOS();                    // 构造OS
//     }

// private:
//     std::shared_ptr<Builder> _builder;
// };

// int main()
// {
//     Builder *builder = new MacBookBuilder();    // 建造苹果笔记本电脑
//     std::unique_ptr<Director> director(new Director(builder));
//     director->Construct("华硕", "三星显示器");
//     std::shared_ptr<Computer> computer = builder->GetBuilder();
//     computer->Print();

//     return 0;
// }
/********************** 建造者模式 ***********************/


/********************** 代理模式 ***********************/
/* 房东要把⼀个房子通过中介租出去理解代理模式 */
// class RentHouse     // 租房
// {
// public:
//     virtual void rentHouse() = 0;       // 纯虚函数
// };

// class LandHoster : public RentHouse     // 房东（目标类）
// {
// public:
//     void rentHouse()    { std::cout << "将房子出租出去" << std::endl; }
// };

// class Intermediary : public RentHouse   // 中介（代理类）     中介类实现了对房东类的功能加强
// {
// public:
//     void rentHouse()
//     { 
//         std::cout << "发布招租启示" << std::endl;
//         std::cout << "带人看房" << std::endl;
//         _landhoster.rentHouse();    // 将房子出租出去
//         std::cout << "负责租后维修" << std::endl;
//     }

// private:
//     LandHoster _landhoster;
// };


// int main()
// {
//     Intermediary intermediary;
//     intermediary.rentHouse();

//     return 0;
// }
/********************** 代理模式 ***********************/