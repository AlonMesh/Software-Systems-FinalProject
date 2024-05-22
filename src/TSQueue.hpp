#ifndef TSQUEUE_HPP
#define TSQUEUE_HPP

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

/**
 * @brief Thread-safe queue.
 * 
 * This class implements a thread-safe queue. It uses a mutex and a condition variable for synchronization.
 * 
 * @tparam T The type of the elements in the queue.
*/
template <typename T>
class TSQueue {
public:
    /**
     * @brief Pushes an element to the queue.
     * 
     * This function pushes an element to the queue in a thread-safe manner. It locks the mutex,
     * adds the item to the queue, and notifies one waiting thread.
     * 
     * @param item The element to be added to the queue.
     */
    void push(T item) {

        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Add item
        m_queue.push(item);

        // Notify one thread that is waiting
        m_cond.notify_one();
    }

    /**
     * @brief Pops an element off the queue.
     * 
     * This function pops an element off the queue in a thread-safe manner. It waits for the queue to 
     * be non-empty, locks the mutex, retrieves the item, and then pops it from the queue.
     * 
     * @return The element popped off the queue.
     */
    T pop() {
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

    /**
     * @brief Returns the number of elements in the queue.
     * 
     * @return The number of elements in the queue.
     */
    int size(){
        return m_queue.size();
    }

    /**
     * @brief Checks if the queue is empty.
     * 
     * @return True if the queue is empty, false otherwise.
     */
    bool empty() {
        // Acquire lock
        std::unique_lock<std::mutex> lock(m_mutex);

        // Check if queue is empty
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;          // The queue to store elements
    std::mutex m_mutex;             // Mutex for synchronization
    std::condition_variable m_cond; // Condition variable for signaling
};

#endif /* TSQUEUE_HPP */
