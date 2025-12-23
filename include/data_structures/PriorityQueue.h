/**
 * @file PriorityQueue.h
 * @brief Priority queue implemented using a linked list.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
 */

#pragma once

#include "LinkedList.h"

namespace project {

/**
 * @brief Priority queue data structure.
 *
 * Elements are sorted based on their priority value. This structure is essential
 * for implementing shortest path algorithms like Dijkstra's.
 * Lower priority value = higher priority (processed first).
 * @tparam T The data type stored.
 */
template <typename T>
class PriorityQueue {
private:
    struct PQNode {
        T data;
        int priority;
        
        PQNode() : data(T()), priority(0) {}
        PQNode(const T& d, int p) : data(d), priority(p) {}
    };

    LinkedList<PQNode> list;

public:
    /**
     * @brief Constructs an empty priority queue.
     */
    PriorityQueue() {}

    /**
     * @brief Inserts an element with a specified priority.
     * 
     * Elements are inserted in sorted order by priority (ascending).
     * @param value The value to insert.
     * @param priority The priority value (lower value = higher priority).
     * @post New element is inserted into the linked list in priority order.
     */
    void push(const T& value, int priority) {
        PQNode newNode(value, priority);
        
        if (list.isEmpty() || list.front().priority > priority) {
            // Create temporary list to rebuild with new front
            LinkedList<PQNode> tempList;
            tempList.pushBack(newNode);
            while (!list.isEmpty()) {
                tempList.pushBack(list.front());
                list.popFront();
            }
            list = tempList;
        } else {
            // Need to insert in middle or end
            LinkedList<PQNode> tempList;
            bool inserted = false;
            
            while (!list.isEmpty()) {
                if (!inserted && list.front().priority > priority) {
                    tempList.pushBack(newNode);
                    inserted = true;
                }
                tempList.pushBack(list.front());
                list.popFront();
            }
            
            if (!inserted) {
                tempList.pushBack(newNode);
            }
            
            list = tempList;
        }
    }

    /**
     * @brief Removes the highest priority element.
     * @pre The queue must not be empty.
     * @post The highest priority item (front) is removed from the list.
     */
    void pop() {
        if (!isEmpty()) {
            list.popFront();
        }
    }

    /**
     * @brief Returns the highest priority element without removing it.
     * @pre The queue must not be empty.
     * @return Reference to the highest priority element.
     */
    T& top() {
        return list.front().data;
    }

    /**
     * @brief Returns the highest priority element (const version).
     * @pre The queue must not be empty.
     * @return Const reference to the highest priority element.
     */
    const T& top() const {
        return list.front().data;
    }

    /**
     * @brief Checks if the queue contains any elements.
     * @return `true` if the queue is empty.
     */
    bool isEmpty() const {
        return list.isEmpty();
    }

    /**
     * @brief Gets the number of elements in the queue.
     * @return Size of the queue.
     */
    int size() const {
        return list.size();
    }
};

} // namespace project