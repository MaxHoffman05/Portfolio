/**
 * Data Structures & Algorithms - Project 4 - Divide & Conquer Algorithms
 * 
 * Author: Max Hoffman
 * Date: 3/26/2024
 * Purpose: Object.hpp - Create Object class for a set of coordinates, name, and distance
 * 
*/
#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_
#include <iostream>

class Object {
private:
    std::string _name;//name of object
    double _xPoint;//x coordinate
    double _yPoint;//y coordinate
    double _distance;//distance it shares with another object
public:
    //constructors
    Object(): _name(), _xPoint(-1), _yPoint(-1), _distance(-1){}
    Object(const std::string nm, double x, double y): _name(nm), _xPoint(x), _yPoint(y), _distance(-1) {}
    
    //accessors and modifiers
    const std::string & name() const {return _name;}
    double xPoint() const {return _xPoint;}
    double yPoint() const {return _yPoint;}
    double distance() const {return _distance;}
    double & distance() {return _distance;}

    //overloads
    std::ostream &print(std::ostream &os) const;
    friend std::ostream& operator<<(std::ostream &os, const Object &itemToAdd);
    friend std::istream&  operator>> (std::istream &is, Object &ToAdd);
};

#endif