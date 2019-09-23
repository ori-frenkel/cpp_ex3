//
// Created by Owner on 23/09/2019.
//
#define INVALID_INPUT "Invalid input"
#define FAILURE -1
#define DIGIT "0123456789"
#define SPAM "SPAM"
#define NOT_SPAM "NOT_SPAM"
#include <iostream>
#include "HashMap.hpp"
//#include <boost/filesystem.hpp>
#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

/**
 * @param str - string
 * @return - true if the string contain a digit, false otherwise
 */
bool strContainDigit(const std::string& str)
{
    return std::string::npos != str.find_first_of(DIGIT);
}

/**
 * Convert string to lower case
 * @param str - string to convert
 */
std::string lower_string(const std::string& str)
{
    std::string loweCaseStr = str;
    for(int i = 0; str[i] != '\0'; i++)
    {
        if (loweCaseStr[i] >= 'A' && loweCaseStr[i] <= 'Z')
        {
            loweCaseStr[i] = static_cast<char>(loweCaseStr[i] + 32); // convert to lower case
        }
    }
    return loweCaseStr;
}

/**
 * @param line - line from data base
 * @param part0 - the part0 from the data base - the bad sequence
 * @param points - part1 from the database of the above line, the points this sequence wroth
 * @return - true if it a valid line, false otherwise
 */
bool checkForValidInput(const std::string& line)
{

    // checking for valid format - there is only one ','
    long numOfColumns = std::count(line.begin(), line.end(), ',');
    if( numOfColumns != 1)
    {
        return false;
    }

    std::size_t dividerPos = line.find(',');
    std::string part0 = line.substr(0, dividerPos); // part0
    std::string points_seq = line.substr(dividerPos + 1, line.length()); // part1

    // checking if there is actually points (part1) and actually a string of bad sequence (part 0)
    if(part0.length() == 0 || points_seq.length() == 0 || !strContainDigit(points_seq))
    {
        return false;
    }

    // number of points must be non-negative (positive or zero)
    if(std::stoi(points_seq) < 0)
    {
        return false;
    }

    return true;
}

/**
 *
 * @param dataBase - hashMap to insert the data into.
 * @param argv - argv (command line parameters)
 * @param gDatBaseIndex - index in argv that contains dataBase path
 * @return true, if succeed, false otherwise.
 */
bool insertDataFromDataBaseToHashMap(HashMap<std::string, int> dataBase, char*argv[], \
                                     const int& gDatBaseIndex)
{
    /*
    path p(argv[gDatBaseIndex]);
    // checking if the path actually exist
    if (!exists(p))
    {
        return false;
    }
    */
    std::string line;
    std::ifstream input_dataBase(argv[gDatBaseIndex]);

    while(getline(input_dataBase, line))
    {
        if(!checkForValidInput(line))
        {
            return false;
        }
        std::size_t dividerPos = line.find(',');
        std::string bad_seq = line.substr(0, dividerPos); // part0
        std::string points_seq = line.substr(dividerPos + 1, line.length()); // part1
        dataBase.insert(bad_seq, std::stoi(points_seq));
    }
    return true;
}

/**
 * Find the number of times str2 is inside str1 (without regards to upper lower case)
 * @param str1 - string
 * @param str2 - string
 * @return - the number of times str2 is inside str1
 */
long countNumberOfStrInStr(const std::string& str1, const std::string& str2)
{
    std::string str1_lower = lower_string(str1);
    std::string str2_lower = lower_string(str2);
    long count = 0;
    std::string::size_type position = 0;
    while ((position = str1_lower.find(str2_lower, position )) != std::string::npos)
    {
        ++ count;
        position += str2_lower.length();
    }
    return count;
}

/**
 *
 * @param argv - the command line argument
 * @param gMsgIndex - index of the msg in the command line argument
 * @param dataBase - hashMap that contain all the bad sequences and their equivalent wroth
 * @return -1 if failed to open the file, non-negative otherwise that represent the total pointer
 *          the file got
 */
long getTotalFilePoint(char* argv[], const int gMsgIndex, const HashMap<std::string, int> &dataBase)
{
    /*
    path p(argv[gMsgIndex]);
    // checking if the path actually exist
    if (!exists(p))
    {
        return FAILURE;
    }
    */
    // convert the file input to string
    std::ifstream tmp(argv[gMsgIndex]);
    std::stringstream msg;
    msg << tmp.rdbuf();
    long totalFilePoints = 0;
    for(auto it = dataBase.cbegin(); it != dataBase.cend(); ++it)
    {
        long numberOfTimesInText = countNumberOfStrInStr(msg.str(), it->first);
        totalFilePoints += numberOfTimesInText * it->second;
    }
    return totalFilePoints;
}

int main(int argc, char* argv[])
{
    const int gDatBaseIndex = 1; // the data base location in command line
    const int gMsgIndex = 2;
    const int gThresholdIndex = 3;
    if(argc != 4)
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }

    HashMap<std::string, int> dataBase;
    if(!insertDataFromDataBaseToHashMap(dataBase, argv, gDatBaseIndex))
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }

    long totalFilePoint = getTotalFilePoint(argv, gMsgIndex, dataBase);
    if(totalFilePoint == FAILURE)
    {
        std::cerr << INVALID_INPUT << std::endl;
        return EXIT_FAILURE;
    }

    if(!strContainDigit(argv[gThresholdIndex]))
    {
        return EXIT_FAILURE;
    }
    long threshold = std::stoi(argv[gThresholdIndex]);

    if(threshold <= totalFilePoint)
    {
        std::cout << SPAM << std::endl;
    }
    else
    {
        std::cout << NOT_SPAM << std::endl;
    }
    
    return 0;
}

