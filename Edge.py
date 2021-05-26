class Edge:

    def __init__(self, flow: int, capacity: int, u: int, v: int):
        self.flow: int = flow
        self.capacity: int = capacity
        self.u: int = u
        self.v: int = v
