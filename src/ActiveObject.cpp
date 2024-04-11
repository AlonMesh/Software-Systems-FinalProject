#include "ActiveObject.hpp"

template<typename T>
ActiveObject<T>::ActiveObject(TSQueue<T>* queue, std::function<void(T&)> task)
        : m_queue(queue), m_task(task), m_running(false) {
    m_thread = std::thread(&ActiveObject::worker, this);
}

template<typename T>
ActiveObject<T>::~ActiveObject() {
    stop();
}

template<typename T>
void ActiveObject<T>::stop() {
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

template<typename T>
void ActiveObject<T>::worker() {
    while (true) {
        T item = m_queue->pop();
        m_task(item); // Execute the provided task on each item
    }
}
