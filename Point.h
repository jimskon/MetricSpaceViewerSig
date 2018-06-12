#ifndef POINT_H
#define POINT_H
#include <iostream>

using namespace std;

class Point
{
public:
    //Default constructor
    Point();
    Point(double new_x, double new_y);
    /*******Member Functions*******/
    //Accessors
    double GetX();
    double GetY();
    //Mutators
    void SetX(double x);
    void SetY(double Y);
private:
    double xCOORD, yCOORD;
};

#endif // POINT_H 