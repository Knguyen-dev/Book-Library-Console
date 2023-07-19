#ifndef BOOK_H
#define BOOK_H
#include<string>

// File for the Book Struct
struct Book {
	std::string title;
	std::string author;
	std::string ISBN;
	int numPages;
	bool isAvailable = true; // when added, books are by default available since they were just added
	// Overloading the comparison operators so it lexicographically compares the titles of the Book objects
	bool operator>(Book& other) {
		return title > other.title;
	}
	bool operator>=(Book& other) {
		return title >= other.title;
	}
	bool operator<(Book& other) {
		return title < other.title;
	}
	bool operator<=(Book& other) {
		return title <= other.title;
	}
	// Checks if two books are the same essentially; we assume that all of them have their own unique ISBN number
	// NOTE: This shouldn't be lumped in with the other comparison operator overloading, which is mainly for sorting purposes
	bool operator==(Book& other) {
		return ISBN == other.ISBN;
	}
};

// Overloading output stream for book class
std::ostream& operator<<(std::ostream& os, Book book) {
	os << "("
		<< book.title << ", "
		<< book.author << ", "
		<< book.ISBN << ", "
		<< book.numPages << ", "
		<< (book.isAvailable ? "Available" : "Unavailable")
		<< ")";
	return os;
}

#endif