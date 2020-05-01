/*
 * Test file for testing the templates.
 */
#include "skiplists.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <vector>

int main() {

    skip_list::Skip_list<int, std::vector<float>> list(5);
    int key = 0, value = 0;

    static std::random_device rd;
    static std::mt19937 engine(rd());
    static std::uniform_int_distribution<> key_gen(1,100);
    static std::uniform_real_distribution<float> value_gen(1, 1000);

    for(int i = 0; i < 10; i++) {
        std::vector<float> data_arr;
    
        key = key_gen(engine);
        for(int i = 0; i < 5; i++) {
            data_arr.push_back(value_gen(engine));
        }
        list.insert(key, data_arr);
        data_arr.empty();
        std::cout<<"\n\n";
        list.print_list();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout<<"Final List: \n";

    list.print_list();
    
    skip_list::Skip_list<int, std::vector<float>>::Iterator it;

    std::cout<<"\nEnter a key to search from the list\nOr -1 to exit.\n";
    while(1) {
        std::cin >> key;
        if(key == -1)
            break;
        it = list.search(key);
        if(it != list.end()) {
            std::cout << "Found:\n";
            std::cout << "Key: "<< (*it).first; 
            std::cout << "\nValue: ";
            for(int i = 0; i < (*it).second.size(); i++)
                std::cout << (*it).second[i] << " ";
            std::cout << "\n";
        }
        else
            std::cout<<"Not Found\n";
        std::cout<<"Enter a key to search from the list\nOr -1 to exit.\n";
    }

    return 0;
}
