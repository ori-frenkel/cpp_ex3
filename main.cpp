#include <iostream>
#include "HashMap.hpp"

int main() {
    HashMap<int, int> *temp = new HashMap<int, int>();

    temp->insert(1,11);
    temp->insert(2,11);
    std::cout << temp->at(2) << std::endl;
    //temp->clear();
    std::cout << temp->size() << std::endl;
    return 0;
}