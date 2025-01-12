#include "SkipList.h"
#include <iostream>
#include <unordered_map>
#include <functional>
class Operations
{
private:
    SkipList<int, int> *_skip_list;
    std::unordered_map<std::string, std::function<void()>> command_map;

public:
    Operations(SkipList<int, int> *skip_list) : _skip_list(skip_list)
    {
        command_map = {
            {"insert", [&]()
             {
                 int key, value;
                 std::cin >> key >> value;
                 if (_skip_list->insert_element(key, value) == 0)
                 {
                     std::cout << "Insert Success" << std::endl;
                 }
                 else
                 {
                     std::cout << "Insert Failed" << std::endl;
                 }
             }},
            {"search", [&]()
             {
                 int key;
                 std::cin >> key;
                 if (_skip_list->search_element(key))
                 {
                     std::cout << "Search Success" << std::endl;
                 }
                 else
                 {
                     std::cout << "Search Failed" << std::endl;
                 }
             }},
            {"delete", [&]()
             {
                 int key;
                 std::cin >> key;
                 _skip_list->delete_element(key);
                 std::cout << "Delete Success" << std::endl;
             }},
            {"display", [&]()
             {
                 _skip_list->display_list();
             }},
            {"load", [&]()
             {
                 _skip_list->load_file();
             }},
            {"dump", [&]()
             {
                 _skip_list->dump_file();
             }}};
    };

    // 以后可以改成bind
    void execute_command(const std::string &command)
    {
        auto it = command_map.find(command);
        if (it != command_map.end())
        {
            it->second();
        }
        else
        {
            std::cout << "Invalid command" << std::endl;
        }
    }
};