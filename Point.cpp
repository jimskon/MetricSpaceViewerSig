#include "Point.h"
#include <iostream>

using namespace std;
              //INITIALIZES
Point::Point():xCOORD(0), yCOORD(0)
{
   //default constructor
   //does nothing
}

//Constructor acts as a mutator
//to get values

Point::Point(double new_x, double new_y)
{
    xCOORD = new_x;
    yCOORD = new_y;
}

//MUTATOR FUNCTIONS
void Point::SetX(double x)
{
    xCOORD = x;
}
void Point::SetY(double y)
{
    yCOORD = y;
}


//ACCESSOR FUNCTIONS
//This is how it is done
//In order to return a Private variable's value.
//In order to access a private variable's value.
double Point::GetX()
{
    return xCOORD;
}
double Point::GetY()
{
    return yCOORD;
}
