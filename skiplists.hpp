#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <climits>
#include <random>
#include <iostream>
#include <utility>
#include <unordered_map>

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
                head = new Node(max_levels, INT_MIN );
            };

            // Copy Constructor
            Skip_list(const Skip_list& other_list) {
                
                copy_list(other_list);
                
            }

            // Copy Assignment
            Skip_list& operator=(const Skip_list& other_list) {
                Node *c_ptr = other_list.head;
                Node *ptr = head;
                
                // Starting from level zero.
                // The current node->ptrnode.
                std::unordered_map<Node *, Node *> hash_map;

                while(c_ptr->forward[0]) {
                    ptr->forward[0] = new Node(c_ptr->forward[0]->level, 
                            c_ptr->forward[0]->key, c_ptr->forward[0]->value);
                
                    ptr->forward[0]->prev = ptr;

                    hash_map[c_ptr->forward[0]] = ptr->forward[0];
                    c_ptr = c_ptr->forward[0];
                    ptr = ptr->forward[0];
                }
            
                c_ptr = other_list.head->forward[0];
                ptr = head->forward[0];
                for (int i = 1; i <= curr_levels; ++i) 
                    head->forward[i] = hash_map[other_list.head->forward[i]];
                
                while(c_ptr) {
                    for(int i = 1; i < c_ptr->level; ++i) {
                        ptr->forward[i] = hash_map[c_ptr->forward[i]];
                    }
                    c_ptr = c_ptr->forward[0];
                    ptr = ptr->forward[0];
                }

                return *this;
            }

            // Move Constructor 
            Skip_list(Skip_list&& other_list): 
                curr_levels{other_list.curr_levels},
                max_levels{other_list.max_levels},
                head{other_list.head}
            {
                other_list.head = nullptr;
            }

            // Move Assignment
            Skip_list& operator=(Skip_list&& other_list) {
                
                std::cout << "Move Constructor called\n";
                curr_levels = other_list.curr_levels;
                max_levels = other_list.max_levels;
                head = other_list.head;

                other_list.head = nullptr;

                return *this;
            }
            ~Skip_list() {
                if(!head)
                    return;
                Node *ptr = head->forward[0];
                Node *temp;
                
                while(ptr) {
                    temp = ptr->forward[0];
                    delete ptr;
                    ptr = temp;
                }
                
                delete head;
            }
            
            // Iterators.
            class Iterator {
                public:
                    Iterator() : curr_it{ nullptr }
                    {
                    };
                    explicit Iterator(Node* pos) : curr_it{ pos }
                    {
                    };
                    
                    bool operator==(const Iterator& rhs) const {
                        return curr_it == rhs.curr_it;
                    }
                    bool operator!=(const Iterator& rhs) const {
                        return !(curr_it == rhs.curr_it);
                    }
                    std::pair<key_type, value_type> operator*() {
                        return std::make_pair(curr_it->key,curr_it->value);
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
                return Iterator{nullptr}; 
            }


            void insert(const key_type& key, const value_type& value);
            void erase(const key_type& key);
            typename Skip_list<K, T>::Iterator search(const key_type& key);
            void print_list();
        private:        
            typedef struct Node {
                int level;
                key_type key;
                value_type value;
                struct Node **forward;
                struct Node *prev; 
    
                explicit Node(int lvl, const key_type& k) {
                    level = lvl;
                    key = k;
                    forward = new Node*[lvl];
                    for(int i = 0; i < lvl; i++)
                        forward[i] = nullptr;

                    prev = nullptr;
                }
                
                explicit Node(int lvl, const key_type& k, const value_type& v) {
                    level = lvl;
                    key = k;
                    value = v;
                    forward = new Node*[lvl];
                    for(int i = 0; i < lvl; i++)
                        forward[i] = nullptr;

                    prev = nullptr;
                }

                ~Node() {
                    delete[] forward;
                }
            }Node;

            int curr_levels;
            int max_levels;
            Node *head;

            void copy_list(const Skip_list& other);

            int generate_level() {
                // Using random_device for the initial seed.
                //static std::random_device rd;
                // Using the engine based on mersenne-twister algo.
                static std::mt19937 engine(0);
                // Will produce either 0 or 1 with near equal probability.
                static std::uniform_int_distribution<> flip(0,1);

                int level = 1;
                while(flip(engine) && level < max_levels)
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

        int gen_level = generate_level();
       // std::cout<<"List levels:" << curr_levels << std::endl;
       // std::cout<<"gen_level: " << gen_level<<"\n";

        if (gen_level > curr_levels + 1) {
            for(int i = curr_levels + 1; i < gen_level; ++i)
                update[i] = head;
            curr_levels = gen_level-1;
        }

        ptr = new Node(gen_level, key, value);

        for(int i = 0; i < gen_level; ++i) {
            ptr->forward[i] = update[i]->forward[i];
            
            update[i]->forward[i] = ptr;
            
            // Connect the prevs only for level 0.
            if(i == 0) {
                ptr->prev = update[i];
                if(ptr->forward[i])
                    ptr->forward[i]->prev = ptr;
            }
            
           //std::cout<<"Inserted value: "<<update[i]->forward[i]->key<<std::endl;
        }
        //std::cout<<"Value Inserted\n";
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
            // ptr now points to the node to be deleted.
            for(int i = 0; i <= curr_levels; ++i) {
                // making sure it's the ndoe before the node to be deleted.
                if(update[i]->forward[i] != ptr)
                    break;
                update[i]->forward[i] = ptr->forward[i];

                if(i == 0 && ptr->forward[i]) {
                    ptr->forward[i]->prev = update[i];
                }
            }
            
            delete ptr;    
        }
        // Deleting the level if empty.
        while(curr_levels > 0 && head->forward[curr_levels] == nullptr) {
            std::cout<<"In Here\n";
            curr_levels-=1;
        }

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
        if(!ptr) {
            std::cout<<"No such list exist\n";
            return;
        }
            
        for(int i = curr_levels; i >= 0; --i) {
            std::cout<<"Level "<<i<<":";
            ptr = head;

            while(ptr != nullptr) {
                if(ptr != head)
                    std::cout<<ptr->key<<" ";
                ptr = ptr->forward[i];
            }
            std::cout<<"\n";
        }
    }

    // Copy list.
    template<typename K, typename V>
    void Skip_list<K, V>::copy_list(const Skip_list& other_list) {
        
        // From copy constructor, intialize the new list.
        curr_levels = other_list.curr_levels;
        max_levels = other_list.max_levels;
        head = new Node(max_levels, INT_MIN) ;

        
        Node *c_ptr = other_list.head;
        Node *ptr = head;
        
        // Starting from level zero.
        // The current node->ptrnode.
        std::unordered_map<Node *, Node *> hash_map;

        while(c_ptr->forward[0]) {
            ptr->forward[0] = new Node(c_ptr->forward[0]->level, 
                    c_ptr->forward[0]->key, c_ptr->forward[0]->value);
        
            ptr->forward[0]->prev = ptr;

            hash_map[c_ptr->forward[0]] = ptr->forward[0];
            c_ptr = c_ptr->forward[0];
            ptr = ptr->forward[0];
        }
    
        c_ptr = other_list.head->forward[0];
        ptr = head->forward[0];
        for (int i = 1; i <= curr_levels; ++i) 
            head->forward[i] = hash_map[other_list.head->forward[i]];
        
        while(c_ptr) {
            for(int i = 1; i < c_ptr->level; ++i) {
                ptr->forward[i] = hash_map[c_ptr->forward[i]];
            }
            c_ptr = c_ptr->forward[0];
            ptr = ptr->forward[0];
        }
    }
}
#endif
