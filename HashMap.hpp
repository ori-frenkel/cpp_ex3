#include <vector>
#include <assert.h>
#include <iostream> // todo : remove this include
// default hash map size
const int defaultHashMapCapacity = 16;

template <typename KeyT, typename ValueT>
class HashMap
{
private:
    double _lowerBound;
    double _upperBound;
    int _capacityOfArray;
    int _sizeOfArray;
    std::vector<std::pair<KeyT, ValueT>> *_hashMap;
    std::hash<KeyT> _hash;

public:
    /**
     * Default constructor + constructor that gets the lower and upper bound
     * @param lowerBound - of the hashMap
     * @param upperBound - of the hashMap
     */
    HashMap(const double lowerBound = 1.0/4, const double upperBound = 3.0/4) :
                                                     _lowerBound(lowerBound), \
                                                     _upperBound(upperBound), \
                                                     _capacityOfArray(defaultHashMapCapacity), \
                                                     _sizeOfArray(0)
    {
        try
        {
            _hashMap = new std::vector<std::pair<KeyT, ValueT>>[defaultHashMapCapacity];
        }
        catch (std::bad_alloc)
        {
            std::cerr << "Memory allocation failed" << std::endl;
        }

    };

    /**
     * Constructor that gets to vector, and creating hashMap which key[i] -> value[i]
     * @param keysVector
     * @param valueVector
     */
    HashMap(std::vector<KeyT> keysVector, std::vector<ValueT> valueVector) : HashMap()
    {

    }

    std::vector<std::pair<KeyT, ValueT>>* getHashMap()
    {
        return _hashMap;
    }

    const double& getLowerBound() const
    {
        return _lowerBound;
    }

    const double& getUpperBound() const
    {
        return _upperBound;
    }

    // todo: delete this
    int getSize()
    {
        return static_cast<int>(_hashMap->size());
    }


    /**
     * Copy constructor
     * @param other - other hashMap to copy
     */
    HashMap(const HashMap& other) : _lowerBound(other.getLowerBound()), \
                                    _upperBound(other.getUpperBound()), \
                                    _capacityOfArray(other.capacity()), \
                                    _sizeOfArray(other.size())
    {
        /* if current hashMap and other, has different size, than delete the current and allocate
         * new current hashMap with the other size.   */
        try
        {
            _hashMap = new std::vector<std::pair<KeyT, ValueT>>[other.capacity()];
        }
        catch (std::bad_alloc)
        {
            std::cerr << "Memory allocation failed" << std::endl;
        }

        for(int i = 0 ; i < _capacityOfArray; ++i)
        {
            std::copy(other._hashMap[i].begin(), other._hashMap[i].end(), back_inserter(_hashMap[i]));
        }

    }

    /**
     * Move constructor
     * @param other - other object of type HashMap
     */
    HashMap(HashMap && other) : _lowerBound(std::move(other._lowerBound)), \
                                _upperBound(std::move(other._upperBound)), \
                                _hashMap(std::move(other._hashMap)), \
                                _capacityOfArray(std::move(other._capacityOfArray)), \
                                _sizeOfArray(other._sizeOfArray)
    {
    }

    /**
     *
     * @return the size of the HashMap
     */
    int size() const
    {
        return _sizeOfArray;
    }

    /**
     *
     * @return - the capacity of the HashMap
     */
    int capacity() const
    {
        return _capacityOfArray;
    }

    /**
     *
     * @return the load factor of the hashMap
     */
    double getLoadFactor() const
    {
        return (_sizeOfArray / _capacityOfArray);
    }

    /**
     *
     * @return - true if the HashMap is empty, false otherwise.
     */
    bool empty()
    {
        return _sizeOfArray == 0;
    }


    /**
     * This function rehashing the HashMap
     * @param cutTheCapacityByHalf - true if we need to increase the capacity, false if we want to
     *                               reduce the capacity
     */
    void rehashing(bool increaseTheCapacity)
    {
        int newCapacity = _capacityOfArray;
        if(increaseTheCapacity)
        {
            newCapacity *= 2;
        }
        else
        {

            newCapacity /= 2;
        }

        try
        {
            std::vector<std::pair<KeyT, ValueT>> * newHashMap = \
                                new std::vector<std::pair<KeyT, ValueT>>[newCapacity];

            for(int i = 0; i < _capacityOfArray; ++i)
            {
                for(int j = 0; j < _hashMap[i].size(); j++)
                {
                    int index = _hash(_hashMap[i][j].first) & (newCapacity - 1);
                    newHashMap[index].push_back(\
                        std::pair<KeyT, ValueT> (_hashMap[i][j].first, _hashMap[i][j].second));
                }
            }

            delete[] _hashMap;
            _hashMap = newHashMap;
            _capacityOfArray = newCapacity;
        }
        catch (std::bad_alloc)
        {
            std::cerr << "Memory allocation failed" << std::endl;
        }
    }

    bool insert(KeyT key, ValueT value)
    {

        int index = _hash(key) & (_capacityOfArray - 1);

        // iterate over the vector to check if there is already such key in the hashTable.
        if(containsKey(key))
        {
            return false;
        }
        _hashMap[index].push_back(std::pair<KeyT,ValueT>(key, value));
        ++_sizeOfArray;
        if(getLoadFactor() > _upperBound)
        {
            rehashing(true);
        }
    }

    /**
     * This function check whther the given key is already in the HashMap
     * @param key - key
     * @return - true if there is already such key in the HashMap, false otherwise
     */
    bool containsKey(KeyT key) const
    {
        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if(_hashMap[index][i].first == key)
            {
                return true;
            }
        }
        return false;
    }

    /**
     * const version of at.
     * @param key - key of the pair
     * @return - the value of that key, if exist, otherwise, will throw an exception.
     */
    const ValueT& at(KeyT key) const
    {
        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i].first == key)
            {
                return _hashMap[index][i].second;
            }
        }

        // todo : need to free all the memory, here or in main with try and catch block
        throw std::invalid_argument("at function must get a valid key");
    }

    /**
     * Non const version of at.
     * @param key - key of the pair
     * @return - the value of that key, if exist, otherwise, will throw an exception.
     */
    ValueT& at(KeyT key)
    {
        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i].first == key)
            {
                return _hashMap[index][i].second;
            }
        }

        throw std::invalid_argument("at function must get a valid key");

    }

    /**
     *  erase the pair with that key from the HashSet
     * @param key - key of the pair
     * @return - true if succeed to delete the pair with that key from the HashSet, false otherwise
     */
    bool erase(KeyT key)
    {
        if(!containsKey(key))
        {
            return false;
        }

        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i].first == key)
            {
                _hashMap[index].erase(_hashMap[index].begin() + i);
                break;
            }
        }

        --_sizeOfArray;
        if(getLowerBound() > getLoadFactor())
        {
            rehashing(false);
        }
        return true;
    }

    /**
     *
     * @param key - the key within the bucket we want
     * @return - the size of the bucket which the key is with in
     */
    int bucketSize(KeyT key) const
    {
        if(!containsKey(key))
        {
            throw std::invalid_argument("bucketSize function must get a valid key");
        }

        int index = _hash(key) & (_capacityOfArray - 1);
        return static_cast<int>(_hashMap[index].size());
    }

    /**
     * removing all the elements from the HashSet
     */
    void clear()
    {
        for(int i = 0; i < _capacityOfArray; i++)
        {
            _hashMap[i].clear();
        }
        _sizeOfArray = 0;
    }

    /**
     * overload the operator '='
     * @param other - other HashMap to 'copy'
     * @return current HashMap after the operator '=' been done.
     */
    HashMap& operator = (const HashMap& other)
    {
        /* if current hashMap and other, has different size, than delete the current and allocate
         * new current hashMap with the other size.   */
        int otherCapacity = other.capacity();
        if(_capacityOfArray != otherCapacity)
        {
            delete[] _hashMap;
            try
            {
                _hashMap = new std::vector<std::pair<KeyT, ValueT>>[otherCapacity];
            }
            catch (std::bad_alloc)
            {
                std::cerr << "Memory allocation failed" << std::endl;
            }
            _capacityOfArray = otherCapacity;
        }

        _sizeOfArray = other.size();

        for(int i = 0 ; i < otherCapacity; ++i)
        {
            std::copy(other._hashMap[i].begin(), other._hashMap[i].end(), back_inserter(_hashMap[i]));
        }

        _lowerBound = other.getLowerBound();
        _upperBound = other.getUpperBound();
        _capacityOfArray = other.capacity();
        _sizeOfArray = other.size();

        return *this;
    }

    /**
     * overload subscript operator - non const version
     * @param key - the key that we want to find the value of.
     * @return - nullptr, if the key is invalid, or the value that belong to the key otherwise
     */
    ValueT& operator [] (const KeyT& key) noexcept
    {
        ValueT val = nullptr;
        try
        {
            val = at(key);
        }
        catch (std::invalid_argument)
        {
            return val;
        }
        return at(key);
    }

    /**
     * overload subscript operator - const version
     * @param key - the key that we want to find the value of.
     * @return - nullptr, if the key is invalid, or the value that belong to the key otherwise
     */
    HashMap& operator [] (const KeyT& key) const noexcept
    {
        const ValueT *val = nullptr;
        try
        {
            *val = at(key);
        }
        catch (std::invalid_argument)
        {
            return nullptr;
        }
        return at(key);
    }

    bool operator == (const HashMap& other) const
    {
        if(_sizeOfArray != other._sizeOfArray)
        {
            return false;
        }
        iterator p;
        std::pair<KeyT, ValueT> *arrOfAllTheItemsCurr = begin().getAllThePairs();
        std::pair<KeyT, ValueT> *arrOfAllTheItemsOther = other.begin().getAllThePairs();
        int numOfSamePairs = 0;
        for(int i = 0; i < _sizeOfArray; ++i)
        {
            for(int j = 0; j < _sizeOfArray; j++)
            {
                if(arrOfAllTheItemsCurr[i] == arrOfAllTheItemsOther[j])
                {
                    ++numOfSamePairs;
                    break;
                }
            }
        }
        if(numOfSamePairs == _sizeOfArray)
        {
            return true;
        }
        return false;
    }

    bool operator != (const HashMap& other) const
    {
        return !operator==(other);
    }

    ~HashMap()
    {
        delete[] _hashMap;
    }

    class iterator
    {
    private:
        std::vector<std::pair<KeyT, ValueT>> *_hashMap;

        // array of all the pairs in the hashMap
        std::pair<KeyT, ValueT> *_arrOfAllTheItems;
        // current location in the _arrOfAllTheItems
        int _currentLocation;
        int _capacityOfHash;

    public:
        /**
         * Constructor of iterator
         * @param hashMap - the array of vector of pairs that the HashMap uses
         * @param sizeOfHashMap - The number of pairs in the HashMap
         * @param capacityOfHash - The Capacity of the HashMap
         */
        iterator(std::vector<std::pair<KeyT, ValueT>> *hashMap = nullptr, int sizeOfHashMap = 0, \
                int capacityOfHash = 0) : _hashMap(hashMap)
        {
            int index = 0;
            _capacityOfHash = capacityOfHash;
            _arrOfAllTheItems = new std::pair<KeyT, ValueT>[_capacityOfHash];
            for(int i = 0; i < capacityOfHash; i++)
            {
                if(_hashMap[i].size() == 0)
                {
                    continue;
                }
                for(int j = 0; j < _hashMap[i].size(); j++)
                {
                    _arrOfAllTheItems[j] = _hashMap[i][j];
                }
            }
            _currentLocation = 0;
        };

        /**
         *
         * @return - the pair that the iterator is pointing to.
         */
        std::pair<KeyT, ValueT>& operator * () const
        {
            /*
            if(_currentLocation == _capacityOfHash)
            {
                return nullptr;
            }
             */
            return _arrOfAllTheItems[_currentLocation];
        }

        /**
         *
         * @return - The Address of the pair that the iterator is pointing to.
         */
        std::pair<KeyT, ValueT>* operator -> () const
        {
            if(_currentLocation == _capacityOfHash)
            {
                return nullptr;
            }
            return &(_arrOfAllTheItems[_currentLocation]);
        }

        /**
         * prefix increment operator - '++i'
         * @return current iterator after '++'
         */
        iterator& operator++()
        {
            ++_currentLocation;
            return *this;
        }

        /**
         * postfix increment operator - 'i++'
         * @return
         */
        iterator& operator++(int)
        {
            iterator *tmp = *this;
            ++_currentLocation;
            return *this;
        }

        /**
         *
         * @param other - other iterator
         * @return - true, if both iterator pointing to the same pair, false otherwise
         */
        bool operator == (iterator const& other) const
        {
            if(_currentLocation == _capacityOfHash && other == nullptr)
            {
                return true;
            }
            return _arrOfAllTheItems[_currentLocation] == \
                                               other._arrOfAllTheItems[other._currentLocation];
        }

        /**
         * overload the operator '!='
         * @param other - other iterator
         * @return - false, if both iterator pointing to the same pair, true otherwise
         */
        bool operator != (iterator const&other) const
        {
            if(_currentLocation == _capacityOfHash && other == nullptr)
            {
                return false;
            }
            return _arrOfAllTheItems[_currentLocation] != \
                                               other._arrOfAllTheItems[other._currentLocation];
        }

        /**
         * use this as helper function in overload the operator '==' and '!=' in HashMap
         * @return
         */
        std::pair<KeyT, ValueT> *getAllThePairs() const
        {
            return _arrOfAllTheItems;
        }

        ~iterator(){delete[] _arrOfAllTheItems;}

    };
    iterator begin() {return iterator(_hashMap, _sizeOfArray, _capacityOfArray);}
    iterator begin() const {return iterator(_hashMap, _sizeOfArray, _capacityOfArray);}
    iterator end() { return nullptr;}

};