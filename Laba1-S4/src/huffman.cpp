//
//  huffman.cpp
//  АиСД лаба1 RLE
//
//  Created by Егор on 28.03.2025.
//

#include "huffman.hpp"
#include <queue>
#include <map>
#include <bitset>
#include <iomanip>
#include <iostream>

Huffman::Huffman(size_t block_size) : block_size(block_size) {}

Huffman::~Huffman() {}

void Huffman::build_codes(Node* root, std::string code, std::map<char, std::string>& codes) {
    if (!root) return;
    if (root->is_leaf()) {
        codes[root->symbol] = code;
        return;
    }
    build_codes(root->left, code + "0", codes);
    build_codes(root->right, code + "1", codes);
}

void Huffman::delete_tree(Node* root) {
    if (!root) return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

void Huffman::print_codes(const std::map<char, std::string>& codes) {
    std::cout << "Huffman Codes:" << std::endl;
    std::cout << "-----------------" << std::endl;
    std::cout << "| Symbol | Code  |" << std::endl;
    std::cout << "-----------------" << std::endl;
    for (const auto& [symbol, code] : codes) {
        std::cout << "|   " << std::setw(3) << symbol << "   | " << std::setw(6) << code << " |" << std::endl;
    }
    std::cout << "-----------------" << std::endl;
}

std::string Huffman::encode(const std::string& data) {
    std::string result;
    auto blocks = BlockProcessor::split_blocks(data, block_size);
    
    for (const auto& block : blocks) {
        std::map<char, int> freq;
        for (char c : block) freq[c]++;

        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
        for (auto& [c, f] : freq)
            pq.push(new Node(f, c));

        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            pq.push(new Node(left->freq + right->freq, '\0', left, right));
        }

        std::map<char, std::string> codes;
        if (!pq.empty())
            build_codes(pq.top(), "", codes);

        print_codes(codes);

        std::string bitstream;
        for (char c : block)
            bitstream += codes[c];

        // Add padding
        size_t padding = (8 - (bitstream.size() % 8)) % 8;
        bitstream += std::string(padding, '0');

        // Create header
        std::string header;
        header += static_cast<char>(padding);
        header += static_cast<char>((freq.size() >> 8) & 0xFF);
        header += static_cast<char>(freq.size() & 0xFF);
        
        for (auto& [c, f] : freq) {
            header += c;
            header += static_cast<char>((f >> 24) & 0xFF);
            header += static_cast<char>((f >> 16) & 0xFF);
            header += static_cast<char>((f >> 8) & 0xFF);
            header += static_cast<char>(f & 0xFF);
        }

        // Convert bits to bytes
        std::string encoded_block;
        for (size_t i = 0; i < bitstream.size(); i += 8) {
            std::string byte_str = bitstream.substr(i, 8);
            encoded_block += static_cast<char>(
                std::bitset<8>(byte_str).to_ulong()
            );
        }

        result += BlockProcessor::add_block_header(header + encoded_block);
        delete_tree(pq.top());
    }
    return result;
}

std::string Huffman::decode(const std::string& data) {
    std::string result;
    size_t ptr = 0;
    
    while (ptr < data.size()) {
        auto [block, new_ptr] = BlockProcessor::read_block(data, ptr);
        ptr = new_ptr;
        if (block.empty()) break;

        uint8_t padding = block[0];
        uint16_t num_syms = (static_cast<uint8_t>(block[1]) << 8
                           | static_cast<uint8_t>(block[2]));
        
        std::map<char, int> freq;
        size_t pos = 3;
        for (uint16_t i = 0; i < num_syms; ++i) {
            char c = block[pos];
            uint32_t f = static_cast<uint8_t>(block[pos+1]) << 24
                       | static_cast<uint8_t>(block[pos+2]) << 16
                       | static_cast<uint8_t>(block[pos+3]) << 8
                       | static_cast<uint8_t>(block[pos+4]);
            freq[c] = f;
            pos += 5;
        }

        // Rebuild tree
        std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
        for (auto& [c, f] : freq)
            pq.push(new Node(f, c));

        while (pq.size() > 1) {
            auto left = pq.top(); pq.pop();
            auto right = pq.top(); pq.pop();
            pq.push(new Node(left->freq + right->freq, '\0', left, right));
        }

        // Decode bits
        std::string bitstream;
        for (size_t i = pos; i < block.size(); ++i) {
            bitstream += std::bitset<8>(
                static_cast<unsigned char>(block[i])
            ).to_string();
        }
        if (padding > 0)
            bitstream.erase(bitstream.size() - padding);

        Node* root = pq.empty() ? nullptr : pq.top();
        Node* curr = root;
        std::string decoded;
        
        for (char bit : bitstream) {
            curr = (bit == '0') ? curr->left : curr->right;
            if (curr->is_leaf()) {
                decoded += curr->symbol;
                curr = root;
            }
        }
        
        result += decoded;
        delete_tree(root);
    }
    return result;
}
