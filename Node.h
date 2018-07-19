/* 
 * File:   Node.h
 * Author: skonjp
 *
 * Created on August 21, 2017, 10:33 AM
 */
#ifndef NODE_H
#define NODE_H
#include <climits>
#include <list>
#include "Point.h"

using namespace std;
static int trans1[] = {0, 1, -1, 2, -2, 3};
static int trans2[] = {0, 1, 2, -2, 3};

class Node {
public:
    Node();
    
    Node(int c, list<int> s, Point p);
    
    virtual ~Node();

    void display();
    void next(int i);

    Point getPoint();
    void xmlNodeXY();

    vector<int> getNeighbors();

    void setDist(int d);
    void clearDist();
    int getDist();
    
    void setColor(int c);
    int getColor();
        
    vector<int> resize(int l);
    
    int sizeSig();
    list<int> getSig();
    
    void pushSigBack(int v);
    void pushSigBack2(int x, int y);
    
    string listToString();

private:

    vector<int> signature;
    vector<int> neighbors;  
    
    list<int> sigLis;
    
    Point p;
    int color;
    int dist;
};


#endif // NODE_H 
