#include "include/server.hpp"
#include "include/tpool.hpp"

ThreadPool::ThreadPool(int32_t worker_capacity = CLIENTS_MAX_CAPACITY)

{
    m_Workers.emplace_back([this] {
        while (true)
        {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(m_Mutex);

                m_CondVar.wait(lock, [this] {
                    return !m_Tasks.empty() || m_Stop;
                });

                if (m_Stop && m_Tasks.empty())
                    return;

                task = std::move(m_Tasks.front());
                m_Tasks.pop();
            }
            task();
        }
    });
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Stop = true;
    }

    m_CondVar.notify_all();

    for (auto &thread: m_Workers)
        thread.join();
}

void ThreadPool::add_task(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Tasks.emplace(std::move(task));
    }
    m_CondVar.notify_one();
}

int32_t ThreadPool::active_tasks_count() const
{
    return m_Tasks.size();
}