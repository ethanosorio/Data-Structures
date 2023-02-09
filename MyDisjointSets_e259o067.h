#ifndef _MY_DISJOINTSETS_H_
#define _MY_DISJOINTSETS_H_

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "MyVector_e259o067.h"

class MyDisjointSets 
{
  public:
    // constructor
	explicit MyDisjointSets(const size_t size)
	{
        // code begins
        for(int i=0; i<size; i++) parents.push_back(-1);
        numSets = theSize = size;
        // code ends
	}

    // destructor
    ~MyDisjointSets(void)
    {
        // code begins

        // code ends
    }
	
    // finds the ID of the set that contains x
    // implements path compression
    int find(const int x)
    {
        // code begins
        if(parents[x] < 0) return x;
        else return parents[x] = find(parents[x]);
        // code ends
    }

    // unions the two sets represented by r1 and r2, respectively
    // (r1 and r2 should be roots)
    // implements the union-by-size approach
	void unionSets(const int r1, const int r2)
	{
        // code begins
        parents[find(r1)] = parents[find(r1)] + parents[find(r2)];
        parents[find(r2)] = find(r1);
        numSets--;
        // code ends
	}

    // returns the size of the set that contains x
    size_t setSize(const int x)
    {
        // code begins
        return abs(parents[find(x)]);
        // code ends
    }

    // returns the total number of elements in the disjoint set
    size_t size()
    {
        // code begins
        return theSize;
        // code ends
    }

    // returns the number of disjoint sets
    size_t numDisjointSets()
    {
        // code begins
        return numSets;
        // code ends
    }

  private:
	size_t theSize;             // the number of elements in the disjoint sets
    size_t numSets;             // the number of disjoint sets maintained
    MyVector<int> parents;      // the info array; if the index is not the root, it contains the ID of its parent; otherwise it contains the size of the set as a negative number

};

#endif  // _MY_DISJOINTSETS_H_
