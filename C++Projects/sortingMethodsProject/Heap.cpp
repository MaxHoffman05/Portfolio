//    FILE: Heap.cpp
//  AUTHOR: S. Blythe - Modified by Max Hoffman to build a heap of strings instead of ints
//    DATE: 1/20/2024
// PURPOSE: C++ class implementation for a heap (data structure)
//  containing strings.  Smallest value is at the "top" of the heap.

//left all the code pretty much the same I just changed return types from ints to strings and changed the array of int pointers to string pointers
#include "Heap.hpp"

// adds a node to the heap. Returns true if successful, false otherwise. 
bool
Heap::add(string newVal)
{
  // if there is no room left in the heap, then adding is not
  // possible.
  // i.e. if the last addition ended up putting a node in
  // the last slot of the array, there's no room left for another
  // value.
  if (_lastIndex>=_size)
    return false;

  // we've got one more element in the heap ...
  _lastIndex++;

  // store that element in the next open slot ...
  _values[_lastIndex]=newVal;

  // ... but that may not be the right place to put this new value, so
  // upheap the node until it is in the right place.
  upHeap(_lastIndex);

  // if we get here, evrything went fine. 
  return true;
}

// remove the min value from the heap (ie. the "root")
string
Heap::removeMin()
{
  // get the min value;
  string mval = _values[1];

  // we are removing something, so "move" the last element to the root ...
  _values[1] = _values[_lastIndex];
  // ... and remember that we now have one fewer element in the heap. 
  _lastIndex--;

  // unfortunately, the element we just moved to the top/root is probably
  // not the new min value. Keep pushing it down the heap (from the
  // root) until it is appropriately re-located.
  downHeap(1);

  //return the min value
  return mval;
}



// ~~~~~~ PRIVATE METHODS FOLLOW ~~~~~~


// takes a value found at the passed in index (fromIndex) and moves it
// up the heap until it is properly located. NOTE: private method.
void
Heap::upHeap(int fromIndex)
{
  // as long as current node (found at fromIndex) has a parent and that parent
  // is bigger than the current Node ...
  while (parent(fromIndex)!=0 && _values[fromIndex]<_values[parent(fromIndex)])
    {
      // ... swap the parent and the current node ...
      string temp =_values[fromIndex];
      _values[fromIndex]=_values[parent(fromIndex)];
      _values[parent(fromIndex)] = temp;

      // ... and repeat at the "new" parent
      fromIndex = parent(fromIndex);
    }
}

// takes a value found at the passed in index (fromIndex) and moves it
// down the heap until it is properly located. NOTE: private method.
void
Heap::downHeap(int fromIndex)
{
  bool movedDown; // have we moved down at all? 
  
  do{
    movedDown=false; // we have not (yet) moved a node down in this pass.

    // find the minimum value of the current node and its (possible) children
    //   start by assuming the current node is the minimum
    int minIndex=fromIndex;

    // if left child exists and is smaller, then the left child is a
    // better guess at the minimum.
    if (left(fromIndex) <= _lastIndex &&
	_values[left(fromIndex)]< _values[minIndex])
      minIndex = left(fromIndex);

    // if right child exists and is smaller, then the right child is
    // the minumum
    if (right(fromIndex) <= _lastIndex &&
	_values[right(fromIndex)]< _values[minIndex])
      minIndex = right(fromIndex);

    // if the current node is not the minimim, we need to swap with
    // the smaller child.
    if (minIndex!=fromIndex)
      {
	// do the swap with the smaller child. 
	string temp = _values[fromIndex];
	_values[fromIndex]=_values[minIndex];
	_values[minIndex]=temp;

	// update the fromIndex to be the smaller child's index. That is,
	// move down the heap one position
        fromIndex=minIndex;

	// we have moved down, so set the boolean to make sure we try
	// to move this node down (at least) one more time
	movedDown=true;
      }
  }
  while(movedDown);  // stop when we didn't have to move the crrent node down
}
