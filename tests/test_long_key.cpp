/// @file test_long_key.cpp
/// @brief Test for inserting and finding a long key in the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    std::string long_key(1000, 'a');
    trie.insert(long_key, 42);

    int value;
    if (!trie.find(long_key, value) || value != 42) {
        return 1;
    }
    return 0;
}
