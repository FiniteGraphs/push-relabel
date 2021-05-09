#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

struct Edge {
    
    int flow, capacity, u, v;
    
    Edge(int flow, int capacity, int u, int v) {
        this->flow = flow;
        this->capacity = capacity;
        this->u = u;
        this->v = v;
    }
};

struct Vertex {
    int h, e_flow, id;
    
    Vertex(int h, int e_flow, int id) {
        this->h = h;
        this->e_flow = e_flow;
        this->id = id;
    }
};

class Graph {
    vector<Vertex> vertices;
    vector<Edge> edges;
    
    bool push(int u);
    
    void relabel(int u);
    
    void preflow(int s);
    
    void updateReverseEdgeFlow(int i, int flow);
    
    void globalRelabeling();
    
    void bfs(bool isReverse, int s);
    
    void printVertices();
    
    
public:
    int V;
    
    Graph(int V);
    
    void addEdge(int u, int v, int w);
    
    int getMaxFlow(int s, int t);
};

Graph::Graph(int V) {
    this->V = V;
    
    for (int i = 0; i < V; i++)
    vertices.push_back(Vertex(0, 0, i));
}

void Graph::addEdge(int u, int v, int capacity) {
    edges.push_back(Edge(0, capacity, u, v));
}

void Graph::printVertices() {
    cout << "\nVERTICES";
    for (int i = 0; i < vertices.size(); i++) {
        cout << "\n" << vertices[i].id << " h: " << vertices[i].h << " e_flow: " << vertices[i].e_flow;
    }
}

void Graph::preflow(int s) {
    
    globalRelabeling();
    
    vertices[s].h = int(vertices.size());
    
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].u == s) {
            edges[i].flow = edges[i].capacity;
            
            vertices[edges[i].v].e_flow += edges[i].flow;
            
            edges.push_back(Edge(-edges[i].flow, 0, edges[i].v, s));
        }
    }
    
    //    printVertices();
}

int overFlowVertex(vector<Vertex>& vertices) {
    
    vector<Vertex> verticesCpy;
    
    for (int i = 1; i < vertices.size() - 1; i++)
    if (vertices[i].e_flow > 0)
        verticesCpy.push_back(vertices[i]);
    
    if (verticesCpy.empty()) {
        return -1;
    }
    
    
    std::sort(verticesCpy.begin(), verticesCpy.end(), [](Vertex a, Vertex b) {
        return a.h > b.h;
    });
    
    
    //    cout << "\nOVERFLOW VERTICES";
    //    for (int i = 0; i < verticesCpy.size(); i++) {
    //        cout << "\n" << verticesCpy[i].id << " " << verticesCpy[i].h;
    //    }
    
    return verticesCpy[0].id;
    
}

void Graph::bfs(bool isReverse, int s) {
    
    queue<Vertex> queue;
    
    queue.push(vertices[s]);
    
    while (!queue.empty()) {
        Vertex currVertex = queue.front();
        //        cout << "\nVisited " << currVertex.id << " ";
        queue.pop();
        
        for (int i = 0; i < edges.size(); i++) {
            if (isReverse) {
                if (edges[i].v == currVertex.id) {
                    //                    cout << "\n" << edges[i].u << " " << edges[i].v;
                    if (vertices[edges[i].u].h == 0) {
                        vertices[edges[i].u].h = currVertex.h++;
                        queue.push(vertices[edges[i].u]);
                    }
                }
            } else {
                if (edges[i].u == currVertex.id) {
                    //                    cout << "\n" << edges[i].u << " " << edges[i].v;
                    if (vertices[edges[i].v].h == 0) {
                        vertices[edges[i].v].h = currVertex.h++;
                        queue.push(vertices[edges[i].v]);
                    }
                }
            }
        }
    }
}

void Graph::globalRelabeling() {
    bfs(false, 0);
    bfs(true, int(vertices.size()-1));
}

void Graph::updateReverseEdgeFlow(int i, int flow) {
    int u = edges[i].v, v = edges[i].u;
    
    for (int j = 0; j < edges.size(); j++) {
        if (edges[j].v == v && edges[j].u == u) {
            edges[j].flow -= flow;
            return;
        }
    }
    
    Edge e = Edge(0, flow, u, v);
    edges.push_back(e);
}

bool Graph::push(int u) {
    
    for (int i = 0; i < edges.size(); i++) {
        
        if (edges[i].u == u) {
            
            if (edges[i].flow == edges[i].capacity)
                continue;
            
            if (vertices[u].h > vertices[edges[i].v].h) {
                int flow = min(edges[i].capacity - edges[i].flow,
                               vertices[u].e_flow);
                
                vertices[u].e_flow -= flow;
                
                vertices[edges[i].v].e_flow += flow;
                
                edges[i].flow += flow;
                
                updateReverseEdgeFlow(i, flow);
                
                return true;
            }
        }
    }
    return false;
}

void Graph::relabel(int u) {
    int mh = 1000000000;
    
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].u == u)    {
            
            if (edges[i].flow == edges[i].capacity)
                continue;
            
            if (vertices[edges[i].v].h < mh) {
                mh = vertices[edges[i].v].h;
                
                vertices[u].h = mh + 1;
            }
        }
    }
}

int Graph::getMaxFlow(int s, int t) {
    preflow(s);
    
    int globalRelabelingInterval = 10;
    int beforeGlobalRelabing = 0;
    
    while (overFlowVertex(vertices) != -1) {
        
        if (beforeGlobalRelabing == globalRelabelingInterval) {
            globalRelabeling();
            beforeGlobalRelabing = 0;
        } else {
            beforeGlobalRelabing++;
        }
        
        int u = overFlowVertex(vertices);
        if (!push(u))
            relabel(u);
    }
    
    return vertices.back().e_flow;
}

Graph getFromConsole() {
    int V = -1;
    cout << "Количество вершин:\n" << ">>> ";
    cin >> V;
    
    int E = -1;
    cout << "Количество ребер:\n" << ">>> ";
    cin >> E;
    
    Graph g(V);
    
    for (int i = 0; i < E; i++) {
        int u, v,capacity;
        cout << "u, v, capacity\n" << ">>> ";
        cin >> u >> v >> capacity;
        g.addEdge(u, v, capacity);
    }
    
    return g;
}

vector<string> split(string s, string sep = " ") {
    
    vector<string> words;
    
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(sep)) != std::string::npos) {
        words.push_back(s.substr(0, pos));
        s.erase(0, pos + sep.length());
    }
    
    return words;
}

Graph menu() {
    
    Graph g(0);
    
    //    while (true) {
    //        int option = -1;
    //        cout << "ОПЦИИ\n" << "1)Ввести ручками\n"<<"2)Скормить файл\n"<<"3)Запустить тесты\n" << ">>> ";
    //        cin >> option;
    //
    //        if (option == 1) {
    //            return getFromConsole();
    //        } else if (option == 2) {
    //            throw "NOT IMPLEMENTED";
    //        } else if (option == 3) {
    //            throw "NOT IMPLEMENTED";
    //        } else {
    //            cout << "ВВЕДЕНА НЕВЕРНАЯ ОПЦИЯ!\n";
    //        }
    //    }
    
    return getFromConsole();
}

int main() {
    
    Graph g = menu();
    
    cout << g.getMaxFlow(0, g.V - 1) << "\n";
    
    return 0;
}

