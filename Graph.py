import sys
from collections import deque

from Edge import Edge
from Vertex import Vertex


class Graph:

    def __init__(self, v: int):
        self.V: int = v
        self.vertices: list[Vertex] = []
        self.edges: list[Edge] = []

        for i in range(0, self.V):
            self.vertices.append(Vertex(0, 0, i))

    def addEdge(self, u: int, v: int, capacity: int):
        self.edges.append(Edge(0, capacity, u, v))

    def printVertices(self):
        print("VERTICES")
        for v in self.vertices:
            print("\nid: ", v.id, " h: ", v.h, " e_flow: ", v.e_flow)

    def bfs(self, is_reverse: bool, s: int):
        queue = deque()
        queue.append(self.vertices[s])

        while len(queue) > 0:
            curr_vertex: Vertex = queue[0]

            queue.pop()

            for i in range(len(queue)):
                if is_reverse:
                    if self.edges[i].v == curr_vertex.id:
                        if self.vertices[self.edges[i].u].h == 0:
                            self.vertices[self.edges[i].u].h = curr_vertex.h + 1
                            queue.append(self.vertices[self.edges[i].u])

                else:
                    if self.edges[i].u == curr_vertex.id:
                        if self.vertices[self.edges[i].v].h == 0:
                            self.vertices[self.edges[i].v].h = curr_vertex.h + 1
                            queue.append(self.vertices[self.edges[i].v])

    def globalRelabeling(self):
        self.bfs(False, 0)
        self.bfs(True, int(len(self.vertices) - 1))

    def preflow(self, s: int):

        self.globalRelabeling()

        self.vertices[s].h = int(len(self.vertices))

        for i in range(len(self.edges)):
            if self.edges[i].u == s:
                self.edges[i].flow = self.edges[i].capacity

                self.vertices[self.edges[i].v].e_flow += self.edges[i].flow

                self.edges.append(Edge(-self.edges[i].flow, 0, self.edges[i].v, s))

    def overFlowVertex(self, vertices: list[Vertex]):

        verticesCpy: list[Vertex] = []

        for i in range(1, len(self.vertices) - 1):
            if self.vertices[i].e_flow > 0:
                verticesCpy.append(vertices[i])

        if len(verticesCpy) == 0:
            return -1

        sorted(verticesCpy, key=lambda vertex: vertex.h)

        return verticesCpy[0].id

    def updateReverseEdgeFlow(self, i: int, flow: int):

        u: int = self.edges[i].v
        v: int = self.edges[i].u

        for j in range(len(self.edges)):
            if self.edges[j].v == v and self.edges[j].u == u:
                self.edges[j].flow -= flow
                return

        self.edges.append(Edge(0, flow, u, v))

    def push(self, u: int) -> bool:

        for i in range(len(self.edges)):
            if self.edges[i].u == u:
                if self.edges[i].flow == self.edges[i].capacity:
                    continue

                if self.vertices[u].h > self.vertices[self.edges[i].v].h:
                    flow = min(self.edges[i].capacity - self.edges[i].flow, self.vertices[u].e_flow)

                    self.vertices[u].e_flow -= flow;

                    self.vertices[self.edges[i].v].e_flow += flow;

                    self.edges[i].flow += flow;

                    self.updateReverseEdgeFlow(i, flow);

                    return True

        return False

    def relabel(self, u: int):
        mh: int = 1000000

        for i in range(len(self.edges)):
            if self.edges[i].u == u:
                if self.edges[i].flow == self.edges[i].capacity:
                    continue

                if self.vertices[self.edges[i].v].h < mh:
                    mh = self.vertices[self.edges[i].v].h

                    self.vertices[u].h = mh + 1

    def getMaxFlow(self, s: int, t: int):
        self.preflow(s)

        globalRelabelingInterval: int = 10
        beforeGlobalRelabeling: int = 0

        while self.overFlowVertex(self.vertices) != -1:
            if beforeGlobalRelabeling == globalRelabelingInterval:
                self.globalRelabeling()
                beforeGlobalRelabeling = 0
            else:
                beforeGlobalRelabeling += 1

            u: int = self.overFlowVertex(self.vertices)
            if not self.push(u):
                self.relabel(u)

        self.printVertices()
        return self.vertices[self.V - 1].e_flow
