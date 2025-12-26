/**
 * @file HashTable.h
 * @brief Hash Table with Open Addressing (Linear Probing).
 * @author Miray Duygulu, Kerem Akdeniz, İlber Eren Tüt, İrem Irmak Ünlüer, İpek Çelik
 * @date 2025-12-26
 */

#pragma once
#include <string>

namespace project {

/**
 * @struct HashEntry
 * @brief Slot for the hash table.
 */
struct HashEntry {
    std::string key;
    int value;
    bool isOccupied;

    HashEntry();
};

/**
 * @class HashTable
 * @brief Linear Probing hash table for string-to-int mapping.
 */
class HashTable {
private:
    HashEntry* table;
    int capacity;
    int size;

    /**
     * @brief DJB2 hashing algorithm.
     * @param key String to hash.
     * @return Hash value.
     */
    int hashFunction(const std::string& key) const;

    /** @brief Resizes and rehashes the table. */
    void resize();

public:
    /** @param initialCap Starting array size. */
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