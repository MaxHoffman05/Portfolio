//    FILE: Heap.hpp
//  AUTHOR: S. Blythe - Modified to take strings instead of ints by Max Hoffman
//    DATE: 1/20/2024
// PURPOSE: C++ class definition for a heap (data structure) containing strings.
//          Smallest value is at the "top" of the heap. 

//left all the code pretty much the same I just changed return types from ints to strings and changed the array of int pointers to string pointers
#ifndef _HEAP_HPP_
#define _HEAP_HPP_
#include <string>
using namespace std;

class Heap
{
private:
  std::string *_values;    // actual values of the heap, stored in an array
  int _lastIndex;  // index of the last element currently found in the heap
  int _size;       // max number of elements allowed in the heap. 
public:
  
  // constructor:
  //   size - parameter representing the maximum possible number of
  //    elements in the heap
  // notes:
  //  * the array is made large enough to store maximum specified
  //    number of values. For parent/child index calculations to be
  //    easy, index 0 is not used in this implementation
  //  * when starting up, no elements are in the array, so the "last"
  //    element placed in the array (before anything was actually
  //    placed) would be found at the unused index 0.
  Heap(int size) : _values(new string[size+1]), _lastIndex(0), _size(size) {}

  // the following are the two core methods for utilizing a heap.

  // adds a new element to the heap.
  //   - newVal - the new element to add. 
  bool add(string newVal);

  // removes the minimum element from the heap. 
  string removeMin();

  // For some users, the following accessor methods could be
  // uefull. You may or may not find them usefull, depending on what
  // you want to do with the heap and its storage.
  // In other words, DO NOT FEEL OBLIGATED TO USE THESE METHODS!!!
  string* contents() const {return _values;}   //return array holding heap values
  int lastIndex() const {return _lastIndex;}//return last heap element's index
  int size() const {return _size;}          //return max possible element count

private:
  // the following methods are really only usefull in the implementation
  // of the heap, so are made private here. If you find yourself wanting to
  // call these methods from outside of this class, you are probably making
  // things MILLIONS of times harder than they should be.

  // what is the index of the parent of the provided node's index?
  int parent(int index) {return index/2;}

  // what is the index of the left child of the provided node's index?
  int left(int index) {return 2*index;}
  // what is the index of the right child of the provided node's index?
  int right(int index) {return 2*index + 1;}

  // given a node's index, try to move it up the heap as far as possible. 
  void upHeap(int fromIndex);
  // given a node's index, try to move it down the heap as far as possible. 
  void downHeap(int fromIndex);
};
#endif
  
