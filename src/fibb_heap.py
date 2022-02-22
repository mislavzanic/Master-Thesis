from typing import Any, TypeVar, Generic, Union

NodeOrNone = Union['Node', None]

class Node(object):
    degree: int = 0
    size:   int = 1
    mark:   bool = False
    child:  NodeOrNone = None
    parent: NodeOrNone = None
    next: NodeOrNone
    prev: NodeOrNone

    def __init__(self, value: Any):
        self.value = value
        self.next = self
        self.prev = self
        ...

    def __getattribute__(self, __name: str) -> Any:
        return super().__getattribute__(__name)
    def __setattr__(self, __name: str, __value: Any) -> None:
        super().__setattr__(__name, __value)

    def add_child(self, child: NodeOrNone):
        if self.child is None:
            self.child = child
        else:
            self.child.insert_next(child)

    def insert_next(self, node: NodeOrNone):
        assert node is not None
        node.prev = self
        if self.next is not None:
            self.next.prev = node
        node.next = self.next
        self.next = node

    def insert_prev(self, node: NodeOrNone):
        assert node is not None
        node.next = self
        node.prev = self.prev
        if self.prev is not None:
            self.prev.next = node
        self.prev = node

    def remove_prev(self):
        if self.next == self:
            self.prev = self.next = None
        else:
            if self.prev is not None and self.prev.prev is not None:
                self.prev.prev.next = self


class FibbHeap(object):
    def __init__(self):
        ...


def main():
    n = Node(1)

if __name__ == '__main__':
    main()
