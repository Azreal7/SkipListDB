#pragma once

#include "Node.h"
#include <random>
#include <cstdlib>
#include <iostream>
#include <cstring>

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
private:
    int _max_level;
    int _skip_list_level;
    Node<K, V> *_header;
    int _element_count;
};

template <typename K, typename V>
Node<K, V> *SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
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