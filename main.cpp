#include "skiplists.hpp"
#include <iostream>


int main() {

    skip_list::Skip_list<int, int> list(5);
    int key = 0, value = 0;

    while(1) {
        std::cin>>key>>value;
        if(key == -1)
            break;
        list.insert(key, value);
        list.print_list();
    }
    list.print_list();

    list.print_list();
    
    skip_list::Skip_list<int, int>::Iterator it;
    skip_list::Skip_list<int, int>::Iterator rev_it;
    for(it = list.begin(); it != list.end(); ++it) {
        std::cout << (*it).first << " " ;
        rev_it = it;
    }
    std::cout << "\n";
    for(rev_it; rev_it != list.begin(); --rev_it ) {
        std::cout << (*rev_it).first << " " ;
    }
    std::cout << (*rev_it).first << " ";
    std::cout << "\n";
    
    while(1) {
        std::cin>>key;
        if(key == -1)
            break;
        list.erase(key);
        list.print_list();
    }

    for(it = list.begin(); it != list.end(); ++it) {
        std::cout << (*it).first << " " ;
        rev_it = it;
    }
    std::cout << "\n";
    for(rev_it; rev_it != list.begin(); --rev_it ) {
        std::cout << (*rev_it).first << " " ;
    }
    std::cout << (*rev_it).first << " ";
    std::cout << "\n";
    
    /*
     *std::cout<<"Search\n"; 
     *while(1) {
     *    std::cin>>key;
     *    if(key == -1)
     *        break;
     *    skip_list::Skip_list<int, int>::Iterator it;
     *    it = list.search(key);
     *    if(it != list.end())
     *        std::cout << (*it).first << " " << (*it).second << std::endl;
     *}
     */
    return 0;
}
