#ifndef TPOOL_IMPL_H
#define TPOOL_IMPL_H

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "server.hpp"

using boost::asio::ip::tcp;

class ThreadPool
{
public:
    ThreadPool(int32_t worker_capacity);
    ~ThreadPool();
    void add_task(std::function<void()> task);
    int32_t active_tasks_count() const;
private:
    std::vector<std::thread> m_Workers;
    std::queue<std::function<void()>> m_Tasks;
    std::mutex m_Mutex;
    std::condition_variable m_CondVar;
    bool m_Stop = false;
};

#endif