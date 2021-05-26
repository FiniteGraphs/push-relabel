from Vertex import Vertex


class Graph:

    def __init__(self, v: int):
        self.V: int = v
        self.vertices: list[Vertex] = []

        for i in range(0, self.V):
            self.vertices.append(Vertex(0, 0, i))
