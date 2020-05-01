/*
 * Test file for insertion into skiplists.
 */
#include "skiplists.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

int main() {

    skip_list::Skip_list<int, int> list(10);
    int key = 0, value = 0;

    static std::random_device rd;
    static std::mt19937 engine(rd());
    static std::uniform_int_distribution<> gen(1,1000);


    for(int i = 0; i < 15; i++) {
        key = gen(engine);
        value = gen(engine);

        list.insert(key, value);
        list.print_list();
        
        std::cout << "\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout<<"\n\nFinal List: \n";

    list.print_list();

    return 0;
}
