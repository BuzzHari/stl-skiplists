#include "skiplists.hpp"
#include <iostream>


int main() {

    skip_list::Skip_list<int, int> list(10);
    int key = 0, value = 0;

    while(1) {
        std::cin>>key>>value;
        if(key == -1)
            break;
        list.insert(key, value);
        list.print_list();
    }
    list.print_list();
    return 0;
}
