/* 
 * File:   Node.cpp
 * Author: skonjp
 * 
 * Created on August 21, 2017, 10:33 AM
 */

#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <iostream>
#include <math.h> 
#include <map>
#include <limits>
#include "Node.h"

Node::Node() {
    color = 0;
}

Node::Node(Point p) {
    color = 0;
    this->p = p;
}

//Node::Node(const Node& orig) {
//   (*this) = orig;
//}

Node::~Node() {
}

void Node::display() {

    cout << " (" << p.GetX() << "," << p.GetY() << ") ";
    cout << "Color: " << color << " ";

    if (neighbors.size() > 0) {
        cout << "Next:";
    }
    int i = 0;
    while (i < neighbors.size()) {
        cout << neighbors.at(i) << ":";
        i++;
    }

}

void Node::next(int i) {
    this->neighbors.push_back(i);
}

Point Node::getPoint() {
    return p;
}

void Node::xmlNodeXY() {
    cout << "<x>" << p.GetX() << "</x><y>" << p.GetY() << "</y>";
}

vector<int> Node::getNeighbors() {
    return neighbors;
}

void Node::clearDist() {
    dist = INT_MAX;
}

void Node::setDist(int d) {
    dist = d;
}

int Node::getDist() {
    return dist;
}

void Node::setColor(int c) {
    color = c;
}

int Node::getColor() {
    return color;
}

// Converts a vector into a string.
string Node::retreiveSig() {
    std::ostringstream vss;
    
    if (!signature.empty()) {
        // Convert all but the last element to avoid a trailing ","
        std::copy(signature.begin(), signature.end()-1, std::ostream_iterator<int>(vss, ","));

        // Now add the last element with no delimiter
        vss << signature.back();
    }
    
    return vss.str();
}

// Returns the signature of a node.
vector<int> Node::getSig() {
    return signature;
}

// Pass in the level that you are looking at and return the vector but with  
// a new restricted size.
vector<int> Node::resize(int l) {
    signature.resize(l);
    return signature;
}

// Adds a number to a signature of a node.
void Node::pushSig(int v) {
    signature.push_back(v);
}

// Copy the signature of a vector onto a node.
void Node::copySig(vector<int> v) {
    this->signature = v; 
}

// Return the size of the signature.
int Node::size() {
    return signature.size();
}
