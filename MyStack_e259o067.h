#ifndef __MYSTACK_H__
#define __MYSTACK_H__

#include <iostream>
#include <algorithm>

#include "MyVector.h"

template <typename DataType>
class MyStack : private MyVector<DataType>
{  
  public:

    // default constructor
    explicit MyStack(size_t initSize = 0)
    {
        
    }

    // copy constructor
    MyStack(const MyStack & rhs) : MyVector<DataType>(rhs)
    {
        
    }

    // move constructor
    MyStack(MyStack && rhs) : MyVector<DataType>(std::move(rhs))
    {
        
    }

    // destructor
    ~MyStack()
    {

    }

    // copy assignment
    MyStack & operator= (const MyStack & rhs)
    {
        MyStack copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // move assignment
    MyStack & operator= (MyStack && rhs)
    {
        std::swap(*this, rhs);
        return(*this);
    }

    // insert x to the stack
    void push(const DataType & x)
    {
        MyVector<DataType>::push_back(x);
    }

    // insert x to the stack
    void push(DataType && x)
    {
        MyVector<DataType>::push_back(x);
    }

    // remove the last element from the stack
    void pop(void)
    {
        MyVector<DataType>::pop_back();
    }

    // access the last element of the stack
    const DataType & top(void) const
    {
        return MyVector<DataType>::back();
    }

    // check if the stack is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        return MyVector<DataType>::empty();
    }

    // access the size of the stack
    size_t size() const
    {
        return MyVector<DataType>::size();
    }

    // access the capacity of the stack
    size_t capacity(void) const 
    {
        return MyVector<DataType>::capacity();
    }

};


#endif // __MYSTACK_H__