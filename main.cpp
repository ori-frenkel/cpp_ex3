#include <iostream>
#include "HashMap.hpp"

int main() {
    HashMap<int, int> *temp = new HashMap<int, int>();
    /*
    std::pair<int,int> *pair1 = new std::pair<int,int>(1,3);
    std::pair<int,int> *pair2 = new std::pair<int,int>(1,2);

    if(*pair1 == *pair2)
    {
        std::cout << "TRUE" << std::endl;
    }
    else
    {
        std::cout << "False" << std::endl;
    }
    return 0;
     */
    temp->insert(1,11);
    temp->insert(2,11);
    //std::cout << temp->(2) << std::endl;
    temp->clear();
    //std::cout << temp->at(2) << std::endl;
    std::cout << temp->size() << std::endl;
    std::cout << temp->capacity() << std::endl;
    return 0;
}