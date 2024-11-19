#include "SkipList.h"
#include <iostream>

int main() {
    SkipList<int, int> *skip_list = new SkipList<int, int>(16);
    std::string inst;
    while(std::cin >> inst) {
        if(inst == "insert") {
            int key;
            int value;
            std::cin >> key >> value;
            if (skip_list->insert_element(key, value) == 0) {
                std::cout << "Insert Success" << std::endl;
            } else {
                std::cout << "Insert Failed" << std::endl;
            }
        }
        else if (inst == "search") {
            int key;
            std::cin >> key;
            if (skip_list->search_element(key)) {
                std::cout << "Search Success" << std::endl;
            } else {
                std::cout << "Search Failed" << std::endl;
            }
        }
        else if (inst == "delete") {
            int key;
            std::cin >> key;
            skip_list->delete_element(key);
            std::cout << "Delete Success" << std::endl;
        }
        else if (inst == "display") {
            skip_list->display_list();
        }
        else if (inst == "load") {
            skip_list->load_file();
        }
        else if (inst == "dump") {
            skip_list->dump_file();
        }
        else {
            std::cout << "Wrong Instruction" << std::endl;
        }
    }

    return 0;
}