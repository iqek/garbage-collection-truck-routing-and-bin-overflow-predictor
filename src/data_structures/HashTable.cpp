/**
 * @file HashTable.cpp
 * @brief Implementation of HashTable with chaining.
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#include "data_structures/HashTable.h"

namespace project {

// HashNode constructors
HashNode::HashNode() : key(""), value(-1), next(nullptr) {}

HashNode::HashNode(const std::string& k, int v) : key(k), value(v), next(nullptr) {}

// HashTable constructor
HashTable::HashTable(int initialCap) : capacity(initialCap), size(0) {
    buckets = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;  // initialize all buckets to null
    }
}

// Destructor
HashTable::~HashTable() {
    clear();
    delete[] buckets;
}

// DJB2 hashing algorithm
int HashTable::hashFunction(const std::string& key) const {
    unsigned long hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }
    return hash % capacity;
}

// Resizes and rehashes the table
void HashTable::resize() {
    int oldCapacity = capacity;
    HashNode** oldBuckets = buckets;

    capacity = capacity * 2 + 1;  // new capacity
    buckets = new HashNode*[capacity];

    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;  // initialize new buckets
    }

    size = 0;  // reset size, will be recounted during rehash

    // Rehash all elements
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = oldBuckets[i];
        while (current != nullptr) {
            insert(current->key, current->value);  // reinsert into new table
            HashNode* temp = current;
            current = current->next;
            delete temp;  // free old node
        }
    }

    delete[] oldBuckets;  // free old bucket array
}

// Inserts or updates a pair
void HashTable::insert(const std::string& key, int value) {
    // Check load factor and resize if needed
    if (size >= capacity * 0.7) {
        resize();
    }

    int index = hashFunction(key);
    HashNode* current = buckets[index];

    // Check if key already exists
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;  // update existing key
            return;
        }
        current = current->next;
    }

    // Insert new node at front of chain
    HashNode* newNode = new HashNode(key, value);
    newNode->next = buckets[index];
    buckets[index] = newNode;
    size++;
}

// Searches for a key
int HashTable::search(const std::string& key) const {
    int index = hashFunction(key);
    HashNode* current = buckets[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;  // key found
        }
        current = current->next;
    }

    return -1;  // key not found
}

// Resets all entries
void HashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = buckets[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;  // free each node in chain
        }
        buckets[i] = nullptr;  // reset bucket head
    }
    size = 0;
}

// Current element count
int HashTable::getSize() const {
    return size;
}

}  // namespace project
