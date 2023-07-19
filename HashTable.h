#ifndef HashTable_H
#define HashTable_H
#include "linkedList.h"
/*

+ Class for general hash table that uses strings keys, but the values can be flexible. The 
hash table uses separate chaining, and the time complexity for most operations is O(n) rather than constant time
due to the fact that we check whether a key is valid often when we manipulate the hash table.

+ NOTE: Since our specific hashtable is only going to take strings for keys,
due to the fact that we're using modular/division hash, we replace template class T with
std::string for better readability. We keep class template U because we won't do anything where it's datatype
would be an issue, unlike the keys where we are solely hashing strings. As a result, now our linked lists only
accept nodes that have string datatype for keys, while our values should be flexible.
*/
template <class U>
class HashTable {
private:
	int numBuckets; // size of the hash table
	HTLinkedList<std::string, U>* buckets; // underlying array of a hash table
	int numPairs; // number of pairs that exist in the hash table
public:
	// Constructor: apparently using prime numbers is good for improving distribution
	HashTable(int _numBuckets = 17) {
		numBuckets = _numBuckets;
		numPairs = 0;
		buckets = new HTLinkedList<std::string, U>[numBuckets];
		// Fill array up with empty linked lists; when the linked list 
		// is empty, we know the bucket is empty
		for (int i = 0; i < numBuckets; i++) {
			buckets[i] = HTLinkedList<std::string, U>();
		}
	}

	// Destructor
	~HashTable() {
		delete[] buckets;
	}

	// Clears all buckets in the hash table
	void destroyHashTable() {
		// Loop through all linked lists, and call method so that all nodes in it are cleared
		for (int i = 0; i < numBuckets; i++) {
			buckets[i].destroyList();
		}
		// Reset number of pairs.
		numPairs = 0;
	}

	// Inserts a new key-value pair into one of the linked lists stored in the 
	// hash table
	bool insertPair(std::string key, U value) {
		// Get the index of the linked list 
		int index = modularHash(key);
		// If pair already exists, then we are erroneously trying to add a duplicate key to the hash table
		// So stop the function call early and output a warning
		if (isExistingKey(key)) {
			return false;
		}
		// Else insert the valid pair at the end of the linked list, and increment number of pairs
		buckets[index].insertLast(key, value);
		numPairs += 1;
		return true;
	}

	// Delete a pair from the hashmap by using the key of that pair
	bool deletePair(std::string key) {
		int index = modularHash(key);
		// If the key-value pair doesn't already exist, then show an error message saying that
		// the program is trying to delete a pair with a key that doesn't exist in the hash table.
		if (!isExistingKey(key)) {
			return false;
		}
		// Else it does exist, so delete the node with the corresponding key; decrement number of pairs
		buckets[index].deleteNode(key);
		numPairs -= 1;
		return true;
	}

	/*
	+ NOTE: The reason updatePair and getValue don't use isExistingPair() and rather just
	searchNode is because they intend to get and manipulate the nodes, whilst isExistingPair() is used
	in functions where we aren't really going to
	*/
	// Updates the key-value pair, but entering in the key, and then the new value 
	bool updatePair(std::string key, U value) {
		int index = modularHash(key);
		// Get the target node
		HTNode<std::string, U>* targetNode = buckets[index].searchNode(key);
		// Remember: searchNode() can return a nullptr if it didn't find the node
		// If it's not an existing key-value pair, show an error
		if (targetNode == nullptr) {
			return false;
		}
		// Else, update the node 
		targetNode->info = value;
		return true;
	}

	// Gets a value from the map that's associated with the given key
	U getValue(std::string key) {
		// Get the index and get the target node
		int index = modularHash(key);
		HTNode<std::string, U>* targetNode = buckets[index].searchNode(key);
		// If targetNode == nullptr, we couldn't find the value in the linked list, so we are returning default constructed object
		if (targetNode == nullptr) {
			return U();
		}
		// Return the value, which is the info of the node
		return targetNode->info;
	}

	// Division or modular hash; outputs the index that 
	// the key-value pair should be placed in the array
	int modularHash(std::string key) {
		int sum = 0;
		for (int i = 0; i < static_cast<int>(key.length()); i++) {
			int asciiValue = static_cast<int>(key[i]);
			sum += asciiValue;
		}
		return sum % numBuckets;
	}

	// Determines if a key already exists/is associated with a value in the hash table 
	bool isExistingKey(std::string key) {
		int index = modularHash(key);
		// Access the corresponding linked list, and check its nodes to see if it has a node with that key.
		// If it does then the key-value pair already exists in our hash-table, else it's a brand new key-value pair.
		return buckets[index].isExistingNode(key);
	}

	// Function for printing out the hash table
	void print() {
		std::cout << "Hash Table: " << std::endl;
		for (int i = 0; i < numBuckets; i++) {
			// If it isn't empty, then print the stuff
			if (!buckets[i].isEmpty()) {
				buckets[i].print();
			}
		}
		// Check if there are actually zero pairs in the hash table.
		if (numPairs == 0) {
			std::cout << "Hash Table is Empty!" << std::endl;
		}
		std::cout << "- End Hash Table!" << std::endl;
	}

	// Returns the number of pairs in the hash table
	int getNumPairs() {
		return numPairs;
	}

	// Returns a vector with all of the values in the table.
	std::vector<U> getAllTableValues() {
		std::vector<U> tableValues;
		// Get all node values from non-empty buckets
		for (int i = 0; i < numBuckets; i++) {
			if (!buckets[i].isEmpty()) {
				// Get vector containing all values in some linked list
				std::vector<U> tempValues = buckets[i].getAllNodeValues();
				// Add values from those vectors to our tableValues that will contain all values
				tableValues.insert(tableValues.end(), tempValues.begin(), tempValues.end());
			}
		}
		return tableValues;
	}
};
#endif