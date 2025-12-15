/**
 * @file LinkedList.h
 * @brief Implementation of a singly linked list.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-15
 */

#pragma once

namespace project {

/**
 * @brief Generic singly linked list structure.
 *
 * This structure serves as the fundamental data type for implementing
 * adjacency lists in the Graph and the custom Priority Queue.
 * @tparam T The data type stored in the list.
 */
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;

public:
    /**
     * @brief Constructs an empty linked list.
     */
    LinkedList();

    /**
     * @brief Destroys the linked list and frees all allocated nodes.
     * @post All dynamically allocated memory for nodes is released.
     */
    ~LinkedList();

    /**
     * @brief Inserts an element at the end of the list.
     * @param value The value to insert.
     * @post The size of the list increases by one, and `value` becomes the new tail.
     */
    void pushBack(const T& value);

    /**
     * @brief Removes the first element from the list.
     * @pre The list must not be empty (`isEmpty()` is false).
     * @post The first node is removed, and its memory is freed.
     */
    void popFront();

    /**
     * @brief Returns a reference to the first element.
     * @pre The list must not be empty (`isEmpty()` is false).
     * @return Reference to the front element of the list.
     */
    T& front();

    /**
     * @brief Checks if the list is empty.
     * @return `true` if the list contains no elements, otherwise `false`.
     */
    bool isEmpty() const;
};

} // namespace project