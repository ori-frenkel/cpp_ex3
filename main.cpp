#include <iostream>
#include "HashMap.hpp"

int main() {
    HashMap<int, int> *temp = new HashMap<int, int>();

    temp->insert(1,1);
    std::cout << temp->size() << std::endl;
    return 0;
}