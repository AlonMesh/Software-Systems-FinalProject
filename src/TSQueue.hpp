#ifndef TSQUEUE_HPP
#define TSQUEUE_HPP

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>

// Thread-safe queue
template <typename T>
class TSQueue {
private:
    // Underlying queue
    std::queue<T> m_queue;

    // mutex for thread synchronization
    std::mutex m_mutex;

    // Condition variable for signaling
    std::condition_variable m_cond;

public:
    // Pushes an element to the queue
    void push(T item);

    // Pops an element off the queue
    T pop();
};

#endif /* TSQUEUE_HPP */