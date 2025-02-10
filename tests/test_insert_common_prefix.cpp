/// @file test_insert_common_prefix.cpp
/// @brief Test for inserting multiple keys with a common prefix in the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    trie.insert("prefix1", 10);
    trie.insert("prefix2", 20);

    int value1, value2;
    if (!trie.find("prefix1", value1) || value1 != 10) {
        return 1;
    }
    if (!trie.find("prefix2", value2) || value2 != 20) {
        return 1;
    }
    return 0;
}