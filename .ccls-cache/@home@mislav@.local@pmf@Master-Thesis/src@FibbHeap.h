#pragma once

#include "util.h"

template <typename T> class FibbHeap;

template <typename T>
class Node {
    T           value;
    std::size_t degree;
    bool        mark;

    Ref<Node>   next;
    Ref<Node>   prev;
    Ref<Node>   child;

public:

    std::size_t getDegree() { return degree; }
    bool        getMark()   { return mark; }

    T &         getValue()  { return value; }
    Ref<Node>   getNext()   { return next;  }
    Ref<Node>   getPrev()   { return prev;  }
    Ref<Node>   getChild()  { return child; }

    void setDegree(std::size_t degree) { this->degree = degree; }
    void increaseDegree()              { this->degree++;        }

    void setMark(bool mark) { this->mark = mark;        }
    void toggleMark()       { this->mark = !this->mark; }

    void insertLeft(Ref<Node<T>> other) {
        other->next = this;
        other->prev = this->prev;
        this->prev = other;
        other->prev->next = other;
    }

    void insertRight(Ref<Node<T>> other) {
        other->prev = this;
        other->next = this->next;
        this->next = other;
        other->next->prev = other;
    }
};

template <typename T>
class FibbHeap {

    Ref<Node<T>> min;
    std::size_t  size;

public:

    FibbHeap();
    void          insert(Ref<Node<T>> node);
    void          remove(Ref<Node<T>> node);

    bool          empty();

    void          meld();
    T             findMin();
    T             extractMin();
    void          decreaseKey();
    FibbHeap<T> & operator+=(FibbHeap<T> other);
};

template <typename T>
void FibbHeap<T>::insert(Ref<Node<T>> node) {
    if (min == nullptr) min = node;
    else {
        min->insertLeft(node);
        if (node->getValue() < min->getValue()) min = node;
    }
    size++;
}

//#include "FibbHeap.cc"
