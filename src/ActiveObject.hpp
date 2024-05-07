#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include <functional>
#include <thread>
#include "TSQueue.hpp"

template<typename T>
class ActiveObject {
public:
    ActiveObject(TSQueue<T>* queue, std::function<void(T&)> task)
            : m_queue(queue), m_task(task), m_running(false) {
        m_thread = std::thread(&ActiveObject::worker, this);
    }

    ~ActiveObject() {
        stop();
    }

    void stop() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

private:
    void worker() {
        while (true) {
            if(!m_queue->empty()){
                T item = m_queue->pop();
                m_task(item); // Execute the provided task on each item
            }
        }
    }

    TSQueue<T>* m_queue;
    std::function<void(T&)> m_task;
    std::thread m_thread;
    bool m_running;
};

#endif /* ACTIVEOBJECT_HPP */
