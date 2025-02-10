/// @file ctrie.hpp
/// @author Enrico Fraccaroli (enry.frak@gmail.com)
/// @brief The ctrie main code.

#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <array>

#if __cplusplus >= 201103L
#include <mutex>
#endif

enum : unsigned char {
    CTRIE_MAJOR_VERSION = 1, ///< Major version of the library.
    CTRIE_MINOR_VERSION = 0, ///< Minor version of the library.
    CTRIE_MICRO_VERSION = 0  ///< Micro version of the library.
};

namespace ctrie
{

/// @brief The maximum number of keys.
#define MAX_KEYS 128

/// @brief The type of the key.
using key_t = char;

/// @brief A node of the prefix tree.
template <typename T>
class SNode
{
public:
    /// @brief Construct a new node.
    /// @param _value The value to store.
    SNode(const T &_value)
        : value(_value)
    {
        // Nothing to do.
    }

    /// @brief Copy constructor.
    /// @param other The instance to copy from.
    SNode(const SNode &other)
        : value(other.value)
    {
        // Nothing to do.
    }

    /// @brief Copy assignment operator.
    /// @param other The instance to copy from.
    /// @return Reference to the instance.
    auto operator=(const SNode &other) -> SNode &
    {
        if (this != &other) {
            value = other.value;
        }
        return *this;
    }

    /// @brief Move constructor.
    /// @param other The instance to move from.
    SNode(SNode &&other) noexcept
        : value(std::move(other.value))
    {
        // Nothing to do.
    }

    /// @brief Move assignment operator.
    /// @param other The instance to move from.
    /// @return Reference to the instance.
    auto operator=(SNode &&other) noexcept -> SNode &
    {
        if (this != &other) {
            value = std::move(other.value);
        }
        return *this;
    }

    /// @brief Destruct the node.
    virtual ~SNode() = default;

    /// @brief Set the value of the node.
    /// @param _value The value to store.
    void setValue(const T &_value) { value = _value; }

    /// @brief Get the value of the node.
    /// @return The value of the node.
    auto getValue() const -> T { return value; }

private:
    /// The stored value.
    T value;
};

/// @brief A node of the prefix tree.
template <typename T>
class CNode
{
public:
    /// @brief Construct a new node.
    /// @param _parent The parent of the node.
    /// @param _key The key of the node.
    CNode(CNode<T> *_parent, key_t _key)
        : parent(_parent)
        , key(_key)
        , snode()
        , children()
    {
        for (auto &i : children) {
            i = NULL;
        }
    }

    /// @brief Construct a new node.
    /// @param _parent The parent of the node.
    /// @param _key The key of the node.
    /// @param _value The value of the node.
    CNode(CNode<T> *_parent, key_t _key, const T &_value)
        : parent(_parent)
        , key(_key)
        , snode(new SNode<T>(_value))
        , children()
    {
        for (auto &i : children) {
            i = NULL;
        }
    }

    /// @brief Destruct the node.
    virtual ~CNode()
    {
        if (snode) {
            delete snode;
        }
        for (auto &child : children) {
            if (child) {
                delete child;
            }
        }
    }

    /// @brief Get the key of the node.
    /// @return The key of the node.
    auto getKey() const -> key_t { return key; }

    /// @brief Get the parent of the node.
    /// @return The parent of the node.
    auto getParent() const -> CNode<T> * { return parent; }

    /// @brief Clear the stored value.
    void clearSNode()
    {
        if (snode) {
            delete snode;
        }
        snode = NULL;
    }

    /// @brief Set the stored value.
    /// @param _value The value to store.
    void setSNode(const T &_value)
    {
        this->clearSNode();
        snode = new SNode<T>(_value);
    }

    /// @brief Get the stored value.
    /// @return The stored value.
    auto getSNode() const -> SNode<T> * { return snode; }

    /// @brief Remove the child with the given key.
    /// @param c The key of the child to remove.
    void removeChild(key_t c)
    {
        if (c < 0) {
            throw std::domain_error("Key is a negative value.");
        }
        _remove_child(static_cast<std::size_t>(c));
    }

    /// @brief Insert a child with the given key.
    /// @param c The key of the child to insert.
    /// @param child The child to insert.
    /// @return The inserted child.
    auto insertChild(key_t c, CNode<T> *child) -> CNode<T> *
    {
        if (c < 0) {
            throw std::domain_error("Key is a negative value.");
        }
        auto index = static_cast<std::size_t>(c);
        _remove_child(index);
        return (children[index] = child);
    }

    /// @brief Get the child with the given key.
    /// @param c The key of the child to get.
    /// @return The child with the given key.
    auto at(key_t c) -> CNode<T> *
    {
        if (c < 0) {
            throw std::domain_error("Key is a negative value.");
        }
        return children[static_cast<std::size_t>(c)];
    }

    /// @brief Get the child with the given key.
    /// @param c The key of the child to get.
    /// @return The child with the given key.
    auto at(key_t c) const -> const CNode<T> *
    {
        if (c < 0) {
            throw std::domain_error("Key is a negative value.");
        }
        return children[static_cast<std::size_t>(c)];
    }

    /// @brief Check if the node has children.
    /// @return true if the node has children, false otherwise.
    auto hasChildren() const -> bool
    {
        for (auto &child : children) {
            if (child) {
                return true;
            }
        }
        return false;
    }

    /// @brief Get the string representation of the node.
    /// @param prefix The prefix to prepend to each line of the output.
    /// @param isLast Whether this node is the last child in a sequence.
    /// @return A string representing the node.
    auto toString(const std::string &prefix = "", bool isLast = true) const -> std::string
    {
        std::stringstream ss;
        // Print the current node with its prefix, except for the root.
        if (parent) {
            ss << prefix;
            ss << (isLast ? "└─" : "├─");
        }
        ss << key;
        if (snode) {
            ss << " : " << snode->getValue();
        }
        ss << "\n";
        // Compute the new prefix for children.
        std::string childPrefix = prefix + (isLast ? "  " : "│ ");
        // Iterate over children.
        for (std::size_t i = 0; i < MAX_KEYS; ++i) {
            if (children[i]) {
                // Determine if this child is the last one.
                ss << children[i]->toString(childPrefix, this->isLastChild(i));
            }
        }
        return ss.str();
    }

private:
    /// @brief Checks if there are more children after the given index.
    /// @param index The index to check.
    auto isLastChild(std::size_t index) const -> bool
    {
        for (std::size_t i = index + 1; i < MAX_KEYS; ++i) {
            if (children[i]) {
                return false;
            }
        }
        return true;
    }

    /// @brief Remove the child at the given index.
    /// @param index The index of the child to remove.
    void _remove_child(std::size_t index)
    {
        if (children[index]) {
            delete children[index];
        }
        children[index] = NULL;
    }

    /// A pointer to the parent.
    CNode<T> *parent;
    /// The key associated with the node.
    key_t key;
    /// The stored value.
    SNode<T> *snode;
    /// The childrens of the node.
    std::array<CNode<T> *, MAX_KEYS> children;
};

/// @brief A prefix tree.
template <typename T>
class CTrie
{
public:
    /// @brief Construct a new ctrie.
    CTrie()
        : _root(NULL)
    {
        // Nothing to do.
    }

    virtual ~CTrie()
    {
        {
            delete _root;
        }
    }

    /// @brief Inserts the key-value pair into the Trie.
    /// @param key the key.
    /// @param value the value.
    /// @return true if the isertion was successfull, false otherwise.
    auto insert(const std::string &key, T value) -> bool
    {
        // Check if the key is empty.
        if (key.empty()) {
            return false;
        }
#if __cplusplus >= 201103L
        // Lock the mutex.
        _mutex.lock();
#endif
        // Check if there is a root.
        if (_root == NULL) {
            _root = new CNode<T>(NULL, 0);
        }
        // Initialize the node to the root.
        CNode<T> *node  = _root;
        CNode<T> *child = nullptr;
        // Iterate on the characters of the node.
        for (char it : key) {
            // Search the child.
            child = node->at(it);
            // If I can't find any child with the respective letter, create one.
            if (child == NULL) {
                child = node->insertChild(it, new CNode<T>(node, it));
            }
            // Node now points to the child.
            node = child;
        }
        // Set the value.
        node->setSNode(value);
#if __cplusplus >= 201103L
        // Unlock the mutex.
        _mutex.unlock();
#endif
        return true;
    }

    /// @brief Find the value associated with the passed key.
    /// @param key the key to use for the search.
    /// @param value the output variable where the found value is stored.
    /// @return true if we have found the value, false otherwise.
    auto find(const std::string &key, T &value) const -> bool
    {
        // Check if the key is empty.
        if (key.empty()) {
            return false;
        }
#if __cplusplus >= 201103L
        // Lock the mutex.
        _mutex.lock();
#endif
        // Check if there is a root.
        if (_root == NULL) {
#if __cplusplus >= 201103L
            // Unlock the mutex.
            _mutex.unlock();
#endif
            return false;
        }
        // Initialize the node to the root.
        const CNode<T> *node  = _root;
        const CNode<T> *child = nullptr;
        // Iterate on the characters of the node.
        for (char it : key) {
            // Search the child.
            child = node->at(it);
            // If I can't find any child with the respective letter, stop the search.
            if (child == nullptr) {
                node = NULL;
                break;
            }
            // Node now points to the child.
            node = child;
        }
        // Set the output variable.
        if (node && node->getSNode()) {
            value = node->getSNode()->getValue();
#if __cplusplus >= 201103L
            // Unlock the mutex.
            _mutex.unlock();
#endif
            return true;
        }
#if __cplusplus >= 201103L
        // Unlock the mutex.
        _mutex.unlock();
#endif
        return false;
    }

    /// @brief Inserisce la coppia chaive valore nel Trie.
    /// @param key   La chiave.
    /// @return Se l'inserimento e' andato a buon fine.
    auto remove(const std::string &key) -> bool
    {
        // Check if the key is empty.
        if (key.empty()) {
            return false;
        }
#if __cplusplus >= 201103L
        // Lock the mutex.
        _mutex.lock();
#endif
        // Check if there is a root.
        if (_root == NULL) {
#if __cplusplus >= 201103L
            // Unlock the mutex.
            _mutex.unlock();
#endif
            return false;
        }
        // Initialize the node to the root.
        CNode<T> *node  = _root;
        CNode<T> *child = nullptr;
        // Iterate on the characters of the node.
        for (char it : key) {
            // Search the child.
            child = node->at(it);
            // If I can't find any child with the respective letter, stop the search.
            if (child == nullptr) {
                node = NULL;
                break;
            }
            // Node now points to the child.
            node = child;
        }
        // Set the output variable.
        if (node && node->getSNode()) {
            // Cancella il valore.
            node->clearSNode();
            // If the node has no children, remove the node.
            while (true) {
                child = node;
                node  = node->getParent();
                if (!child->hasChildren() && child->getParent() && !child->getSNode()) {
                    child->getParent()->removeChild(child->getKey());
                } else {
                    break;
                }
            }
#if __cplusplus >= 201103L
            // Unlock the mutex.
            _mutex.unlock();
#endif
            return true;
        }
#if __cplusplus >= 201103L
        // Unlock the mutex.
        _mutex.unlock();
#endif
        return false;
    }

    /// @brief Get the string representation of the tree.
    /// @return A string representing the tree.
    auto toString() const -> std::string
    {
        if (_root) {
            return _root->toString();
        }
        return std::string();
    }

private:
    /// The root of the tree.
    CNode<T> *_root;
#if __cplusplus >= 201103L
    /// Internal mutex for thread safety.
    mutable std::mutex _mutex;
#endif
};

} // namespace ctrie

/// @brief Overload of the operator << for the CTrie class.
/// @tparam T The type of the value.
/// @param lhs the stream.
/// @param rhs the trie.
/// @return the stream.
template <typename T>
auto operator<<(std::ostream &lhs, const ctrie::CTrie<T> &rhs) -> std::ostream &
{
    lhs << rhs.toString();
    return lhs;
}
