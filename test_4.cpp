#include "skiplists.hpp"
#include <iostream>
#include <random>
#include <thread>
#include <chrono>

int main() {


    skip_list::Skip_list<int, int> list(5);
    int key = 0, value = 0;

    static std::random_device rd;
    static std::mt19937 engine(rd());
    static std::uniform_int_distribution<> gen(1,100);


    for(int i = 0; i < 15; i++) {
        key = gen(engine);
        value = gen(engine);

        list.insert(key, value);
        list.print_list();
        
        std::cout<<"\n\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    std::cout<<"Final List: \n";

    list.print_list();
    
    skip_list::Skip_list<int, int>::Iterator it;
    skip_list::Skip_list<int, int>::Iterator rev_it;
    
    std::cout<< "Printing in ascending order\n";
    for(it = list.begin(); it != list.end(); ++it) {
        std::cout << (*it).first << " " ;
        rev_it = it;
    }

    std::cout<< "\nPrinting in descending order\n";
    for(rev_it; rev_it != list.begin(); --rev_it ) {
        std::cout << (*rev_it).first << " " ;
    }
    std::cout << (*rev_it).first << " ";
    std::cout << "\n";
    
/*
 *    while(1) {
 *        std::cin>>key;
 *        if(key == -1)
 *            break;
 *        list.erase(key);
 *        list.print_list();
 *    }
 *
 *    for(it = list.begin(); it != list.end(); ++it) {
 *        std::cout << (*it).first << " " ;
 *        rev_it = it;
 *    }
 *    std::cout << "\n";
 *    for(rev_it; rev_it != list.begin(); --rev_it ) {
 *        std::cout << (*rev_it).first << " " ;
 *    }
 *    std::cout << (*rev_it).first << " ";
 *    std::cout << "\n";
 */
    
     
    return 0;
}
