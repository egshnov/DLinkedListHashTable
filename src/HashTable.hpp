#pragma once

#include <stdexcept>
#include "DLinkedList.hpp"

//TODO: operator[] - ?
template<class Key, class T, class Hash = std::hash<Key>> //hash полем как функтор?
class HashTable {
private:
    struct table_pair {
        Key key_;
        T val_;

        table_pair(const Key &key = Key(), const T &val = T()) : key_(key), val_(val) {}

        bool operator==(table_pair const &node) {
            return this->key_ == node.key_;
        }

        bool operator!=(table_pair const &node) {
            return this->key_ != node.key_;
        }

        table_pair &operator=(T const &value) {
            this->val_ = value;
            return *this;
        }

    };

    using node_type = table_pair;

    DLinkedList<node_type> *buckets_maintainer;
    size_t num_of_buckets_;
    size_t ind_of_first_not_empty;

    size_t get_ind(Key key) {
        return Hash{}(key) % num_of_buckets_;
    }

public:
    explicit HashTable(const size_t &num_of_buckets = 17) : num_of_buckets_(num_of_buckets),
                                                            ind_of_first_not_empty(num_of_buckets - 1) {
        buckets_maintainer = new DLinkedList<node_type>[num_of_buckets];
    }

    HashTable(HashTable const &table) {
        this->num_of_buckets_ = table.num_of_buckets_;
        this->ind_of_first_not_empty = table.ind_of_first_not_empty;
        buckets_maintainer = new DLinkedList<node_type>[num_of_buckets_];
        for (int i = 0; i < num_of_buckets_; i++) {
            buckets_maintainer[i] = table.buckets_maintainer[i];
        }
    }

    ~HashTable() { // вызывает деструктор для всех прицепленных списков, отрабатывает корректно при корректном деструкторе списка
        delete[] buckets_maintainer;
        // у остального либо тривиальный либо свой и вызовется выше
    }


    HashTable &operator=(HashTable const &table) {
        if (this != &table) {
            HashTable tmp(table);
            std::swap(tmp.num_of_buckets_, this->num_of_buckets_);
            std::swap(tmp.buckets_maintainer, this->buckets_maintainer);
            std::swap(tmp.ind_of_first_not_empty, this->ind_of_first_not_empty);
        }
        return *this;
    }

    HashTable(HashTable &&table) noexcept {
        std::swap(this->buckets_maintainer, table.buckets_maintainer);
        std::swap(this->num_of_buckets_, table.num_of_buckets_);
        std::swap(this->ind_of_first_not_empty, table.ind_of_first_not_empty);
    }

    HashTable &operator=(HashTable &&table) noexcept {
        std::swap(this->buckets_maintainer, table.buckets_maintainer);
        std::swap(this->num_of_buckets_, table.num_of_buckets_);
        std::swap(this->ind_of_first_not_empty, table.ind_of_first_not_empty);
        return *this;
    }

    void insert(const Key &key, const T &value) { // если в таблице уже есть такой ключ то изменяем значение
        size_t ind = get_ind(key);
        if (ind < ind_of_first_not_empty) {
            ind_of_first_not_empty = ind;
        }
        //auto it = std::find(buckets_maintainer[ind].begin(), buckets_maintainer[ind].end(), key);
        auto it = buckets_maintainer[ind].find(key);
        if (it != buckets_maintainer[ind].end()) {
            *it = value;
        } else {
            buckets_maintainer[ind].push_front({key, value});
        }
    }

//    T &operator[](Key key) {
//        size_t ind = get_ind(key);
//        auto it = buckets_maintainer[ind].find(key);
//        if (it != buckets_maintainer[ind].end()) {
//            return (*it).val_;
//        }
//        buckets_maintainer[ind].push_front({key, T()});
//        return (*buckets_maintainer[ind].begin()).val_;
//
//    }

    T at(const Key &key) {
        size_t ind = get_ind(key);
        //auto it = std::find(buckets_maintainer[ind].begin(), buckets_maintainer[ind].end(), key);
        auto it = buckets_maintainer[ind].find(key);
        if (it != buckets_maintainer[ind].end()) {
            return it->val_;
        } else {
            throw std::out_of_range("given key doesn't belong to the hashmap");
        }
    }

    void del(const Key &key) { // если ключа нет ничего не делаем
        size_t ind = get_ind(key);
        auto it = buckets_maintainer[ind].find(key);
        if (it != buckets_maintainer[ind].end()) {
            buckets_maintainer[ind].del(it);
        }
        if (ind_of_first_not_empty == ind && buckets_maintainer[ind].is_empty()) {
            while (ind_of_first_not_empty < this->num_of_buckets_ &&
                   buckets_maintainer[ind_of_first_not_empty].is_empty()) {
                ++ind_of_first_not_empty;
            }
        }
    }

    bool contains(const Key &key) {
        size_t ind = get_ind(key);
        //auto it = std::find(buckets_maintainer[ind].begin(), buckets_maintainer[ind].end(), key);
        auto it = buckets_maintainer[ind].find(key);
        return it != buckets_maintainer[ind].end();
    }

// сделать приватным?
// Чтобы не делать хэшмапу другом двусвязного списка реализован как обёртка вокруг итератора на элемент списка
    struct HashTableIterator {
        friend class HashTable; // чтобы можно было сделать конструктор итератора приватным

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = node_type;
        using pointer = node_type *;
        using reference = node_type &;

        reference operator*() const { return (ptr_.operator*()); };

        pointer operator->() { return (ptr_.operator->()); }


        HashTableIterator &operator++() { //пустые бакеты скипаются
            if (ptr_ == buckets_maintainer[cur_bucket].end()) {
                cur_bucket++;
                while (buckets_maintainer[cur_bucket].is_empty() && cur_bucket < num_of_buckets - 1) {
                    cur_bucket++;
                }
                ptr_ = buckets_maintainer[cur_bucket].begin();
            } else {
                ptr_++;
            }
            return *this;
        };

        HashTableIterator operator++(int) { //postfix
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        HashTableIterator &operator--() {
            if (ptr_ == buckets_maintainer[cur_bucket].begin()) {
                cur_bucket--;
                while (buckets_maintainer[cur_bucket].is_empty() && cur_bucket >= 0) {
                    cur_bucket--;
                }
                ptr_ = (--buckets_maintainer[cur_bucket].end()); // end ?
            } else {
                ptr_--;
            }
            return *this;
        };

        HashTableIterator operator--(int) {
            HashTableIterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(HashTableIterator const &it) {
            return this->ptr_ == it.ptr_;
        }

        bool operator!=(HashTableIterator const &it) {
            return this->ptr_ != it.ptr_;
        }

    private:
        size_t cur_bucket;
        size_t num_of_buckets;
        typename DLinkedList<node_type>::DListIterator ptr_;
        DLinkedList<node_type> *buckets_maintainer;

        HashTableIterator(DLinkedList<node_type> *buckets_maintainer, size_t cur_bucket, size_t num_of_buckets,
                          typename DLinkedList<node_type>::DListIterator ptr) :
                buckets_maintainer(buckets_maintainer), cur_bucket(cur_bucket), num_of_buckets(num_of_buckets),
                ptr_(ptr) {}

    };

    HashTableIterator begin() { // первый элемент первого не пустого бакета
        return HashTableIterator(this->buckets_maintainer, ind_of_first_not_empty, this->num_of_buckets_,
                                 this->buckets_maintainer[ind_of_first_not_empty].begin());
    }

    HashTableIterator end() {
        return HashTableIterator(this->buckets_maintainer, this->num_of_buckets_ - 1, this->num_of_buckets_,
                                 this->buckets_maintainer[num_of_buckets_ - 1].end());
    }

    void resize(size_t new_num_of_buckets) {
        auto *new_buckets_maintainer = new DLinkedList<node_type>[new_num_of_buckets];
        auto it = this->begin();
        while (it != this->end()) {
            size_t ind = Hash{}(it->key_) % new_num_of_buckets;
            new_buckets_maintainer[ind].push_front(*it);
            ++it;
        }
        delete[] this->buckets_maintainer;
        this->num_of_buckets_ = new_num_of_buckets;
        this->buckets_maintainer = new_buckets_maintainer;
    }

};