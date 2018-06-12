
#include <vector>
#include "Graph.h"
#include <string>

Graph::Graph() {
}

Graph::Graph(const Graph& orig) {
}

int Graph::add(Node *n) {
    int i = graph.size();
    graph.push_back(n);
    return i;
}

Node *Graph::at(int i) {
    return graph.at(i);
}

void Graph::addLink(int i, int link) {
    graph.at(i)->next(link);
    graph.at(link)->next(i);
}

int Graph::size() {
    return graph.size();
}

void Graph::display() {
    cout << "Nodes: " << graph.size() << endl;
    for (int i = 0; i < graph.size(); i++) {
        cout << "[" << i << "] ";
        graph.at(i)->display();
        cout << endl;
    }
}

void Graph::xmlNodes() {
    int len = graph.size();
    cout << "<nodes>";
    for (int i = 0; i < len; i++) {
        cout << "<n>";
        graph.at(i)->xmlNodeXY();
        vector<int> neighbors = graph.at(i)->getNeighbors();
        int l = neighbors.size();
        for (int i=0; i<l; i++){
            cout << "<l>";
            cout  << graph.at(neighbors.at(i))->getPoint().GetX() << "," << graph.at(neighbors.at(i))->getPoint().GetY();
            cout << "</l>";
        }
        cout << "<c>";
        int color = graph.at(i)->getColor();
        cout << color;
        cout << "</c>";
        cout << "</n>";
    }
    cout << "</nodes>";
}