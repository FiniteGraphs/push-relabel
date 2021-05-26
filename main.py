from Graph import Graph

if __name__ == "__main__":
    g = Graph(6)
    g.addEdge(0, 1, 10)
    g.addEdge(0, 4, 5)
    g.addEdge(1, 2, 5)
    g.addEdge(2, 3, 4)
    g.addEdge(2, 4, 3)
    g.addEdge(3, 5, 4)
    g.addEdge(4, 3, 2)
    g.addEdge(4, 5, 5)
    print(g.getMaxFlow(0, 5))