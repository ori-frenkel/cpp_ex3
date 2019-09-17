#include <vector>
#include <assert.h>

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

    double& getLowerBound() const
    {
        return _lowerBound;
    }

    double& getUpperBound() const
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
                                    _upperBound(other.getUpperBound())
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
            std::pair<KeyT, ValueT> pair2 = _hashMap[i];
            _hashMap[i] = std::pair<KeyT, ValueT> (pair2.first, pair2.second);
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
            std::hash <KeyT> h;
            std::vector<std::pair<KeyT, ValueT>> * newHashMap = \
                                new std::vector<std::pair<KeyT, ValueT>>[newCapacity];

            for(int i = 0; i < _capacityOfArray; ++i)
            {
                for(int j = 0; j < _hashMap[i].size(); i++)
                {
                    int index = h(_hashMap[i][j].first) & (newCapacity - 1);
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
        std::hash <KeyT> h;
        int index = h(key) & (_capacityOfArray - 1);

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
    bool containsKey(KeyT key)
    {
        std::hash <KeyT> h;
        int index = h(key) & (_capacityOfArray - 1);
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
        int index = std::hash<KeyT>(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i]->first == key)
            {
                return _hashMap[index][i]->second;
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
        int index = std::hash<KeyT>(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i]->first == key)
            {
                return _hashMap[index][i]->second;
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

        int index = std::hash<KeyT>(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); i++)
        {
            if((_hashMap[index])[i]->first == key)
            {
                _hashMap->erase(_hashMap[index][i]);
            }
        }

        --_sizeOfArray;
        if(getLowerBound() > _sizeOfArray)
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

        int index = std::hash<KeyT>(key) & (_capacityOfArray - 1);
        return static_cast<int>(_hashMap[index]->size());
    }

    /**
     * removing all the elements from the HashSet
     */
    void clear()
    {
        while(!_hashMap->empty())
        {
            _hashMap->pop_back();
            --_sizeOfArray;
        }
        assert(_sizeOfArray == 0);
    }

    ~HashMap()
    {
        delete[] _hashMap;
    }

};