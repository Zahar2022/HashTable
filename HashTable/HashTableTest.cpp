
#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <string>
#include <cmath>
#include "HashTable.h"

    int main() {


     

        HashFunction2* hashFunc = new HashFunction2();
        HashTable<std::string> ht(10, hashFunc);
        ht.insert(1, "one");
        ht.insert(2, "two");
        ht.insert(3, "three");

        std::cout << "Table after insert:\n" << ht << std::endl;

        std::cout << "Searching for key 2: " << (ht.contains(2) ? "found" : "not found") << std::endl;
        std::cout << "Value by key 2: " << ht[2] << std::endl << std::endl;

        ht.setValue(2, "two");
        std::cout << "After updating key 2: " << ht[2] << std::endl << std::endl;

        ht.removeByKey(2);
        std::cout << "After removing key 2:\n" << ht << std::endl;

        ht.clear();
        std::cout << "After clear (table size " << ht.getTableSize() << "):\n" << ht << std::endl;

        std::cout << "tests passed successfully!" << std::endl;

        return 0;
    }