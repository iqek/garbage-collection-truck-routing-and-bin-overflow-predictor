/**
 * @file PriorityQueue.h
 * @brief Priority queue implemented using a linked list.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

#include "LinkedList.h"

namespace project {

/**
 * @brief Priority queue data structure.
 *
 * Elements are sorted based on their priority value. This structure is essential
 * for implementing shortest path algorithms like Dijkstra's.
 * @tparam T The data type stored.
 */
template <typename T>
class PriorityQueue {
private:
    struct PQNode {
        T data;
        int priority;
    };

    LinkedList<PQNode> list;

public:
    /**
     * @brief Constructs an empty priority queue.
     */
    PriorityQueue();

    /**
     * @brief Inserts an element with a specified priority.
     * @param value The value to insert.
     * @param priority The priority value (lower value usually indicates higher priority).
     * @post New element is inserted into the linked list according to its priority order.
     */
    void push(const T& value, int priority);

    /**
     * @brief Removes the highest priority element.
     * @pre The queue must not be empty.
     * @post The highest priority item (`top()`) is removed from the list.
     */
    void pop();

    /**
     * @brief Returns the highest priority element without removing it.
     * @pre The queue must not be empty.
     * @return Reference to the highest priority element.
     */
    T& top();

    /**
     * @brief Checks if the queue contains any elements.
     * @return `true` if the queue is empty.
     */
    bool isEmpty() const;
};

} // namespace project