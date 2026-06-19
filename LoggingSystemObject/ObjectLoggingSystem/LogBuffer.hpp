/* 实现异步日志缓冲区 */


#ifndef MY_LOGBUFFER_H
#define MY_LOGBUFFER_H

#include <vector>
#include <iostream>

namespace Log
{
    #define DEFAULT_BUFFER_SIZE (1 * 1024 * 1024)   // 1MB
    #define RESERVE_THRESHOLD_BUFFER_SIZE (8 * 1024 * 1024)  // 8MB     buffer 空间的阈值
    #define INCREATE_BUFFER_SIZE (1 * 1024 * 1024)   // 达到阈值后，每次扩容的大小

    class LogBuffer
    {
    public:
        LogBuffer(): _buffer(DEFAULT_BUFFER_SIZE), _read_idx(0), _write_idx(0) { }

        void Push(const char* data, size_t len)    // 向缓冲区写入数据
        {
            // 缓冲区空间不足够的情况：1. 扩容；2. 阻塞/返回false
            // 这里只考虑空空间不够就扩容，空间不够怎么样交给 LogLooper 模块决定
            // 情况2. 动态空间，用于性能极限测试
            Reserve(len);

            // 1. 将数据拷贝进缓冲区
            std::copy(data, data + len, &_buffer[_write_idx]);      // 将 len 长度的数据写入到缓冲区中

            // 2. 将当前写入位置向后偏移
            MoveWriter(len);
        }

        // 对于扩容思路来说，不存在可写空间大小，因为总是可写，因此这个接口仅仅针对提供固定大小缓冲区的接口
        size_t WriteAbleLength()     // 返回可写数据的长度
        {
            return (_buffer.size() - _write_idx);
        }

        const char* begin()         // 返回可读数据的起始地址
        {
            return &_buffer[_read_idx];
        }

        size_t ReadAbleLength()       // 返回可读数据的长度
        {
            // 当前实现的缓冲区并不是循环缓冲区，只会向后写，处理完毕就交换，不存在空间的循环使用
            return (_write_idx - _read_idx);
        }

        void MoveReader(size_t len)           // 对读指针进行向后偏移操作
        {
            assert(len <= ReadAbleLength());     // 读指针向后移动的大小要小于等于可读数据的长度，不然会越界
            _read_idx += len;
        }

        void ReSet()                // 重置读写位置，初始化缓冲区
        {
            _read_idx = _write_idx = 0;     // 缓冲区的所有空间都是空闲的，没有数据可读
        }

        void Swap(LogBuffer &buffer)                 // 对 buffer 实现交换操作
        {
            _buffer.swap(buffer._buffer);
            std::swap(_read_idx, buffer._read_idx);
            std::swap(_write_idx, buffer._write_idx);
        }

        bool Empty()                // 判断缓冲区是否为空
        {
            return (_read_idx == _write_idx);   // 读位置与写位置相等，说明缓冲区为空
        }

    private:
        void MoveWriter(size_t len)           // 对写指针进行向后偏移操作
        {
            assert(_write_idx + len <= _buffer.size());     // 写指针向后移动 len 后，不能超过缓冲区的大小
            _write_idx += len;
        }

        void Reserve(size_t len)              // 扩容
        {
            if(len < WriteAbleLength()) { return; }     // 不需要扩容
            size_t new_size = 0;
            while(new_size < len)   // 避免 buffer 的大小翻倍之后还是小于 len 的情况
            {
                if(_buffer.size() < RESERVE_THRESHOLD_BUFFER_SIZE)  // 需要扩容，小于阈值
                {
                    new_size = _buffer.size() * 2;      // 小于阈值则翻倍增长
                }
                else        // 需要扩容，大于阈值
                {
                    new_size = _buffer.size() + INCREATE_BUFFER_SIZE;    // 大于阈值着线性增长
                }
            }

            _buffer.resize(new_size); 
        }
    private:
        std::vector<char> _buffer;
        size_t _read_idx;               // 当前可读数据的指针 —— 本质是下标
        size_t _write_idx;              // 当前可写数据的指针
    };
}

#endif