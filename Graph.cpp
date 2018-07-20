#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <math.h> 
#include <map>
#include <algorithm>
#include <set>
#include "Point.h"
#include "Node.h"
#include "Graph.h"

Graph::Graph() {
}

Graph::Graph(const Graph& orig) {
}

int Graph::add(Node *n) {
    int i = graph.size();
    graph.push_back(n);
    return i;
}

// Indicates where in the graph the user is looking.
Node *Graph::at(int i) {
    return graph.at(i);
}

// Connects two nodes and returns their indexes.
void Graph::addLink(int i, int link) {
    graph.at(i)->next(link);
    graph.at(link)->next(i);
}

// Returns the size of the graph.
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

// Output node daya in CSV form:
// x:y:color:neighbors(csv),signature(csv)
void Graph::CSVData() {
    int length = graph.size();
    for (int i = 0; i < length; i++) {
          
        // Gets the (x , y) coordinates of each node.
        Point p=graph.at(i)->getPoint();
        cout << p.GetX() << ":" << p.GetY() << ":";
       
        // Displays the color of each node.      
        int color = graph.at(i)->getColor();
        cout << color << ":";
          
        // Gets the (x , y) coordinates of the nodes surrounding the previously
        // mentioned node.
        vector<int> neighbors = graph.at(i)->getNeighbors();
        int l = neighbors.size();
        for (int i=0; i<l; i++){
            cout  << graph.at(neighbors.at(i))->getPoint().GetX() << "," << graph.at(neighbors.at(i))->getPoint().GetY();
            if (i+1<l) cout << ",";
        }
        cout << ":";
                
        // Displays the signature of each node.
        string signature = graph.at(i)->listToString();
        cout << signature;
        cout << endl;    
    }
}

void Graph::xmlNodes() {
    int length = graph.size();
    cout << "<nodes>";
    for (int i = 0; i < length; i++) {
        cout << "<n>";
          
        // Gets the (x , y) coordinates of each node.
        graph.at(i)->xmlNodeXY();
          
        // Gets the (x , y) coordinates of the nodes surrounding the previously
        // mentioned node.
        vector<int> neighbors = graph.at(i)->getNeighbors();
        int l = neighbors.size();
        for (int i=0; i<l; i++){
            cout << "<l>";
            cout  << graph.at(neighbors.at(i))->getPoint().GetX() << "," << graph.at(neighbors.at(i))->getPoint().GetY();
            cout << "</l>";
        }
        
        // Displays the color of each node.      
        cout << "<c>";
        int color = graph.at(i)->getColor();
        cout << color;
        cout << "</c>";
                
        // Displays the signature of each node.
        cout << "<s>";
        string signature = graph.at(i)->listToString();
        cout << signature;
        cout << "</s>";

        cout << "</n>";
                
    }
    cout << "</nodes>";
}
