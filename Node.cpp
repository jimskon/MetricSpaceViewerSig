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
#include <list>
#include <math.h> 
#include <map>
#include <limits>
#include "Node.h"

Node::Node() {
    color = 0;
}

Node::Node(int c, list<int> s, Point p) {
    color = c;
    
    this->sigLis = s;
        
    this->p = p;
}

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

// Returns the (x , y) coordinates.
Point Node::getPoint() {
    return p;
}

// Displays the (x , y) coordinates.
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

// Pass in the level and return the vector but with  
// a new restricted size.
vector<int> Node::resize(int l) {
    signature.resize(l);
    return signature;
}

// Return the size of the signature.
int Node::sizeSig() {
    return sigLis.size();
}

// Returns the signature.
list<int> Node::getSig() {
    return sigLis;
}

// Adds a number to the ending of signature.
void Node::pushSigBack(int v) {
    sigLis.push_back(v);
}

// Adds two numbers to the ending of signature.
void Node::pushSigBack2(int x, int y) {
    sigLis.push_back(x);
    sigLis.push_back(y);
}

// Converts a list<int> into a string.
string Node::listToString() {
    std::ostringstream out;
    
    if (!sigLis.empty()) {
        // Convert all but the last element to avoid a trailing ","
        copy(sigLis.begin(), --sigLis.end(), ostream_iterator<int>(out, ","));

        // Now add the last element with no delimiter
        out << sigLis.back();
    }
    
    return out.str();
}
