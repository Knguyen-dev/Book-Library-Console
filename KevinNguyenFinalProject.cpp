#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "BookLibrary.h"
#include "utilities.h"
void loadBookData(BookLibrary& someLibrary, std::string fileName, char delimiter) {
	std::ifstream bookDataFile;
	std::string currentLine; 
	bookDataFile.open(fileName);
	while (std::getline(bookDataFile, currentLine)) {
		// book attributes put into vector form, we convert bookVector[3] in the data-file it represents an integer, but was converted into a string in the vector
		// Now we convert it back into a vector
		std::vector<std::string> bookVector = splitLine(currentLine, delimiter);	
		someLibrary.addBook(bookVector[0], bookVector[1], bookVector[2], std::stoi(bookVector[3]));
	}
}

void loadStudentData(BookLibrary& someLibrary, std::string fileName, char delimiter) {
	std::ifstream studentDataFile;
	std::string currentLine;
	studentDataFile.open(fileName);
	while (std::getline(studentDataFile, currentLine)) {
		std::vector<std::string> studentVector = splitLine(currentLine, delimiter);
		someLibrary.addStudent(studentVector[0], studentVector[1], studentVector[2]);
	}
}

// Displays 
void displayMainMenu() {
	std::cout << "Home Menu: " << std::endl;
	std::cout << "1. Search Books" << std::endl;
	std::cout << "2. Issue Book" << std::endl;
	std::cout << "3. Return Book" << std::endl;
	std::cout << "4. Add Book" << std::endl;
	std::cout << "5. Delete Book" << std::endl;
	std::cout << "6. Add Student" << std::endl;
	std::cout << "7. Delete Student" << std::endl;
	std::cout << "8. Quit" << std::endl;
	std::cout << "Enter the number for your choice: ";
}

int main() {
	// Create library
	BookLibrary myLibrary;
	// Load the library instance with data for book objects
	loadBookData(myLibrary, "bookData.txt", ',');
	// Load the library instance with pre made student objects
	loadStudentData(myLibrary, "studentData.txt", ',');

	// Boolean for continuing the loop
	bool continueLoop = true;
	// Integer representing the user's choice from a menu of options
	int userChoice; 

	while (continueLoop) {
		displayMainMenu();
		std::cin >> userChoice;
		userChoice = validateMenuInput(userChoice, 1, 8);
		switch (userChoice) {
			case 1:
				myLibrary.promptSearchBook();
				break;
			case 2:
				myLibrary.promptIssueBook();
				break;
			case 3: 
				myLibrary.promptReturnBook();
				break;
			case 4:
				myLibrary.promptAddBook();
				break;
			case 5:
				myLibrary.promptDeleteBook();
				break;
			case 6:
				myLibrary.promptAddStudent();
				break;
			case 7:
				myLibrary.promptDeleteStudent();
				break;
			case 8:
				// Set booelan to false 
				continueLoop = false;
		}
	}

	std::cout << "Book Library: End of program!" << std::endl;
}
