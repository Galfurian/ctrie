# CTrie

[![Ubuntu](https://github.com/Galfurian/ctrie/actions/workflows/ubuntu.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/ubuntu.yml)
[![Windows](https://github.com/Galfurian/ctrie/actions/workflows/windows.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/windows.yml)
[![MacOS](https://github.com/Galfurian/ctrie/actions/workflows/macos.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/macos.yml)
[![Documentation](https://github.com/Galfurian/ctrie/actions/workflows/documentation.yml/badge.svg)](https://github.com/Galfurian/ctrie/actions/workflows/documentation.yml)

CTrie is a **concurrent prefix tree** (trie) implementation in C++. It provides
an efficient, hierarchical data structure for storing key-value pairs where the
keys are strings. The library supports thread-safe operations (when compiled
with C++11 or later) and is suitable for use cases like autocomplete, dictionary
storage, or hierarchical data management.

## Features

- **Efficient Storage**: Stores keys hierarchically, minimizing redundancy for common prefixes.
- **Thread Safety**: Supports thread-safe operations when compiled with C++11 or later.
- **Key-Value Storage**: Associates string keys with arbitrary values.
- **Customizable**: Fully templated to store values of any type.
- **Pretty Printing**: Provides a string representation of the trie structure.

## Requirements

- **C++11 or later** (for thread safety and modern features)
- A standard C++ compiler

## Installation

Clone the repository:

```bash
git clone https://github.com/yourusername/ctrie.git
```

Include the ctrie.hpp header file in your project:

```cpp
#include "ctrie/ctrie.hpp"
```

That's it! There's no additional setup or dependencies.

## API Reference

`CTrie`

Member Functions:

- `bool insert(const std::string &key, T value)` Inserts a key-value pair into the trie.
- `bool find(const std::string &key, T &value)` const Finds the value associated with a key.
- `bool remove(const std::string &key)` Removes a key-value pair from the trie.
- `std::string toString() const` Returns a string representation of the trie.

## Examples

Here are a couple of examples.

### Example 1: Basic Insertion and Retrieval

```cpp
#include "ctrie/ctrie.hpp"
#include <iostream>

int main() {
    ctrie::CTrie<int> trie;

    // Insert key-value pairs
    trie.insert("key1", 100);
    trie.insert("key2", 200);

    int value;

    // Retrieve values
    if (trie.find("key1", value)) {
        std::cout << "Found key1 with value: " << value << std::endl;
    } else {
        std::cout << "key1 not found." << std::endl;
    }

    if (trie.find("key2", value)) {
        std::cout << "Found key2 with value: " << value << std::endl;
    } else {
        std::cout << "key2 not found." << std::endl;
    }

    return 0;
}
```

### Example 2: Removing a Key

```cpp
#include "ctrie/ctrie.hpp"
#include <iostream>

int main() {
    ctrie::CTrie<int> trie;

    // Insert a key-value pair
    trie.insert("key1", 100);

    int value;

    // Verify insertion
    if (trie.find("key1", value)) {
        std::cout << "Found key1 with value: " << value << std::endl;
    }

    // Remove the key
    trie.remove("key1");

    // Verify removal
    if (!trie.find("key1", value)) {
        std::cout << "key1 successfully removed." << std::endl;
    } else {
        std::cout << "Failed to remove key1." << std::endl;
    }

    return 0;
}
```

### Example 3: Handling Non-existent Keys

```cpp
#include "ctrie/ctrie.hpp"
#include <iostream>

int main() {
    ctrie::CTrie<int> trie;

    int value;

    // Attempt to find a non-existent key
    if (!trie.find("missing_key", value)) {
        std::cout << "missing_key not found in the trie." << std::endl;
    }

    // Attempt to remove a non-existent key
    trie.remove("missing_key");
    std::cout << "Attempted to remove missing_key (no error expected)." << std::endl;

    return 0;
}
```

## Contributing

Contributions are welcome! Feel free to submit a pull request or open an issue
if you have suggestions, bug reports, or improvements.

## License

This project is licensed under the MIT License - see the `LICENSE.md` file for details.

## Acknowledgments

Author: Enrico Fraccaroli (<enry.frak@gmail.com>)
