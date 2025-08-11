
#include <iostream>
template <typename T>
struct Node {
    T data;
    Node<T>* next;
};

template <typename T>
void insert(Node<T>*& head, const T& value) {
    Node<T>* newNode = new Node<T>;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

template <typename T>
void freeList(Node<T>* head) {
    while (head) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void printList(Node<T>* head) {
    while (head) {
        std::cout << head->data << " -> ";
        head = head->next;
    }
    std::cout << "NULL\n";
}

bool isHtmlLink(const char* url);
bool strEndsWith(const char* str, const char* suffix);
int extractLinks(const char* filename, Node<char*>*& head);


