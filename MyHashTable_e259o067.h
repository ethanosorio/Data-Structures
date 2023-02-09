#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector_e259o067.h"
#include "MyLinkedList_e259o067.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
    // code begins
    return(n % mersenne_prime);
    // code ends
}

// definition of the template hash function class
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
        long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;      // the base used for inflating each character
  public:
    long long univHash(const std::string& key, const long long table_size) const
    {
        long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {
            hv = param_base * hv + static_cast<long long>(key[i]);
        }
        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// definition of the template hashed object class
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:

    KeyType key;
    ValueType value;

    HashedObj()
    {
        return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {
        return;
    }

    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {
        return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
        return (key == rhs.key);
    }

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
        return !(*this == rhs);
    }

};

template <typename KeyType, typename ValueType>
class MyHashTable
{
  private:
    size_t theSize;     // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> > > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
        // code begins
        MyVector<size_t> vector;
        bool arr[n + 1] = {0};
        for (int i=2; i<=n; i++)
            for (int j=i*i; j<=n; j+=i)
                arr[j] = 1;
        for (int i=2; i<=n; i++)
            if (!arr[i]){
                vector.push_back(i);}
        primes = std::move(vector);
        // code ends
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
        // code begins
        if(n > primes.back())
            preCalPrimes(n*2);
        if(n < 2)
            return(2);
        size_t low = 0;
        size_t high = primes.size() - 1;
        size_t mid = (low + high) / 2;
        while (low <= high) {
            mid = (low + high) / 2;
            if (primes[mid] == n)
                return primes[mid];
            if (primes[mid] < n)
                low = mid + 1;
            else
                high = mid - 1;
        }
        if(primes[mid] < n)
            return primes[mid+1];
        return primes[mid];
        // code ends
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash;
        long long index = hash.univHash(key, capacity());
        auto & list = hash_table[ index ];
        auto itr = list.begin();
        while(itr != list.end()){
           if((*itr).key == key)
               return(itr);
           itr++;
        }
        return(itr);
        // code ends
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
        // code begins
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType> > > old_table;
        for(size_t i=0; i<capacity(); i++){
            old_table.push_back(hash_table[i]);
            hash_table[i].clear();
        }
        if(capacity() == 3) 
            hash_table.reserve(17);
        else
            hash_table.reserve(new_size);
        theSize = 0;
        for(size_t i=0; i<old_table.capacity(); i++){
            auto itr = old_table[i].begin();
            while(itr != old_table[i].end()){
                insert(*itr);
                itr++;
            }
        }
        // code ends
    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
        size_t new_size = nextPrime(2 * capacity());
        this->rehash(new_size);
        return;
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
        size_t new_size = nextPrime(ceil(capacity() / 2));
        this->rehash(new_size);
        return;
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
        // code begins
        theSize = 0;
        hash_table.reserve(init_size);
        preCalPrimes(10);
        // code ends
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
        // code begins

        // code ends
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash;
        long long index = hash.univHash(key, capacity());
        auto & list = hash_table[ index ];
        return find( key ) != list.end();
        // code ends
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data)
    {
        // code begins
        if(contains(key)){
          auto itr = find(key);
          data = *itr;
          return true;
        }
        else
          return false;
        // code ends
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
        // code begins
        HashFunc<KeyType> hash;
        long long index = hash.univHash(x.key, capacity());
        auto & list = hash_table[ index ];
        if(find(x.key) != list.end())
            return false;
        list.push_back(x);
        if( ++theSize > capacity() / 2 )
            doubleTable();
        return true;
        // code ends
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
        // code begins
        HashFunc<KeyType> hash;
        long long index = hash.univHash(x.key, capacity());
        auto & list = hash_table[ index ];
        if(find(x.key) != list.end())
            return false;
        list.push_back(std::move(x));
        if( ++theSize > capacity() / 2 )
            doubleTable();
        return true;
        // code ends
    }

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> hash;
        long long index = hash.univHash(key, capacity());
        auto & list = hash_table[ index ];
        auto itr = find( key );
        if( itr == list.end() )
            return false;
        list.erase( itr );
        if( --theSize < (capacity()/8 + (capacity()%8 != 0)))
            halveTable();
        return true;
        // code ends
    }

    // returns the number of data elements stored in the hash table
    size_t size()
    {
        // code begins
        return(theSize);
        // code ends
    }

    // returns the capacity of the hash table
    size_t capacity()
    {
        // code begins
        return(hash_table.capacity());
        // code ends
    }

};


#endif // __MYHASHTABLE_H__
