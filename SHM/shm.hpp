#include <iostream>
#include <sys/shm.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

// const int gsize = 4096;
// const int gsize = 4097;
const int gsize = 64;

// 用于生成 System V IPC key —— 用户指明的
#define PATHNAME "/tmp"     // 这个路径随便选
#define PROJ_ID 0x66         // 数字随便选

// 错误码
enum
{
    NO_PROBLEM = 0,     // 没有问题
    GETKEY_ERROR,       // 获取键值失败
    CREATSHM_ERROR,     // 创建共享内存失败
    ATTACHSHM_ERROR,    // 挂接共享内存失败
};

// 定义共享内存类
class Shm
{
public:
    Shm() : _shmid(-1), _size(gsize), _start_address(nullptr)
    {}

    // void CreateShm()        // 创建共享内存
    // {
    //     key_t key = GetKey();   // 生成 key
    //     if(key < 0)      // 生成键值失败
    //     { 
    //         std::cerr << "GetKey error" << std::endl;
    //         exit(GETKEY_ERROR);
    //     }
    //     printf("key = %d; key = 0x%x\n", key, key);     // 以十六进制的形式打印键值 key
    //     // int shmget(key_t key, size_t size, int shmflg);
    //     _shmid = shmget(key , _size, IPC_CREAT | IPC_EXCL | 0666);  // 设置权限
    //     if(_shmid < 0)      // 创建失败
    //     {
    //         std::cerr << "shmget error" << std::endl;
    //         exit(CREATSHM_ERROR);
    //     }

    //     // 创建共享内存成功
    //     // 创建 shm 成功后，键值是多少；以及 shmget 函数的返回值 _shmid 是多少
    //     printf("key = %d; key = 0x%x\n", key, key);
    //     printf("shmid = %d; shmid = 0x%x\n", _shmid, _shmid);
    // }

    void CreateShm()         // 创建共享内存
    {
        GetHelper(IPC_CREAT | IPC_EXCL | 0666);
    }

    // void GetShm()           // 获取共享内存
    // {
    //     key_t key = GetKey();   // 获取相同的 key
    //     if(key < 0)      // 生成键值失败
    //     { 
    //         std::cerr << "GetKey error" << std::endl;
    //         exit(GETKEY_ERROR);
    //     }
    //     printf("key = %d; key = 0x%x\n", key, key);     // 以十六进制的形式打印键值 key

    //     // int shmget(key_t key, size_t size, int shmflg);
    //     _shmid = shmget(key , _size, IPC_CREAT);
    // }

    void GetShm()           // 获取共享内存
    {
        GetHelper(IPC_CREAT);
    }

    void DeleteShm()        // 删除共享内存
    {
        // int shmctl(int shmid, int op, struct shmid_ds *buf);
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        std:: cout << "delete success" << std::endl;
    }

    void AttachShm()        // 将共享内存挂接到虚拟地址
    {
        // void *shmat(int shmid, const void *_Nullable shmaddr, int shmflg);
        _start_address = shmat(_shmid, nullptr, 0);
        if((long)_start_address == -1)       // 挂接失败,返回值为-1
        { exit(ATTACHSHM_ERROR); }
        std::cout << "shmat success" << std::endl; 
    }

    void DeleteAttachShm()  //共享内存去关联
    {
        // int shmdt(const void *shmaddr);
        int n = shmdt(_start_address);
        std::cout << "shmdt success" << std::endl;
    }

    void *GetShmAddr() { return _start_address; }      // 获取共享内存挂接时的起始虚拟地址

    int GetShmSize() { return _size; }      // 获取共享内存的大小

    ~Shm()
    {}

private:
    key_t GetKey()          // 获取 key
    {
        // 调用系统调用: key_t ftok(const char *pathname, int proj_id);
        return ftok(PATHNAME, PROJ_ID);
    }

    void GetHelper(int shmflg)
    {
        key_t key = GetKey();   // 生成 key
        if(key < 0)      // 生成键值失败
        { 
            std::cerr << "GetKey error" << std::endl;
            exit(GETKEY_ERROR);
        }
        printf("key = %d; key = 0x%x\n", key, key);     // 以十六进制的形式打印键值 key
        // int shmget(key_t key, size_t size, int shmflg);
        _shmid = shmget(key , _size, shmflg);  // 设置权限
        if(_shmid < 0)      // 创建失败
        {
            std::cerr << "shmget error" << std::endl;
            exit(CREATSHM_ERROR);
        }

        // 创建共享内存成功
        // 创建 shm 成功后，键值是多少；以及 shmget 函数的返回值 _shmid 是多少
        printf("key = %d; key = 0x%x\n", key, key);
        printf("shmid = %d; shmid = 0x%x\n", _shmid, _shmid);
    }

private:
    int _shmid;
    int _size;      // 共享内存的大小
    void *_start_address;   // 共享内存的起始虚拟地址
};