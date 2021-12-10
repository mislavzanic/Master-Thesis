#pragma once

#include "util.h"
#include <cmath>
#include <array>

template <typename T> class FibbHeap;

template <typename T>
class Node {
    T           value;
    std::size_t degree;
    std::size_t size;
    bool        mark;

    Ref<Node>   next;
    Ref<Node>   prev;
    Ref<Node>   child;
    Ref<Node>   parent;

public:

    std::size_t getDegree() { return degree; }
    bool        getMark()   { return mark; }

    T &         getValue()  { return value;  }
    Ref<Node>   getNext()   { return next;   }
    Ref<Node>   getPrev()   { return prev;   }
    Ref<Node>   getChild()  { return child;  }

    Ref<Node>   getParent()                    { return parent; }
    void        setParent(Ref<Node<T>> parent) { this->parent = parent; }


    void setDegree(std::size_t degree) { this->degree = degree; }
    void increaseDegree()              { this->degree++;        }

    void setMark(bool mark) { this->mark = mark;        }
    void toggleMark()       { this->mark = !this->mark; }

    void insertPrev(Ref<Node<T>> other) {
        other->next = this;
        other->prev = this->prev;
        this->prev = other;
        other->prev->next = other;
    }

    void insertNext(Ref<Node<T>> other) {
        other->prev = this;
        other->next = this->next;
        this->next = other;
        other->next->prev = other;
    }

    void removePrev() {
        prev->prev->next = this;
        prev = prev->prev;
        this->prev = this->next = this;
    }

    void removeNext() {
        next->next->prev = this;
        next = next->next;
        this->prev = this->next = this;
    }
};

template <typename T>
class FibbHeap {

    Ref<Node<T>> min;
    std::size_t  size;

    void         consolidate();
    std::size_t  maxDegree() { return std::floor(std::log10(size)); }
    void         fibHeapLink(Ref<Node<T>> x, Ref<Node<T>> y);
    void         removeFromRoot(Ref<Node<T>> x);

public:

    FibbHeap();
    void          insert(Ref<Node<T>> node);
    void          remove(Ref<Node<T>> node);

    bool          empty() { return size == 0; }

    T             findMin();
    T             extractMin();
    void          decreaseKey(Ref<Node<T>> node, T key);
    FibbHeap<T> & operator+=(FibbHeap<T> other);
};

template <typename T>
void FibbHeap<T>::insert(Ref<Node<T>> node) {
    node->setDegree(0);
    if (empty()) min = node;
    else {
        min->insertPrev(node);
        if (node->getValue() < min->getValue()) min = node;
    }
    size++;
}

template <typename T>
T FibbHeap<T>::extractMin() {
    Ref<Node<T>> temp = min;
    if (temp != nullptr) {
        if (min->getChild() != nullptr) {
            Ref<Node<T>> child = min->getChild();
            while(child != min->getChild()->getPrev()) {
                min->insertPrev(child);
                child->setParent(nullptr);
                child = child->getNext();
            }
            min->insertPrev(child);
            child->setParent(nullptr);
            min->setChild(nullptr);
        }
        //temp->getNext()->removePrev();
        //temp->getPrev()->removeNext();
        if (temp == temp->getNext()) min = nullptr;
        else consolidate();
        size--;
    }
    return temp;
}

template <typename T>
void FibbHeap<T>::consolidate() {
    std::array<Ref<Node<T>>, maxDegree()> degreeArr;
    for(auto ref : degreeArr) ref = nullptr;
    auto node = min;
    while (node != min->getPrev()) {
        auto x = node;
        auto d = x->getDegree();
        while (degreeArr[d] != nullptr) {
            auto y = degreeArr[d];
            if (x->getValue() > y->getValue()) {
                auto temp = y;
                y = x;
                x = temp;
            }

        }
        node = node->getNext();
    }

}

template <typename T>
void FibbHeap<T>::fibHeapLink(Ref<Node<T>> x, Ref<Node<T>> y) {
    this->removeFromRoot(y);
    x->addChild(y);
    y->setMark(false);
}

template <typename T>
void FibbHeap<T>::removeFromRoot(Ref<Node<T>> x) {
    Ref<Node<T>> temp = min;
    while (temp->getNext() != x) temp = temp->getNext();
    temp->removeNext();
}
