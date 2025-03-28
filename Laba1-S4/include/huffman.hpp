//
//  huffman.hpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <string>
#include <map>
#include <queue>
#include <vector>

class Huffman {
    struct Node {
        int freq;
        char symbol;
        Node* left;
        Node* right;
        
        Node(int f, char s, Node* l = nullptr, Node* r = nullptr)
            : freq(f), symbol(s), left(l), right(r) {}
            
        bool is_leaf() const { return !left && !right; }
    };

    struct Compare {
        bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
    };

    void build_codes(Node* root, std::string code, std::map<char, std::string>& codes);
    void delete_tree(Node* root);
    void print_codes(const std::map<char, std::string>& codes);

    size_t block_size;

public:
    Huffman(size_t block_size);
    ~Huffman();
    
    std::string encode(const std::string& data);
    std::string decode(const std::string& data);
};

#endif // HUFFMAN_H
