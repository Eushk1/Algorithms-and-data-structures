//
//  test.cpp
//  Лаба 2 Деревья
//
//  Created by Егор on 03.12.2024.
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

enum Color { RED, BLACK };

class TreeNode {
public:
    int key;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int value) : key(value), left(nullptr), right(nullptr) {}

    void print() const {
        std::cout << key;
    }
};


class AVLTreeNode {
public:
    int key;
    AVLTreeNode* left;
    AVLTreeNode* right;
    int height;

    AVLTreeNode (int value) : key(value), left(nullptr), right(nullptr), height(1) {}

    void print() const{
        std::cout << key;
    }
};

class BRTreeNode {
public:
    int key;
    BRTreeNode* left;
    BRTreeNode* right;
    Color color;
    BRTreeNode* parent;

    BRTreeNode(int value) : key(value), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}

    void print() const{
        std::cout <<  key << "(" << (color == RED ? "R)" : "B)");
    }
};


class BinarytreeInterface{
protected:

    template <typename T>
    void inorderHelper(T* node) { //Одинаковая функция
        if (node != nullptr) {
            inorderHelper(node->left);
            node->print();
            std::cout << " ";
            inorderHelper(node->right);
        }
    }
    
    template <typename T>
    void preorderHelper(T* node) { //Одинаковая функция
        if (node != nullptr) {
            node->print();
            std::cout << " ";
            preorderHelper(node->left);
            preorderHelper(node->right);
        }
    }
    
    template <typename T>
    void postorderHelper(T* node) { //Одинаковая функция
        if (node != nullptr) {
            postorderHelper(node->left);
            postorderHelper(node->right);
            node->print();
            std::cout << " ";
        }
    }
    
    template <typename T>
    void clearHelper(T *node) { //Одинаковая функция
           if (node != nullptr) {
               clearHelper(node->left);
               clearHelper(node->right);
               delete node;
           }
   }
   template <typename T>
    T* findHelper(T* node, int key) { //разная функция
        if (node == nullptr || node->key == key) {
            return node;
        }
        if (key < node->key) {
            return findHelper(node->left, key);
        } else {
            return findHelper(node->right, key);
        }
    }
  template <typename T>
    int Сalculateheight(T *node) {
        if (node == nullptr) {
            return 0;
        }
        
        int leftHeight = Сalculateheight(node->left);
        int rightHeight = Сalculateheight(node->right);
        
        return std::max(leftHeight, rightHeight) + 1;
    }
    template <typename T>
    void levelOrderHelper(T* root) {
        if (root == nullptr) {
            std::cout << "V" << std::endl;
            return;
        }

        std::queue<T*> q;
        q.push(root);

        while (!q.empty()) {
            int levelSize = q.size();
            bool hasNonNull = false;
            
            for (int i = 0; i < levelSize; ++i) {
                T* current = q.front();
                q.pop();

                if (current != nullptr) {
                    current->print();
                    std::cout << " ";
                    q.push(current->left);
                    q.push(current->right);
                    hasNonNull = true;
                } else {
                    std::cout<< "V ";
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
public:
    virtual void insert(int key) = 0;
    virtual void deleteNode(int key) = 0;
    virtual bool find(int key) = 0;
    
    virtual void inorder() = 0;
    virtual void postorder() = 0;
    virtual void preorder() = 0;
    virtual void levelorder() = 0;
    
    virtual void Getheight() = 0;
    
    virtual void clear() = 0;

    virtual ~BinarytreeInterface() {}
};

class BinarySearchTree :BinarytreeInterface {
private:
    TreeNode* root;

    TreeNode* insert(TreeNode* node, int key) { //разная функция
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

    TreeNode* deleteNode(TreeNode* node, int key) { //разная функция
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

    TreeNode* minValueNode(TreeNode* node) { //разная функция
        TreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    

public:
    BinarySearchTree() : root(nullptr) {}
    
    void insert(int key) override{
        root = insert(root, key);
    }

    void deleteNode(int key) override{
        root = deleteNode(root, key);
    }

    bool find(int key) override{
        return findHelper(root, key) != nullptr;
    }

    void inorder() override{
        inorderHelper(root);
        std::cout << std::endl;
    }
    void postorder() override{
        postorderHelper(root);
        std::cout << std::endl;
    }
    
    void preorder() override{
        preorderHelper(root);
        std::cout << std::endl;
    }
    
    void levelorder() override{
        levelOrderHelper(root);
        std::cout << std::endl;
    }
    
    TreeNode* GetRoot(){
        return root;
    }
    
    void clear() override{
        clearHelper(root);
        root = nullptr;
    }
    
    void Getheight() override{
        std::cout<<"Height of tree: "<<Сalculateheight(root)<<std::endl;
    }
    
};


class AVLTree :BinarytreeInterface{
private:
    AVLTreeNode* root;

    int height(AVLTreeNode* node) {
        return node ? node->height : 0;
    }

    int getBalance(AVLTreeNode* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    AVLTreeNode* rightRotate(AVLTreeNode* y) {
        AVLTreeNode* x = y->left;
        AVLTreeNode* T2 = x->right;


        x->right = y;
        y->left = T2;


        y->height = std::max(height(y->left), height(y->right)) + 1;
        x->height = std::max(height(x->left), height(x->right)) + 1;


        return x;
    }

    AVLTreeNode* leftRotate(AVLTreeNode* x) {
        AVLTreeNode* y = x->right;
        AVLTreeNode* T2 = y->left;


        y->left = x;
        x->right = T2;


        x->height = std::max(height(x->left), height(x->right)) + 1;
        y->height = std::max(height(y->left), height(y->right)) + 1;


        return y;
    }

    AVLTreeNode* insert(AVLTreeNode* node, int key) {
        if (node == nullptr) {
            return new AVLTreeNode(key);
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

    AVLTreeNode* minValueNode(AVLTreeNode* node) {
        AVLTreeNode* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    AVLTreeNode* deleteNode(AVLTreeNode* root, int key) {
        if (root == nullptr) {
            return root;
        }

        if (key < root->key) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->key) {
            root->right = deleteNode(root->right, key);
        } else {
    
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLTreeNode* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else {
                    *root = *temp;
                }
                delete temp;
            } else {

                AVLTreeNode* temp = minValueNode(root->right);
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

    

public:
    AVLTree() : root(nullptr) {}
    
    void insert(int key) override{
        root = insert(root, key);
    }

    void deleteNode(int key) override{
        root = deleteNode(root, key);
    }

    bool find(int key) override{
        return findHelper(root, key) != nullptr;
    }

    void inorder() override{
        inorderHelper(root);
        std::cout << std::endl;
    }
    void postorder() override{
        postorderHelper(root);
        std::cout << std::endl;
    }
    
    void preorder() override{
        preorderHelper(root);
        std::cout << std::endl;
    }
    
    void levelorder() override{
        levelOrderHelper(root);
        std::cout << std::endl;
    }
    
    AVLTreeNode* GetRoot(){
        return root;
    }
    
    void clear() override{
        clearHelper(root);
        root = nullptr;
    }
    
    void Getheight() override{
        std::cout<<"Height of tree: "<<Сalculateheight(root)<<std::endl;
    }

};


class RedBlackTree :BinarytreeInterface {
private:
    BRTreeNode *root;

    void rotateLeft(BRTreeNode *&node) {
        BRTreeNode *node_right = node->right;
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

    void rotateRight(BRTreeNode *&node) {
        BRTreeNode *node_left = node->left;
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

    void fixViolation(BRTreeNode *&node) {
        BRTreeNode *parent_node = nullptr;
        BRTreeNode *grandparent_node = nullptr;

        while ((node != root) && (node->color == RED) && (node->parent->color == RED)) {
            parent_node = node->parent;
            grandparent_node = parent_node->parent;


            if (parent_node == grandparent_node->left) {
                BRTreeNode *uncle_node = grandparent_node->right;

  
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
                BRTreeNode *uncle_node = grandparent_node->left;


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
    
    
    void fixDoubleBlack(BRTreeNode *&node) {
            if (node == root) return;

           BRTreeNode *sibling = nullptr;
          BRTreeNode *parent = nullptr;

            while (node != root && (node == nullptr || node->color == BLACK)) {
                parent = node->parent;

                if (node == parent->left) {
                    sibling = parent->right;

                    if (sibling != nullptr && sibling->color == RED) {
                        sibling->color = BLACK;
                        parent->color = RED;
                        rotateLeft(parent);
                        sibling = parent->right;
                    }

                    if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                        (sibling->right == nullptr || sibling->right->color == BLACK)) {
                        sibling->color = RED;
                        node = parent;
                    } else {
                        if (sibling->right == nullptr || sibling->right->color == BLACK) {
                            if (sibling->left != nullptr) sibling->left->color = BLACK;
                            sibling->color = RED;
                            rotateRight(sibling);
                            sibling = parent->right;
                        }

                        sibling->color = parent->color;
                        parent->color = BLACK;
                        if (sibling->right != nullptr) sibling->right->color = BLACK;
                        rotateLeft(parent);
                        node = root;
                    }
                } else {
                    sibling = parent->left;

                    if (sibling != nullptr && sibling->color == RED) {
                        sibling->color = BLACK;
                        parent->color = RED;
                        rotateRight(parent);
                        sibling = parent->left;
                    }

                    if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                        (sibling->right == nullptr || sibling->right->color == BLACK)) {
                        sibling->color = RED;
                        node = parent;
                    } else {
                        if (sibling->left == nullptr || sibling->left->color == BLACK) {
                            if (sibling->right != nullptr) sibling->right->color = BLACK;
                            sibling->color = RED;
                            rotateLeft(sibling);
                            sibling = parent->left;
                        }

                        sibling->color = parent->color;
                        parent->color = BLACK;
                        if (sibling->left != nullptr) sibling->left->color = BLACK;
                        rotateRight(parent);
                        node = root;
                    }
                }
            }

            if (node != nullptr) node->color = BLACK;
        }
    
    BRTreeNode* insertHelper(BRTreeNode *root, BRTreeNode *newNode) {
        if (root == nullptr) {
            return newNode;
        }

        if (newNode->key < root->key) {
            root->left = insertHelper(root->left, newNode);
            root->left->parent = root;
        } else if (newNode->key > root->key) {
            root->right = insertHelper(root->right, newNode);
            root->right->parent = root;
        }

        return root;
    }
    
    void deleteNodeHelper(BRTreeNode *&node, const int &key) {
         if (node == nullptr) return;

         if (key < node->key) {
             deleteNodeHelper(node->left, key);
         } else if (key > node->key) {
             deleteNodeHelper(node->right, key);
         } else {
             if (node->left == nullptr || node->right == nullptr) {
                 BRTreeNode *temp = node->left ? node->left : node->right;

                 if (temp == nullptr) {
                     temp = node;
                     node = nullptr;
                 } else {
                     *node = *temp;
                 }
                 delete temp;
             } else {
                 BRTreeNode *temp = minValueNode(node->right);
                 node->key = temp->key;
                 deleteNodeHelper(node->right, temp->key);
             }
         }

         if (node == nullptr) return;

         if (node->color == BLACK) {
             fixDoubleBlack(node);
         }
     }

    BRTreeNode* minValueNode(BRTreeNode *node) {
        BRTreeNode *current = node;
         while (current && current->left != nullptr) {
             current = current->left;
         }
         return current;
     }



public:
    RedBlackTree() : root(nullptr) {}

    
    void insert(int key) override{
        BRTreeNode *newNode = new BRTreeNode(key);
        root = insertHelper(root, newNode);
        fixViolation(newNode);
    }

    void deleteNode(int key) override{
        deleteNodeHelper(root, key);
    }


    bool find(int key) override{
        return findHelper(root, key) != nullptr;
    }

    void inorder() override{
        inorderHelper(root);
        std::cout << std::endl;
    }
    void postorder() override{
        postorderHelper(root);
        std::cout << std::endl;
    }
    
    void preorder() override{
        preorderHelper(root);
        std::cout << std::endl;
    }
    
    void levelorder() override{
        levelOrderHelper(root);
        std::cout << std::endl;
    }
    
    BRTreeNode* GetRoot(){
        return root;
    }
    
    void clear() override{
        clearHelper(root);
        root = nullptr;
    }
    
    void Getheight() override{
        std::cout<<"Height of tree: "<<Сalculateheight(root)<<std::endl;
    }
    

};

int main() {
    RedBlackTree bst;

    // Вставка элементов
    std::cout << "Вставка элементов: 50, 30, 20, 40, 70, 60, 80" << std::endl;
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);
    bst.insert(70);
    bst.insert(60);
    bst.insert(80);

    // Обход дерева
    std::cout << "Обход в порядке inorder: ";
    bst.inorder();

    std::cout << "Обход в порядке preorder: ";
    bst.preorder();

    std::cout << "Обход в порядке postorder: ";
    bst.postorder();

    std::cout << "Уровневый обход: "<<std::endl;
    bst.levelorder();

    // Поиск элемента
    int keyToFind = 40;
    std::cout << "Поиск элемента " << keyToFind << ": "
              << (bst.find(keyToFind) ? "найден" : "не найден") << std::endl;

    // Удаление элемента
    std::cout << "Удаление элемента 20" << std::endl;
    bst.deleteNode(20);
    std::cout << "Обход в порядке inorder после удаления: ";
    bst.inorder();
    
    bst.Getheight();

    // Очистка дерева
    bst.clear();
    std::cout << "Дерево очищено." << std::endl;

    return 0;
}
