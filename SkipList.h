#pragma once
#define STORE_FILE "store/dumpfile"

#include "Node.h"
#include <random>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <cstring>
#include <fstream>

std::string delimiter = ":";
std::mutex mtx;

template <typename K, typename V>
class SkipList {
public:
    SkipList(int);
    int get_random_level();
    Node<K, V> *create_node(K, V, int);
    int insert_element(K, V);
    bool search_element(K);
    void delete_element(K);
    void display_list();
    void dump_file();
    void load_file();

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

private:
    int _max_level;
    int _skip_list_level;
    Node<K, V> *_header;
    int _element_count;
    std::ofstream _file_writer;
    std::ifstream _file_reader;
};

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
    std::lock_guard<std::mutex> lock(mtx);
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current != NULL && current->get_key() == key) {
        return 1;
    }
    if (current == NULL || current->get_key() != key) {
        int random_level = get_random_level();
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level + 1; i < random_level + 1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }
        Node<K, V> *inserted_node = create_node(key, value, random_level);
        for (int i = 0; i <= random_level; i++) {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        _element_count++;
    }
    return 0;
}

template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    Node<K, V> *current = _header;
    for (int i = _skip_list_level; i >= 0; i--)
    {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];
    if (current and current->get_key() == key) {
        return true;
    }
    return false;
}

template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;
    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
};

template<typename K, typename V>
int SkipList<K, V>::get_random_level(){
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
};

template<typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    std::lock_guard<std::mutex> lock(mtx);
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V> *) * (_max_level + 1));
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if(current && current->get_key() == key) {
        for(int i = 0; i <= _skip_list_level; i++) {
            if(update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        while(_skip_list_level > 0 && _header->forward[_skip_list_level] == nullptr) {
            _skip_list_level--;
        }
        delete current;
        _element_count--;
    }
    return;
}

template<typename K, typename V>
void SkipList<K, V>::display_list() {
    for (int i = _skip_list_level; i >= 0; i--) {
        Node<K, V>* node = this->_header->forward[i]; // 获取当前层的头节点
        std::cout << "Level " << i << ": ";
        // 遍历当前层的所有节点
        while (node != nullptr) {
            // 打印当前节点的键和值，键值对之间用":"分隔
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            // 移动到当前层的下一个节点
            node = node->forward[i];
        }
        std::cout << std::endl; // 当前层遍历结束，换行
    }
}

template<typename K, typename V>
void SkipList<K, V>::dump_file() {
    _file_writer.open(STORE_FILE);
    Node<K, V>* node = this->_header->forward[0];

    while(node != nullptr) {
        _file_writer << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {
    return !str.empty() && str.find(delimiter) != std::string::npos;
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) {
    if (!is_valid_string(str)) {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter) + 1);
}

template <typename K, typename V>
void SkipList<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    std::string line;
    std::string *key = new std::string();
    std::string *value = new std::string();

    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) {
            continue;
        }
        // Define key as int type
        insert_element(stoi(*key), stoi(*value));
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }

    delete key;
    delete value;
    _file_reader.close();
}