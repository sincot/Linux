#include "shm.hpp"

int main()
{
    Shm sharemem_server;
    sharemem_server.CreateShm();
    // sleep(5);
    sharemem_server.AttachShm();
    // sleep(5);

    char* shm_start = (char*)sharemem_server.GetShmAddr();  // 获取共享内存的起始虚拟地址
    int shm_size = sharemem_server.GetShmSize();            // 获取共享内存的大小
    while(true)
    {
        for(int i = 0; i < shm_size; i++)
        {
            std::cout << shm_start[i] << ' ';
        }
        std::cout << std::endl;
        sleep(1);
    }

    sharemem_server.DeleteAttachShm();
    // sleep(1);
    sharemem_server.DeleteShm();

    return 0;
}