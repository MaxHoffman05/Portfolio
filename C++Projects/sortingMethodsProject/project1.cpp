/*
Project 1 - Data Structures and Algorithms
Author : Max Hoffman
Date : 1/10/2024 - 1/20/2024

Goal : Get the times of different sorting methods to then compare them to each other

*/

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <iterator>

#include "Heap.hpp"

using namespace std;

//I used these sources for syntax
//bubble sort used diagram to help understand it: https://www.productplan.com/glossary/bubble-sort/
//List syntax and methods : https://cplusplus.com/reference/list/list/front/
//https://cplusplus.com/reference/list/list/pop_front/
//https://cplusplus.com/reference/list/list/
//vector syntax and methods : https://cplusplus.com/reference/vector/vector/
//I also modified the code from in class on how to build a heap of ints to a heap of strings

//print function to print lists
void printVec(vector<string> &  toPrint) {
     for(int i = 0; i < toPrint.size(); i++) {
        cout << toPrint.at(i) << endl;
    }
}
void printList(list<string>  toPrint) {
    for(string val : toPrint) {
        cout << val << endl;
    }
}

//used for bubble sort, when after going through the whole vector and switching if they can be switched this is used to check if we are done yet
bool checkVector(vector<string> toCheck) {
    //check every item against the ones above it
    //starting at the first item then going up if this all is true then return true if not we still need to keep swapping
    for(int i = 0; i < toCheck.size() - 1; i++) {
        for(int k = i + 1; k < toCheck.size() - 1; k++) {
            if(toCheck.at(i) > toCheck.at(k)) {
                return false;
            }
        }
    }
    return true;//we made it here so the whole vector is correct
}

//each of the sort methods
vector<string> bubbleSort(vector<string> & toSort) {
    bool flag = true;
    string tempVar;
    while(flag) {
        //check and switch if applicable
        for(int i = 0; i < toSort.size() - 1; i++) {
                
                if(toSort.at(i) > toSort.at(i+1)) {
                    //cout << "TEST1!!" << endl;
                    //switch them
                    tempVar = toSort.at(i);
                    //cout << "TEMP VAR " << tempVar << endl;

                    toSort.at(i) = toSort.at(i+1);

                    toSort.at(i+1) = tempVar;
                }
        }
        //base case is that all strings do not have to be switched at all
        //so now we check that if not we keep doing this until that is met
        if(checkVector(toSort)) {
            flag = false;
        }
    }
    return toSort;
}

//helper method used for merge sort, based off of the psuedocode from the slides
//this took a lot longer than I thought it would because I kept going between using vectors or using lists
//this method combines all the lists at the end by comparing them to each other
list<string> merge(list<string> list1, list<string> list2) {
    list<string> toReturn;
    while(list1.size() != 0 && list2.size() != 0) {
        if(list1.front() < list2.front()) {
            toReturn.push_back(list1.front());
            list1.pop_front();
        }
        else {
            toReturn.push_back(list2.front());
            list2.pop_front();
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
//followed slides psuedocode for merge sort
//I spent a couple hours trying to figure out what was wrong then realized I needed to pass toSort by Reference and that solved my issues
list<string> mergeSort(list<string> & toSort) {
    if(toSort.size() <= 1) {
        return toSort;
    }
    else {
        //get our two sizes
        int size1 = toSort.size() / 2;
        int size2 = toSort.size() - size1;

        //create two vectors and load the values into them
        list<string> list1;
        list<string> list2;
        
        //how do we take the parts of the list?
        //take the first part of the original list and make a new list with size1
        //I dont know if there was a better way of deleting the first element of the list 
        //but I didnt want to use an iterator and this was the best that I could come up with
        for(int q = 0; q < size1; q++) {
            string newVal = toSort.front();
            toSort.pop_front();
            list1.push_back(newVal);
        }
        
        //do the same with another list this splits the original list in half
        for(int w = 0; w < size2; w++) {
            string newVal = toSort.front();
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

//method for quick Sort, based off of the slides from class
list<string> quickSort(list<string> toSort) {
    
    list<string> lessL;
    list<string> greaterL;
    list<string> equalL;
  
    if(toSort.size() <= 1) {//base case 
        return toSort;
    }
    //choose the pivot
    string pivot = toSort.front();
    vector<string> tempVec;//temporary vector used later
    equalL.push_back(pivot);//add the pivot to the equal list

    //remove() will not work here since it deletes ALL instances of the pivot which was causing me to not have duplicates that I had in the input file!
    //because I was using lists and didnt know how to use iterators I had to copy the list to a vector and do some trickery converting it back into a list
    //this is definitely not the ideal way this might have been better to use vectors instead of lists.
    int totalSize = toSort.size();
    toSort.pop_front();
    for(int i = 1; i < totalSize; i++) {
        tempVec.push_back(toSort.front());
        toSort.pop_front();
    }//copy to vec

    for(int i = 0; i < tempVec.size(); i++) {
        toSort.push_back(tempVec.at(i));
    }
 
    //now we need to partition the list and add the items to the correct lists
    while(toSort.size() != 0) {
        string valToAdd = toSort.front();
        toSort.pop_front();
        if(valToAdd < pivot) {
            lessL.push_back(valToAdd);
        }
        else if(valToAdd == pivot) {
            equalL.push_back(valToAdd);
           
        }
        else {
            greaterL.push_back(valToAdd);
        }
    }

    //realized I was forgetting to set the lists equal to the recursive call
    lessL = quickSort(lessL);
    greaterL = quickSort(greaterL);
    //put the lists together and return the completed list
    lessL.merge(equalL);
    lessL.merge(greaterL);
    return lessL;
}


//heap sort method, uses modified heap class provided in class
vector<string> heapSort(vector<string> passedVec) {
    Heap newHeap(passedVec.size());//we use the heap class from the in class example 
    //create the heap from the items of the vector passed in
    for(int i = 0; i < passedVec.size(); i++) {
        newHeap.add(passedVec.at(i));
    }
    //now we need to pull off the items in the right order in order to create the sorted array since it is only a heap right now
    vector<string> res;
    for(int i = 0; i < newHeap.size(); i++) {
        res.push_back(newHeap.removeMin());
    }
    return res;
}

int main(int argc, char *argv[]) {
    int numOfDataVals = 0;
    vector<string> toSort;
    list<string> toSortL;

    //setup file
    ifstream inputFile(argv[1]);
    
    if(!inputFile) {
        cout << "That file doesnt exist!" << endl;;
    }
    else {
        //get the type of sort
        string sortType = argv[2];
        vector<string> newlySorted;
        list<string> sortedList;

        //first get first part of input file which shows the num of items
        string nextVal;
        inputFile >> numOfDataVals;//store the number of items in file
        inputFile >> nextVal;
        //load file into list to then be sorted
        while(inputFile) {  
            toSort.push_back(nextVal);
          
            sortedList.push_back(nextVal);
            inputFile >> nextVal;
        }
        newlySorted = toSort;
        
        //each type of sort
        if(sortType == "bubble") {
            newlySorted = bubbleSort(toSort);

        }
        else if(sortType == "merge") {
            sortedList = mergeSort(sortedList);
        }
        else if(sortType == "quick") {
            sortedList = quickSort(sortedList);
            
        }
        else if(sortType == "heap") {
            
            newlySorted = heapSort(toSort);
        }
        else if(sortType == "sys") {
            sortedList.sort();
            newlySorted.clear();
            //set the vector equal to this list
            for(string val : sortedList) {
                newlySorted.push_back(val);
            }
        }
        //check if we need to print the results
        if( (string) argv[argc - 1] == "-print") {
            if(sortType == "merge" || sortType == "quick") {//since I used lists instead of vectors for merge sort and quick sort we print it out differently than vectors
                printList(sortedList);
            }
            else {
                printVec(newlySorted);  
            }
             cout << endl;
        }
    }
    inputFile.close();
}