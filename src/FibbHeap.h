#pragma once

#include "util.h"
#include <cmath>
#include <cassert>
#include <array>

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

    Node(T val)
        : value{val}, degree{0}, size{1}, mark{false},
          next{nullptr}, prev{nullptr}, child{nullptr}, parent{nullptr}
    {}

    std::size_t getDegree()                    { return degree; }
    bool        getMark()                      { return mark; }

    T &         getValue()                     { return value;  }
    void        getValue(T value)              { this->value = value; }
    Ref<Node>   getNext()                      { return next;   }
    Ref<Node>   getPrev()                      { return prev;   }
    Ref<Node>   getChild()                     { return child;  }

    Ref<Node>   getParent()                    { return parent; }
    void        setParent(Ref<Node<T>> parent) { this->parent = parent; }


    void setDegree(std::size_t degree)         { this->degree = degree; }
    void increaseDegree()                      { this->degree++; }
    void decreaseDegree()                      { this->degree--; }

    void setMark(bool mark)                    { this->mark = mark;        }
    void toggleMark()                          { this->mark = !this->mark; }


    void addChild(Ref<Node<T>> child) {
        this->child ? this->child->insertNext(child) : this->child = child;
    }

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
        if (next == this) {
            this->prev = this->next = nullptr;
        }
        next->next->prev = this;
        next = next->next;
        this->prev = this->next = this;
    }
};

template <typename T>
class FibbHeap {

    Ref<Node<T>> min;
    std::size_t  size;

    void consolidate();
    std::size_t maxDegree() { return std::floor(std::log10(size)); }
    void fibHeapLink(Ref<Node<T>> x, Ref<Node<T>> y);

	void addToMasterList(Ref<Node<T>> node);

    void cut(Ref<Node<T>> node, Ref<Node<T>> parent);
    void cascadingCut(Ref<Node<T>> node);

    friend FibbHeap<T> operator|(FibbHeap<T> first, FibbHeap<T> second);

public:

    FibbHeap();
    void          insertNode(Ref<Node<T>> node);
    //void          remove(Ref<Node<T>> node);

    bool          empty() { return size == 0; }

    T             findMin();
    T             extractMin();
    void          decreaseKey(Ref<Node<T>> node, T key);
};

template <typename T>
FibbHeap<T>::FibbHeap()
    : min{nullptr} {}

template <typename T>
void FibbHeap<T>::addToMasterList(Ref<Node<T>> node) {
    if (empty()) min = node;
    else {
        min->insertPrev(node);
        if (node->getValue() < min->getValue()) min = node;
    }
    size++;
}

template <typename T>
void FibbHeap<T>::insertNode(Ref<Node<T>> node) {
    node->setDegree(0);
    addToMasterList(node);
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
            this->fibHeapLink(y, x);
            degreeArr[d++] = nullptr;
        }
        degreeArr[d] = x;
        node = node->getNext();
    }
    min = nullptr;
    for (auto ref : degreeArr) {
        if (ref != nullptr) addToMasterList(ref);
    }
}

template <typename T>
void FibbHeap<T>::fibHeapLink(Ref<Node<T>> x, Ref<Node<T>> y) {
    y->getNext()->removePrev();
    y->getPrev()->removeNext();
    x->addChild(y);
    y->setMark(false);
}

template <typename T>
void FibbHeap<T>::decreaseKey(Ref<Node<T>> node, T key) {
    assert(key <= node->getValue());
    node->setValue(key);
    auto parent = node->getParent();
    if (parent != nullptr && node->getValue() < parent->getValue()) {
        cut(node, parent);
        cascadingCut(parent);
    }
    if (node->getValue() < min->getValue()) min = node;
}

template <typename T>
void FibbHeap<T>::cut(Ref<Node<T>> node, Ref<Node<T>> parent) {
    node->getNext()->removePrev();
    node->getPrev()->removeNext();
    parent->decreaseDegree();
    addToMasterList(node);
    node->setParent(nullptr);
    node->setMark(false);
}

template <typename T>
void FibbHeap<T>::cascadingCut(Ref<Node<T>> node) {
    auto parent = node->getParent();
    if (parent != nullptr) {
        if (!node->getMark()) node->toggleMark();
        else {
            cut(node, parent);
            cascadingCut(parent);
        }
    }
}

template <typename T>
FibbHeap<T> operator|(FibbHeap<T> first, FibbHeap<T> second) {
    FibbHeap<T> U;
    U.min = first.min;
    U.size = first.size;
    while (second.min->getNext() != nullptr) {
        U.insertNode(second.min->getNext());
        second.min->removeNext();
    }
    return U;
}
