/**
 * Data Structures & Algorithms - Project 4 - Divide & Conquer Algorithms
 * 
 * Author: Max Hoffman
 * Date: 3/26/2024
 * Purpose: Object.cpp - Create Object class for a set of coordinates, name, and distance
 * 
*/
#include "Object.hpp"
#include <iostream>
using namespace std;

//same functions from DeadlineJob.cpp example we did in class
ostream & Object::print(std::ostream &os) const {
    os << "[ " << _name
    << ":(" << _xPoint 
    << "," << _yPoint << ")" << "]";
    return os;
}

std::ostream&  operator<< (std::ostream &os, const Object &itemToPrint) {
    return itemToPrint.print(os);
}

std::istream&  operator>> (std::istream &is, Object &toAdd) {
    is >> toAdd._name;
    is >> toAdd._xPoint;
    is >> toAdd._yPoint;
    return is;
}