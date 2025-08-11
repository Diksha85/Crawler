#include "stringutils.h"
#include <cstdlib>
#include <iostream>

const int TABLE_SIZE = 1000;

template <typename T>
class HashTable {
private:
    struct Node {
        T value;
        Node* next;
    };

    Node* table[TABLE_SIZE];

    int hash(const char* str) {
        unsigned int hash = 5381;
        int i = 0;
        while (str[i] != '\0') {
            hash = ((hash << 5) + hash) + str[i];
            i++;
        }
        return hash % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current) {
                Node* temp = current;
                current = current->next;
                free(temp->value);
                delete temp;
            }
        }
    }

    bool isVisited(const char* url) {
        int index = hash(url);
        Node* current = table[index];
        while (current) {
            if (strCompare(current->value, url) == 0) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void markVisited(const char* url) {
        if (isVisited(url)) return;

        int len = strLength(url);
        char* copy = (char*)malloc(len + 1);
        strCopy(copy, url);

        Node* newNode = new Node;
        newNode->value = copy;
        int index = hash(url);
        newNode->next = table[index];
        table[index] = newNode;

        std::cout << "[HashTable] Marked as visited: " << url << std::endl;
    }
};

template class HashTable<char*>;
