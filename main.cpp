/*
Nicholas Manoharan
Mr. Galbraith
C++/Data Structures
05/02/2024
*/

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

    // Initializing nodes
    Node(int val) : data(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

class RedBlackTree { 
private:
    Node* root; 
    Node* NIL;  // Sentinel NIL node

    void rotateLeft(Node* node); 
    void rotateRight(Node* node); 
    void fixViolation(Node* node); 
    void fixDelete(Node* node);
    void transplant(Node* u, Node* v);
    Node* minimum(Node* node);
    Node* search(Node* node, int data);

public: 
    RedBlackTree(); 
    void add(int data); 
    void readFromFile(const std::string& filename); 
    void print(); 
    bool search(int data);
    void remove(int data);

private: 
    void printHelper(Node* node, int space);
    void deleteNode(Node* node, int key);
};

RedBlackTree::RedBlackTree() : root(nullptr) {
    NIL = new Node(0);
    NIL->color = BLACK;
}

void RedBlackTree::add(int data) { 
    Node* newNode = new Node(data);
    newNode->left = NIL;
    newNode->right = NIL;

    if (root == nullptr) { 
        root = newNode; 
        root->color = BLACK; 
        root->parent = NIL;
        return; 
    }

    Node* current = root; 
    Node* parent = nullptr; 
    while(current != NIL) { 
        parent = current; 
        if (data < current->data) { 
            current = current->left; 
        } else if (data > current->data) { 
            current = current->right; 
        } else { 
            delete newNode;
            return; 
        }
    }

    newNode->parent = parent; 
    if (data < parent->data) { 
        parent->left = newNode; 
    } else { 
        parent->right = newNode; 
    }

    fixViolation(newNode);
} 

void RedBlackTree::rotateLeft(Node* node) { 
    Node* rightChild = node->right; 
    node->right = rightChild->left; 
    if (rightChild->left != NIL) { 
        rightChild->left->parent = node; 
    }

    rightChild->parent = node->parent;
    if (node->parent == nullptr) { 
        root = rightChild; 
    } else if (node == node->parent->left) { 
        node->parent->left = rightChild;             
    } else { 
        node->parent->right = rightChild; 
    }
    
    rightChild->left = node; 
    node->parent = rightChild; 
}

void RedBlackTree::rotateRight(Node* node) { 
    Node* leftChild = node->left; 
    node->left = leftChild->right; 
    if (leftChild->right != NIL) { 
        leftChild->right->parent = node; 
    }

    leftChild->parent = node->parent;
    if (node->parent == nullptr) { 
        root = leftChild; 
    } else if (node == node->parent->left) { 
        node->parent->left = leftChild;             
    } else { 
        node->parent->right = leftChild; 
    }
    
    leftChild->right = node; 
    node->parent = leftChild; 
}

void RedBlackTree::fixViolation(Node* node) {
    Node* parent = nullptr; 
    Node* grandparent = nullptr; 

    while (node != root && node->color != BLACK && node->parent->color == RED) { 
        parent = node->parent;
        grandparent = node->parent->parent; 

        if (parent == grandparent->left) { 
            Node* uncle = grandparent->right;

            if (uncle != nullptr && uncle->color == RED) { 
                grandparent->color = RED; 
                parent->color = BLACK; 
                uncle->color = BLACK; 
                node = grandparent; 
            } else { 
                if (node == parent->right) { 
                    rotateLeft(parent); 
                    node = parent; 
                    parent = node->parent; 
                }

                rotateRight(grandparent); 
                std::swap(parent->color, grandparent->color);
                node = parent; 
            }
        } else { 
            Node* uncle = grandparent->left; 

            if (uncle != nullptr && uncle->color == RED) { 
                grandparent->color = RED; 
                parent->color = BLACK; 
                uncle->color = BLACK; 
                node = grandparent; 
            } else { 
                if (node == parent->left) { 
                    rotateRight(parent); 
                    node = parent; 
                    parent = node->parent; 
                }

                rotateLeft(grandparent);
                std::swap(parent->color, grandparent->color); 
                node = parent; 
            }
        }
    }

    root->color = BLACK; 
}

void RedBlackTree::fixDelete(Node* node) {
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            Node* sibling = node->parent->right;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            Node* sibling = node->parent->left;
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}

void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node* RedBlackTree::minimum(Node* node) {
    while (node->left != NIL) {
        node = node->left;
    }
    return node;
}

Node* RedBlackTree::search(Node* node, int data) {
    if (node == NIL || data == node->data) {
        return node;
    }
    if (data < node->data) {
        return search(node->left, data);
    }
    return search(node->right, data);
}

bool RedBlackTree::search(int data) {
    return search(root, data) != NIL;
}

void RedBlackTree::remove(int data) {
    deleteNode(root, data);
}

void RedBlackTree::deleteNode(Node* node, int key) {
    Node* z = search(node, key);
    if (z == NIL) {
        return;
    }

    Node* y = z;
    Node* x;
    Color y_original_color = y->color;
    if (z->left == NIL) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->left);  // Error: should be z->right
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    delete z;
    if (y_original_color == BLACK) {
        fixDelete(x);
    }
}

void RedBlackTree::readFromFile(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file.is_open()) { 
        std::cerr << "Unable to open file " << filename << std::endl;
        return; 
    }

    std::string line; 
    while (std::getline(file, line)) { 
        std::istringstream iss(line);
        int num;
        while (iss >> num) { 
            add(num); 
        }
    }

    file.close();
}

void RedBlackTree::print() {
    if (root == nullptr) { 
        std::cout << "There ain't nothing in the tree" << std::endl;
        return; 
    }

    printHelper(root, 0); 
}

void RedBlackTree::printHelper(Node* node, int space) {
    const int COUNT = 5; 
    if (node == NIL) { 
        return; 
    }

    space += COUNT; 
    printHelper(node->right, space); 
    std::cout << std::endl;
    for (int i = COUNT; i < space; i++) { 
        std::cout << " "; 
    }

    std::cout << node->data << "(" << (node->color == RED ? "R" : "B") << ") ";
    if (node->parent != NIL) { 
        std::cout << "Parent: " << node->parent->data;
    } else { 
        std::cout << "Root"; 
    }
    std::cout << std::endl;

    printHelper(node->left, space); 
}

int main() {
    RedBlackTree tree;
    
    int choice, num;
    std::string filename;
    
    while (true) {
        std::cout << "Red-Black Tree Options:" << std::endl;
        std::cout << "1. Add a number" << std::endl;
        std::cout << "2. Read in numbers from a file" << std::endl;
        std::cout << "3. Print the tree" << std::endl;
        std::cout << "4. Search for a number" << std::endl;
        std::cout << "5. Remove a number" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                std::cout << "Enter a number to add: ";
                std::cin >> num;
                tree.add(num);
                std::cout << num << " was added to the tree." << std::endl;
                break;
            case 2:
                std::cout << "Enter the filename: ";
                std::cin >> filename;
                tree.readFromFile(filename);
                std::cout << "Numbers added to the tree." << std::endl;
                break;
            case 3:
                std::cout << "Tree:" << std::endl;
                tree.print();
                break;
            case 4:
                std::cout << "Enter a number to search: ";
                std::cin >> num;
                if (tree.search(num)) {
                    std::cout << num << " found in the tree." << std::endl;
                } else {
                    std::cout << num << " not found in the tree." << std::endl;
                }
                break;
            case 5:
                std::cout << "Enter a number to remove: ";
                std::cin >> num;
                tree.remove(num);
                std::cout << num << " was removed from the tree." << std::endl;
                break;
            case 6:
                std::cout << "Ending the program." << std::endl;
                return 0;
            default:
                std::cout << "Please enter a valid option." << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
