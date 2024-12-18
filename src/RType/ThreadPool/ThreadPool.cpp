/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** thread pool
*/

#include <ThreadPool.hpp>

namespace rtype {

    ThreadPool::ThreadPool(int numThreads) {
        for (int i = 0; i < numThreads; i++) {
            _threads.emplace_back(&ThreadPool::workerThread, this);
        }
    }

    ThreadPool::~ThreadPool() {
        _stop = true;
        _condition.notify_all();
        for (auto &thread : _threads) {
            thread.join();
        }
    }

    void ThreadPool::addTask(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _tasks.push(std::move(task));
        }
        _condition.notify_one();
    }

    void ThreadPool::workerThread() {
        while (!_stop) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _condition.wait(lock, [this] { return !_tasks.empty() || _stop; });
                if (_stop && _tasks.empty())
                    return;
                task = _tasks.front();
                _tasks.pop();
            }
            task();
        }
    }
}