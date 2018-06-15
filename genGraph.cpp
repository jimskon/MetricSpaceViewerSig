// Jim Skon, Kenyon College, 2017

#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"
#include <string>
#include <iostream>
#include <algorithm> // Needed for transform
#include <fstream>
#include <dirent.h> // Needed for directory search
#include <string>
#include <iostream>
#include <vector>
#include <math.h>
#include <map>
#include <algorithm>
#include <limits>
#include <set>


#include "Node.h"
#include "Point.h"
#include "Graph.h"

#define XML_USE_STL

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

static int DIVIDEFACTOR = 4;

ofstream logFile;
void createMetricSpace(Node *n1, Node *n2,int l, Graph *g);
void genNodes(int l);
void colorAllNodes(Graph *g, int maxDist);
  
int main() {
  Cgicc cgi;    // Ajax object

  // Create AJAX objects to recieve information from web page.
  form_iterator levelIt = cgi.getElement("levels");
  string level = **levelIt;
  int ilevel = atoi(level.c_str());

  // Debug to file example
  logFile.open("/tmp/skon.log",ios::out | ios::app);
  logFile << "Got: " <<  "levels:" << ":" << ilevel << endl;
  logFile.close();


  // output required message for AJAX
  cout << "Content-Type: text/plain\n\n";
  //cout << "Hello world!! << ilevel:" << ilevel;
  genNodes(ilevel);
}

void clearDists(Graph *g) {
  for (int i = 0; i < g->size(); i++) {
    g->at(i)->clearDist();
  }
}

// Recursively set the distances of all nodes adjacent to this node
// as long as distance <= MAX

void getNodesInRange(Graph *g, std::set<int> &inRange, int index, int max) {
  int dist = g->at(index)->getDist(); // Distance of this node
  //cout << "Node: " << index << " Dist: " << dist << endl;
  if (dist < 32000 && dist > max) return; //  This is the base case for recursion - we have reached the max
  //cout << "Add: " << index << endl;
  inRange.insert(index);
  // we can go deeper, recurse on neighbors if they are farther away then our length + 1
  for (int i = 0; i < g->at(index)->getNeighbors().size(); i++) { // For each neighbor
    int j = g->at(index)->getNeighbors().at(i); // Get the neighbors index
    if (g->at(j)->getDist() > dist + 1) {
      (g->at(j))->setDist(dist + 1);
      getNodesInRange(g, inRange, j, max);
    }
  }
}

std::set<int> findNodesInRange(Graph *g, int startIndex, int max) {
  std::set<int> ball; // Nodes in ball of max diameter
  clearDists(g); // reset distances
  g->at(startIndex)->setDist(0);
  getNodesInRange(g, ball, startIndex, max);
  return ball;
}


void genNodes(int l) {
  Graph *g = new Graph();

  Point pStart(0,250);
  Point pEnd(1000,250);

  Node *n1= new Node(pStart);
  Node *n2= new Node(pEnd);

  createMetricSpace(n1,n2,l,g);
  

}

Point computeP2(Point pStart, Point pEnd) {
  int x = (3 * pStart.GetX() + pEnd.GetX()) / DIVIDEFACTOR;
  int y = (3 * pStart.GetY() + pEnd.GetY()) / DIVIDEFACTOR;
  Point p(x,y);
  return p;
}

Point computeP3(Point pStart, Point pEnd) {
  int x = (pStart.GetX() + pEnd.GetX()) / 2 + (pEnd.GetY() - pStart.GetY()) / DIVIDEFACTOR;
  int y = (pStart.GetY() + pEnd.GetY()) / 2 - (pEnd.GetX() - pStart.GetX()) / DIVIDEFACTOR;
  Point p(x,y);
  return p;
}

Point computeP4(Point pStart, Point pEnd) {
  int x = (pStart.GetX() + pEnd.GetX()) / 2 - (pEnd.GetY() - pStart.GetY()) / DIVIDEFACTOR;
  int y = (pStart.GetY() + pEnd.GetY()) / 2 + (pEnd.GetX() - pStart.GetX()) / DIVIDEFACTOR;
  Point p(x,y);
  return p;
}

Point computeP5(Point pStart, Point pEnd) {
  int x = (pStart.GetX() + 3 * pEnd.GetX()) / DIVIDEFACTOR;
  int y = (pStart.GetY() + 3 * pEnd.GetY()) / DIVIDEFACTOR;
  Point p(x,y);
  return p;
}

// Create nested levels between n1 and n2, pushing on g
// return the graph index of first element in the group
// If skipFirst is true, don't add first element (ni)
// Creates all of the nodes, links, and signatures.
void createGroup(Node *n1, Node *n6, int level, int sigLen, Graph *g, int &si, int &fi) {    
    // Links the nodes
    if (level == 0) {  
        if (si < 0) {
            si = g->add(n1);
        }
        if (fi < 0) {
            fi = g->add(n6);
        }
        g->addLink(si, fi);
        return;
    }
    
    // Creates four new nodes that are between the end points.
    Node *n2 = new Node(computeP2(n1->getPoint(), n6->getPoint()));
    Node *n3 = new Node(computeP3(n1->getPoint(), n6->getPoint()));
    Node *n4 = new Node(computeP4(n1->getPoint(), n6->getPoint()));
    Node *n5 = new Node(computeP5(n1->getPoint(), n6->getPoint()));
    
    // Creates the signature for the nodes.
    vector<int> ts = n1->getSig();
    
    while (ts.size() >= sigLen) {
        ts.pop_back();
    }
  
    // Assigns (-1) to the point in the signature that should be going down.
    int j=0;
    while(j < ts.size()) {
        if ( (n6->getSig().at(j) < 0 && ts.at(j) > 0)) {
            ts.at(j) = -1;
            break;
        } 
        j++;   
    }
    
    if (n6->getSig().back() == -2) {
        ts.pop_back();
        ts.push_back(-1);
    }
    
    if (n1->size() < sigLen) {
        n1->pushSig(0);
    }
        
    if (n6->size() < sigLen) {
        n6->pushSig(0);
    }
        
    n2->copySig(ts);
    n3->copySig(ts);
    n4->copySig(ts);
    n5->copySig(ts);
    
    n2->pushSig(1);
    n3->pushSig(2);
    n4->pushSig(-2);
    n5->pushSig(3);
    
    int b = -1, c = -1, d = -1, e = -1;
    
    // Repeat the process until all nodes are accounted for.
    createGroup(n1, n2, level - 1, sigLen+1, g, si, b);
    createGroup(n2, n3, level - 1, sigLen+1, g, b, c);
    createGroup(n2, n4, level - 1, sigLen+1, g, b, d);
    createGroup(n3, n5, level - 1, sigLen+1, g, c, e);
    createGroup(n4, n5, level - 1, sigLen+1, g, d, e);
    createGroup(n5, n6, level - 1, sigLen+1, g, e, fi);
    
}

void createMetricSpace(Node *n1, Node *n2,int l, Graph *g) {

  if (l<1) {
    cout << "Levels must be at least one" << endl;
    return;
  }

  // create all levels (points) between these nodes
  // point first to the first point in created group
  int si=-1;
  int fi=-1;
  
  // Push Initial Signatures
  n1->pushSig(0);
  n2->pushSig(1);
  
  //createGroup(n1,n2,l-1,g, si, fi);
  createGroup(n1, n2, l - 1, 2, g, si, fi);

  // Color nodes that are maximal ball centers
  /*  for (int i=0 ; i<g->size(); i++) {
    std::set<int> ball = findNodesInRange(g, i, 10);
    //maxBall = max(maxBall,ball.size);
    if (ball.size()==64) {
      g->at(i)->setColor(1);
      //cout << i << " ";
    }
    }*/
  colorAllNodes(g, 10);
  
  
  g->xmlNodes();

}

// Fill set with maxColors colors (ints)

void initColors(std::set<int> &colors, int maxColors) {
  for (int i = 1; i <= maxColors; i++) {
    colors.insert(i);
  }
}


void removeUsedColors(Graph *g, std::set<int> &colors, std::set<int> ball) {
  std::set<int>::iterator it;
  for (it = ball.begin(); it != ball.end(); ++it) {
    int index = *it;
    int color = g->at(index)->getColor();
    if (color > 0) {
      colors.erase(color);
    }
  }

}
void colorAllNodes(Graph *g, int maxDist) {
  const int MaxColors = 65;
  std::set<int> colors; //set of currently available colors
  std::set<int>::iterator it;

  for (int i = 0; i < g->size(); i++) {
    std::set<int> ball = findNodesInRange(g, i, maxDist);

    initColors(colors, MaxColors);
    removeUsedColors(g, colors, ball);
    it=colors.begin();

    g->at(i)->setColor(*it);
    //maxColorsUsed = (maxColorsUsed>*it) ? maxColorsUsed : *it;
  }
}
