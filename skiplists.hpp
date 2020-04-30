#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <climits>
#include <random>
#include <iostream>

namespace skip_list {
    
    template<typename K, typename T>
    class Skip_list {
         
        private: 
            struct Node;
        public:
            using value_type = T;
            using key_type = K;
            explicit Skip_list(int mlevels = 5) {
                
                curr_levels = 0;
                max_levels = mlevels; 
                head = new Node(max_levels, INT_MIN, INT_MIN);
            };
            
            // Iterators.
            class Iterator {
                public:
                    Iterator() : curr_it{ nullptr }
                    {
                    };
                    explicit Iterator(Node* pos) : curr_it{ pos }
                    {
                    };
                    
                    // Comparing by key.
                    bool operator==(const Iterator& rhs) const {
                        return curr_it == rhs.curr_it;
                    }
                    bool operator!=(const Iterator& rhs) const {
                        return !(curr_it == rhs.curr_it);
                    }
                    value_type& operator*() {
                        return curr_it->value;
                    }
                    value_type* operator->() {
                        return &curr_it->value;
                    }

                    // pre-inc
                    Iterator& operator++() {
                        if (curr_it == nullptr)
                            return *this;

                        curr_it = curr_it->forward[0];
                        return *this;
                    }
                    // post-inc
                    Iterator& operator++(int) {
                        Iterator temp(*this);
                        ++*this;
                        return temp;
                    }
                    
                    // pre-dec
                    Iterator& operator--() {
                        if (curr_it == nullptr)
                            return *this;
                        
                        curr_it = curr_it->prev;
                        return *this;
                    }
                    // post-dec
                    Iterator& operator--(int) {
                        Iterator temp(*this);
                        --*this;
                        return temp;
                    }
                private:
                    Node *curr_it;
            };

            Iterator begin() {
                return Iterator{ head->forward[0] };
            }
            
            // The end ptr is always null. 
            Iterator end() {
                return{ nullptr };
            }


            void insert(const key_type& key, const value_type& value);
            void erase(const key_type& key);
            typename Skip_list<K, T>::Iterator search(const key_type& key);
            void print_list();
        private:        
            typedef struct Node {
                key_type key;
                value_type value;
                struct Node **forward;
                struct Node *prev; 

                explicit Node(int level, const key_type& k, const value_type& v) {
                    key = k;
                    value = v;
                    forward = new Node*[level];
                    for(int i = 0; i < level; i++)
                        forward[i] = nullptr;

                    prev = nullptr;
                }
            }Node;

            int curr_levels;
            int max_levels;
            Node *head;

            int generate_level() {
                // Using random_device for the initial seed.
                static std::random_device rd;
                // Using the engine based on mersenne-twister algo.
                static std::mt19937 engine(rd());
                // Will produce either 0 or 1 with near equal probability.
                static std::uniform_int_distribution<> flip(0,1);

                int level = 1;
                while(flip(engine) && level <= max_levels)
                    level+=1;

                return level;
            }
    };
    
    // Insert.
    template<typename K, typename V>
    void Skip_list<K, V>::insert(const K& key, const V& value) {
        Node *update[max_levels];

        Node *ptr = head;

        for(int i = curr_levels; i >= 0; --i) {
            while(ptr->forward[i] && ptr->forward[i]->key < key )
                ptr = ptr->forward[i];
            update[i] = ptr;
        }

        ptr = ptr->forward[0];
        
        // Update the value if key already exists.
        if(ptr && ptr->key == key) {
            ptr->value = value;
            std::cout<<"Value Updated"<<std::endl;
            return;
        }

        int gen_level= generate_level();
        std::cout<<"List levels:" << curr_levels << std::endl;
        
        if (gen_level > curr_levels + 1) {
            for(int i = curr_levels + 1; i < gen_level; ++i)
                update[i] = head;
            curr_levels = gen_level-1;
        }

        ptr = new Node(gen_level, key, value);

        for(int i = 0; i < gen_level; ++i) {
            ptr->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = ptr;
            std::cout<<"Inserted value: "<<update[i]->forward[i]->key<<std::endl;
        }
        std::cout<<"Value Inserted\n";
    }

    // Delete.
    template<typename K, typename V>
    void Skip_list<K, V>::erase(const K& key) {

        Node *update[max_levels];

        Node *ptr = head;
        for(int i = curr_levels; i >= 0; --i) {
            while(ptr->forward[i] && ptr->forward[i]->key < key)
                ptr = ptr->forward[i];
            update[i] = ptr;
        }

        ptr = ptr->forward[0];

        if(ptr && ptr->key == key) {
            for(int i = 0; i < curr_levels; ++i) {
                // making sure it's the ndoe before the node to be deleted.
                if(update[i]->forward[i] != ptr)
                    break;
                update[i]->forward[i] = ptr->forward[i];
            }
            delete ptr;
        }
        // Deleting the level if empty.
        while(curr_levels > 0 && head->forward[curr_levels] == NULL)
            --curr_levels;

        std::cout<<"Delete Successfull\n";
    }

    // Search.
    template<typename K, typename V>
    typename Skip_list<K, V>::Iterator Skip_list<K, V>::search(const K& key) {
        // Just in case if head is null, return null.
        if(!head)
            return Iterator();

        Node *ptr = head;
        for(int i = curr_levels; i >= 0; --i) {
            while(ptr->forward[i] && ptr->forward[i]->key < key)
                ptr = ptr->forward[i];
        }
        ptr = ptr->forward[0];

        if(ptr && ptr->key == key)
            return Iterator(ptr);
        
        // return end iterator if not found. 
        return end();
    }

    // Display.
    template<typename K, typename V> 
    void Skip_list<K, V>::print_list() {
        Node *ptr = head;
        for(int i = curr_levels; i >= 0; --i) {
            std::cout<<"Level "<<i<<":";
            ptr = head;

            while(ptr) {
                std::cout<<ptr->key<<" ";
                ptr = ptr->forward[i];
            }
            std::cout<<"\n";
        }
    }
}
#endif
