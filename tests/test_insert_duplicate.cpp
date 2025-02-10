/// @file test_insert_duplicate.cpp
/// @brief Test for inserting a duplicate key in the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    trie.insert("key1", 10);
    trie.insert("key1", 20);

    int value;
    if (!trie.find("key1", value) || value != 20) {
        return 1;
    }
    return 0;
}