#ifndef ACTIVEOBJECT_HPP
#define ACTIVEOBJECT_HPP

#include <functional>
#include <thread>
#include "TSQueue.hpp"

/**
 * @brief Active object that processes items from a thread-safe queue.
 * 
 * This class implements an active object pattern, which processes items from a thread-safe queue
 * in a separate thread. The provided task is executed on each item in the queue.
 * 
 * @tparam T The type of the elements in the queue.
 */
template<typename T>
class ActiveObject {
public:
    /**
     * @brief Constructs an ActiveObject.
     * 
     * This constructor initializes the ActiveObject with a queue and a task. It starts a worker thread 
     * that continuously processes items from the queue.
     * 
     * @param queue The thread-safe queue from which items are processed.
     * @param task The task to be executed on each item from the queue.
     */
    ActiveObject(TSQueue<T>* queue, std::function<void(T&)> task)
            : m_queue(queue), m_task(task) {
        m_thread = std::thread(&ActiveObject::worker, this);
    }

    /**
     * @brief Destructor for ActiveObject.
     * 
     * This destructor stops the worker thread by joining it.
     */
    ~ActiveObject() {
        stop();
    }

    /**
     * @brief Stops the worker thread.
     * 
     * This function stops the worker thread by joining it if it is joinable.
     */
    void stop() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

private:
    /**
     * @brief Worker function that processes items from the queue.
     * 
     * This function runs in a separate thread and continuously processes items from the queue
     * by executing the provided task on each item.
     */
    void worker() {
        while (true) {
            T item = m_queue->pop();
            m_task(item); // Execute the provided task on each item
        }
    }

    TSQueue<T>* m_queue;            // The thread-safe queue from which items are processed
    std::function<void(T&)> m_task; // The task to be executed on each item
    std::thread m_thread;           // The worker thread
};

#endif /* ACTIVEOBJECT_HPP */
