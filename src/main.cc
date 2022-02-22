#include <iostream>
#include "FibbHeap.h"


int main(int argc, char *argv[]) {

    FibbHeap<int> F;
    auto n = Node<int>(4);
    auto m = Node<int>(3);
    F.insertNode(&n);
    F.insertNode(&m);
    std::cout << F.extractMin() << std::endl;
    return 0;
}
