#include <iostream> 
#include <fstream> 
#include <sstream> 

enum Color { RED, BLACK }; 

struct Node { 

    int data; 
    Color color; 
    Node* parent; 
    Node* left; 
    Node* right; 
    Node (int val) : data(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree { 

private:
    Node* root; 
    void rotateleft(Node* node); 
    void rotateRight(Node* node); 
    void fixViolation(Node* node); 

public: 
    RedBlackTree(); 
    void add(int data); 
    void readFromFile(const std::string& filename); 
    void print(); 

private: 
    void printHelper(Node* node, int space);

};

RedBlackTree::RedBlackTree() : root(nullptr) {}

void RedBlackTree::add(int data) { 

    Node* newNode = new Node(data); 
    fixViolation(newNode); 

