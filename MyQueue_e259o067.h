#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;

    // changes the size of the array to newSize
    void resize(size_t newSize)
    {
        if(newSize > capacity())
            reserve(newSize*2);
        MyVector<DataType>::resize(newSize);
    }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        if(newCapacity < size())
            return;
        DataType *newArray = new DataType[newCapacity];
        for(size_t i=0; i<size(); ++i)
            newArray[i] = std::move((*this)[(dataStart+i)%size()]);
        dataStart = 0;
        dataEnd = size();
        for(size_t i=0; i<size(); ++i)
            (*this)[i] = newArray[i];
        delete [] newArray;
        MyVector<DataType>::reserve(newCapacity);
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0) : MyVector<DataType>()
    {
        dataStart = dataEnd = 0;
    }

    // copy constructor
    MyQueue(const MyQueue & rhs) : 
        MyVector<DataType>(rhs),
        dataStart{rhs.dataStart},
        dataEnd{rhs.dataEnd}
    {
        
    }

    // move constructor
    MyQueue(MyQueue && rhs) : 
        MyVector<DataType>(std::move(rhs)),
        dataStart{rhs.dataStart},
        dataEnd{rhs.dataEnd}
    {
        
    }

    // destructor
    ~MyQueue()
    {
        
    }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
        MyQueue copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
        std::swap(dataStart, rhs.dataStart);
        std::swap(dataEnd, rhs.dataEnd);
        std::swap(*this, rhs);
        return(*this);
    }

    // insert x into the queue
    void enqueue(const DataType & x)
    {
        resize(size()+1);
        (*this)[dataEnd] = (x);
        dataEnd = ++dataEnd%capacity();
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        resize(size()+1);
        (*this)[dataEnd] = std::move(x);
        dataEnd = ++dataEnd%capacity();
    }

    // remove the first element from the queue
    void dequeue(void)
    {
        if(!empty())
        {
            dataStart = ++dataStart%capacity();
            resize(size()-1);
        }
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
        return(*this)[dataStart];
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        return MyVector<DataType>::empty();
    }

    // access the size of the queue
    size_t size() const
    {
        return MyVector<DataType>::size();
    }

    // access the capacity of the queue
    size_t capacity(void) const 
    {
        return MyVector<DataType>::capacity();
    }

};


#endif // __MYQUEUE_H__