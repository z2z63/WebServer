#include <iostream>
#include <chrono>
#include <functional>
#include "ThreadPool.h"

void taskFunction(int id) {
    std::cout << "Task " << id << " is being processed by thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(id));
    std::cout << "Task " << id << " completed" << std::endl;
}

int main() {

    ThreadPool<std::function<void(int)>,int> threadPool;


    for (int i = 0; i < 10; ++i) {
        threadPool.submitTask(taskFunction, i);
    }
    return 0;
}
