#ifndef linkedList_H
#define linkedList_H
#include <iostream>
#include <string>

// File that contains linked list and nodes specialized for using in hashtables; for collision resolution of chaining.

// Generic node for a linked list that's used in a hash table
// T is key datatype and U is value/info datatype
template <class T, class U>
struct HTNode {
	T key;
	U info;
	HTNode<T, U>* link;
};

// Class for linked list that's used in hash table chaining
template <class T, class U>
class HTLinkedList {
private:
	HTNode<T, U>* head;
	HTNode<T, U>* tail;
	int count;
public:
	HTLinkedList() {
		head = nullptr;
		tail = nullptr;
		count = 0;
	}

	~HTLinkedList() {}

	// Resets a linked list
	void destroyList() {
		HTNode<T, U>* current = head;
		while (current != nullptr) {
			HTNode<T, U>* temp = current;
			current = current->link;
			delete temp;
		}
		head = nullptr;
		tail = nullptr;
		count = 0;
	}

	// Checks if a linked list is empty
	bool isEmpty() {
		return count == 0;
	}

	// Inserts node at head; we pass in the key that's associated with the key
	// in the hash table and then the value of that key
	/*
	+ NOTE: When using separate chaining or implementing collision resolution in general, it's important to place both
	the key and value. The value obviously because you wanted to place your important data somewhere, but the key is critical
	to making sure you're finding the right node or cell. Keys can hash to the same index, position, and address, but the
	we use the key because if that address is already taken up, we'd have to start comparing other cells. In the end the correct cell
	we're looking for has matching keys.

	NOTE: Before Insertion we should check if the key already exists in the linked list
	*/
	void insertFirst(T key, U info) {
		// Create new node and fill in the data
		HTNode<T, U>* newNode = new HTNode<T, U>();
		newNode->key = key;
		newNode->info = info;

		// Empty linked list, so we're actually adding the first node, so redirect both head and tail as the new node
		if (isEmpty()) {
			head = newNode;
			tail = newNode;
		}
		else {
			// Else there's at least one node in the list, so connect the node to the head nad update the head
			newNode->link = head;
			head = newNode;
		}
		// Increase count since we're adding one more node
		count += 1;
	}

	// Inserts new node at tail 
	void insertLast(T key, U info) {
		// Create new node and fill in the data
		HTNode<T, U>* newNode = new HTNode<T, U>();
		newNode->key = key;
		newNode->info = info;
		newNode->link = nullptr;

		// If the list is initialily empty, put both head and tail as the new node
		if (isEmpty()) {
			head = newNode;
			tail = newNode;
		}
		else {
			// Else there's at least one node in the linked list, so just connect the tail to the new node
			// Then update the tail
			tail->link = newNode;
			tail = newNode;
		}
		count += 1;
	}

	// Search linked list and returns a node with matching key; if it's not found it'll return a nullptr
	HTNode<T, U>* searchNode(T key) {
		HTNode<T, U>* current = head;
		while (current != nullptr) {
			// If the keys match, it's the node we're looking for
			if (current->key == key) {
				break;
			}
			current = current->link;
		}
		return current;
	}

	// Deleting a node based on its key; assumes there are no duplicates and key-value pair already exists
	/*
	+ Various Cases:
	1. If list is Empty
	2. If the node we isn't found in the linked list
	3. If we are deleting the head. And if deleting the head lead to an empty list.
	4. If We are deleting a node that isn't the head.
	*/
	void deleteNode(T key) {
		HTNode<T, U>* previous = nullptr; // node after the current node in linked list
		HTNode<T, U>* current = head; // represents current node in linked list,

		// If the linked list is already empty then abort the mission
		if (isEmpty()) {
			std::cout << "HTLinked List Deletion Error: List is already empty!" << std::endl;
			return;
		}

		// Search for target node in linked list
		while (current != nullptr) {
			// If the keys match, then we found it; break out of loop
			if (current->key == key) {
				break;
			}
			// Set the previous node, then advance current node
			previous = current;
			current = current->link;
		}

		// if current is a nullptr, it couldn't find a node that matched the key given
		if (current == nullptr) {
			std::cout << "HTLinkedList Deletion Error: Couldn't find node with key in list!" << std::endl;
			return;
		}

		// If we're deleting the head, first update the head to the next node in the chain
		if (current == head) {
			head = head->link;
			// Means there was no node after head so the linked list is empty, so set the tail to nullptr also
			if (head == nullptr) {
				tail = nullptr;
			}
		} else {
			// Else the node we are deleting is not the head
			// Connect the previous node to the node after the target node.
			previous->link = current->link;
			// If the target node being deleted is the tail, also have to update hte tail 
			if (current == tail) {
				tail = previous;
			}
		}
		// Finally delete our target node and decrement the count
		delete current;
		count -= 1;
	}

	// Check if a node, which is a key-value pair, exists already in the linked list given the key
	bool isExistingNode(T key) {
		HTNode<T, U>* current = head;
		bool found = false;
		while (current != nullptr) {
			if (current->key == key) {
				found = true;
				break;
			}
			current = current->link;
		}
		return found;
	}

	// Returns a vector of all values in the linked list; if it's empty we return an empty vector
	std::vector<U> getAllNodeValues() {
		std::vector<U> nodeValues;
		HTNode<T, U>* current = head;
		while (current != nullptr) {
			nodeValues.push_back(current->info);
			current = current->link;
		}
		return nodeValues;
	}

	// Function prints the linked list
	// NOTE: Assumes current->info has its output stream overloaded with a custom print fucntion
	void print() {
		if (isEmpty()) {
			std::cout << "Linked List: Empty!" << std::endl;
			return;
		}
		HTNode<T, U>* current = head;
		std::cout << "Linked List: ";
		while (current != nullptr) {
			std::cout << "(" << current->key << ":" << current->info << ")" << ", ";
			current = current->link;
		}
		std::cout << std::endl;
	}

	// Get the length or number of nodes in the linked list
	int getLength() {
		return count;
	}

	// NOTE: These, and any other functions that return an HTNode will also return a nullptr if the node in 
	// question doesn't exist.

	// Get node located at the head of hte linked list
	U getHeadValue() {
		if (isEmpty()) {
			std::cout << "HTLinkedList Error: Empty so can't get head value, returning default value!" << std::endl;
			return U();
		}
		return head->info;
	}

	// Get value at the linked list's tail 
	U getTailValue() {
		if (isEmpty()) {
			std::cout << "HTLinkedList Error: Empty so can't get tail value, returning default value!" << std::endl;
			return U();
		}
		return tail->info;
	}
};


#endif