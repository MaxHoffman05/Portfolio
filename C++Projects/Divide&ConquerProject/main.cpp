/**
 * Data Structures & Algorithms - Project 4 - Divide & Conquer Algorithms
 * 
 * Author: Max Hoffman
 * Date: 3/26/2024 - 4/3/2024
 * Purpose: Main.cpp - Make a divide and conquer algorithm to get the minimum distance between two objects that have a x and y coordinate.
 * 
*/

#include "Object.hpp"
#include <iostream>
#include <fstream>
#include <list>
#include <math.h>//for the pow and sqrt function

using namespace std;


bool sortByY = false;//helps us sort by y point later
int begLine = 0;//dividing line
Object inf = Object("inf",INFINITY, INFINITY);//infinity object we return if there is only one point on one of the sides of the vertical line
list<Object> leftRightMins;//used to print out the object names and coordinates later if they end up being used for the minimum
const int CUTOFF = 12;
list<Object> cpy;
list<Object> midList;//list for all 'middle points' that are in the middle strip of the plane

//same merge sort I used for past projects that I made for project 1, repurposed for lists of objects
list<Object> merge(list<Object> list1, list<Object> list2) {//O(nlogn)
    list<Object> toReturn;
    while(list1.size() != 0 && list2.size() != 0) {
        if(sortByY) {
             if(list1.front().yPoint() < list2.front().yPoint()) {
                toReturn.push_back(list1.front());
                list1.pop_front();
            }
            else {
                toReturn.push_back(list2.front());
                list2.pop_front();
            } 
        }
        else {
            if(list1.front().xPoint() < list2.front().xPoint()) {
                toReturn.push_back(list1.front());
                list1.pop_front();
            }
            else {
                toReturn.push_back(list2.front());
                list2.pop_front();
            } 
        }
    }
    while(list1.size() != 0) {
        toReturn.push_back(list1.front());
        list1.pop_front();
    }
    while(list2.size() != 0) {
        toReturn.push_back(list2.front());
        list2.pop_front();
    }
    return toReturn;
}

//merge sort
list<Object> mergeSort(list<Object> & toSort) {//O(nlogn)
    if(toSort.size() <= 1) {
        return toSort;
    }
    else {
        //get our two sizes
        int size1 = toSort.size() / 2;
        int size2 = toSort.size() - size1;

        //create two vectors and load the values into them
        list<Object> list1;
        list<Object> list2;
        
        //how do we take the parts of the list?
        //take the first part of the original list and make a new list with size1
        //I dont know if there was a better way of deleting the first element of the list 
        //but I didnt want to use an iterator and this was the best that I could come up with
        for(int q = 0; q < size1; q++) {
            Object newVal = toSort.front();
            toSort.pop_front();
            list1.push_back(newVal);
        }
        
        //do the same with another list this splits the original list in half
        for(int w = 0; w < size2; w++) {
            Object newVal = toSort.front();
            toSort.pop_front();
            list2.push_back(newVal);
        }
        
        //recursively call
        mergeSort(list1);
        mergeSort(list2);
        toSort = merge(list1, list2);//merge the lists together then return the final list
        return toSort;
    }  
}

//recursive solve method that uses divide and conquer
//we return a pair of objects which both have thier distance variable set to the distance between them
//this was the best way I could end up printing out the information of each point with the distance at the end
pair<Object,Object> solve(list<Object> & fullL) {
    if(fullL.size() == 0) {//base case 1: we have no points in the list so we return an infinite item
        inf.distance() = INFINITY;//also set the distance to infinity
        return pair(inf, inf);
    }
    else if(fullL.size() == 1) {//base case 2: we have only one point in this list so we need to return an infinite item to not interfere with the actual minimum
        inf.distance() = INFINITY;
        return pair(inf,inf);
    }
    else if(fullL.size() == 2) {//base case 3: if we have two points we can find the distance between them
        //compute distance formula
        double dist = sqrt(pow(fullL.front().xPoint()-fullL.back().xPoint(),2) + pow(fullL.front().yPoint()-fullL.back().yPoint(),2));
        fullL.front().distance() = dist;//set each objects distance param equal to dist
        fullL.back().distance() = dist;
        if(leftRightMins.size() == 0) {//setup our global list to print these objects out later
            leftRightMins.push_back(fullL.front());
            leftRightMins.push_back(fullL.back());
        }
        else {
            if(dist < leftRightMins.front().distance()) {//new minimum so we need to clear out the list and replace it
                leftRightMins.clear();
                leftRightMins.push_back(fullL.front());
                leftRightMins.push_back(fullL.back());
            }
        }
        return pair(fullL.front(), fullL.back());//return our pair of points
    }
    else {//else: divide the list in half
        int line = fullL.size() / 2;
        //split our list into two lists
        list<Object> leftHalf;
        list<Object> rightHalf;
        for(int i = 0; i < line; i++) {//O(logn)
            leftHalf.push_back(fullL.front());
            fullL.pop_front();
        }
        rightHalf.swap(fullL);//O(1)
        
        if(begLine == 0) {//make sure we only do this part once
            begLine = (leftHalf.back().xPoint() + rightHalf.front().xPoint()) / 2;//calculate the x coord of where our starting 'vertical line' is by taking the average of both the nearest points to the line
        }

        //recursively solve each side
        pair<Object,Object> min1 = solve(leftHalf);
        pair<Object,Object> min2 = solve(rightHalf);

        //create new pair for our minimum objects
        pair<Object,Object>  minObject;
        if(min1.first.distance() < min2.first.distance()) {//return the minimum of each pair
            return min1;
        }
        else {
            return min2;
        }
    }
}   

int main(int argc, char *argv[]) {

    if(!argv[1]) {
        cerr << "No input file name given! e.x: ./runMe input.txt" << endl;
        return 1;
    }
    else { 
        ifstream file(argv[1]);
        int numObjects;//total number of objects in the sky
        file >> numObjects;
        int currSize = 0;
        list<Object> list;
        string name;//all the parameters to make an object that we read in
        string xPos;
        string yPos;
        if(!file) {
            cout << "file does not exist!" << endl;
            return 0;
        }
        
        while(file) {//O(n)
            file >> name;
            file >> xPos;
            file >> yPos;
            Object  newObj = Object(name, atof(xPos.c_str()), atof(yPos.c_str()));//create our new object
            if(currSize < numObjects) {//prevents us from adding the last item twice
                list.push_back(newObj);
                currSize += 1;
            }
        }

        mergeSort(list);//sort the list by x coordinate
        cpy = list;//temporary copy of list before it gets deleted in the solve() method
        pair<Object, Object> obj = solve(list);//recursive method

        //3. combine solutions of each into overall solution
        //now do middle strip on the plane
        sortByY = true;//next time we do merge sort we sort by y value now instead of x
        list.swap(cpy);//since we ended up popping things off of list in solve() we saved a copy before solve() call

        double minDistance = obj.first.distance();//sigma
        for(Object i : list) {//O(n)
            if((i.xPoint() >= begLine && i.xPoint() <= begLine + minDistance) || (i.xPoint() <= begLine && i.xPoint() >= begLine + minDistance)) {
                midList.push_back(i);
            }
        }

        double minMid = INFINITY;
        Object prev;
        pair<Object,Object> midObj;//minimum pair of two objects

        mergeSort(midList);//Sort by y value in the middle strip like how we discussed in class, O(nlogn)
        //calculate distances on the middle strip of points
        int curr = 0;
        for(Object i : midList) {//O(n)
            if(curr == CUTOFF) {//only look at up to 12 points based off of what we discussed in class
                break;
            }
            if(prev.xPoint() != -1 && prev.yPoint() != -1) {
                double distanceMid = sqrt(pow(prev.xPoint()-i.xPoint(),2) + pow(prev.yPoint()-i.yPoint(),2));//calculate the distance
                if(distanceMid <= minMid) {//check if this is the new minimum
                    minMid = distanceMid;
                    prev.distance() = distanceMid;
                    i.distance() = distanceMid;
                    midObj.first = prev;//set our pair to hold both points that are our new minimums
                    midObj.second = i;
                }
            }
            prev = i;
            curr += 1;
        }

        
        if(minMid < obj.first.distance()) {//if the middle minimum distance was smaller that is our minimum for all the points
            cout << "Minimum Distance : " << minMid << endl;
            cout << "Between Objects : " << midObj.first << " and " << midObj.second << endl;
        }
        else {//else it was a left or right return
            cout << "Minimum Distance : " << obj.first.distance() << endl;
            cout << "Between Objects : " << leftRightMins.front() << " and " << leftRightMins.back() << endl;//we saved the objects in a list back in the solve method
        }
        return 0;
    }
}