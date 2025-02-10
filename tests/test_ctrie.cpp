/// @file test_ctrie.cpp
/// @brief Simple ctrie test.
/// Copyright (c) 2024-2025. All rights reserved.
/// Licensed under the MIT License. See LICENSE file in the project root for details.

#include "ctrie/ctrie.hpp"
#include <iostream>

#define KEY1 "test"
#define KEY2 "test2test"

int main(int, char *[])
{
    int value;
    ctrie::CTrie<int> trie;
    trie.insert(KEY1, 5);
    if (!trie.find(KEY1, value))
        return 1;
    if (value != 5)
        return 1;
    trie.insert(KEY2, 3);
    if (!trie.find(KEY2, value))
        return 1;
    if (value != 3)
        return 1;
    trie.remove(KEY2);
    if (!trie.find(KEY1, value))
        return 1;
    if (value != 5)
        return 1;
    if (trie.find(KEY2, value))
        return 1;
    return 0;
}
