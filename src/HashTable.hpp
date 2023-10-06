#pragma once

#include "DLinkedList.hpp"

//TODO: constructor, copy/move constructor, copy/move assignment operator, insert, operator[], at(), find, contains, insert, resize
template<class Key, class T, class Hash = std::hash<Key>>
class HashTable {
private:
    using node_type = std::pair<Key, T>;
    DLinkedList<node_type> *buckets_maintainer;
    size_t num_of_buckets;
public:
    explicit HashTable(size_t num_of_buckets = 17) {
        buckets_maintainer = new DLinkedList<node_type>[num_of_buckets];
    }
    ~HashTable(){
        for(int i = 0; i < num_of_buckets; i ++){
            buckets_maintainer[i].
        }

    }
};