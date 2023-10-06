#pragma once

#include "DLinkedList.hpp"

//TODO: constructor, copy/move constructor, copy/move assignment operator, insert, operator[], at(), find, contains, insert, resize
template<class Key, class T, class Hash = std::hash<Key>>
class HashTable {
private:
    DLinkedList<std::pair<Key, T>> *buckets_maintainer;
    size_t num_of_buckets;
public:
    HashTable(size_t num_of_buckets = 17){
        buckets_maintainer
    }
};