#include <vector>
#include <assert.h>
#include <iostream>

// default hash map size
const int defaultHashMapCapacity = 16;

template <typename KeyT, typename ValueT>
/**
 * This class represent a generic Hash Map
 * @tparam KeyT - the type of key in the hash map
 * @tparam ValueT - the type of value in the hashMap
 */
class HashMap
{
private:
    double _lowerBound; /**< lower bound of the array */
    double _upperBound; /**< upper bound ratio of the array */
    int _capacityOfArray; /**< the capacity of the array that store the hashMap */
    int _sizeOfArray; /**< the actual number of items in the hashMap */
    std::vector<std::pair<KeyT, ValueT>> *_hashMap;
    std::hash<KeyT> _hash;

public:
    /**
     * Default constructor + constructor that gets the lower and upper bound
     * @param lowerBound - of the hashMap
     * @param upperBound - of the hashMap
     */
    HashMap(const double lowerBound = (1.0 / 4), const double upperBound = (3.0 / 4)) :
                                                     _lowerBound(lowerBound), \
                                                     _upperBound(upperBound), \
                                                     _capacityOfArray(defaultHashMapCapacity), \
                                                     _sizeOfArray(0)
    {
        if(lowerBound >= upperBound || lowerBound <= 0 || upperBound >= 1)
        {
            throw std::out_of_range("lowerBound < upperBound &&  lowerBound > 0 && upperBound <1");
        }
        try
        {
            _hashMap = new std::vector<std::pair<KeyT, ValueT>>[defaultHashMapCapacity];
        }
        catch (const std::bad_alloc& e)
        {
            throw e;
        }

    };

    /**
     * Constructor that gets two vector, and creating hashMap which key[i] -> value[i]
     * @param keysVector
     * @param valueVector
     */
    HashMap(std::vector<KeyT> keysVector, std::vector<ValueT> valueVector) : HashMap()
    {
        if(keysVector.size() != valueVector.size())
        {
            throw std::invalid_argument("vector are from different sizes");
        }
        for(int i = 0; i < keysVector.size(); i ++)
        {
            insert(keysVector[i], valueVector[i]);
            // override the key - value if exist.
            at(keysVector[i]) = valueVector[i];
        }
    }

    /**
     *
     * @return the lower bound of the hashMap
     */
    const double& getLowerBound() const
    {
        return _lowerBound;
    }

    /**
     *
     * @return the upper bound of the array
     */
    const double& getUpperBound() const
    {
        return _upperBound;
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
        catch (const std::bad_alloc& e)
        {
            std::cerr << "Memory allocation failed" << std::endl;
            throw e;
        }

        for(int i = 0 ; i < _capacityOfArray; ++i)
        {
            std::copy(other._hashMap[i].begin(), other._hashMap[i].end(), \
                      back_inserter(_hashMap[i]));
        }

    }

    /**
     * Move constructor
     * @param other - other object of type HashMap
     */
    HashMap(HashMap && other) : _lowerBound(other.getLowerBound()), \
                                _upperBound(other.getUpperBound()), \
                                _capacityOfArray(other._capacityOfArray), \
                                _sizeOfArray(other._sizeOfArray), \
                                _hashMap(std::move(other._hashMap))
    {
       other._hashMap = nullptr;
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
        return (double(_sizeOfArray) / _capacityOfArray);
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

        std::vector<std::pair<KeyT, ValueT>> * newHashMap = nullptr;
        try
        {
            newHashMap = new std::vector<std::pair<KeyT, ValueT>>[newCapacity];

            for(int i = 0; i < _capacityOfArray; ++i)
            {
                for(size_t j = 0; j < _hashMap[i].size(); j++)
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
        catch (const std::bad_alloc& e)
        {
            delete[] newHashMap;
            throw e;
        }
    }

    /**
     * Insert (key, value) to the HashMap
     * @param key - the key to insert
     * @param value - the value to insert
     * @return - true if insert succeed, false otherwise.
     */
    bool insert(KeyT key, ValueT value)
    {

        int index = _hash(key) & (_capacityOfArray - 1);

        // iterate over the vector to check if there is already such key in the hashTable.
        if(containsKey(key))
        {
            return false;
        }
        _hashMap[index].push_back(std::pair<KeyT, ValueT>(key, value));
        ++_sizeOfArray;
        if(getLoadFactor() > _upperBound)
        {
            try
            {
                rehashing(true);
            }
            catch (const std::bad_alloc& e)
            {
                throw e;
            }
        }
        return true;
    }

    /**
     * This function check whther the given key is already in the HashMap
     * @param key - key
     * @return - true if there is already such key in the HashMap, false otherwise
     */
    bool containsKey(KeyT key) const
    {
        int index = _hash(key) & (_capacityOfArray - 1);
        for(size_t i = 0; i < _hashMap[index].size(); ++i)
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
    const ValueT& at(const KeyT& key) const
    {
        if(_hashMap == nullptr)
        {
            throw std::out_of_range("hashMap is null");
        }
        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); ++i)
        {
            if((_hashMap[index])[i].first == key)
            {
                return _hashMap[index][i].second;
            }
        }

        throw std::invalid_argument("at function must get a valid key");
    }

    /**
     * Non const version of at.
     * @param key - key of the pair
     * @return - the value of that key, if exist, otherwise, will throw an exception.
     */
    ValueT& at(const KeyT& key)
    {
        if(_hashMap == nullptr)
        {
            throw std::out_of_range("hashMap is null");
        }
        int index = _hash(key) & (_capacityOfArray - 1);
        for(int i = 0; i < _hashMap[index].size(); ++i)
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
        for(int i = 0; i < _hashMap[index].size(); ++i)
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
            try
            {
                rehashing(false);
            }
            catch (const std::bad_alloc& e)
            {
                throw e;
            }
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
        for(int i = 0; i < _capacityOfArray; ++i)
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
            catch (const std::bad_alloc& e)
            {
                throw e;
            }
            _capacityOfArray = otherCapacity;
        }

        _sizeOfArray = other.size();

        for(int i = 0 ; i < otherCapacity; ++i)
        {
            std::copy(other._hashMap[i].begin(), \
                      other._hashMap[i].end(), back_inserter(_hashMap[i]));
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
     * @return - random variable, if the key is invalid, or the value that belong to the key
     * otherwise
     */
    ValueT& operator [] (const KeyT& key) noexcept
    {
        try
        {
            at(key);
        }
        catch (std::exception)
        {
            // insert the key with some value
            insert(key, ValueT());
            return at(key);

        }
        return at(key);
    }

    /**
     * overload subscript operator - const version
     * @param key - the key that we want to find the value of.
     * @return - random value, if the key is invalid, or the value that belong to the key otherwise
     */
    ValueT operator [] (const KeyT& key) const noexcept
    {
        try
        {
            at(key);
        }
        catch (std::invalid_argument)
        {
            return ValueT();
        }
        return at(key);
    }

    /**
     * Checking if this hashMap is the same as other hashMap
     * @param other - hashMap
     * @return true if this and other hashMap are equal, false otherwise.
     */
    bool operator == (const HashMap& other) const
    {
        if(_sizeOfArray != other.size() || _capacityOfArray != other.capacity() || \
           _lowerBound != other.getLowerBound() || _upperBound != other.getUpperBound())
        {
            return false;
        }

        std::pair<KeyT, ValueT> *arrOfAllTheItemsCurr = begin().getAllThePairs();
        for (auto it = this->cbegin(); it != this->cend(); ++it)
        {
            try
            {
                if(other.at(it->first) != this->at(it->first))
                {
                    return false;
                }
            }
            catch(std::invalid_argument)
            {
                return false;
            }
        }
        return true;

    }

    /**
     * Checking if this hashMap is different from other hashMap
     * @param other - hashMap
     * @return - true if this and other hashMap are not equal, false otherwise.
     */
    bool operator != (const HashMap& other) const
    {
        return !operator==(other);
    }

    /**
     * Destructor
     */
    ~HashMap()
    {
        delete[] _hashMap;
    }

    /**
     * iterator of the hashMap
     */
    class const_iterator
    {
    private:
        std::vector<std::pair<KeyT, ValueT>> *_hashMap; /**< the hashMap to iterate on */
        std::pair<KeyT, ValueT> *_arrOfAllTheItems; /**< array of all the pairs in the hashMap */
        int _currentLocation; /**< current location in the _arrOfAllTheItems */
        int _capacityOfHash; /**< capacity of the hash */
        int _endLocation = 0; /**< end location of the hashMap */

    public:
        /**
         * Constructor of iterator
         * @param hashMap - the array of vector of pairs that the HashMap uses
         * @param sizeOfHashMap - The number of pairs in the HashMap
         * @param capacityOfHash - The Capacity of the HashMap
         */
        const_iterator(std::vector<std::pair<KeyT, ValueT>> *hashMap = nullptr, \
                       int sizeOfHashMap = 0, int capacityOfHash = 0, int currentLocation = 0) : \
                       _hashMap(hashMap)
        {
            int index = 0;
            _capacityOfHash = capacityOfHash;
            _arrOfAllTheItems = new std::pair<KeyT, ValueT>[sizeOfHashMap];
            for(int i = 0; i < capacityOfHash; i++)
            {
                if(_hashMap[i].size() == 0)
                {
                    continue;
                }
                for(size_t j = 0; j < _hashMap[i].size(); j++)
                {
                    _arrOfAllTheItems[index] = _hashMap[i][j];
                    ++index;
                }
            }
            _currentLocation = currentLocation;
            if(currentLocation == sizeOfHashMap)
            {
                _currentLocation = index;
            }
            _endLocation = index;
        };

        /**
         * Copy constructor
         * @param other - other iterator to copy.
         */
        const_iterator(const const_iterator& other)
        {
            _hashMap = other._hashMap;
            _currentLocation = other._currentLocation;
            _arrOfAllTheItems = new std::pair<KeyT, ValueT>[other._capacityOfHash];
            for(int i = 0; i < other._capacityOfHash; i++)
            {
                _arrOfAllTheItems[i] = other._arrOfAllTheItems[i];
            }
            _currentLocation = other._currentLocation;
            _endLocation = other._endLocation;
        }

        /**
         *
         * @return - the pair that the iterator is pointing to.
         */
        const std::pair<KeyT, ValueT>& operator * () const
        {
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
        const_iterator& operator++()
        {
            ++_currentLocation;
            return *this;
        }

        /**
         * postfix increment operator - 'i++'
         * @return
         */
        const_iterator& operator++(int)
        {
            const_iterator tmp = *this;
            ++_currentLocation;
            return *this;
        }

        /**
         *
         * @param other - other iterator
         * @return - true, if both iterator pointing to the same pair, false otherwise
         */
        bool operator == (const_iterator const& other) const
        {
            if((_currentLocation == other._currentLocation) && (_currentLocation == _endLocation) \
               && _currentLocation == other._endLocation)
            {
                return true;
            }
            if(_currentLocation != other._currentLocation || _endLocation != other._endLocation)
            {
                return false;
            }
            return _arrOfAllTheItems[_currentLocation] == \
                                               other._arrOfAllTheItems[other._currentLocation];
        }

        /**
         * overload the operator '!='
         * @param other - other iterator
         * @return - false, if both iterator pointing to the same pair, true otherwise
         */
        bool operator != (const_iterator const&other) const
        {
            return !operator==(other);
        }

        /**
         * use this as helper function in overload the operator '==' and '!=' in HashMap
         * @return
         */
        std::pair<KeyT, ValueT> *getAllThePairs() const
        {
            return _arrOfAllTheItems;
        }

        /**
         * Destructor
         */
        ~const_iterator(){delete[] _arrOfAllTheItems; }

    };


    /**
     * const version
     * @return the start of the iterator
     */
    const_iterator begin() const
    {
        return const_iterator(_hashMap, _sizeOfArray, _capacityOfArray);
    }

    /**
     * const version
     * @return the start of the iterator
     */
    const_iterator cbegin() const
    {
        return const_iterator(_hashMap, _sizeOfArray, _capacityOfArray);
    }

    /**
     * const version
     * @return return iterator of last+1
     */
    const_iterator end() const
    {
        return const_iterator(_hashMap, _sizeOfArray, _capacityOfArray, _sizeOfArray);
    }

    /**
     * const version
     * @return return iterator of last+1
     */
    const_iterator cend() const
    {
        return const_iterator(_hashMap, _sizeOfArray, _capacityOfArray, _sizeOfArray);
    }


};
