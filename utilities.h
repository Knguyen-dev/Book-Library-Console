#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <string>
#include <vector>
// Returns a lower cased version of the string
std::string lowerCaseString(std::string inputStr) {
	std::string newStr;
	for (int i = 0; i < inputStr.length(); i++) {
		newStr += tolower(inputStr[i]);
	}
	return newStr;
}

// Splits a line of text by its delimiter, then a vector of data
std::vector<std::string> splitLine(std::string myStr, char delimiter) {
    std::vector<std::string> currentLine;
    int position = 0;
    int count = 0;
    std::string field; // Represents a single piece of data from the text file, such as the abbreviation, full name, or governor name, etc.
    for (int i = 0; i < myStr.length(); i++) {
        // If the current position is a comma
        if (myStr[i] == delimiter) {
            field = myStr.substr(position, count);
            currentLine.push_back(field); // Put the data piece in the vector      
            position = i + 1; // set position to after the delimiter
            count = 0; // set count back zero so we can count the letters for next string
        } else if (i == myStr.length() - 1) {
            // Else if it's the last index then just get the whole last part of the string; this is because at this point the 'position' or index would be at 'C'
            field = myStr.substr(position);
            currentLine.push_back(field);
        } else {
            // If it's not a comma then we increment the count to keep track of the amount of letters we are going to use when we use substr.
            count += 1;
        }
    }
    return currentLine;
}

// Continues to prompt menu input for a user so that their input value is in range
int validateMenuInput(int inputValue, int minValue, int maxValue) {
    while (inputValue < minValue || inputValue > maxValue) {
        std::cout << "Please enter a value within range!: ";
        std::cin >> inputValue;
	}
    // Clear the buffer of the newlines since we could be using getline after
    std::cin.ignore(64, '\n');
    return inputValue;
}

// Merge Sort:
// divideList, firstList should contain the items, and we are sharing it with the second list
template <class T>
void divideList(std::vector<T>& firstList, std::vector<T>& secondList) {
    if (firstList.size() <= 1) {
        return;
    }
    int mid = firstList.size() / 2;
    // Assigns the secondList with the firstList's items from the middle to the end
    secondList.assign(firstList.begin() + mid, firstList.end());
    // Since we allocated those elements from firstList, we have to delete those elemenst
    firstList.erase(firstList.begin() + mid, firstList.end());
}

// Merge part of merge sort
template <class T>
std::vector<T> mergeList(std::vector<T>& firstList, std::vector<T>& secondList, bool ascending) {
    std::vector<T> mergedList;
    size_t i = 0; // index for first list
    size_t j = 0; // index for second list
    // Start putting sorted items in the merged list
    while (i != firstList.size() && j != secondList.size()) {
        // The logic if we're sorting in ascending order; so we're pushing in the smaller values first
        if (ascending) {
            if (firstList[i] < secondList[j]) {
                mergedList.push_back(firstList[i]);
                i += 1;
            } else {
                mergedList.push_back(secondList[j]);
                j += 1;
            }
        } else {
            // It's putting in descending order, so we prioritize pushing the larger items first
            // First item is larger so push it
            if (firstList[i] > secondList[j]) {
                mergedList.push_back(firstList[i]);
                i += 1;
            } else {
                // Second item is larger or equal so push it
                mergedList.push_back(secondList[j]);
                j += 1;
            }
        }
    }
    // Now put the remaining items at the end of the mergedList vector
    for (size_t index = i; index < firstList.size(); index++) {
        mergedList.push_back(firstList[index]);
    }
    for (size_t index = j; index < secondList.size(); index++) {
        mergedList.push_back(secondList[index]);
    }
    // return the merged list
    return mergedList;
}

template <class T>
std::vector<T> mergeSort(std::vector<T>& items, bool ascending = true) {
    // If the list is empty or there's only one thing, just return the list/stop procedure for that call
    if (items.size() <= 1) {
        return items;
    }
    // Create a second branch or sublist
    std::vector<T> items2;
    // Divide the list
    divideList(items, items2);
    // call merge sort
    items = mergeSort(items, ascending);
    // call merge sort
    items2 = mergeSort(items2, ascending);
    // mergeList: combine the lists and return the result up the stack
    items = mergeList(items, items2, ascending);
    return items;
}

#endif