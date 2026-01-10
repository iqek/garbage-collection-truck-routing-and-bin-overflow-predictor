/**
 * @file PriorityQueue.h
 * @brief Priority queue implemented using a node-based binary heap.
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#pragma once

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
    struct HeapNode {
        T data;
        int priority;
        HeapNode* parent;
        HeapNode* left;
        HeapNode* right;
        
        HeapNode(const T& d, int p) 
            : data(d), priority(p), parent(nullptr), left(nullptr), right(nullptr) {}
    };

    HeapNode* root;
    int heapSize;

    /**
     * @brief Finds the last node in level-order traversal for insertion.
     */
    HeapNode* findLastNode() const {
        if (root == nullptr) return nullptr;
        
        // Use level-order traversal to find insertion point
        HeapNode** queue = new HeapNode*[heapSize];
        int front = 0, rear = 0;
        
        queue[rear++] = root;
        HeapNode* lastNode = nullptr;
        
        while (front < rear) {
            lastNode = queue[front++];
            
            if (lastNode->left != nullptr) {
                queue[rear++] = lastNode->left;
            }
            if (lastNode->right != nullptr) {
                queue[rear++] = lastNode->right;
            }
        }
        
        delete[] queue;
        return lastNode;
    }

    /**
     * @brief Finds the insertion parent for the next node.
     */
    HeapNode* findInsertionParent() const {
        if (root == nullptr) return nullptr;
        
        HeapNode** queue = new HeapNode*[heapSize + 1];
        int front = 0, rear = 0;
        
        queue[rear++] = root;
        HeapNode* insertParent = nullptr;
        
        while (front < rear) {
            HeapNode* current = queue[front++];
            
            if (current->left == nullptr || current->right == nullptr) {
                insertParent = current;
                break;
            }
            
            queue[rear++] = current->left;
            queue[rear++] = current->right;
        }
        
        delete[] queue;
        return insertParent;
    }

    /**
     * @brief Swaps data between two nodes.
     */
    void swapNodes(HeapNode* a, HeapNode* b) {
        T tempData = a->data;
        int tempPriority = a->priority;
        
        a->data = b->data;
        a->priority = b->priority;
        
        b->data = tempData;
        b->priority = tempPriority;
    }

    /**
     * @brief Maintains min-heap property by moving element up.
     */
    void heapifyUp(HeapNode* node) {
        while (node->parent != nullptr && node->priority < node->parent->priority) {
            swapNodes(node, node->parent);
            node = node->parent;
        }
    }

    /**
     * @brief Maintains min-heap property by moving element down.
     */
    void heapifyDown(HeapNode* node) {
        while (true) {
            HeapNode* smallest = node;
            
            if (node->left != nullptr && node->left->priority < smallest->priority) {
                smallest = node->left;
            }
            
            if (node->right != nullptr && node->right->priority < smallest->priority) {
                smallest = node->right;
            }
            
            if (smallest == node) break;
            
            swapNodes(node, smallest);
            node = smallest;
        }
    }

    /**
     * @brief Recursively copies a subtree.
     */
    HeapNode* copyTree(HeapNode* node, HeapNode* parent) {
        if (node == nullptr) return nullptr;
        
        HeapNode* newNode = new HeapNode(node->data, node->priority);
        newNode->parent = parent;
        newNode->left = copyTree(node->left, newNode);
        newNode->right = copyTree(node->right, newNode);
        
        return newNode;
    }

    /**
     * @brief Recursively deletes a subtree.
     */
    void deleteTree(HeapNode* node) {
        if (node == nullptr) return;
        
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    /**
     * @brief Constructs an empty priority queue.
     */
    PriorityQueue() : root(nullptr), heapSize(0) {}

    /**
     * @brief Destructor to free allocated memory.
     */
    ~PriorityQueue() {
        deleteTree(root);
    }

    /**
     * @brief Copy constructor.
     */
    PriorityQueue(const PriorityQueue& other) : root(nullptr), heapSize(other.heapSize) {
        if (other.root != nullptr) {
            root = copyTree(other.root, nullptr);
        }
    }

    /**
     * @brief Assignment operator.
     */
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            deleteTree(root);
            root = nullptr;
            heapSize = other.heapSize;
            
            if (other.root != nullptr) {
                root = copyTree(other.root, nullptr);
            }
        }
        return *this;
    }

    /**
     * @brief Inserts an element with a specified priority.
     * 
     * Elements are inserted in sorted order by priority (ascending).
     * @param value The value to insert.
     * @param priority The priority value (lower value = higher priority).
     * @post New element is inserted into the heap in priority order.
     */
    void push(const T& value, int priority) {
        HeapNode* newNode = new HeapNode(value, priority);
        
        if (root == nullptr) {
            root = newNode;
            heapSize = 1;
            return;
        }
        
        HeapNode* parent = findInsertionParent();
        newNode->parent = parent;
        
        if (parent->left == nullptr) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        
        heapSize++;
        heapifyUp(newNode);
    }

    /**
     * @brief Removes the highest priority element.
     * @pre The queue must not be empty.
     * @post The highest priority item (root) is removed from the heap.
     */
    void pop() {
        if (root == nullptr) return;
        
        if (heapSize == 1) {
            delete root;
            root = nullptr;
            heapSize = 0;
            return;
        }
        
        HeapNode* lastNode = findLastNode();
        
        // Swap root with last node
        root->data = lastNode->data;
        root->priority = lastNode->priority;
        
        // Remove last node
        if (lastNode->parent->left == lastNode) {
            lastNode->parent->left = nullptr;
        } else {
            lastNode->parent->right = nullptr;
        }
        
        delete lastNode;
        heapSize--;
        
        if (root != nullptr) {
            heapifyDown(root);
        }
    }

    /**
     * @brief Returns the highest priority element without removing it.
     * @pre The queue must not be empty.
     * @return Reference to the highest priority element.
     */
    T& top() {
        return root->data;
    }

    /**
     * @brief Returns the highest priority element (const version).
     * @pre The queue must not be empty.
     * @return Const reference to the highest priority element.
     */
    const T& top() const {
        return root->data;
    }

    /**
     * @brief Checks if the queue contains any elements.
     * @return `true` if the queue is empty.
     */
    bool isEmpty() const {
        return root == nullptr;
    }

    /**
     * @brief Gets the number of elements in the queue.
     * @return Size of the queue.
     */
    int size() const {
        return heapSize;
    }
};

} // namespace project
