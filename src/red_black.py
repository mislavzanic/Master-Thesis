from enum import Enum

class Color(Enum):
    RED = 0
    BLACK = 1

class RB_Node:
    def __init__(self):
        self.left = None
        self.right = None
        self.parent = None
        self.color = None
