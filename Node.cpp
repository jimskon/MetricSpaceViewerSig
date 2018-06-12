/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Node.cpp
 * Author: skonjp
 * 
 * Created on August 21, 2017, 10:33 AM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <math.h> 
#include <map>
#include <algorithm>
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
  dist = 32000;
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
