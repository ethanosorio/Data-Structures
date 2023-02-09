#ifndef __MYLEFTISTHEAP_H__
#define __MYLEFTISTHEAP_H__

#include <iostream>
#include <cstdlib>
#include <stdexcept>

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyLeftistHeap
{
  public:
    // definition of HeapNode
    struct HeapNode
    {
        ComparableType data;
        HeapNode* left;
        HeapNode* right;
        HeapNode* parent;
        int npl;

        HeapNode(const ComparableType & d, HeapNode* lt = nullptr, HeapNode* rt = nullptr, HeapNode* pt = nullptr, int np = 0) :
            data{d},
            left{lt},
            right{rt},
            parent{pt},
            npl{np}
        {
            ;
        }

        HeapNode(ComparableType && d, HeapNode* lt = nullptr, HeapNode* rt = nullptr, HeapNode* pt = nullptr, int np = 0) :
            data{std::move(d)},
            left{lt},
            right{rt},
            parent{pt},
            npl{np}
        {
            ;
        }
    };

    // default constructor
    MyLeftistHeap() :
        root{nullptr},
        theSize(0)
    {
        // code begins

        // code ends
    }

    // copy constructor
    MyLeftistHeap(const MyLeftistHeap& rhs) :
        root{nullptr},
        theSize(rhs.theSize)
    {
        // code begins
        root = clone(rhs.root);
        // code ends
    }

    // move constructor
    MyLeftistHeap(MyLeftistHeap && rhs) :
        root{rhs.root},
        theSize(rhs.theSize)
    {
        // code begins
        rhs.root = nullptr;
        // code ends
    }

    // destructor
    ~MyLeftistHeap( )
    {
        // code begins
        clear();
        // code ends
    }

    // copy assignment
    MyLeftistHeap& operator=(const MyLeftistHeap& rhs)
    {
        // code begins
        MyLeftistHeap<ComparableType> copy = rhs;
        std::swap(*this, copy);
        return *this;
        // code ends
    }

    // move assignment
    MyLeftistHeap& operator=(MyLeftistHeap && rhs)
    {
        // code begins
        std::swap(root, rhs.root);
        std::swap(theSize, rhs.theSize);
        return *this;
        // code ends
    }

    // returns the data element with the highest priority
    const ComparableType& front() const
    {
        // code begins
        return root->data;
        // code ends
    }

    // inserts x into the priority queue (copy)
    void enqueue(const ComparableType& x)
    {
        // code begins
        root = merge(new HeapNode{x}, root);
        theSize++;
        // code ends
    }

    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        // code begins
        root = merge(new HeapNode{std::move(x)}, root);
        theSize++;
        // code ends
    }

    // deletes the data element with the highest priority from the queue
    void dequeue()
    {
        // code begins
        if(empty()) return;
        HeapNode *old = root;
        root = merge(root->left, root->right);
        delete old;
        theSize--;
        // code ends
    }

    // increases the priority measure of the data element locates at node t
	void increaseKey(HeapNode *t, const unsigned int d)
    {
        // code begins
        t->data = t->data + d;
        percolateUp(t);
        // code ends
    }

    // decreases the priority measure of the data element locates at node t
    // if the current priority is smaller than the requested decrement, assign priority 0
	void decreaseKey(HeapNode* t, const unsigned int d)
    {
        // code begins
        if(t->data < d) t->data = 0;
        else t->data = t->data - d;
        percolateDown(t);
        // code ends
    }

    // merges two leftist heaps
    void merge(MyLeftistHeap && rhs)
    {
        // code begins
        if(this == &rhs) return;
        root = merge(root, rhs.root);
        theSize += rhs.theSize;
        rhs.root = nullptr;
        rhs.theSize = 0;
        // code ends
    }

    // verifies whether the subtree rooted at t satisfies the heap property
    bool verifyHeapProperty(void)
    {
        // code begins
        return verifyHeapProperty(root);
        // code ends
    }

    // returns a random node in the heap
    HeapNode* randNode(void)
    {
        if(empty())
            return nullptr;

        size_t num_trials = theSize;    // the number of steps of the random walk
        HeapNode* n = root;             // begins the random walk at the root
        // defines all three neighbors of a given node as: the parent, the left child, and the right child
        // they are stored in order in the size-3 array "neighbors"
        HeapNode** neighbors = new HeapNode* [3];
        for(size_t i = 0; i < num_trials; ++ i)
        {
            neighbors[0] = n->parent;
            neighbors[1] = n->left;
            neighbors[2] = n->right;
            // picks a random neighbor to proceed
            size_t k = ((int) rand()) % 3;
            while(neighbors[k] == nullptr)
            {
                ++ k;
                if(k > 2)   k -= 3;
            }
            n = neighbors[k];
        }
        delete [] neighbors;
        return n;
    }

    // disrupts heap property by randomly shuffling the nodes
    void disruptHeapProperty(void)
    {
        if(empty())
            return;

        HeapNode* n = root;             // begins the random walk at the root
        // defines all three neighbors of a given node as: the parent, the left child, and the right child
        // they are stored in order in the size-3 array "neighbors"
        HeapNode** neighbors = new HeapNode* [3];
        HeapNode** path = new HeapNode* [theSize];
        for(size_t i = 0; i < theSize; ++ i)
        {
            neighbors[0] = n->parent;
            neighbors[1] = n->left;
            neighbors[2] = n->right;
            // picks a random neighbor to proceed
            size_t k = ((int) rand()) % 3;
            while(neighbors[k] == nullptr)
            {
                ++ k;
                if(k > 2)   k -= 3;
            }
            path[i] = n = neighbors[k];
        }
        // swap
        for(size_t i = 0; i < theSize; ++ i)
        {
            size_t x = ((int) rand()) % theSize;
            size_t y = ((int) rand()) % theSize;
            std::swap(path[x]->data, path[y]->data);
        }
        delete [] neighbors;
        delete [] path;
        return;
    }

    // disrupts heap topology by randomly swapping left-right children
    void disruptLeftistTopology(void)
    {
        if(empty())
            return;

        for(size_t i = 0; i < theSize; ++ i)
        {
            HeapNode* t = randNode();
            std::swap(t->left, t->right);
        }
        return;
    }

    // clears all data in the queue
    void clear()
    {
        // code begins
        clear(root);
        root = nullptr;
        // code ends
    }

    // checks of the heap is empty
    bool empty() const
    {
        // code begins
        return theSize == 0;
        // code ends
    }

    // checks the size of the heap
    size_t size() const
    {
        // code begins
        return theSize;
        // code ends
    }

  private:

    HeapNode* root; // root of the heap tree
    size_t theSize; // the number of data elements

    // merges the two trees rooted at r1 and r2, respectively
    HeapNode* merge(HeapNode* r1, HeapNode* r2)
    {
        // code begins
        if(r1 == nullptr) return r2;
        if(r2 == nullptr) return r1;
        if(r1->data > r2->data) return mergeSubtree(r1, r2);
        else return mergeSubtree(r2, r1);
        // code ends
    }

    // merges st (sub-tree) with the right subtree of mt (main-tree)
    HeapNode* mergeSubtree(HeapNode* mt, HeapNode* st)
    {
        // code begins
        if(mt->left == nullptr){
            mt->left = st;
            mt->left->parent = mt;
        }
        else{
            mt->right = merge(mt->right, st);
            mt->right->parent = mt;
            if(mt->left->npl < mt->right->npl) swapChildren(mt);
            mt->npl = mt->right->npl + 1;
        }
        return mt;
        // code ends
    }

    // swaps the two children of the node t
    void swapChildren(HeapNode* t)
    {
        // code begins
        std::swap(t->left, t->right);
        // code ends
    }

    // deletes the subtree rooted at t
    void clear(HeapNode *t)
    {
        // code begins
        if(t != nullptr){
            clear(t->left);
            clear(t->right);
            delete t;
            theSize--;
        }
        t = nullptr;
        // code ends
    }

    // clones the tree rooted at t; returns the root of the cloned tree
    HeapNode* clone(HeapNode* t)
    {
        // code begins
        if(t == nullptr) return nullptr;
        else{
            HeapNode* copy = new HeapNode(t->data, nullptr, nullptr, nullptr, t->npl);
            copy->left = clone(t->left);
            if(copy->left != nullptr) copy->left->parent = copy;
            copy->right = clone(t->right);
            if(copy->right != nullptr) copy->right->parent = copy;
            return copy;
        }
        // code ends
    }

    // percolates the node t up the heap
    void percolateUp(HeapNode* t)
    {
        // code begins
        if(t->parent == nullptr) return;
        if(t->data > t->parent->data){
            std::swap(t->data, t->parent->data);
            percolateUp(t->parent);
        }
        // code ends
    }

    // percolates the node t down the heap
    void percolateDown(HeapNode* t)
    {
        // code begins
        if(t->left == nullptr && t->right == nullptr) return;
        char path = 'n';
        if(t->left != nullptr && t->right != nullptr){
            if(t->left->data > t->data && t->left->data > t->right->data)
                path = 'l';
            else if(t->right->data > t->data && t->right->data > t->left->data)
                path = 'r';
        }
        else if(t->left != nullptr && t->left->data > t->data) path = 'l';
        else if(t->right != nullptr && t->right->data > t->data) path = 'r';
        switch(path){
            case 'l':
                std::swap(t->data, t->left->data);
                percolateDown(t->left);
                break;
            case 'r':
                std::swap(t->data, t->right->data);
                percolateDown(t->right);
                break;
            default:
                return;
        }
        // code ends
    }

    // verifies whether the subtree rooted at t satisfies the heap property and leftist topology
    bool verifyHeapProperty(HeapNode* t)
    {
        // code begins
        if(t == nullptr) return true;
        if(t->left == nullptr && t->right != nullptr) return false;
        if(t->left != nullptr && t->right != nullptr && t->left->npl < t->right->npl) return false;
        if(t->parent != nullptr && t->parent->data < t->data) return false;
        bool v1 = verifyHeapProperty(t->left);
        bool v2 = verifyHeapProperty(t->right);
        return v1 && v2;
        // code ends
    }

};

#endif // __MYLEFTISTHEAP_H__
