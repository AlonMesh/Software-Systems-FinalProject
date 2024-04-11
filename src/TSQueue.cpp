#include "TSQueue.hpp"

// Pushes an element to the queue
template <typename T>
void TSQueue<T>::push(T item) {
    // Acquire lock
    std::unique_lock<std::mutex> lock(m_mutex);

    // Add item
    m_queue.push(item);

    // Notify one thread that is waiting
    m_cond.notify_one();
}

// Pops an element off the queue
template <typename T>
T TSQueue<T>::pop() {
    // Acquire lock
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait until queue is not empty
    m_cond.wait(lock, [this]() { return !m_queue.empty(); });

    // Retrieve item
    T item = m_queue.front();
    m_queue.pop();

    // Return item
    return item;
}
