/*
 * Test file for searching nodes in the skiplists.
 */
#include "skiplists.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

int main() {

    skip_list::Skip_list<int, int> list(5);
    int key = 0, value = 0;

    static std::random_device rd;
    static std::mt19937 engine(rd());
    static std::uniform_int_distribution<> gen(1,100000);


    for(int i = 0; i < 15; i++) {
        key = gen(engine);
        value = gen(engine);

        list.insert(key, value);
        list.print_list();
    
        std::cout << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout<<"Final List: \n";

    list.print_list();
    
    skip_list::Skip_list<int, int>::Iterator it;

    std::cout<<"\nEnter a key to search from the list\nOr -1 to exit.\n";
    while(1) {
        std::cin >> key;
        if(key == -1)
            break;
        it = list.search(key);
        if(it != list.end()) {
            std::cout << "Found:\n";
            std::cout << "Key: "<< (*it).first << " Value: " <<(*it).second;
            std::cout << "\n";
        }
        else
            std::cout<<"Not Found\n";
        std::cout<<"Enter a key to search from the list\nOr -1 to exit.\n";
    }

    return 0;
}
