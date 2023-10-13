#pragma once

#include <iterator>
#include <iostream>
#include <gtest/gtest.h>

//template<class Key, class T, class Hash> class HashTable;
//TODO: Подумать есть ли смысл сделать отдельный тип для ноды без значения и отнаследовать ноду списка от неё, сделать sentinel типа базового класса
//


template<class T> // head и tail - sentinel nodes
class DLinkedList {

private:
    template<class Type>
    class DNode {
    public:
        Type value_;
        DNode *next_;
        DNode *prev_;

        explicit DNode(Type const &value = Type(), DNode *prev = nullptr, DNode *next = nullptr) :
                value_(value), prev_(prev), next_(next) {}

        Type GetValue() {
            return this->value_;
        };
    };

    DNode<T> *head; // перед первой нодой
    DNode<T> *tail; // после последней
public:
    DLinkedList() {
        head = new DNode<T>();
        tail = new DNode<T>();
        head->next_ = tail;
        tail->prev_ = head;
    }

    void push_back(const T &value) {
        auto *target = new DNode<T>(value);
        target->prev_ = tail->prev_;
        target->next_ = tail;
        tail->prev_->next_ = target;
        tail->prev_ = target;
    }

    void push_front(const T &value) {
        auto *target = new DNode<T>(value);
        target->prev_ = head;
        target->next_ = head->next_;
        head->next_->prev_ = target;
        head->next_ = target;
    }

    DLinkedList(DLinkedList const &list) {
        this->head = new DNode<T>();
        this->tail = new DNode<T>();
        head->next_ = tail;
        tail->prev_ = head;
        auto it = list.head->next_;
        while (it != list.tail) {
            this->push_back(it->value_);
            it = it->next_;
        }
    }

    ~DLinkedList() {
        auto it = this->head;
        while (it != this->tail) {
            auto tmp = it;
            it = it->next_;
            delete tmp;
        }
        delete it;
    }

    DLinkedList &operator=(DLinkedList const &list) {
        if (this != &list) {
            DLinkedList tmp(list);
            std::swap(tmp.head, this->head);
            std::swap(tmp.tail, this->tail);
        }
        return *this;
    }

    DLinkedList(DLinkedList &&list) noexcept {
        head = new DNode<T>(); // т.к. надо оставить переданный объект в согласованном состоянии
        tail = new DNode<T>();
        head->next_ = tail;
        tail->prev_ = head;
        std::swap(this->head, list.head);
        std::swap(this->tail, list.tail);
    }

    DLinkedList &operator=(DLinkedList &&list) noexcept {
        std::swap(this->head, list.head);
        std::swap(this->tail, list.tail);
        return *this;
    }


    struct DListIterator {
        friend class DLinkedList;

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
//        using value_type = DNode<T>;
//        using pointer = DNode<T> *;
//        using reference = DNode<T> &;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        reference operator*() const { return (ptr_->value_); };

        pointer operator->() { return &(ptr_->value_); }

        DListIterator(DNode<T> *ptr) : ptr_(ptr) {} // private ?

        DListIterator &operator++() { //prefix
            ptr_ = ptr_->next_;
            return *this;
        };

        DListIterator operator++(int) { //postfix
            DListIterator tmp = *this;
            ++(*this);
            return tmp;
        }

        DListIterator &operator--() {
            ptr_ = ptr_->prev_;
            return *this;
        };

        DListIterator operator--(int) {
            DListIterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(DListIterator const &it) {
            return this->ptr_ == it.ptr_;
        }

        bool operator!=(DListIterator const &it) {
            return this->ptr_ != it.ptr_;
        }

    private:
        DNode<T> *ptr_;
    };

    DListIterator begin() const {
        return DListIterator(head->next_);
    }

    DListIterator end() const {
        return DListIterator(tail);
    }

    DNode<T> *insert_after_pointer(T const &value, DNode<T> *prev_node) {
        auto target = new DNode<T>(value, prev_node, prev_node->next_);
        prev_node->next_->prev_ = target;
        prev_node->next_ = target;
        return target;
    }

    DListIterator insert_after(T const &value, DListIterator const &it) {
        return DListIterator(insert_after_pointer(value, it.ptr_));
    }

    DListIterator insert_after(T const &value, size_t ind) {
        auto it = this->begin();
        for (size_t i = 0; i < ind; i++) {
            it++;
        }
        return insert_after(value, it);
    }

    void del_by_pointer(DNode<T> *target) {
        target->next_->prev_ = target->prev_;
        target->prev_->next_ = target->next_;
        delete target;
    }

    void del(DListIterator const &it) {
        del_by_pointer(it.ptr_);
    }

    void del(size_t ind) {
        auto it = this->begin();
        for (size_t i = 0; i < ind; i++) {
            it++;
        }
        del(it);
    }

    bool is_empty() {
        return this->head->next_ == this->tail;
    }

    DListIterator find(const T &value) {
        // return std::find(this->begin(),this->end(),value);
        auto it = this->begin();
        while (it != this->end()) {
            if (*it == value) break;
            ++it;
        }
        return it;
    }
//    std::string to_string() {
//        std::string s = "";
//        for (auto it = this->begin(); it != this->end(); it++) {
//            s.append(std::to_string(*it) + " ");
//        }
//        return s;
//    }
};

template<class T>
bool operator==(DLinkedList<T> const &l1, DLinkedList<T> const &l2) {
    auto it1 = l1.begin();
    auto it2 = l2.begin();

    while (it1 != l1.end() && it2 != l2.end()) {
        if (*it1 != *it2) {
            return false;
        }
        it1++;
        it2++;
    }
    return it1 == l1.end() && it2 == l2.end();
}

template<class T>
bool operator!=(DLinkedList<T> const &l1, DLinkedList<T> const &l2) {
    return !(l1 == l2);
}