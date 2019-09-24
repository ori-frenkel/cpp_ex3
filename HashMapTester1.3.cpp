#include "HashMap.hpp"
#include <cassert>
#include <map>
#include <cstdlib>
#include <random>
#include <iostream>

void testDefConstructor()
{
    HashMap<int, int> myMap;
    assert(myMap.getLoadFactor() == 0);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 0);
    assert(myMap.empty());
    // checking that an assertion is thrown
    try
    {
        myMap.at(4);
        assert(false);
    }
    catch (...)
    {
    }
    std::cout << "passed test DefConstructor\n";
}

void testConstructorWithFactor()
{
    HashMap<int, int> myMap(0.25, 0.5);
    assert(myMap.getLoadFactor() == 0);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 0);
    assert(myMap.empty());
    for (int i = 0; i < 8; ++i)
    {
        myMap.insert(i, i);
    }
    assert(myMap.getLoadFactor() == 0.5);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 8);
    myMap.insert(9, 9);
    assert(myMap.getLoadFactor() == 0.28125);
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 9);
    myMap.erase(0);
    myMap.erase(1);
    assert(myMap.size() == 7);
    assert(myMap.capacity() == 16);

    // now checking invalid input for this ctor
    // lower can't be bigger than higher
    try
    {
        HashMap<int, int> myMap2(0.5, 0.2);
        assert(false);
    }
    catch (...)
    {
    }

    // lower can't be smaller than 0
    try
    {
        HashMap<int, int> myMap2(-0.2, 0.2);
        assert(false);
    }
    catch (...)
    {
    }

    // higher can't be bigger than 1
    try
    {
        HashMap<int, int> myMap2(0.2, 1.1);
        assert(false);
    }
    catch (...)
    {
    }
    std::cout << "passed test ConstructorWithFactor\n";
}

void testConstructorVectors()
{
    // both vectors should be in the same length
    std::vector<char> keysVec{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'};
    std::vector<int> illegalValsVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    try
    {
        HashMap<char, int> myMap(keysVec, illegalValsVec);
        assert(0);
    }
    catch (...)
    {
    }
    std::vector<int> valsVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    HashMap<char, int> myMap(keysVec, valsVec);
    // capacity should be increased because default upper load factor was crossed
    assert(myMap.capacity() == 32);
    assert(myMap.getLoadFactor() == 0.40625);
    assert(myMap.size() == 13);
    assert(!myMap.empty());

    // shouldn't throw an exception
    try
    {
        for (char c = 'a'; c <= 'm'; c++)
        {
            assert(myMap.at(c) == (int) c - 96);
        }
    }
    catch (...)
    {
    }
    // if a same key appears again, the new corresponding value should overwrite the old one
    std::vector<char> keysVec2{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                               'c'};
    std::vector<int> valsVec2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 2};
    HashMap<char, int> myMap2(keysVec2, valsVec2);
    assert(myMap2['c'] == 2);

    std::cout << "passed testConstructorVectors\n";
}

void testCopyAndAssign()
{
    HashMap<int, int> myMap;
    HashMap<int, int> copy1 = myMap;
    HashMap<int, int> copy2(myMap);
    assert(copy1.getLoadFactor() == 0 && copy1.capacity() == 16 &&
           copy1.size() == 0 &&
           copy1.empty());
    assert(copy2.getLoadFactor() == 0 && copy2.capacity() == 16 && copy2.size() == 0 &&
           copy2.empty());
    for (int i = 0; i < 193; ++i)
    {
        myMap.insert(i, i);
    }
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy1.getLoadFactor() == 0 && copy1.capacity() == 16 && copy1.size() == 0 &&
           copy1.empty());
    assert(copy2.getLoadFactor() == 0 && copy2.capacity() == 16 && copy2.size() == 0 &&
           copy2.empty());
    HashMap<int, int> copy3 = myMap;
    HashMap<int, int> copy4(myMap);
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy3.getLoadFactor() == 0.376953125 && copy3.capacity() == 512 && copy3.size() == 193 &&
           !copy3.empty());
    assert(copy4.getLoadFactor() == 0.376953125 && copy4.capacity() == 512 && copy4.size() == 193 &&
           !copy4.empty());
    for (int i = 0; i < 66; ++i)
    {
        copy3.erase(i);
    }
    assert(copy3.getLoadFactor() == 0.49609375 && copy3.capacity() == 256 && copy3.size() == 127 &&
           !copy3.empty());
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy4.getLoadFactor() == 0.376953125 && copy4.capacity() == 512 && copy4.size() == 193 &&
           !copy4.empty());
    std::cout << "passed testCopyAndAssign\n";
}

template<typename T1, typename T2>
void isPairConst(const std::pair<T1, T2> &pair)
{
}

template<typename T3, typename T4>
void isPairConst(std::pair<T3, T4> &pair)
{
    assert(false);
}

void testIterator()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 100; ++i)
    {
        myMap.insert(i, i + 1);
    }
    // the iterator should be able to work on const objects, therefore converted to const
    const HashMap<int, int> myMap2 = myMap;

    // checks the postfix
    int count = 0;
    for (HashMap<int, int>::const_iterator it = myMap2.begin(); it != myMap2.end(); it++)
    {
        count += 1;
        assert((*it).first + 1 == (*it).second);
    }
    assert(count == 100);
    // checks the prefix
    count = 0;
    for (HashMap<int, int>::const_iterator it = myMap2.begin(); it != myMap2.end(); ++it)
    {
        count += 1;
        assert((*it).first + 1 == (*it).second);
    }
    assert(count == 100);
    // checks the for-each
    count = 0;
    for (std::pair<int, int> val : myMap2)
    {
        count += 1;
        assert(val.first + 1 == val.second);
    }
    assert(count == 100);

    HashMap<int, int>::const_iterator it = myMap2.begin();
    // the next function has a const version and a non const version. will draw an assert if *it is
    // non-const
    isPairConst(*it);

    // cbegin and cend should exist as well and be identical
    assert(myMap2.cbegin() == myMap2.begin());
    assert(myMap2.cend() == myMap2.end());

    std::cout << "passed testIterator\n";
}

// makes sure that the copy is deep and therefore clear doesn't hurt the copy
void testClearAndCopy()
{
    HashMap<int, int> myMap;

    for (int i = 0; i < 13; ++i)
    {
        myMap.insert(i, i);
    }
    HashMap<int, int> copyOfMap = myMap;
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 13);
    assert(myMap.at(2) == 2);
    myMap.clear();
    //capacity should stay the same
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 0);
    //should throw an exception
    try
    {
        std::cout << myMap.at(2) << std::endl;
        assert(0);
    }
    catch (...)
    {
    }
    //copy shouldn't be affected
    assert(copyOfMap.capacity() == 32);
    assert(copyOfMap.size() == 13);
    assert(copyOfMap.at(2) == 2);
    // shouldn't throw an exception
    try
    {
        assert(copyOfMap.at(2) == 2);
    }
    catch (...)
    {
        assert(0);
    }
    //should contain all
    for (int i = 0; i < 13; ++i)
    {
        assert(copyOfMap.containsKey(i));
    }
    std::cout << "passed testClearAndCopy\n";
}

void testBucketSize()
{
    HashMap<std::string, int> myMap;
    myMap.insert("aacagj", 1);
    myMap.insert("aacag", 2);
    myMap.insert("aacagjl1220", 2);
    assert(myMap.bucketSize("aacagj") == 3);

    // function should throw an exception if key isn't in the map
    try
    {
        myMap.bucketSize("hello");
        assert(false);
    }
    catch (...)
    {
    }
    std::cout << "passed testBucketSize\n";
}

void testEraseEasyDefaultFactors()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 13; ++i)
    {
        myMap.insert(i, i + 1);
    }
    assert(myMap.size() == 13);
    assert(myMap.capacity() == 32);
    // removing 5 elements, shouldn't resize the map (because 8 isn't < 0.25 * 32)
    for (int i = 0; i < 5; ++i)
    {
        myMap.erase(i);
    }
    assert(myMap.size() == 8);
    assert(myMap.capacity() == 32);
    for (int i = 0; i < 5; ++i)
    {
        assert(!myMap.containsKey(i));
    }
    // in the next removal there should be resizing
    myMap.erase(5);
    assert(myMap.size() == 7);
    assert(myMap.capacity() == 16);
    // removing 3 more elements, shouldn't resize the map (because 4 isn't < 0.25 * 16)
    for (int i = 6; i < 9; ++i)
    {
        myMap.erase(i);
    }
    assert(myMap.size() == 4);
    assert(myMap.capacity() == 16);
    for (int i = 6; i < 9; ++i)
    {
        assert(!myMap.containsKey(i));
    }
    // in the next removal there should be resizing
    myMap.erase(9);
    assert(myMap.size() == 3);
    assert(myMap.capacity() == 8);
    std::cout << "passed testEraseEasyDefaultFactors\n";
}

void testInsertAndEraseRandom()
{
    std::map<int, int> stdMap;
    HashMap<int, int> myMap;
    int r;
    for (int i = 0; i < 100; ++i)
    {
        r = rand() % 200;
        stdMap[r] = 2 * r;
        myMap[r] = 2 * r;
    }
    assert(myMap.size() == stdMap.size());
    int countStd = 0;
    int countMyMap = 0;
    // now checking that all elements are identical
    try
    {
        for (std::pair<int, int> val : stdMap)
        {
            countStd++;
            assert(myMap.at(val.first) == val.second);
        }

        for (std::pair<int, int> val : myMap)
        {
            countMyMap++;
            assert(stdMap.at(val.first) == val.second);
        }
    }
    catch (...)
    {
        assert(false);
    }
    assert(countMyMap == countStd);

    //now erasing some random
    for (int i = 0; i < 20; ++i)
    {
        r = rand() % 200;
        stdMap.erase(r);
        myMap.erase(r);
    }
    assert(myMap.size() == stdMap.size());
    // counters to make sure that the number of iterations is the same
    countStd = 0;
    countMyMap = 0;
    // and checking that all elements are identical
    try
    {
        for (std::pair<int, int> val : stdMap)
        {
            countStd++;
            assert(myMap.at(val.first) == val.second);
        }

        for (std::pair<int, int> val : myMap)
        {
            countMyMap++;
            assert(stdMap.at(val.first) == val.second);
        }
    }
    catch (...)
    {
        assert(false);
    }
    assert(countMyMap == countStd);
    std::cout << "passed testInsertAndEraseRandom\n";
}

template<typename T5>
void isValConst(const T5 &val)
{
}

template<typename T6>
void isValConst(T6 &val)
{
    assert(false);
}

void testBracketsOperator()
{
    HashMap<char, int> myMap;
    myMap.insert('a', 1);
    assert(myMap['a'] == 1);
    myMap['b'] = 2;
    assert(myMap['b'] == 2);
    myMap['b'] = 3;
    assert(myMap['b'] == 3);

    // now checks if the [] operator returns a const value when using const objects
    const HashMap<char, int> myMap2 = myMap;
    // the next function has 2 versions. the function which receives a const argument should be
    // called because the operator [] is activated on a const object
    isValConst(myMap2['a']);
    std::cout << "passed testBracketsOperator\n";
}

void testEqualityOperators()
{
    HashMap<char, int> myMap1;
    HashMap<char, int> myMap2;
    for (int i = 0; i < 100; ++i)
    {
        myMap1.insert(i, i);
        myMap2.insert(i, i);
    }
    assert(myMap1 == myMap2);
    myMap2.insert(100, 100);
    assert(myMap1 != myMap2);
    myMap2.erase(100);
    myMap1.insert(100, 100);
    assert(myMap1 != myMap2);
    myMap1.erase(100);

    // equal again, and now checking situation when keys equal and values not
    myMap1.insert(100, 101);
    myMap2.insert(100, 100);
    assert(myMap1 != myMap2);

    // this operator should work also on const objects, therefore:
    HashMap<char, int> myMap7 = myMap1;
    HashMap<char, int> myMap8 = myMap2;
    assert(myMap7 != myMap8);

    // according to the forum, they have to be equal according to all parameters
    // therefore checking maps with same elements but different parameters
    HashMap<char, int> myMap3;
    HashMap<char, int> myMap4(0.2, 0.8);
    myMap3.insert('a', 1);
    myMap4.insert('a', 1);
    assert(myMap1 != myMap2);
    // now checking different capacities
    HashMap<char, int> myMap5;
    HashMap<char, int> myMap6;
    myMap5.insert('a', 1);
    myMap5.erase('a');
    // now myMap5 capacity decreases to 8
    assert(myMap5 != myMap6);
    std::cout << "passed testEqualityOperators\n";
}

void insertionTest()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 300; ++i)
    {
        myMap[i] = i + 1;
    }

    for (int i = 0; i < 300; ++i)
    {
        myMap[i] = 2 * i;
    }
    assert(myMap.size() == 300);
    std::cout << "passed insertionTest\n";
}

void testEraseContent()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 100; ++i)
    {
        myMap[i] = 2 * i;
    }
    for (int i = 0; i < 30; ++i)
    {
        myMap.erase(i);
    }
    assert(myMap.size() == 70);
    int count = 0;
    for (std::pair<int, int> val : myMap)
    {
        count++;
    }
    assert(count == 70);
    // erasing a key which doesn't exist should return false
    assert(!myMap.erase(200));
    std::cout << "passed testEraseContent\n";
}

void testMove()
{
    HashMap<int, int> map1;
    for (int i = 0; i < 10; ++i)
    {
        map1[i] = i;
    }
    // checks move in assignment operator
    HashMap<int, int> map2 = std::move(map1);
    // the values should no longer exist in map 1 after being moved
    try
    {
        map1.at(1);
        assert(false);
    }
    catch (...)
    {
    }

    //checks the same, but now with move ctor
    HashMap<int, int> map3(std::move(map2));
    // the values should no longer exist in map 2 after being moved
    try
    {
        map2.at(1);
        assert(false);
    }
    catch (...)
    {
    }
    std::cout << "passed testMove\n";
}

// checks if at function also enables to edit values
void checkAtUpdate()
{
    HashMap<char, int> myMap;
    myMap.insert('a', 1);
    myMap.at('a') = 2;
    assert(myMap['a'] == 2);
    std::cout << "passed checkAtUpdate\n";
}

// checks if a copying a map to itself works good
void testCopyToItself()
{
    HashMap<char, int> myMap;
    myMap.insert('a', 1);
    myMap = myMap;
    assert(myMap.at('a') == 1);
    std::cout << "passed testCopyToItself\n";
}

int main()
{
    testDefConstructor();
    testConstructorWithFactor();
    testConstructorVectors();
    testCopyAndAssign();
    testIterator();
    testClearAndCopy();
    testCopyToItself();
    testBucketSize();
    testEraseContent();
    testEraseEasyDefaultFactors();
    testBracketsOperator();
    testEqualityOperators();
    insertionTest();
    testInsertAndEraseRandom();
    testMove();
    checkAtUpdate();
    std::cout << '\n';
    std::cout << "good job!! you passed all tests!\n";
    return 0;
}