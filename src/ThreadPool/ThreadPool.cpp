/*
** EPITECH PROJECT, 2024
** Rtype
** File description:
** thread pool
*/

#include "./ThreadPool.hpp"

namespace rtype {

    ThreadPool::ThreadPool(int numThreads) {
        for (int i = 0; i < numThreads; i++) {
            _threads.emplace_back(&ThreadPool::workerThread, this);
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_stop == true) {
                return;
            }
            _stop = true;
        }

        _condition.notify_all();

        waitForTasksToComplete();
    }

    void ThreadPool::stop() {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_stop == true) {
                return;
            }
            _stop = true;
        }

        _condition.notify_all();

        for (auto &thread : _threads) {
            if (thread.joinable()) {
                thread.join();
            }
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
                task = std::move(_tasks.front());
                _tasks.pop();
            }
            task();
        }
    }

    void ThreadPool::waitForTasksToComplete() {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition.wait(lock, [this] { return _tasks.empty(); });
        }

        for (auto &thread : _threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

}