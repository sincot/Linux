#include "shm.hpp"

int main()
{
    Shm sharemem_client;
    sharemem_client.GetShm();
    // sleep(5);
    sharemem_client.AttachShm();
    // sleep(5);

    // Client 向共享内存写入数据
    char* shm_start = (char*)sharemem_client.GetShmAddr();  // 获取共享内存的起始虚拟地址
    int shm_size = sharemem_client.GetShmSize();            // 获取共享内存的大小
    int index = 0;
    while(true)
    {
        std::cout << "Please Enter$ ";
        char ch;    // 输入的数据
        std::cin >> ch;
        shm_start[index++] = ch;

        index %= shm_size;  // 避免超出共享内存的大小
    }
    
    sharemem_client.DeleteAttachShm();

    return 0;
}