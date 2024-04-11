#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include <functional>
#include <thread>
#include "TSQueue.hpp"

template<typename T>
class ActiveObject {
public:
    ActiveObject(TSQueue<T>* queue, std::function<void(T&)> task);

    ~ActiveObject();

    void stop();

private:
    void worker();

    TSQueue<T>* m_queue;
    std::function<void(T&)> m_task;
    std::thread m_thread;
    bool m_running;
};

#endif /* ACTIVEOBJECT_HPP */
