#ifndef __MYBINARYHEAP_H__
#define __MYBINARYHEAP_H__

#include <iostream>
#include <cstdlib>

#include "MyVector_e259o067.h"

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyBinaryHeap 
{
  private:
	MyVector<ComparableType> data;  // the array that holds the data elements

    // moves the data element at the pth position of the array up
    void percolateUp(const size_t p) 
    {
        // code begins
        size_t hole = p;
        ComparableType temp = std::move(data[p]);
        while(temp > data[hole/2]){
            if(hole/2 < 1) break;
            data[hole] = std::move(data[hole/2]);
            hole /= 2;
        }
        data[hole] = std::move(temp);
        // code ends
    }

    // moves the data element at the pth position of the array down
    void percolateDown(const size_t p) 
    {
        // code begins
        size_t parent = p;
        size_t child;
        ComparableType temp = std::move(data[p]);
        for(; parent*2 <= size(); parent = child){
            child = parent*2;
            if(child != size() && data[child+1] > data[child])
                ++child;
            if(data[child] > temp)
                data[parent] = std::move(data[child]);
            else break;
        }
        data[parent] = std::move(temp);
        // code ends
    }

    // reorders the data elements in the array to ensure heap property
    void buildHeap() 
    {
        // code begins
        for(int i = data.size()/2; i>0; --i)
            percolateDown(i);
        // code ends
    }	

  public: 

    // default constructor
    explicit MyBinaryHeap() :
        data(1)     // reserve data[0]
    {
        // code begins

        // code ends
    }

    // constructor from a set of data elements
    MyBinaryHeap(const MyVector<ComparableType>& items) : 
        data(1)     // reserve data[0]
    {
        // code begins
        for(size_t i=0; i<items.size(); ++i)
            data[i+1] = items[i];
        buildHeap();
        // code ends
    }

    // copy constructor
    MyBinaryHeap(const MyBinaryHeap<ComparableType>& rhs)
    {
        // code begins
        data = rhs.data;
        // code ends
    }

    // move constructor
    MyBinaryHeap(MyBinaryHeap<ComparableType> && rhs)
    {
        // code begins
        data = std::move(rhs.data);
        // code ends
    }

    // copy assignment
    MyBinaryHeap& operator=(const MyBinaryHeap<ComparableType>& rhs)   
    {
        // code begins
        MyBinaryHeap<ComparableType> copy = rhs;
        std::swap(*this, copy);
        return *this;
        // code ends
    }

    // move assignment
    MyBinaryHeap& operator=(MyBinaryHeap<ComparableType> && rhs)
    {
        // code begins
        std::swap(data, rhs.data);
        return *this;
        // code ends
    }

    // inserts x into the priority queue (copy)
	void enqueue(const ComparableType& x)
    {
        // code begins
        data.push_back(x);
        percolateUp(size());
        // code ends
    } 

    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        // code begins
        data.push_back(std::move(x));
        percolateUp(size());
        // code ends
    }

    // accesses the data element with the highest priority
	const ComparableType& front()
    {
        // code begins
        return data[1];
        // code ends
    } 

    // deletes the data element with the highest priority from the queue
    void dequeue()
    {
        // code begins
        if(empty()) return;
        data[1] = std::move(data.back());
        data.pop_back();
        percolateDown(1);
        // code ends
    }

    // verifies whether the array satisfies the heap property
    bool verifyHeapProperty(void)
    {
        // code begins
        for (size_t i=1; i<=size()/2; i++){
            if(data[2*i] > data[i]) 
                return false;
            if(data[2*i+1] > data[i]) 
                return false;
        }
        return true;
        // code ends
    }

    // disrupts heap property by random shuffling
    void disruptHeapProperty(void)
    {
        if(data.size() <= 3)
            return;
        for(size_t i = 0; i < 1000; ++ i)
        {
            size_t p = 1 + ((int) rand()) % (data.size() - 1);
            size_t q = 1 + ((int) rand()) % (data.size() - 1);
            std::swap(data[p], data[q]);
        }
        return;
    }

    // merges two heaps; the second heap can be destructed after the merge
    MyBinaryHeap& merge(MyBinaryHeap<ComparableType> && rhs) 
    {
        // code begins
        for(size_t i=1; i<=rhs.size(); i++){
            data.push_back(rhs.data[i]);
        }
        buildHeap();
        return *this;
        // code ends
    }

    // increases the priority measure of an element at a specific position and reorder the heap
	void increaseKey(const size_t p, const unsigned int d)
    {    
        // code begins
        data[p] = data[p] + d;
        percolateUp(p);
        // code ends
    }

    // decreases the priority measure of an element at a specific position and reorder the heap
    // if the current priority is smaller than the requested decrement, assign priority 0
	void decreaseKey(const size_t p, const unsigned int d) 
    {
        // code begins
        if(data[p] < d) data[p] = 0;
        else data[p] = data[p] - d;
        percolateDown(p);
        // code ends   
    }

    // checks whether the heap is empty
    bool empty()
    {
        // code begins
        return size() == 0;
        // code ends
    }

    // removes all data elements from the heap
    void clear()
    {
        // code begins
        while(size() != 0) data.pop_back();
        // code ends
    }

    // returns the size of the heap
    size_t size()
    {
        // code begins
        return data.size() - 1;
        // code ends
    }

    // prints the data in the array
    void print() 
    {
        const char delim = '\t';
        for (size_t i = 1; i < data.size(); ++ i) {
            std::cout << data[i] << delim;
        }
        std::cout << std::endl;
        return;
    }

};

#endif // __MYBINARYHEAP_H__