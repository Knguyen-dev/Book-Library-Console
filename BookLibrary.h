#ifndef BookLibrary_H
#define BookLibrary_H
#include <iostream>
#include <string>
#include <vector>
#include "Student.h"
#include "Book.h"
#include "HashTable.h"
#include "linkedList.h"
#include "utilities.h"

// Struct representing issued book entry, which 
// contains the book that was issued, and who issued the book.
// Is exclusive to the BookLibrary class since it's solely related to 
// functionality for the book library
struct issuedBookEntry {
	Book issuedBook;
	Student issuedStudent;

	// When overloading the comparison operators foor issuedBookEntry objects, we want to be sorting alphabetically by titles of the books since we are 
	// kind of more focusing on the books, rather than the students
	bool operator<(issuedBookEntry& other) {
		return issuedBook < other.issuedBook;
	}
	bool operator<=(issuedBookEntry& other) {
		return issuedBook <= other.issuedBook;
	}
	bool operator>(issuedBookEntry& other) {
		return issuedBook > other.issuedBook;
	}
	bool operator>=(issuedBookEntry& other) {
		return issuedBook >= other.issuedBook;
	}

	// Operator for checking when two issuedBookEntry objects are equal
	// NOTE: The above overloaded operators are more for sorting purposes, in contrast to this one.
	bool operator==(issuedBookEntry& other) {
		return (issuedBook == other.issuedBook) && (issuedStudent == other.issuedStudent);
	}
};

// Overloading output stream for book class
std::ostream& operator<<(std::ostream& os, issuedBookEntry bookEntry) {
	os << "('"
		<< bookEntry.issuedBook.title << "' by "
		<< bookEntry.issuedBook.author << " - Issued to: "
		<< bookEntry.issuedStudent.getName() << ", student ID#: "
		<< bookEntry.issuedStudent.getStudentID() << ")";
	return os;
}

// BookLibrary class for managing books in the hash table, such as 
// modifying the books, book/library related error messages, issuing books, and 
// other things that we think of 
// Book Library class
class BookLibrary {
private:
	HashTable<Book> bookMap; // Hash table containing the 
	std::vector<issuedBookEntry> issuedBookList; // list of objects that contain an issued book and the student the book was issued to
	std::vector<Student> libraryStudents; // list of students 'registered' into the library
public:
	BookLibrary() {}
	~BookLibrary() {}

	// Function will clear the book library and reset it back to a blank state 
	void destroyBookLibrary() {
		// First clear the bookMap hash table
		bookMap.destroyHashTable();
		// Now clear the list for the issued books and library students
		issuedBookList.clear();
		libraryStudents.clear();
	}

	// Given the attributes of a book object 
	void addBook(std::string title, std::string author, std::string ISBN, int numPages) {
		Book newBook = {title, author, ISBN, numPages};
		// When adding a book, we lower case the title. This is to make it more forgiving for input validation
		// as when the user types in a title, we will lower case their input as well, so that the corresponding book
		// will show up regardless whether or not they 
		bool result = bookMap.insertPair(lowerCaseString(newBook.title), newBook);
		if (result) {
			std::cout << "Book Library: Successfully added '" << newBook.title << "' to the library!" << std::endl;
		} else {
			std::cout << "Book Library: Could not add '" << newBook.title << "' since it is a duplicate title!" << std::endl;
		}
	}

	// Function which allows us to delete a book given the book's info
	void deleteBook(std::string title) {
		// Get the book based on its title
		Book targetBook = getBook(title);
		// Check if the book title they entered was valid and returned an actual book
		if (targetBook.ISBN == "") {
			std::cout << "Book Library: Could not remove '" << title << "' since it wasn't found in the library!" << std::endl;
			return;
		}
		// Check if the book has been checked out, if it has been checked out, then we also can't delete it
		if (targetBook.isAvailable == false) {
			std::cout << "Book Library: This book is currently issued/checked out, so it can't be deleted from the library!" << std::endl;
			return;
		}
		// At this point, the book they entered exists, and it's available to be deleted, so call the function to delete it from the hash table
		// lowercase the title so it can be matched with the lowercased keys on the hash table.
		bookMap.deletePair(lowerCaseString(title));
		std::cout << "Book Library: Successfully removed '" << title << "' from library!" << std::endl;
	}

	// Returns a book based on its title; if book wasn't found we return a default book object
	// Then after we should be able to follow up with either editing, checking out, etc.
	Book getBook(std::string title) {
		// Lowercasing the key, our title, since we are accessing and messing with the hash table
		Book targetBook = bookMap.getValue(lowerCaseString(title));
		return targetBook;
	}

	// Issues a book to student and updates the issuedBookEntry vector
	void issueBook(Book book, Student student) {
		// If the book is already unavailable, then we aren't allowed to check it out or issue it
		// NOTE: This should have already been checked in promptIssueBook, but we have the check here if we 
		// we want to use a separate function
		if (book.isAvailable == false) { 
			std::cout << "Book Library: Cannot issue '" << book.title << "' by " << book.author << " since it has already been issued!" << std::endl;
			return;
		}
		// Else the book is available so take steps to issue the book to said student
		book.isAvailable = false; // Make the book now not available since it's being issued to someone
		issuedBookEntry currentEntry = { book, student }; // Create new issuedBookEntry object with the book being issued and student that the book's being issued to
		issuedBookList.push_back(currentEntry); // Now update the book in the bookMap so that it's marked as unavailable
		// NOTE: We lower case the title since we're expecting the user to get a book object that's stored in the program
		// In this case, Book object being added by addBook, will have its title not lowercased, so we have to lowercase
		// it so it correctly indexes in the hash table to the right position, and it correctly matches the node. 
		bookMap.updatePair(lowerCaseString(book.title), book); // Show a message from the library that tells the user that the book has been issued
		std::cout << "Book Library: Successfully issued '" << book.title << "' to " << student << "!" << std::endl;
	}

	// Returns an issued book given a book and a student object
	void returnBook(Book book, Student student) {
		bool found = false;
		issuedBookEntry tempEntry = { book, student };
		for (size_t i = 0; i < issuedBookList.size(); i++) {
			// If the entry matches, then our book and student matches one in the record
			// Now we know it exists, we can delete
			if (tempEntry == issuedBookList[i]) {
				// Delete the issuedBookEntry object from the vector
				issuedBookList.erase(issuedBookList.begin() + i);
				found = true;
				break;
			}
		}
		// If we found a mathcing tempEntry object update the bookMap to show that the book is now available
		// Then show the user that it was successfully returned
		if (found) {
			// Now update the book in the bookMap by first making it available and updating the pair
			Book returnedBook = tempEntry.issuedBook;
			returnedBook.isAvailable = true;
			bookMap.updatePair(lowerCaseString(returnedBook.title), returnedBook);
			std::cout << "Book Library: Successfully returned '" << book.title << "' from " << student << "!" << std::endl;
		} else {
			// Else tell the user that we couldn't return their book
			std::cout << "Book Library: Couldn't return '" << book.title << "' from " << student << "!" << std::endl;
		}
	}

	// Prompts input for adding a new book, and if successful, it adds a new book to the library 
	void promptAddBook() {
		// Variables representing user input values for the Book object's attributes
		std::string inputTitle;
		std::string inputAuthor;
		std::string inputISBN;
		int inputNumPages;
		// Prompt input for book title
		std::cout << "Enter book title: ";
		std::getline(std::cin, inputTitle);
		// Prompt input for book author
		std::cout << "Enter book author: ";
		std::getline(std::cin, inputAuthor);
		// Prompt input for book's isbn (typically 13 digit)
		std::cout << "Enter book ISBN: ";
		std::cin >> inputISBN;
		// Prompt input for book's number of pages
		std::cout << "Enter number of pages: ";
		std::cin >> inputNumPages;
		std::cin.ignore(64, '\n');
		addBook(inputTitle, inputAuthor, inputISBN, inputNumPages);
	}

	// Prompts input for deleting a book from the library, and then if successful it deletes an existing 
	// book from the library.
	void promptDeleteBook() {
		// If there are no books, then we can't delete any books
		if ((getAllBooks()).size() == 0) {
			std::cout << "Book Library: No books stored in library to delete!" << std::endl;
			return;
		}
		std::string inputTitle;
		// Prompt input for the title that they want to delete 
		std::cout << "Enter book title: ";
		std::getline(std::cin, inputTitle);
		deleteBook(inputTitle);
	}

	// Prompts user for a book title and displays the book's information if book was found
	void promptSearchBook() {
		// If the library is empty then abort the process 
		if ((getAllBooks()).size() == 0) {
			std::cout << "Book Library: No books in the library to show or search for!" << std::endl;
			return; 
		}
		// Show all books in the library since there are some books
		showAllBooks();
		// Prompt input on the book title that the user wants to look at 
		std::string inputTitle;
		std::cout << "Enter a book title to view it: ";
		std::getline(std::cin, inputTitle);
		Book targetBook = getBook(inputTitle);
		if (targetBook.ISBN == "") {
			std::cout << "Book Library: Book titled '" << inputTitle << "' does not exist in this library!" << std::endl;
			return;
		}
		// Display the corresponding book with the inputted title
		displayBookInfo(targetBook);
	}

	// Prompts input for issuing a book to a student, and if successful it issues a book
	void promptIssueBook() {
		// Get an array of all books from the table
		std::vector<Book> allBooks = getAllBooks();
		// If there are no students, or no book, then we can't issue books
		if (libraryStudents.size() == 0 || allBooks.size() == 0) {
			std::cout << "Book Library: Can't issue books since there are either no books or no students in library!" << std::endl;
			return;
		}
		// Prompt input for the title and get the book
		std::string inputTitle;
		std::cout << "Enter book title: ";
		std::getline(std::cin, inputTitle);
		Book targetBook = getBook(inputTitle);
		// If the book is invalid/not found, then we return the user to the home screen
		// From there, they can look up the proper book titles or navigate to another menu
		if (targetBook.ISBN == "") {
			std::cout << "Book Library: Book with title '" << inputTitle << "' not found!" << std::endl;
			return;
		}
		// Else the book exists so we can print out detailed information about it
		displayBookInfo(targetBook);
		// If the book isn't available, then tell the user, and then return.
		// to the home screen again.
		if (targetBook.isAvailable == false) {
			std::cout << "Book Library: '" << targetBook.title << "' is currently not available to be issued!" << std::endl;
			return;
		}
		// Else we have a valid book that's available
		// List position of the student chosen by the user from the libraryStudents vector
		int studentChoice;
		// Show all students that the user can issue book to and prompt input. 
		// Ensure that they enter something in range.
		showAllStudents();
		std::cout << "Enter the number corresponding to the student: ";
		std::cin >> studentChoice;
		studentChoice = validateMenuInput(studentChoice, 1, libraryStudents.size());
		// Access the student object that the user wanted to choose
		// Decrement by one to get the correct element
		Student targetStudent = libraryStudents[studentChoice - 1];
		// Now we have a valid student and a valid book, so we can issue it now
		issueBook(targetBook, targetStudent);
	}

	// Prompts input for returning a book, and if successful it returns the book
	void promptReturnBook() {
		// Before proceeding, first check if there are any books that have been issued
		// Using our vector of issued book entries.
		if (issuedBookList.size() == 0) {
			std::cout << "Book Library: Failure to proceed returning books since no books have been issued yet!" << std::endl;
			return;
		}
		// Integer representing the eventually index position of the element they want
		// to pick in issuedBooklist
		int issueBookChoice;
		// Show the record of issued books and students they were issued to, and prompt input
		showIssuedEntries();
		std::cout << "Enter the number corresponding to the entry: ";
		std::cin >> issueBookChoice;
		issueBookChoice = validateMenuInput(issueBookChoice, 1, issuedBookList.size());
		// Get the entry that the user picked and then call the function to return the book
		// by passing in the target book and the target student that we want to delete using 
		// targetEntry.
		issuedBookEntry targetEntry = issuedBookList[issueBookChoice - 1];
		returnBook(targetEntry.issuedBook, targetEntry.issuedStudent);
	}

	// Adds a student to the library, allowing the user to issue that student a book
	void addStudent(std::string firstName, std::string lastName, std::string studentID) {
		if (isExistingStudent(studentID)) {
			std::cout << "Book Library: Student with ID '" << studentID << "' already exists in the library!" << std::endl;
			return;
		}
		// Then a new Student class instance into the libraryStudents vector and sort the vector
		Student newStudent = Student(firstName, lastName, studentID);
		libraryStudents.push_back(newStudent);
		std::cout << "Book Library: Successfully added student " << newStudent << std::endl;
	}

	// Deletes a student based on its studentID
	// NOTE: Don't need to sort, since we already assumed it's been sorted from addStudent, so removing an element wouldn't mess with the order
	void deleteStudent(std::string studentID) {
		bool found = false;
		Student targetStudent;
		for (size_t i = 0; i < libraryStudents.size(); i++) {
			// If the student if of current student matches passed ID
			// Then we found the target student that the user wanted to delete
			if (libraryStudents[i].getStudentID() == studentID) {
				// Erase the student from vector
				targetStudent = libraryStudents[i];
				libraryStudents.erase(libraryStudents.begin() + i);
				found = true;
				break;
			}
		}
		// Then output the result to the user
		if (found) {
			std::cout << "Book Library: Successfully deleted student " << targetStudent << " from library!" << std::endl;
		} else {
			std::cout << "Book Library: Failed to find and delete student with ID: " << studentID << std::endl;
		}
	}

	// Prompts input for adding a student to the library, and then adds said student to the library
	void promptAddStudent() {
		std::string firstName;
		std::string lastName;
		std::string studentID;
		// Prompt input for the attributes of the student class instance
		std::cout << "Enter student's first name: ";
		std::cin >> firstName;
		std::cout << "Enter student's last name: ";
		std::cin >> lastName;
		std::cout << "Enter student's ID number: ";
		std::cin >> studentID;
		// Then call function to add student to the library
		addStudent(firstName, lastName, studentID);
		// Then after you've added a new student, sort the library student vector
	}

	// Prompts input for deleting a student from the library.
	void promptDeleteStudent() {
		// Check if there are students in the library
		if (libraryStudents.size() == 0) {
			std::cout << "Book Library: There are no students registerd with library!" << std::endl;
			return;
		}
		// Then show all students, since we know there are students in the library
		showAllStudents();
		// Prompt input for a student and access the student from the vector
		int studentChoice;
		std::cout << "Select student based on the menu number: ";
		std::cin >> studentChoice;
		studentChoice = validateMenuInput(studentChoice, 1, libraryStudents.size());
		Student targetStudent = libraryStudents[studentChoice - 1];
		// Then pass the id of that student to the function to delete the student from the library instance
		deleteStudent(targetStudent.getStudentID());
	}

	// Displays detailed information about a book
	void displayBookInfo(Book book) {
		std::cout << "Book info: " << std::endl;
		std::cout << "Title: " << book.title << std::endl;
		std::cout << "Author: " << book.author << std::endl;
		std::cout << "ISBN#: " << book.ISBN << std::endl;
		std::cout << "Num Pages: " << book.numPages << std::endl;
		std::cout << "Availability: " << (book.isAvailable ? "Available" : "Unavailable") << std::endl;
	}

	/*
	NOTE: Our show functions will first sort the vector, and then display it. So just now know that normally, if you directly access the vectors, they likely 
	aren't going to be sorted, but if you call one of the "get" functions, you can sort the vectors and receive them. So in situations where we're displaying a list of options,
	you'd want to call one of the sort functions to display a sorted version of the menu options or use the "get" to sort the vector and get it, and use a loop to display the menu
	yourself.
	*/
	// Shows all books in the library
	void showAllBooks() {
		// Get all table values from the book map, which will be book objects
		std::vector<Book> allBooks = getAllBooks();
		// If there are no book objects in the vector
		if (allBooks.size() == 0) {
			std::cout << "Book Library: Library is empty!" << std::endl;
			return;
		}
		// Sort it since it has elements
		mergeSort(allBooks);
		// Show output by showing all books
		std::cout << "Book Library All Books: " << std::endl;
		for (size_t i = 0; i < allBooks.size(); i++) {
			std::cout << i+1 << ". " << allBooks[i] << std::endl;
		}
	}

	// Sorts and shows a record of issuedBookEntry objects.
	void showIssuedEntries() {
		// If there are no items in issuedBookList, then we can't really show anything
		if (issuedBookList.size() == 0) {
			std::cout << "Book Library: There are no issued book records to be shown!" << std::endl;
			return;
		}
		mergeSort(issuedBookList);
		std::cout << "Book Library: Issued Book Record: " << std::endl;
		for (size_t i = 0; i < issuedBookList.size(); i++) {
			std::cout << i + 1 << ". " << issuedBookList[i] << std::endl;
		}
		std::cout << "Book Library: End Record!" << std::endl;
	}

	// Sorts and shows all registered students
	void showAllStudents() {
		if (libraryStudents.size() == 0) {
			std::cout << "Book Library: No students in library that can be shown!" << std::endl;
			return;
		}
		mergeSort(libraryStudents);
		for (int i = 0; i < libraryStudents.size(); i++) {
			std::cout << i + 1 << ". " << libraryStudents[i] << std::endl;
		}
	}

	// Sees if student is already registered into the library by seeing if an given student id matches any of the student id in the students list
	bool isExistingStudent(std::string studentID) {
		bool found = false;
		for (int i = 0; i < libraryStudents.size(); i++) {
			if (libraryStudents[i].getStudentID() == studentID) {
				found = true;
				break;
			}
		}
		return found;
	}


	// Sorts and Returns issuedBooklist
	std::vector<issuedBookEntry> getAllIssuedBookEntries() {
		mergeSort(issuedBookList);
		return issuedBookList;
	}

	// Returns an array of books that are stored in the library's hash table
	std::vector<Book> getAllBooks() {
		std::vector<Book> allBooks = bookMap.getAllTableValues();
		return mergeSort(allBooks, true);
	}

	// Sorts and returns the array of students registered with the library instance
	std::vector<Student> getLibraryStudents() {
		mergeSort(libraryStudents);
		return libraryStudents;
	}

	// Since issuing and returning books involves the students list from hte outside
	// and using student objects, it's probably going to be a function outside of the 
	// confines of this class definition. Next thing is setting up menu list structure
	// and then the idea of issuing and returning books then

	// editBook; be careful if they decide to edit the title, which is our key
	// if that's the case then we'd delete the old key-value pair, and then add the new value.
	// Else if it's like the book's information we can just update the pair like normal
	// Assume that they're going to get a screen 

	// Then we'd have our functions for prompting input 
	// Like function for editing, or prompt add book
};


#endif