#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <string>
#include <cmath>



class InterfaceHashFunction {

public:
    virtual ~InterfaceHashFunction() = default;
    virtual size_t hashFunction(int key, size_t tableSize) = 0;

};

class HashFunction1 : public InterfaceHashFunction {

public:
    HashFunction1() {}
    size_t hashFunction(int key, size_t tableSize) override {
        return (static_cast<size_t>(key + 5) % tableSize);
    }

};

class HashFunction2 : public InterfaceHashFunction {

    const double a = (sqrt(5) - 1) / 2;

public:
    HashFunction2() {}
    size_t hashFunction(int key, size_t tableSize) override {
        return (static_cast<size_t>(key * a * tableSize) % tableSize);
    }

};

class HashFunction3 : public InterfaceHashFunction {

public:
    HashFunction3() {}
    size_t hashFunction(int key, size_t tableSize) override {
        return ((key % tableSize + (1 + key % (tableSize - 2))) % tableSize);
    }
};



template <typename T>
class HashTable {
public:
    HashTable();
    HashTable(size_t tableSize, InterfaceHashFunction* currentFunction);
    HashTable(const HashTable<T>& other);
    ~HashTable();

    void setTableSize(size_t tableSize);
    void setHashFunction(InterfaceHashFunction* newHashFunction);
    void setValue(int key, T value);
    size_t getTableSize() const;
    const std::vector<std::list<std::pair<int, T>>>& getHashTable() const;
    InterfaceHashFunction* getCurrentFunction();


    void swap(HashTable<T>& other);
    bool contains(int key);
    void insert(int key, T value);
    void removeByKey(int key);
    void removeByIndex(std::list<std::pair<int, T>>& list, int index);
    int keyIndex(int key);
    void clear();
    const T& operator[](int key) const;
    HashTable<T>& operator=(const HashTable<T>& other);
 

private:
    InterfaceHashFunction* currentFunction = new HashFunction1();
    std::vector<std::list<std::pair<int, T>>> table;
    size_t tableSize = 3;
};

template <typename T>
HashTable<T>::HashTable() : tableSize(3), currentFunction(new HashFunction1()) {
    table.resize(tableSize);
}

template <typename T>
HashTable<T>::HashTable(size_t tableSize, InterfaceHashFunction* currentFunction)
    : tableSize(tableSize), currentFunction(currentFunction) {
    table.resize(tableSize);
}

template <typename T>
HashTable<T>::HashTable(const HashTable& other) : tableSize(other.tableSize), currentFunction(other.currentFunction), table(other.table) {}

template <typename T>
HashTable<T>::~HashTable() {
    delete currentFunction;
    currentFunction = nullptr;
}

template<typename T>
InterfaceHashFunction* HashTable<T>::getCurrentFunction() {
    return currentFunction;
}

template <typename T>
size_t HashTable<T>::getTableSize() const {
    return tableSize;
}

template <typename T>
const std::vector<std::list<std::pair<int, T>>>& HashTable<T>::getHashTable() const {
    return table;
}

template<typename T>
void HashTable<T>::setValue(int key, T value) {
    size_t hash = getCurrentFunction()->hashFunction(key, tableSize);

    auto& list = table[hash];
    for (auto it = list.begin(); it != list.end(); it++) {
        if (it->first == key) {
            it->second = value;
            return;
        }
    }
    list.push_back({key, value});
}

template <typename T>
void HashTable<T>::setHashFunction(InterfaceHashFunction* newFunction) {

    if (newFunction == nullptr)
        return;

    std::vector<std::list<std::pair<int, T>>> newTable(tableSize);
    for (auto it1 = table.begin(); it1 != table.end(); it1++) {
        for (auto it2 = it1->begin(); it2 != it1->end(); it2++) {
            size_t hash = newFunction->hashFunction(it2->first, tableSize);
            newTable[hash].push_back(*it2);
        }
    }
    currentFunction = newFunction;
    table = newTable;
}

template <typename T>
void HashTable<T>::setTableSize(size_t newSize) {
    if (newSize == tableSize)
        return;

    std::vector<std::list<std::pair<int, T>>> newTable(newSize);
    for (auto it1 = table.begin(); it1 != table.end(); it1++) {
        for (auto it2 = it1->begin(); it2 != it1->end(); it2++) {
            size_t hash = currentFunction->hashFunction(it2->first, newSize);
            newTable[hash].push_back(*it2);
        }
    }

    table = newTable;
    tableSize = newSize;
}

template<typename T>
void HashTable<T>::clear() {
    table.clear();
    table.resize(tableSize);
}

template <typename T>
void HashTable<T>::swap(HashTable<T>& other) {
    std::swap(table, other.table);
    std::swap(tableSize, other.tableSize);
    std::swap(currentFunction, other.currentFunction);
}

template <typename T>
bool HashTable<T>::contains(int key) {

    if (currentFunction == nullptr) {
        return false;
    }

    size_t hash = currentFunction->hashFunction(key, tableSize);

    for (auto it = table[hash].begin(); it != table[hash].end(); ++it) {
        if (it->first == key) {
            return true;
        }
    }
    return false;

}

template <typename T>
void HashTable<T>::insert(int key, T value) {
    if (contains(key)) return;

    size_t hash = currentFunction->hashFunction(key, tableSize);
    if (hash >= tableSize) return;

    table[hash].push_back({key, value});

   
}

template <typename T>
int HashTable<T>::keyIndex(int key) {

    size_t hash = currentFunction->hashFunction(key, tableSize);
    int index = 0;

    for (auto it = table[hash].begin(); it != table[hash].end(); it++) {
        if (it->first == key) {
            return index;
        }
        index++;
    }
    return -1;
}


template<typename T>
void HashTable<T>::removeByKey(int key) {
    if (!contains(key))
        return;

    size_t hash = currentFunction->hashFunction(key, tableSize);
    auto& list = table[hash];

    for (auto it = list.begin(); it != list.end();) {
        if (it->first == key) {
            it = list.erase(it);
            return;
        }
        else it++;
    }
}

template<typename T>
void HashTable<T>::removeByIndex(std::list<std::pair<int, T>>& list, int index) {
    if (index < 0) return;
    auto it = list.begin();
    for (int i = 0; i < index && it != list.end(); i++) {
        ++it;
    }
    if (it != list.end()) {
        list.erase(it);
        return;
    }
}

template <typename T>
HashTable<T>& HashTable<T>::operator=(const HashTable<T>& other) {
    if (this == &other)
        return *this;

    HashTable<T> temp(other);
    swap(temp);

    return (*this);
}

template <typename T>
const T& HashTable<T>::operator[](int key) const {
    size_t hash = currentFunction->hashFunction(key, tableSize);

    for (auto it = table[hash].begin(); it != table[hash].end(); it++) {
        if (it->first == key) {
            return it->second;
        }
    }

    std::cerr << "Key not found" << std::endl;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, HashTable<T>& table){
    for (size_t i = 0; i < table.getTableSize(); i++) {
        os << "hash = " << i << ":\t";

        const auto& list = table.getHashTable()[i];
        for (auto it = list.begin(); it != list.end(); it++) {
            os << "key = " << it->first << ", value = " << it->second << " -----> ";
        }
        os << "NULL" << std::endl;
    }
    return os;
}