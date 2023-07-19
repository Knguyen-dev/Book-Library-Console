#ifndef STUDENT_H
#define STUDENT_H
#include <string>

/*
+  Student class: Realistically in this program, the librarian shouldn't really have 
the option to add students, as all students should have access to the library. So this 
class is just here as a data-structure to represent a student, and students will be loaded 
into the program from a file so that we have some students to issue books to.
*/

// Student class: realistically we use this class by loading in data
class Student {
private:
	std::string firstName;
	std::string lastName;
	std::string studentID; // An 8 digit ID 
public:
	// Constructor
	Student(std::string _firstName, std::string _lastName, std::string _studentID) {
		firstName = _firstName;
		lastName = _lastName;
		studentID = _studentID;
	}

	// Default constructor
	Student() {
		firstName = "";
		lastName = "";
		studentID = "";
	}

	void setFirstName(std::string _firstName) {
		firstName = _firstName;
	}
	void setLastName(std::string _firstName) {
		firstName = _firstName;
	}
	void setStudentID(std::string _studentID) {
		studentID = _studentID;
	}
	std::string getFirstName() {
		return firstName;
	}
	std::string getLastName() {
		return lastName;
	}
	std::string getName() {
		return firstName + " " + lastName;
	}
	std::string getStudentID() {
		return studentID;
	}
	// Overloading the comparison operators so it lexicographically compares the titles of the Book objects
	bool operator>(Student& other) {
		return getName() > other.getName();
	}
	bool operator>=(Student& other) {
		return getName() >= other.getName();
	}
	bool operator<(Student& other) {
		return getName() < other.getName();
	}
	bool operator<=(Student& other) {
		return getName() <= other.getName();
	}
	// Overloading equality operator to check whether two objects 
	bool operator==(Student& other) {
		return studentID == other.studentID;
	}
};

std::ostream& operator<<(std::ostream& os, Student student) {
	os << "("
		<< student.getName()
		<< " - ID#: "
		<< student.getStudentID()
		<< ")";
	return os;
}
#endif // !STUDENT_H
