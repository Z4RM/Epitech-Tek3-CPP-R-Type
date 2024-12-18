/*
** EPITECH PROJECT, 2024
** plazza
** File description:
** thread pool
*/

#pragma once

#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <atomic>

namespace rtype {
    class ThreadPool {
    public:
        explicit ThreadPool(int numThreads);

        ~ThreadPool();

        /**
        * @brief Add a task to the thread pool
        * @param task the task to be added
        **/
        void addTask(std::function<void()> task);

        /**
        * @brief Worker thread function
        * @details This function is called by the worker threads to get tasks from the queue
        **/
        void workerThread();

    private:
        std::vector<std::thread> _threads; ///< Vector of worker threads
        std::queue<std::function<void()>> _tasks; ///< Queue of tasks
        std::mutex _mutex; ///< Mutex to protect the queue
        std::condition_variable _condition; ///< Condition variable to notify the worker threads
        std::atomic<bool> _stop = false; ///< Atomic boolean to stop the worker threads
    };
}