import math

class FibbHeap(object):

    class Node(object):
        degree = 0
        size = 1
        mark = False
        child = None
        parent = None

        def __init__(self, value):
            self.value = value
            self.next_node = self
            self.prev_node = self

        def __getattribute__(self, __name):
            return super().__getattribute__(__name)
        def __setattr__(self, __name: str, __value):
            super().__setattr__(__name, __value)

        def add_child(self, child):
            if self.child:
                self.child.insert_next(child)
            else:
                self.child = child

        def insert_next(self, node):
            assert node is not None
            node.prev_node = self
            node.next_node = self.next_node
            if self.next_node:
                self.next_node.prev_node = node
            self.next_node = node

        def insert_prev(self, node):
            node.next_node = self
            node.prev_node = self.prev_node
            if self.prev_node:
                self.prev_node.next_node = node
            self.prev_node = node

        def remove_prev(self):
            if self.next_node == self:
                self.prev_node = self.next_node = None
            else:
                if self.prev_node and self.prev_node.prev_node:
                    self.prev_node.prev_node.next_node = self
                    self.prev_node = self.prev_node.prev_node

        def remove_next(self):
            if self.next_node == self:
                self.prev_node = self.next_node = None
            else:
                if self.next_node and self.next_node.next_node:
                    self.next_node.next_node.prev_node = self
                    self.next_node = self.next_node.next_node

        def iterate(self):
            node = self
            while True:
                yield node
                node = node.next_node
                if node == self:
                    break



    def __init__(self):
        self.min_node = None
        self.size = 0

    def __getattribute__(self, __name: str):
        return super().__getattribute__(__name)
    def __setattr__(self, __name: str, __value) -> None:
        super().__setattr__(__name, __value)

    def max_degree(self) -> int:
        return math.floor(math.log10(self.size))

    def empty(self) -> bool:
        return self.size == 0

    def add_to_master_list(self, node):
        if self.empty():
            self.min_node = node;
        else:
            assert self.min_node is not None
            self.min_node.insert_prev(node)
        self.size += 1

    def insert_node(self, node):
        node.degree = 0
        self.add_to_master_list(node)

    def extract_min(self):
        temp = self.min_node;
        assert temp is not None

        if self.min_node is None:
            return self.min_node

        if self.min_node.child is not None:
            for child in self.min_node.child.iterate():
                self.min_node.insert_prev(child)
                child.parent = None

        if temp == temp.next_node:
            self.min_node = None
        else:
            self.consolidate()

        self.size -= 1
        return temp.value

    def consolidate(self):
        degree_array = [None for _ in range(self.max_degree())]
        node = self.min_node
        assert node is not None and self.min_node is not None
        while node != self.min_node.prev_node:
            x, d = node, node.degree
            while degree_array[d] != None:
                y = degree_array[d]
                assert y is not None
                if x.value > y.value:
                    temp = x
                    x = y
                    y = temp
                self.fib_heap_ilnk(y, x)
                degree_array[d + 1] = None
                d += 1
            degree_array[d] = x
            node = node.next_node
        self.min_node = None
        for ref in degree_array:
            if ref is not None: self.add_to_master_list(ref)

    def fib_heap_link(self, x, y):
        y.next.remove_prev
        x.prev.remove_next
        x.add_child(y)
        y.mark = False

    def decrease_key(self, node: Node, key):
        assert(key <= node.value)
        node.value = key
        parent = node.parent
        if parent is not None and node.value < parent.value:
            self.cut(node, parent)
            self.cascading_cut(parent)
        if node.value < self.min_node.value:
            self.min_node = node

    def cut(self, node: Node, parent: Node):
        node.next_node.remove_prev()
        node.prev_node.remove_next()
        parent.degree -= 1
        self.add_to_master_list(node)
        node.parent = None
        node.mark = False

    def cascadingCut(self, node: Node):
        parent = node.parent
        if parent is not None:
            if not node.mark:
                node.mark = not node.mark
            else:
                self.cut(node, parent)
                self.cascadingCut(parent)


def main():
    n = FibbHeap.Node(1)
    f = FibbHeap()
    f.insert_node(FibbHeap.Node(1))
    f.insert_node(FibbHeap.Node(2))
    f.insert_node(FibbHeap.Node(122))
    f.insert_node(FibbHeap.Node(5))
    child = f.min_node
    while True:
        assert child is not None
        print(child.value)
        child = child.next_node
        if child == f.min_node: break

if __name__ == '__main__':
    main()
