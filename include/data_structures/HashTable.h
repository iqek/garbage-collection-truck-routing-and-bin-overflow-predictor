/**
 * @file HashTable.h
 * @brief Hash Table with Chaining using pointer-based linked lists.
 * @author İrem Irmak Ünlüer
 * @date 2026-01-10
 */

#pragma once
#include <string>

namespace project {

/**
 * @struct HashNode
 * @brief Node for the chaining linked list.
 */
struct HashNode {
    std::string key;
    int value;
    HashNode* next;

    HashNode();
    HashNode(const std::string& k, int v);
};

/**
 * @class HashTable
 * @brief Hash table with chaining for string-to-int mapping.
 */
class HashTable {
private:
    HashNode** buckets;    ///< Array of pointers to linked list heads
    int capacity;          ///< Number of buckets
    int size;              ///< Number of elements

    /**
     * @brief DJB2 hashing algorithm.
     * @param key String to hash.
     * @return Hash value.
     */
    int hashFunction(const std::string& key) const;

    /** @brief Resizes and rehashes the table. */
    void resize();

public:
    /** @param initialCap Starting number of buckets. */
    HashTable(int initialCap = 101);
    ~HashTable();

    /**
     * @brief Inserts or updates a pair.
     * @param key String identifier.
     * @param value Integer value.
     */
    void insert(const std::string& key, int value);

    /**
     * @brief Searches for a key.
     * @param key String identifier.
     * @return Stored value or -1 if not found.
     */
    int search(const std::string& key) const;

    /** @brief Resets all entries. */
    void clear();

    /** @return Current element count. */
    int getSize() const;
};

} // namespace project
