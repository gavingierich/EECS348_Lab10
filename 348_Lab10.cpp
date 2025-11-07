/*
* Author: Gavin Gierich
* Date: 11/6/2025
* Description: This program reads pairs of numbers from a file and
*	performs addition or subtraction based on their signs. The program
*	uses only string manipulation to handle numbers.
* Input: A text file with pairs of numbers on each line.
* Output: The sum of each pair of numbers printed to the console.
*/


#include <iostream>  // For input and output
#include <fstream>   // For file operations
#include <sstream>   // For string stream operations
#include <string>    // For string operations

using namespace std; // Use the standard namespace

// Functions
static bool isNumber(string str);
static string calculateStrings(string str1, string str2);
static string addStrings(string int1, string int2, string dec1, string dec2, bool n1);
static string subtractStrings(string int1, string int2, string dec1, string dec2, bool n1);
static int compareStrings(string int1, string int2, string dec1, string dec2);


// Main function
int main() {
	string filename; // Variable to hold filename
	 
	cout << "Enter the filename: "; // Prompt user for filename

	cin >> filename; // Get filename from user

	ifstream file(filename); // Open the file

	if (!file.is_open()) { // Check if file opened successfully
		cout << "Error opening file: " << filename << '\n';
		return 1; // Exit with error code
	}

	string line; // Variable to hold each line from the file

	int lineNumber = 0; // Line number tracker

	// While there are lines to read from the file
	while (getline(file, line)) {
		lineNumber++; // Increment line number

		string num1, num2;       // Variables to hold two numbers as strings
		istringstream iss(line); // Create a string stream from the line

		iss >> num1 >> num2; // Extract two numbers from the line

		// Validate the numbers
		if ( !isNumber(num1) || !isNumber(num2) ) {

			// If invalid, print error message and skip to next line
			cout << "\nInvalid number(s) found in line: " << lineNumber << '\n';
			continue; // Skip to the next line
		}

		// Calculate the sum or difference
		string sum = calculateStrings(num1, num2);

		// Print the result
		cout << '\n' << sum << '\n';
	}

	file.close(); // Close the file

	return 0; // End of the program
}


// Function to check if a string is a valid number
bool isNumber(string str) {
	if (str.empty()) return false; // Empty string check

	int i = 0; // Var for index

	// Check for sign at beginning and set index to 1 if so
	if (str.front() == '+' || str.front() == '-') i++;

	bool beforeDecimal = false; // Keep track of digits before decimal
	bool afterDecimal = false;  // Keep track of digits after decimal
	bool decimal = false;       // Keep track of decimal point

	// For each character in the string
	for (; i < str.size(); i++) {

		// If the character is a digit
		if (isdigit(str[i])) { 

			// Before decimal point
			if (!decimal) { 

				// There is a digit before the decimal
				beforeDecimal = true;
			} else {

				// There is a digit after the decimal
				afterDecimal = true;
			}
		
		// If the character is a decimal point
		} else if (str[i] == '.') {

			if (decimal) return false; // More than one decimal
			decimal = true; // Mark that we have seen a decimal point

		} else {
			return false; // Return false for any other character
		}
	}

	// If there's a decimal point, ensure digits on both sides
	if (decimal) return beforeDecimal && afterDecimal; 

	return beforeDecimal; // At least one digit found
}


// Function to add/subtract two strings
string calculateStrings(string str1, string str2) {
	bool n1 = false; // Negative var for str1
	bool n2 = false; // Negative var for str2

	bool subtraction = false; // Var for subtraction

	// Handle signs for str1
	if (!str1.empty() && str1.front() == '-') { // If negative
		n1 = true;             // Mark str1 as negative
		str1 = str1.substr(1); // Remove sign
	}
	else if (str1.front() == '+') { // If positive
		str1 = str1.substr(1); // Remove sign
	}

	// Handle signs for str2
	if (!str2.empty() && str2.front() == '-') { // If negative
		n2 = true; // Mark str2 as negative
		str2 = str2.substr(1); // Remove sign
	}
	else if (str2.front() == '+') { // If positive
		str2 = str2.substr(1); // Remove sign
	}

	// Determine if we are doing addition or subtraction
	if (n1 != n2) subtraction = true;

	// Find decimal positions
	size_t decimalPos1 = str1.find('.');
	size_t decimalPos2 = str2.find('.');

	// Create integer and decimal parts
	string int1, int2, dec1, dec2;

	// Split str1 into integer and decimal parts
	if (decimalPos1 != string::npos) {
		int1 = str1.substr(0, decimalPos1);  // Integer part
		dec1 = str1.substr(decimalPos1 + 1); // Decimal part
	}
	else { // No decimal point
		int1 = str1; // Integer part
		dec1 = "";	 // No decimal part
	}

	// Split str2 into integer and decimal parts
	if (decimalPos2 != string::npos) {
		int2 = str2.substr(0, decimalPos2);  // Integer part
		dec2 = str2.substr(decimalPos2 + 1); // Decimal part
	}
	else { // No decimal point
		int2 = str2; // Integer part
		dec2 = "";	 // No decimal part
	}

	// Pad the right side of the decimal with zeros
	while (dec1.size() < dec2.size()) dec1 += '0';
	while (dec2.size() < dec1.size()) dec2 += '0';

	// Pad the left side of the integer with zeros
	while (int1.size() < int2.size()) int1 = '0' + int1;
	while (int2.size() < int1.size()) int2 = '0' + int2;

	// Perform addition or subtraction
	if (!subtraction) {
		// Addition
		return addStrings(int1, int2, dec1, dec2, n1);
	}
	else { // Subtraction
		// Determine which number is larger
		int compare = compareStrings(int1, int2, dec1, dec2);
		if (compare > 0) {
			// str1 is larger
			return subtractStrings(int1, int2, dec1, dec2, n1);
		} else if (compare < 0) {
			// str2 is larger
			return subtractStrings(int2, int1, dec2, dec1, !n1);
		} else {
			// They are equal
			return "0";
		}
	}
}


// Function to add two strings
string addStrings(string int1, string int2, string dec1, string dec2, bool n1) {
	
	// Result strings
	string resultInt = "";
	string resultDec = "";

	// Carry for addition
	int carry = 0;

	// Add decimal parts
	for (int i = dec1.size() - 1; i >= 0; i--) {

		// Sum of digits and carry
		int sum = (dec1[i] - '0') + (dec2[i] - '0') + carry;

		// Update carry
		carry = sum / 10;

		// Append resulting digit to resultDec
		resultDec = char((sum % 10) + '0') + resultDec;
	}

	// Add integer parts
	for (int i = int1.size() - 1; i >= 0; i--) {

		// Sum of digits and carry
		int sum = (int1[i] - '0') + (int2[i] - '0') + carry;

		// Update carry
		carry = sum / 10;

		// Append resulting digit to resultInt
		resultInt = char((sum % 10) + '0') + resultInt;
	}

	// If there's a carry left add it to the front
	if (carry) resultInt = '1' + resultInt;

	// Remove trailing zeros from decimal part
	while (!resultDec.empty() && resultDec.back() == '0') resultDec.pop_back();

	// Remove leading zeros from integer part
	while (resultInt.size() > 1 && resultInt.front() == '0') resultInt.erase(resultInt.begin());

	// Construct final result string
	string result = resultInt;

	// Add decimal part if it exists
	if (!resultDec.empty()) result += "." + resultDec;

	// Add negative sign if needed
	if (n1 && !result.empty() && result != "0") result = "-" + result;

	// Return the result
	return result;
}


// Function to subtract two strings
string subtractStrings(string int1, string int2, string dec1, string dec2, bool n1) {
	string resultInt = ""; // Result integer part
	string resultDec = ""; // Result decimal part

	int carry = 0; // Borrow for subtraction

	// Subtract decimal parts
	for (int i = dec1.size() - 1; i >= 0; i--) {
		// Calculate difference with borrow
		int diff = (dec1[i] - '0') - (dec2[i] - '0') - carry;

		if (diff < 0) {
			// If needed, borrow from next digit
			diff += 10;
			carry = 1;
		} else {
			// No borrow needed
			carry = 0;
		}

		// Append resulting digit to resultDec
		resultDec = char(diff + '0') + resultDec;
	}

	// Subtract integer parts
	for (int i = int1.size() - 1; i >= 0; i--) {
		// Calculate difference with borrow
		int diff = (int1[i] - '0') - (int2[i] - '0') - carry;

		if (diff < 0) {
			// If needed, borrow from next digit
			diff += 10;
			carry = 1;
		} else {
			// No borrow needed
			carry = 0;
		}

		// Append resulting digit to resultInt
		resultInt = char(diff + '0') + resultInt;
	}

	// Remove leading zeros from integer part
	while (resultInt.size() > 1 && resultInt.front() == '0') resultInt.erase(0, 1);

	// Remove trailing zeros from decimal part
	while (!resultDec.empty() && resultDec.back() == '0') resultDec.pop_back();

	// Construct final result string
	string result = resultInt;
	if (!resultDec.empty()) result += "." + resultDec;

	// Add negative sign if needed
	if (n1 && !result.empty() && result != "0") result = "-" + result;

	// Return the result
	return result;
}


// Function to compare two strings
int compareStrings(string int1, string int2, string dec1, string dec2) {
	// If negative, reverse comparison
	if (int1 > int2) return 1;
	if (int1 < int2) return -1;

	// Integers are equal, compare decimals
	if (dec1 > dec2) return 1;
	if (dec1 < dec2) return -1;

	// They are equal
	return 0;
}



