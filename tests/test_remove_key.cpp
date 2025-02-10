/// @file test_remove_key.cpp
/// @brief Test for removing a key from the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    trie.insert("key1", 10);
    trie.remove("key1");

    int value;
    if (trie.find("key1", value)) {
        return 1;
    }
    return 0;
}