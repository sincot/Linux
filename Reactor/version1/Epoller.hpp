#pragma once

#include "Logger.hpp"
#include "Exitcode.hpp"

#include <iostream>
#include <sys/epoll.h>
#include <cstdlib>

using namespace LOGMOUDLE;

class Epoller
{
private:
    // 用户告诉内核，要关心哪些文件描述符上的事件
    void EpollCtl(int sockfd, int op, uint32_t events)
    {
        // 若 op 是 EPOLL_CTL_DEL，则不需要关心事件是什么
        if(op == EPOLL_CTL_DEL)
        {
            int n = epoll_ctl(_epfd, op, sockfd, nullptr);
        }
        else
        {
            // int epoll_ctl(int epfd, int op, int fd, struct epoll_event event);
            struct epoll_event event;
            event.events = events;
            event.data.fd = sockfd;
            int n = epoll_ctl(_epfd, op, sockfd, &event);
        }
    }

public:
    Epoller()
    {
        // int epoll_create(int __size)
        _epfd = epoll_create(256);
        if(_epfd < 0)
        {
            LOG(LogLevel::FATAL) << "epoll_create error";
            exit(EXIT_CODE::EPOLLER_ERROR);
        }
        LOG(LogLevel::INFO) << "epoll_create success, epfd: " << _epfd;

    }

    // 内核告诉用户，哪些事件就绪了
    int EpollWait(struct epoll_event revents[], int revent_num, int timeout)    // 获取就绪事件
    {
        // int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)
        int n = epoll_wait(_epfd, revents, revent_num, timeout);
        return 0;
    }

    void EpollAddSocket(int sockfd, uint32_t events) { EpollCtl(sockfd, EPOLL_CTL_ADD, events); }



    ~Epoller()
    {
        if(_epfd > 0)   close(_epfd);
    }

private:
    int _epfd;
};