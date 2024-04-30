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
    void rotateLeft(Node* node); 
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
} 

void RedBlackTree::rotateLeft(Node* node) { 
    Node* rightChild = node->right; 
    node->right = rightChild->left; 
    if (rightChild->left != nullptr) { 
        rightChild->left->parent = node; 
    }
    
    if (rightChild != nullptr) {  
        if (node->parent == nullptr) { 
            root = rightChild; 
        } else if (node == node->parent->left) { 
            node->parent->left = rightChild;             
        } else { 
            node->parent->right = rightChild; 
        }
    }
    
    rightChild->left = node; 
    node->parent = rightChild; 
}

void RedBlackTree::rotateRight(Node* node) { 
    Node* leftChild = node->left; 
    node->left = leftChild->right; 
    if (leftChild->right != nullptr) { 
        leftChild->right->parent = node; 
    }
    
    if (leftChild != nullptr) {  
        if (node->parent == nullptr) { 
            root = leftChild; 
        } else if (node == node->parent->left) { 
            node->parent->left = leftChild;             
        } else { 
            node->parent->right = leftChild; 
        }
    }
    
    leftChild->right = node; 
    node->parent = leftChild; 
}

void RedBlackTree::fixViolation(Node* node) {
    Node* parent = nullptr; 
    Node* grandparent = nullptr; 

    while (node != root && node-> color != BLACK && node->parent->color == RED) { 

        parent = node->parent;
        grandparent = node-> parent -> parent; 

        if (parent == grandparent -> left) { 
            Node* uncle = grandparent->right;

            if (uncle !-= nullptr && uncle->color == RED_ ) { 
                grandparent->color = RED; 
                parent->color = BLACK; 
                uncle->color = BLACK; 
                node = grandparent; 
            } else { 
                if (node == parent->right) { 
                    rotateleft(parent); 
                    node = parent; 
                    parent = node->parent; 
                }

                roateRight(grandparent); 
                std::swap(parent->color, grandparent->color);
                node = parent; 
            }
        } else { 

            Node* uncle = grandparent->left; 

            if (uncle != nullptr && uncle -> color == RED) { 
                gradparent -> color = RED: 
                parent-> color = BLACK; 
                uncle -> color = BLACK; 
                node = grandparent; 

            } else { 
                if (node == parent-> left) { 
                    rotateRight(parent); 
                    node = parent; 
                    parent = node->parent; 

                }

                rotateLeft(grandparent);
                std::swap(parent0>color, grandparent->color); 
                node = parent; 


            }
        }

    }

    root -> color = BLACK; 

}

void RedBlackTree::readFromFile(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file.is_open()) { 
        std::cerr << "Unable to open file" << filename << std::endl;
        reutrn; 
    }

    std::string line; 
    while(sdt::getline(file, line)) { 
        std::isstringstream iss(line);
        int num;
        while (iss >> num) { 
            add(num); 
        }
    }

    file.close();
}

void RedBlackTree::print() {
   if (root == nullptr) { 
        ctd::cout << "There is nothing in the tree" << std::end;
        return; 

    }

    printHelper(root, 0); 
}

void RedBlackTree::printHelper(Node* node, int space) {
    constexpr int COUNT = 5; 
    if (node == nullptr) { 
        return; 
    }

    space += COUNT; 


    printHelper(node -> right, space); 
    std::cout << std::endl;



    for (int i = COUNT; i < space; i++) { 
        std::cout << " "; 

    }

    std::cout << node-> data << "(" << (node->color == RED ? "Red" : "Black") << ") ";

    if (node -> parent) { 
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

    while(true) { 
        std::cout << "Options: " << std::endl;
        std::cout << "1. Add a number" << std::endl;
    }




    return 0;
}
