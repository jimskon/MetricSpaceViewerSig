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

void buildGraph(Graph *g, bool p, int s, int e, int level, int l);
void buildLevel3(Graph *g, bool p, int s, int e);

static int DIVIDEFACTOR = 4;

ofstream logFile;
void createMetricSpace(Node *n1, Node *n2,int l, Graph *g);
void genNodes(int l);
void colorAllNodes(Graph *g, int maxDist);
  


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

// Accepts the graph and the level as the parameters. Then, checks to see 
// which situation the level applies to. Depending on the input of level, there
// will be various outcomes. Once the situation is decided, the code will do as
// dictated.

void buildInitialGraph(Graph *g, int level) {
    // s = Start and e = End. Both of which refer to the beginning and end nodes.
    int s, e;
    
    // The initial signatures.
    list<int> st1 = {0}, st2 = {1};
    
    // The starting (x , y) coordinates of the first and last node.
    Point pStart(0, 250);
    Point pEnd(1000, 250);
    
    // In order to create a graph there has to be a base case and level 1 is
    // the base case
    if (level < 1) {
        cout << "The level must be at least 1. " << endl;
        return;
    }
    
    // The simplest the fractal can be.
    if (level == 1) {
        s = g->add(new Node(1, st1, pStart));
        e = g->add(new Node(2, st2, pEnd)); 
        g->addLink(s, e);
        return;
    }
    
    // The generic shape that the fractal will follow so long as the level is
    // 2 or greater.
    if (level == 2) {
        s = g->add(new Node(1, st1, pStart));
        e = g->add(new Node(2, st2, pEnd));
        
        // Takes the signature of the starting node and copies it into 
        // a temporary list.
        list<int> temp = g->at(s)->getSig();
        
        // The node color, temporary signature, and relative (x,y) coordinates
        // are then sent to the graph which records the data to later be 
        // displayed to the user. 
        int a = g->add(new Node(3, temp, computeP2(g->at(s)->getPoint(), g->at(e)->getPoint())));
        int b = g->add(new Node(4, temp, computeP3(g->at(s)->getPoint(), g->at(e)->getPoint())));
        int c = g->add(new Node(5, temp, computeP4(g->at(s)->getPoint(), g->at(e)->getPoint())));
        int d = g->add(new Node(6, temp, computeP5(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // Pushes a value onto the recorded signature of each node 
        g->at(s)->pushSigBack(0);
        g->at(a)->pushSigBack(1);
        g->at(b)->pushSigBack(2);
        g->at(c)->pushSigBack(-2);
        g->at(d)->pushSigBack(3);
        g->at(e)->pushSigBack(0);
        
        // Links each node in the graph
        g->addLink(s, a);
        g->addLink(a, b);
        g->addLink(a, c);
        g->addLink(b, d);
        g->addLink(c, d);
        g->addLink(d, e);
        
        return;
    }
    
    if (level > 2) {
        s = g->add(new Node(31, st1, pStart));
        e = g->add(new Node(31, st2, pEnd));
        
        // Creates the rest of the nodes inbetween the start and ending node of
        // the graph
        bool p = true;
        buildGraph(g, p, s, e, level, 2); 
        
        // Adds 0 to the end of signatures for those that do not already have
        // enough zeros. The maximum and minimum length of the signature must
        // be the level and this code secures that for the user.
        int j=0, k=0;
        while (j < level) {
            for (int i=0; i < g->size(); i++) {
                int j=0;
                if (g->at(i)->getSig().size() < level) {
                    g->at(i)->pushSigBack(0);
                }
            }
            j++;
        }
        
        return;
    }
}

// Goes through and creates the other parts of the fractal until the level is 3.
// once the level is 3, the graph, boolean value, and the 2 indexes at that
// relative area are then sent to buildLevel3().
void buildGraph(Graph *g, bool p, int s, int e, int level, int l) {
    
    // Once the level is three, the inbetween nodes are created and added to the
    // graph. Note that a level three piece is acting as the base case for the
    // construction of the fractal.
    if (level == 3) {
        buildLevel3(g, p, s, e);
        return;
    } 
            
    list<int> tempS = g->at(s)->getSig();
    list<int> tempE = g->at(e)->getSig();
    
    while(tempS.size() >= l) {
        tempS.pop_back();
    }
    
    // Assigns (-1) to the point in the signature that should be going down. 
    // The conversion from a list to a vector back to a list is because it was
    // easier to look at a particular element of a signature with the vector
    // .at() command than iterating through a list.
    vector<int> x,y;
    copy(tempS.begin(), tempS.end(), back_inserter(x));
    copy(tempE.begin(), tempE.end(), back_inserter(y));
    
    int j = 0;
    while (j < x.size()) {
        if ( (y.at(j) < 0) && (x.at(j) > 0) ) {
            x.at(j) = -1;
            break;
        }
        j++;
    }
    
    list<int> tp;
    copy(x.begin(), x.end(), back_inserter(tp));
    
    // Pushes 0 onto the recorded signature of that particular start index.
    if (g->at(s)->sizeSig() < l) {
        g->at(s)->pushSigBack(0);
    }
    
    // Pushes 0 onto the recorded signature of that particular end index.
    if (g->at(e)->sizeSig() < l) {
        g->at(e)->pushSigBack(0);
    }
    
    // Creates the four new nodes that are between the start and end indexes
    // so that the fractal can be created. The order of the inputs to
    // new Node are color, temporary signature, and the (x , y) coordinates.
    int a = g->add(new Node(31, tp, computeP2(g->at(s)->getPoint(), g->at(e)->getPoint())));
    int b = g->add(new Node(31, tp, computeP3(g->at(s)->getPoint(), g->at(e)->getPoint())));
    int c = g->add(new Node(31, tp, computeP4(g->at(s)->getPoint(), g->at(e)->getPoint())));
    int d = g->add(new Node(31, tp, computeP5(g->at(s)->getPoint(), g->at(e)->getPoint()))); 
        
    // Pushes a value onto the recorded signature of each node
    g->at(a)->pushSigBack(1);
    g->at(b)->pushSigBack(2);
    g->at(c)->pushSigBack(-2);
    g->at(d)->pushSigBack(3);

    // Repeats the process for the rest of the fractal until everything is 
    // fully accounted for.
    buildGraph(g, !p, s, a, level -1, l+1);
    buildGraph(g, !p, a, b, level -1, l+1);
    buildGraph(g, p, a, c, level -1, l+1);
    buildGraph(g, !p, b, d, level -1, l+1);
    buildGraph(g, p, c, d, level -1, l+1);
    buildGraph(g, !p, d, e, level -1, l+1);
}

// The variable abbreviations stand for -
// st = signature
// A = Piece A
// B = Piece B
// - - -
// The number associated with A and B are irrelevant since they 
// serve as a way to distinguish one from another.
// - - -
// The comments that have < _ , _ , _ > are the signature or signature grouping
// for each new node added. Hence, there is a quick reference to what part of 
// a level 3 graph is being viewed.
// - - -
// Recognizes the bool p and executes the code accordingly. So
// if the value is true, then piece A is initialized and recorded. If the value 
// is false, then the same holds true for piece B. 
// - - -
//This is where all of the nodes are added to the graph.
void buildLevel3(Graph *g, bool p, int s, int e) {
    list<int> tempS = g->at(s)->getSig();
    list<int> tempE = g->at(e)->getSig();
    
    // Assigns (-1) to the point in the signature that should be going down. 
    // The conversion from a list to a vector back to a list is because it was
    // easier to look at a particular element of a signature with the vector
    // .at() command than iterating through a list.
    vector<int> x,y;
    copy(tempS.begin(), tempS.end(), back_inserter(x));
    copy(tempE.begin(), tempE.end(), back_inserter(y));
    
    int j = 0;
    while (j < x.size()) {
        if ( (y.at(j) < 0) && (x.at(j) > 0) ) {
            x.at(j) = -1;
            break;
        }
        j++;
    }
    
    list<int> tp;
    copy(x.begin(), x.end(), back_inserter(tp)); 
        
    // Piece 3A
    if (p == true) {
        
        // <0,1,0>
        int A6  =  g->add(new Node(6,  tp,  computeP2(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // <0,2,0>
        int A11 =  g->add(new Node(11, tp, computeP3(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // <0,-2,0>
        int A16 =  g->add(new Node(16, tp, computeP4(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // <0,3,0>
        int A21 =  g->add(new Node(2,  tp, computeP5(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // Segment between <0,0,0> and <0,1,0>
        int A2 =   g->add(new Node(36, tp, computeP2(g->at(s)->getPoint(), g->at(A6)->getPoint()))); 
        int A3  =  g->add(new Node(3,  tp, computeP3(g->at(s)->getPoint(), g->at(A6)->getPoint())));
        int A4  =  g->add(new Node(4,  tp, computeP4(g->at(s)->getPoint(), g->at(A6)->getPoint())));
        int A5  =  g->add(new Node(5,  tp, computeP5(g->at(s)->getPoint(), g->at(A6)->getPoint())));
        
        // Segment between <0,1,0> and <0,2,0>
        int A7  =  g->add(new Node(7,  tp, computeP2(g->at(A6)->getPoint(), g->at(A11)->getPoint())));
        int A8  =  g->add(new Node(8,  tp, computeP3(g->at(A6)->getPoint(), g->at(A11)->getPoint())));
        int A9  =  g->add(new Node(9,  tp, computeP4(g->at(A6)->getPoint(), g->at(A11)->getPoint())));
        int A10 =  g->add(new Node(10, tp, computeP5(g->at(A6)->getPoint(), g->at(A11)->getPoint())));
        
        // Segment between <0,-1,0> and <0,-2,0>
        int A12 =  g->add(new Node(12, tp, computeP2(g->at(A6)->getPoint(), g->at(A16)->getPoint())));
        int A13 =  g->add(new Node(13, tp, computeP3(g->at(A6)->getPoint(), g->at(A16)->getPoint())));
        int A14 =  g->add(new Node(14, tp, computeP4(g->at(A6)->getPoint(), g->at(A16)->getPoint())));
        int A15 =  g->add(new Node(15, tp, computeP5(g->at(A6)->getPoint(), g->at(A16)->getPoint())));

        // Segment between <0,2,0> and <0,3,0>
        int A17 =  g->add(new Node(17, tp, computeP2(g->at(A11)->getPoint(), g->at(A21)->getPoint())));
        int A18 =  g->add(new Node(18, tp, computeP3(g->at(A11)->getPoint(), g->at(A21)->getPoint())));
        int A19 =  g->add(new Node(19, tp, computeP4(g->at(A11)->getPoint(), g->at(A21)->getPoint())));
        int A20 =  g->add(new Node(1,  tp, computeP5(g->at(A11)->getPoint(), g->at(A21)->getPoint())));
        
        // Segment between <0,-2,0> and <0,3,0>
        int A22 =  g->add(new Node(20, tp, computeP2(g->at(A16)->getPoint(), g->at(A21)->getPoint())));
        int A23 =  g->add(new Node(21, tp, computeP3(g->at(A16)->getPoint(), g->at(A21)->getPoint())));
        int A24 =  g->add(new Node(22, tp, computeP4(g->at(A16)->getPoint(), g->at(A21)->getPoint())));
        int A25 =  g->add(new Node(23, tp, computeP5(g->at(A16)->getPoint(), g->at(A21)->getPoint())));
        
        // Segment between <0,3,0> and <1,0,0>
        int A26 =  g->add(new Node(30, tp, computeP2(g->at(A21)->getPoint(), g->at(e)->getPoint())));
        int A27 =  g->add(new Node(32, tp, computeP3(g->at(A21)->getPoint(), g->at(e)->getPoint())));
        int A28 =  g->add(new Node(33, tp, computeP4(g->at(A21)->getPoint(), g->at(e)->getPoint())));
        int A29 =  g->add(new Node(34, tp, computeP5(g->at(A21)->getPoint(), g->at(e)->getPoint())));
       
        // Adds the level 3 signature to the end of the node.
        g->at(A2)->pushSigBack2(0,1);
        g->at(A3)->pushSigBack2(0,2);
        g->at(A4)->pushSigBack2(0,-2);
        g->at(A5)->pushSigBack2(0,3);
        g->at(A6)->pushSigBack2(1,0);
        g->at(A7)->pushSigBack2(1,1);
        g->at(A8)->pushSigBack2(1,2);
        g->at(A9)->pushSigBack2(1,-2);
        g->at(A10)->pushSigBack2(1,3);
        g->at(A11)->pushSigBack2(2,0);
        g->at(A12)->pushSigBack2(-1,1);
        g->at(A13)->pushSigBack2(-1,2);
        g->at(A14)->pushSigBack2(-1,-2);
        g->at(A15)->pushSigBack2(-1,3);
        g->at(A16)->pushSigBack2(-2,0);
        g->at(A17)->pushSigBack2(2,1);
        g->at(A18)->pushSigBack2(2,2);
        g->at(A19)->pushSigBack2(2,-2);
        g->at(A20)->pushSigBack2(2,3);
        g->at(A21)->pushSigBack2(3,0);
        g->at(A22)->pushSigBack2(-2,1);
        g->at(A23)->pushSigBack2(-2,2);
        g->at(A24)->pushSigBack2(-2,-2);
        g->at(A25)->pushSigBack2(-2,3);
        g->at(A26)->pushSigBack2(3,1);
        g->at(A27)->pushSigBack2(3,2);
        g->at(A28)->pushSigBack2(3,-2);
        g->at(A29)->pushSigBack2(3,3);
        
        // Links the nodes in the graph.
        g->addLink(s, A2);
        g->addLink(A2, A3);
        g->addLink(A2, A4);
        g->addLink(A3, A5);
        g->addLink(A4, A5);
        g->addLink(A5, A6);
        g->addLink(A6, A7);
        g->addLink(A7, A8);
        g->addLink(A7, A9);
        g->addLink(A8, A10);
        g->addLink(A9, A10);
        g->addLink(A10, A11);
        g->addLink(A6, A12);
        g->addLink(A12, A13);
        g->addLink(A12, A14);
        g->addLink(A13, A15);
        g->addLink(A14, A15);
        g->addLink(A15, A16);
        g->addLink(A11, A17);
        g->addLink(A17, A18);
        g->addLink(A17, A19);
        g->addLink(A18, A20);
        g->addLink(A19, A20);
        g->addLink(A20, A21);
        g->addLink(A16, A22);
        g->addLink(A22, A23);
        g->addLink(A22, A24);
        g->addLink(A23, A25);
        g->addLink(A24, A25);
        g->addLink(A25, A21);
        g->addLink(A21, A26);
        g->addLink(A26, A27);
        g->addLink(A26, A28);
        g->addLink(A27, A29);
        g->addLink(A28, A29);
        g->addLink(A29, e);
                
    } else { // Piece B
                
        // <0,1,0>
        int B6  =  g->add(new Node(15, tp, computeP2(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // <0,2,0>
        int B11 =  g->add(new Node(5,  tp, computeP3(g->at(s)->getPoint(), g->at(e)->getPoint())));
       
        // <0,-2,0>
        int B16 =  g->add(new Node(2,  tp, computeP4(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // <0,3,0>
        int B21 =  g->add(new Node(10, tp, computeP5(g->at(s)->getPoint(), g->at(e)->getPoint())));
        
        // Segment between <0,0,0> and <0,1,0>
        int B2 =   g->add(new Node(24, tp, computeP2(g->at(s)->getPoint(), g->at(B6)->getPoint()))); 
        int B3  =  g->add(new Node(25, tp, computeP3(g->at(s)->getPoint(), g->at(B6)->getPoint())));
        int B4  =  g->add(new Node(26, tp, computeP4(g->at(s)->getPoint(), g->at(B6)->getPoint())));
        int B5  =  g->add(new Node(16, tp, computeP5(g->at(s)->getPoint(), g->at(B6)->getPoint())));
        
        // Segment between <0,1,0> and <0,2,0>
        int B7  =  g->add(new Node(18, tp, computeP2(g->at(B6)->getPoint(), g->at(B11)->getPoint())));
        int B8  =  g->add(new Node(20, tp, computeP3(g->at(B6)->getPoint(), g->at(B11)->getPoint())));
        int B9  =  g->add(new Node(22, tp, computeP4(g->at(B6)->getPoint(), g->at(B11)->getPoint())));
        int B10 =  g->add(new Node(6,  tp, computeP5(g->at(B6)->getPoint(), g->at(B11)->getPoint())));
        
        // Segment between <0,-1,0> and <0,-2,0>
        int B12 =  g->add(new Node(19, tp, computeP2(g->at(B6)->getPoint(), g->at(B16)->getPoint())));
        int B13 =  g->add(new Node(21, tp, computeP3(g->at(B6)->getPoint(), g->at(B16)->getPoint())));
        int B14 =  g->add(new Node(23, tp, computeP4(g->at(B6)->getPoint(), g->at(B16)->getPoint())));
        int B15 =  g->add(new Node(7,  tp, computeP5(g->at(B6)->getPoint(), g->at(B16)->getPoint())));
        
        // Segment between <0,2,0> and <0,3,0>
        int B17 =  g->add(new Node(35, tp, computeP2(g->at(B11)->getPoint(), g->at(B21)->getPoint())));
        int B18 =  g->add(new Node(13, tp, computeP3(g->at(B11)->getPoint(), g->at(B21)->getPoint())));
        int B19 =  g->add(new Node(14, tp, computeP4(g->at(B11)->getPoint(), g->at(B21)->getPoint())));
        int B20 =  g->add(new Node(8,  tp, computeP5(g->at(B11)->getPoint(), g->at(B21)->getPoint())));
        
        // Segment between <0,-2,0> and <0,3,0>
        int B22 =  g->add(new Node(1,  tp, computeP2(g->at(B16)->getPoint(), g->at(B21)->getPoint())));
        int B23 =  g->add(new Node(17, tp, computeP3(g->at(B16)->getPoint(), g->at(B21)->getPoint())));
        int B24 =  g->add(new Node(12, tp, computeP4(g->at(B16)->getPoint(), g->at(B21)->getPoint())));
        int B25 =  g->add(new Node(9,  tp, computeP5(g->at(B16)->getPoint(), g->at(B21)->getPoint())));
        
        // Segment between <0,3,0> and <1,0,0>
        int B26 =  g->add(new Node(11, tp, computeP2(g->at(B21)->getPoint(), g->at(e)->getPoint())));
        int B27 =  g->add(new Node(27, tp, computeP3(g->at(B21)->getPoint(), g->at(e)->getPoint())));
        int B28 =  g->add(new Node(28, tp, computeP4(g->at(B21)->getPoint(), g->at(e)->getPoint())));
        int B29 =  g->add(new Node(29, tp, computeP5(g->at(B21)->getPoint(), g->at(e)->getPoint())));
        
        // Adds the level 3 signature to the end of the node.
        g->at(B2)->pushSigBack2(0,1);
        g->at(B3)->pushSigBack2(0,2);
        g->at(B4)->pushSigBack2(0,-2);
        g->at(B5)->pushSigBack2(0,3);
        g->at(B6)->pushSigBack2(1,0);
        g->at(B7)->pushSigBack2(1,1);
        g->at(B8)->pushSigBack2(1,2);
        g->at(B9)->pushSigBack2(1,-2);
        g->at(B10)->pushSigBack2(1,3);
        g->at(B11)->pushSigBack2(2,0);
        g->at(B12)->pushSigBack2(-1,1);
        g->at(B13)->pushSigBack2(-1,2);
        g->at(B14)->pushSigBack2(-1,-2);
        g->at(B15)->pushSigBack2(-1,3);
        g->at(B16)->pushSigBack2(-2,0);
        g->at(B17)->pushSigBack2(2,1);
        g->at(B18)->pushSigBack2(2,2);
        g->at(B19)->pushSigBack2(2,-2);
        g->at(B20)->pushSigBack2(2,3);
        g->at(B21)->pushSigBack2(3,0);
        g->at(B22)->pushSigBack2(-2,1);
        g->at(B23)->pushSigBack2(-2,2);
        g->at(B24)->pushSigBack2(-2,-2);
        g->at(B25)->pushSigBack2(-2,3);
        g->at(B26)->pushSigBack2(3,1);
        g->at(B27)->pushSigBack2(3,2);
        g->at(B28)->pushSigBack2(3,-2);
        g->at(B29)->pushSigBack2(3,3);
         
        // Links the nodes in the graph.
        g->addLink(s, B2);
        g->addLink(B2, B3);
        g->addLink(B2, B4);
        g->addLink(B3, B5);
        g->addLink(B4, B5);
        g->addLink(B5, B6);
        g->addLink(B6, B7);
        g->addLink(B7, B8);
        g->addLink(B7, B9);
        g->addLink(B8, B10);
        g->addLink(B9, B10);
        g->addLink(B10, B11);
        g->addLink(B6, B12);
        g->addLink(B12, B13);
        g->addLink(B12, B14);
        g->addLink(B13, B15);
        g->addLink(B14, B15);
        g->addLink(B15, B16);
        g->addLink(B11, B17);
        g->addLink(B17, B18);
        g->addLink(B17, B19);
        g->addLink(B18, B20);
        g->addLink(B19, B20);
        g->addLink(B20, B21);
        g->addLink(B16, B22);
        g->addLink(B22, B23);
        g->addLink(B22, B24);
        g->addLink(B23, B25);
        g->addLink(B24, B25);
        g->addLink(B25, B21);
        g->addLink(B21, B26);
        g->addLink(B26, B27);
        g->addLink(B26, B28);
        g->addLink(B27, B29);
        g->addLink(B28, B29);
        g->addLink(B29, e);

    }

}

void genNodes(int level) {
    Graph *g = new Graph();
    
    buildInitialGraph(g, level);
    
    if (level >= 1) {
       g->xmlNodes(); 
    }
}
int main() {
  Cgicc cgi;    // Ajax object

  // Create AJAX objects to recieve information from web page.
  form_iterator levelIt = cgi.getElement("levels");
  string level = **levelIt;
  int ilevel = atoi(level.c_str());
  if (ilevel<1) ilevel=4;

  // Debug to file example
  logFile.open("/tmp/skon.log",ios::out | ios::app);
  logFile << "Got: " <<  "levels:" << ":" << ilevel << endl;
  logFile.close();


  // output required message for AJAX
  cout << "Content-Type: text/plain\n\n";
  //cout << "Hello world!! << ilevel:" << ilevel;
  genNodes(ilevel);
}
