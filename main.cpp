//
//  main.cpp
//  Лаба 2 Деревья
//
//  Created by Егор on 23.11.2024.
//

#include <iostream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cmath>
#include <queue>



std::vector<int> generateRandomVector(int n) {
  std::vector<int> vec(n);
  for (int i = 1; i <= n; ++i) {
    vec[i - 1] = i;
  }
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(vec.begin(), vec.end(), g);
  return vec;
}

class TreeNode {
public:
    int key;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int value) : key(value), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int key) {
        if (node == nullptr) {
            return new TreeNode(key);
        }
        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        }
        return node;
    }

    TreeNode* find(TreeNode* node, int key) {
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return find(node->left, key);
        } else {
            return find(node->right, key);
        }
    }

    TreeNode* deleteNode(TreeNode* node, int key) {
        if (node == nullptr) {
            return node;
        }
        if (key < node->key) {
            node->left = deleteNode(node->left, key);
        } else if (key > node->key) {
            node->right = deleteNode(node->right, key);
        } else {

            if (node->left == nullptr) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            TreeNode* temp = minValueNode(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    TreeNode* minValueNode(TreeNode* node) {
        TreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void inorder(TreeNode* node) {
        if (node != nullptr) {
            inorder(node->left);
            std::cout << node->key << " ";
            inorder(node->right);
        }
    }
    
    void clearHelper(TreeNode *node) {
           if (node != nullptr) {
               clearHelper(node->left);
               clearHelper(node->right);
               delete node;
           }
   }
public:
    BinarySearchTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void deleteNode(int key) {
        root = deleteNode(root, key);
    }

    bool find(int key) {
        return find(root, key) != nullptr;
    }

    void inorder() {
        inorder(root);
        std::cout << std::endl;
    }
    int Сalculateheight(TreeNode *node) {
        if (node == nullptr) {
            return 0;
        }
        
        int leftHeight = Сalculateheight(node->left);
        int rightHeight = Сalculateheight(node->right);
        
        return std::max(leftHeight, rightHeight) + 1;
    }
    TreeNode* GetRoot(){
        return root;
    }
    void clear() {
            clearHelper(root);
            root = nullptr;
    }
    
};


class TreeNode2 {
public:
    int key;
    TreeNode2* left;
    TreeNode2* right;
    int height;

    TreeNode2(int value) : key(value), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    TreeNode2* root;

    int height(TreeNode2* node) {
        return node ? node->height : 0;
    }

    int getBalance(TreeNode2* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    TreeNode2* rightRotate(TreeNode2* y) {
        TreeNode2* x = y->left;
        TreeNode2* T2 = x->right;


        x->right = y;
        y->left = T2;


        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;


        return x;
    }

    TreeNode2* leftRotate(TreeNode2* x) {
        TreeNode2* y = x->right;
        TreeNode2* T2 = y->left;


        y->left = x;
        x->right = T2;


        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;


        return y;
    }

    TreeNode2* insert(TreeNode2* node, int key) {
        if (node == nullptr) {
            return new TreeNode2(key);
        }

        if (key < node->key) {
            node->left = insert(node->left, key);
        } else if (key > node->key) {
            node->right = insert(node->right, key);
        } else {
            return node;
        }


        node->height = 1 + std::max(height(node->left), height(node->right));


        int balance = getBalance(node);


        if (balance > 1 && key < node->left->key) {
            return rightRotate(node);
        }


        if (balance < -1 && key > node->right->key) {
            return leftRotate(node);
        }


        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }


        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    TreeNode2* minValueNode(TreeNode2* node) {
        TreeNode2* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    TreeNode2* deleteNode(TreeNode2* root, int key) {
        if (root == nullptr) {
            return root;
        }

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        } else {
    
            if ((root->left == nullptr) || (root->right == nullptr)) {
                TreeNode2* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {

                TreeNode2* temp = minValueNode(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr) {
            return root;
        }


        root->height = 1 + std::max(height(root->left), height(root->right));


        int balance = getBalance(root);


        if (balance > 1 && getBalance(root->left) >= 0) {
            return rightRotate(root);
        }


        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0) {
            return leftRotate(root);
        }


        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    void inorder(TreeNode2* node) {
        if (node != nullptr) {
            inorder(node->left);
            std::cout << node->key << " ";
            inorder(node->right);
        }
    }
    void clearHelper(TreeNode2 *node) {
        if (node != nullptr) {
            clearHelper(node->left);
            clearHelper(node->right);
            delete node;
        }
    }
    


    
public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void deleteNode(int key) {
        root = deleteNode(root, key);
    }

    bool find(int key) {
        TreeNode2* current = root;
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            } else if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return false;
    }

    void inorder() {
        inorder(root);
        std::cout << std::endl;
    }
    
    int Сalculateheight(TreeNode2 *node) {
        if (node == nullptr) {
            return 0;
        }
        
        int leftHeight = Сalculateheight(node->left);
        int rightHeight = Сalculateheight(node->right);
        
        return std::max(leftHeight, rightHeight) + 1;
    }
    TreeNode2* GetRoot(){
        return root;
    }
    void clear() {
            clearHelper(root);
            root = nullptr;
    }
    void levelOrder(TreeNode2* root) {
        if (root == nullptr) return;

        std::queue<TreeNode2*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();

            for (int i = 0; i < levelSize; ++i) {
                TreeNode2* current = q.front();
                q.pop();
                std::cout << current->key << " ";

                if (current->left != nullptr) {
                    q.push(current->left);
                }
                if (current->right != nullptr) {
                    q.push(current->right);
                }
            }

            std::cout << std::endl;
        }
    }
    
};

enum Color { RED, BLACK };

class TreeNode3 {
public:
    int key;
    Color color;
    TreeNode3 *left, *right, *parent;

    TreeNode3(int value) : key(value), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    TreeNode3 *root;

    void rotateLeft(TreeNode3 *&node) {
        TreeNode3 *node_right = node->right;
        node->right = node_right->left;

        if (node->right != nullptr) {
            node->right->parent = node;
        }

        node_right->parent = node->parent;

        if (node->parent == nullptr) {
            root = node_right;
        } else if (node == node->parent->left) {
            node->parent->left = node_right;
        } else {
            node->parent->right = node_right;
        }

        node_right->left = node;
        node->parent = node_right;
    }

    void rotateRight(TreeNode3 *&node) {
        TreeNode3 *node_left = node->left;
        node->left = node_left->right;

        if (node->left != nullptr) {
            node->left->parent = node;
        }

        node_left->parent = node->parent;

        if (node->parent == nullptr) {
            root = node_left;
        } else if (node == node->parent->left) {
            node->parent->left = node_left;
        } else {
            node->parent->right = node_left;
        }

        node_left->right = node;
        node->parent = node_left;
    }

    void fixViolation(TreeNode3 *&node) {
        TreeNode3 *parent_node = nullptr;
        TreeNode3 *grandparent_node = nullptr;

        while ((node != root) && (node->color == RED) && (node->parent->color == RED)) {
            parent_node = node->parent;
            grandparent_node = parent_node->parent;


            if (parent_node == grandparent_node->left) {
                TreeNode3 *uncle_node = grandparent_node->right;

  
                if (uncle_node != nullptr && uncle_node->color == RED) {
                    grandparent_node->color = RED;
                    parent_node->color = BLACK;
                    uncle_node->color = BLACK;
                    node = grandparent_node;
                } else {
  
                    if (node == parent_node->right) {
                        rotateLeft(parent_node);
                        node = parent_node;
                        parent_node = node->parent;
                    }


                    rotateRight(grandparent_node);
                    std::swap(parent_node->color, grandparent_node->color);
                    node = parent_node;
                }
            } else {
                TreeNode3 *uncle_node = grandparent_node->left;


                if ((uncle_node != nullptr) && (uncle_node->color == RED)) {
                    grandparent_node->color = RED;
                    parent_node->color = BLACK;
                    uncle_node->color = BLACK;
                    node = grandparent_node;
                } else {

                    if (node == parent_node->left) {
                        rotateRight(parent_node);
                        node = parent_node;
                        parent_node = node->parent;
                    }

     
                    rotateLeft(grandparent_node);
                    std::swap(parent_node->color, grandparent_node->color);
                    node = parent_node;
                }
            }
        }
        root->color = BLACK;
    }

    void inorderHelper(TreeNode3 *node) {
        if (node != nullptr) {
            inorderHelper(node->left);
            std::cout << node->key << "(" << (node->color == RED ? "R" : "B") << ") ";
            inorderHelper(node->right);
        }
    }
    
    void clearHelper(TreeNode3 *node) {
           if (node != nullptr) {
               clearHelper(node->left);
               clearHelper(node->right);
               delete node;
           }
       }


public:
    RedBlackTree() : root(nullptr) {}

    void insert(const int &key) {
        TreeNode3 *newNode = new TreeNode3(key);
        root = bstInsert(root, newNode);
        fixViolation(newNode);
    }

    TreeNode3* bstInsert(TreeNode3 *root, TreeNode3 *newNode) {
        if (root == nullptr) {
            return newNode;
        }

        if (newNode->key < root->key) {
            root->left = bstInsert(root->left, newNode);
            root->left->parent = root;
        } else if (newNode->key > root->key) {
            root->right = bstInsert(root->right, newNode);
            root->right->parent = root;
        }

        return root;
    }

    void inorder() {
        inorderHelper(root);
        std::cout << std::endl;
    }
    
    int Сalculateheight(TreeNode3 *node) {
        if (node == nullptr) {
            return 0;
        }
        
        int leftHeight = Сalculateheight(node->left);
        int rightHeight = Сalculateheight(node->right);
        
        return std::max(leftHeight, rightHeight) + 1;
    }
    TreeNode3* GetRoot(){
        return root;
    }
    
    void clear() {
            clearHelper(root);
            root = nullptr;
        }
    void levelOrder(TreeNode3* root) {
        if (root == nullptr) {
            std::cout << "V" << std::endl;
            return;
        }

        std::queue<TreeNode3*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            bool hasNonNull = false;
            
            for (int i = 0; i < levelSize; ++i) {
                TreeNode3* current = q.front();
                q.pop();

                if (current != nullptr) {
                    std::cout<<std::setw(5) << current->key << "(" << (current->color == RED ? "R" : "B") << ") ";
                    q.push(current->left);
                    q.push(current->right);
                    hasNonNull = true;
                } else {
                    std::cout<<std::setw(5) << "V";
                    q.push(nullptr);
                    q.push(nullptr);
                }
            }
            

            std::cout << std::endl;
            
            if (!hasNonNull) {
                        break;
                    }

        }
    }

};


int main() {
    RedBlackTree rbt;
    int n = 10;
    std::vector<int> array;
     
    std::fstream Out_Data;
    
    Out_Data.open("/Users/Egor 1/Desktop/Прога курсовик 3 сем/Лабы АиСД/Лаба 2/Outdata.txt", std::ios::app);
    
    if (!Out_Data.is_open()) {
            std::cout << "Error opening file!";
            return 1;
        } else {
           std::cout << "File Open\n";
    }

    
    for(int l=0;l<=0;l++){
        
        for(int i=0; i<1; i++){
            array=generateRandomVector(n);
            
            
            for (const int& value : array) {
                rbt.insert(value);
            }
            
            rbt.levelOrder(rbt.GetRoot());
                
           Out_Data<<n<<" "<<rbt.Сalculateheight(rbt.GetRoot())<<std::endl;
           array.clear();
           rbt.clear();
            
        }
        n+=20;
    }
    Out_Data.close();
    
    return 0;
}
