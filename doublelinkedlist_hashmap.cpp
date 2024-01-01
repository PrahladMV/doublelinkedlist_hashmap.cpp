/* Design a data structure for LRU Cache. It should support the following operations: get and set.
get(key) – Get the value (will always be positive) of the key if the key exists in the cache, otherwise
return -1. set(key, value) – Set or insert the value if the key is not already present. */

#include <iostream>
#include <unordered_map>

using namespace std;

class LRUCache {
public:
    struct Node {
        int key;
        int value;
        Node* prev;
        Node* next;
        Node(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    LRUCache(int capacity) : capacity(capacity), head(nullptr), tail(nullptr) {}

    int get(int key) {
        if (cacheMap.find(key) == cacheMap.end()) {
            return -1; // Key not found
        }

        // Move the accessed node to the front
        Node* node = cacheMap[key];
        moveToHead(node);

        return node->value;
    }

    void set(int key, int value) {
        if (cacheMap.find(key) != cacheMap.end()) {
            // Update the value if the key already exists
            cacheMap[key]->value = value;
            moveToHead(cacheMap[key]);
        } else {
            // Insert a new node
            Node* newNode = new Node(key, value);

            if (cacheMap.size() >= capacity) {
                // Remove the least recently used node
                cacheMap.erase(tail->key);
                removeTail();
            }

            // Add the new node to the front
            addToFront(newNode);
            cacheMap[key] = newNode;
        }
    }

private:
    unordered_map<int, Node*> cacheMap;
    int capacity;
    Node* head;
    Node* tail;

    void moveToHead(Node* node) {
        if (node == head) {
            return; // Already at the front
        }

        if (node == tail) {
            // Update tail if moving the last node to the front
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            // Adjust the pointers for nodes in the middle
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }

        // Add the node to the front
        addToFront(node);
    }

    void addToFront(Node* node) {
        if (!head) {
            head = tail = node;
        } else {
            node->next = head;
            head->prev = node;
            head = node;
        }
    }

    void removeTail() {
        if (!tail) {
            return; // List is empty
        }

        if (head == tail) {
            // Only one node in the list
            delete tail;
            head = tail = nullptr;
        } else {
            // Remove the last node
            Node* temp = tail;
            tail = tail->prev;
            tail->next = nullptr;
            delete temp;
        }
    }
};

int main() {
    // Example usage:
    LRUCache cache(2);

    cache.set(1, 1);
    cache.set(2, 2);
    cout << cache.get(1) << endl; // Output: 1
    cache.set(3, 3); // Remove the least recently used item (2)
    cout << cache.get(2) << endl; // Output: -1 (not found)
    cache.set(4, 4); // Remove the least recently used item (1)
    cout << cache.get(1) << endl; // Output: -1 (not found)
    cout << cache.get(3) << endl; // Output: 3
    cout << cache.get(4) << endl; // Output: 4

    return 0;
}
