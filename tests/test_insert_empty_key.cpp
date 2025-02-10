/// @file test_insert_empty_key.cpp
/// @brief Test for inserting an empty key in the CTrie.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"

#include <iostream>

int main()
{
    ctrie::CTrie<int> trie;
    if (trie.insert("", 10)) {
        return 1;
    }
    return 0;
}
