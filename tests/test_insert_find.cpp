/// @file test_insert_find.cpp
/// @brief Test for inserting and finding a single key in the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    trie.insert("key1", 10);

    int value;
    if (!trie.find("key1", value) || value != 10) {
        return 1;
    }
    return 0;
}