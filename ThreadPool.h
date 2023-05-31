#ifndef WEBSERVER_THREADPOOL_H
#define WEBSERVER_THREADPOOL_H


#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

template<typename T, typename ... Args>
class ThreadPool {
private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks{};
    std::mutex mutex;
    bool exit;
    std::condition_variable cv;

public:
    explicit ThreadPool(int threadNum = 8);

    ~ThreadPool();

    void submitTask(const T &task, Args... args);

private:
    void Thread();
};

template<typename T, typename ... Args>
ThreadPool<T, Args...>::ThreadPool(int threadNum) {
    exit = false;
    for (int i = 0; i < threadNum; i++) {
        workers.emplace_back([this]() { Thread(); });
    }
}


template<typename T, typename ... Args>
ThreadPool<T, Args...>::~ThreadPool() {
    exit = true;
    cv.notify_all();
    for (auto &worker: workers) {
        worker.join();
    }
}

template<typename T, typename ... Args>
void ThreadPool<T, Args...>::submitTask(const T &task, Args... args) {
    std::lock_guard lockGuard(mutex);
//    U boundFunction = std::bind(task, args...);
    std::function<void()> boundFunction = [task, args...] { task(args...); };
//    U boundFunction = [task, args...] { task(args...); };
    tasks.push(boundFunction);
    cv.notify_one();
}

template<typename T, typename ... Args>
void ThreadPool<T, Args...>::Thread() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock lock(mutex);
            cv.wait(lock, [this] { return not tasks.empty() || exit; });
            if (tasks.empty() and exit) {
                return;
            }
            task = tasks.front();
            tasks.pop();
        }
        task();
    }
}

#endif //WEBSERVER_THREADPOOL_H