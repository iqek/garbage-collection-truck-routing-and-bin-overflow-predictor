/**
 * @file LinkedList.h
 * @brief Implementation of a singly linked list.
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-23
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
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int count;

public:
    /**
     * @brief Constructs an empty linked list.
     */
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    /**
     * @brief Destroys the linked list and frees all allocated nodes.
     * @post All dynamically allocated memory for nodes is released.
     */
    ~LinkedList() {
        while (!isEmpty()) {
            popFront();
        }
    }

    /**
     * @brief Copy constructor.
     */
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* current = other.head;
        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    }

    /**
     * @brief Assignment operator.
     */
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            while (!isEmpty()) {
                popFront();
            }
            Node* current = other.head;
            while (current != nullptr) {
                pushBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    /**
     * @brief Inserts an element at the end of the list.
     * @param value The value to insert.
     * @post The size of the list increases by one, and `value` becomes the new tail.
     */
    void pushBack(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    /**
     * @brief Removes the first element from the list.
     * @pre The list must not be empty (`isEmpty()` is false).
     * @post The first node is removed, and its memory is freed.
     */
    void popFront() {
        if (isEmpty())
            return;
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
        count--;
    }

    /**
     * @brief Returns a reference to the first element.
     * @pre The list must not be empty (`isEmpty()` is false).
     * @return Reference to the front element of the list.
     */
    T& front() { return head->data; }

    /**
     * @brief Returns a const reference to the first element.
     * @pre The list must not be empty.
     * @return Const reference to the front element.
     */
    const T& front() const { return head->data; }

    /**
     * @brief Checks if the list is empty.
     * @return `true` if the list contains no elements, otherwise `false`.
     */
    bool isEmpty() const { return head == nullptr; }

    /**
     * @brief Gets the number of elements in the list.
     * @return Size of the list.
     */
    int size() const { return count; }

    /**
     * @brief Iterator support for range-based for loops.
     */
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        const T& operator*() const { return current->data; }
        Iterator& operator++() {
            current = current->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

}  // namespace project
