#include "include/server.hpp"
#include "include/tpool.hpp"
#include "include/defs.hpp"
#include "include/logger.hpp"
#include <format>

static conf::config_opts CONFIG_OPTS = defaults::CONFIG.get_config_opts();

tpool::thread_pool::thread_pool(int32_t worker_capacity)
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

    LOG_INFO(CONFIG_OPTS, std::format("Initialized Thread Pool with {} client capacity", worker_capacity));
}

tpool::thread_pool::~thread_pool()
{
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Stop = true;
    }

    m_CondVar.notify_all();

    for (auto &thread: m_Workers)
        thread.join();

    LOG_INFO(CONFIG_OPTS, "Thread Pool deinitialized");
}

void tpool::thread_pool::add_task(std::function<void()> task)
{
    {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Tasks.emplace(std::move(task));
    }
    m_CondVar.notify_one();
}

int32_t tpool::thread_pool::active_tasks_count() const
{
    return m_Tasks.size();
}