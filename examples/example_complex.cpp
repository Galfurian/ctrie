
#include "ctrie/ctrie.hpp"

int main()
{
    // Create a CTrie to store string values.
    ctrie::CTrie<std::string> trie;

    // Insert a variety of words and phrases.
    trie.insert("apple", "A fruit");
    trie.insert("app", "Short for application");
    trie.insert("application", "A software program");
    trie.insert("banana", "Another fruit");
    trie.insert("band", "A group of musicians");
    trie.insert("bandit", "A thief");
    trie.insert("bandwidth", "The capacity of a network");
    trie.insert("cat", "A small domesticated animal");
    trie.insert("caterpillar", "An insect that becomes a butterfly");
    trie.insert("catalog", "A list of items");
    trie.insert("dog", "A loyal animal");
    trie.insert("dove", "A symbol of peace");
    trie.insert("elephant", "A large mammal");

    // Print the pretty-printed trie structure.
    std::cout << "Complex CTrie Structure:\n";
    std::cout << trie.toString();

    return 0;
}
