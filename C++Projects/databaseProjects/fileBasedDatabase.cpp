/*
* Project 1 Databases with Files
* 
* Name: Max Hoffman
* 
* Date: 8/25/2023 - 9/2/2023
* 
*/

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/*
I used this for syntax on files since I had forgotten from CSII!
https://www.w3schools.com/cpp/cpp_files.asp

source you sent me on how you can append data to a file
https://stackoverflow.com/questions/2393345/how-to-append-text-to-a-text-file-in-c
https://cplusplus.com/reference/fstream/ofstream/ofstream/


//after looking at cin syntax and variations I realized I had forgotten about getline()!! I remembered we mentioned it last semester and I dont think I really used it in any of the projects so I had forgotten about it
https://cplusplus.com/doc/tutorial/basic_io/
https://www.geeksforgeeks.org/cin-in-c/#
*/


//I also included on my read.me a better explanation of which methods are used together and why, since I know I made a lot of them!

//list method to print out the whole contents of the chosen "database"
void list(ifstream& fileToList) {
	string nextVal;

	int count = 0;
	while (fileToList) {//similar code from Proj 1 from CS II last semester
		if (nextVal != "") {
			cout << nextVal << " ";
		}
		fileToList >> nextVal;
		if (nextVal == "|") {//if we see a pipe by itself this means it is the end of a row or the end of a book entry so we will skip over it and cout an enter
			fileToList >> nextVal;
			cout << endl;
		}
	}
}


//function to check if we already have the book we want to add in the book file
bool find(ifstream & fileToCheck, string lookingFor) {
	string nextVal;

	//if we are searching for a book the lookingFor value will be the isbn
	//if we call this method from the addLibraries() method then we will be looking for the same name that is already in the fileToCheck
	while (fileToCheck) {
		fileToCheck >> nextVal;
		if (nextVal == lookingFor) {
			//if we found the same value we return false
			return false;
		}

	}

	return true;//we can add the entry to the database since we didnt find it
}

//add method for adding books
void addBook(string command) {
	//Order of info that is inputted: ISBN, year, Author Last Name, Title
	//Get the ISBN
	int lastPlace;
	string isbn;
	
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			lastPlace = i + 1;//start on the spot that is right after the space that was just found
			i = command.length();
		}
		else {
			isbn += command[i];
		}
	}

	//first check that this book does not exist in the book file already
	ifstream checkBook("books.txt");

	if (find(checkBook, isbn)) {//call the findBook method to check to see if we already have the book present if not then we will add it
		//write to the book file
		ofstream bookOutput("books.txt", ios::app);
		bookOutput << command << " |" << endl; //we use a pipe to mark the end of the data to be used later when printing out the rows correctly
		bookOutput.close();

	}
	
	//we cannot add the book so nothing happens
	
}

//add to the library file
void addLibrary(string command) {
	//file for all libaries

	ifstream checkLibraries("libraries.txt");
	string libraryN;
	int lastPlace = 0;


	//get the name from the library that was passed to this method
	//first we iterate over the zip code
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			lastPlace = i + 1;
			i = command.length();
		}
	}

	//iterate around the city name
	for (int k = lastPlace; k < command.length(); k++) {
		if (command[k] == ' ') {
			lastPlace = k + 1;
			k = command.length();
		}
	}
	//whatever is left is the name of the library
	for (int j = lastPlace; j < command.length(); j++) {
		if (command[j] == '|') {
			j = command.length();
		}
		else {
			libraryN += command[j];
		}
	}

	//we need to check if the library is already in the database so we dont want to add one if it has the same name as a library that is already in the database. It can however have the same ZIP code since multiple libaries could reside in the same ZIP code
	if (find(checkLibraries, libraryN)) {
		ofstream outputLibrary("libraries.txt", ios::app);
		outputLibrary << command << " |" << endl;//same pipe used to mark the end of a library row like in the book file

		outputLibrary.close();
	}

	
}

//Both findHold() and countCopies() methods are similar code to find info from the database
//used to find if there are any other holds on the same book that is going to be added to the holding text file
bool findHold(ifstream & searchHoldings, string isbn, string libraryN) {
	string nextVal;


	while (searchHoldings) {
		searchHoldings >> nextVal;
		if (nextVal == isbn) {
			searchHoldings >> nextVal;
			if (nextVal == libraryN) {
				return false;//we found it so we must also count how many copies there are 
			}
			
		}
	}
	return true;//we didnt find it so we can return true and this will be the first copy
}


//used to count the copies of books in the holding file
int countCopies(ifstream& searchHoldings, string isbn, string libraryN) {
	string nextVal2;
	int count = 1;//this is initalized to 1 since by defualt a new holding (a holding that does not exist in the database) always is called copy 1 so no matter what we will have to start counting at 1

	while (searchHoldings) {
		searchHoldings >> nextVal2;
		if (nextVal2 == isbn) {
			searchHoldings >> nextVal2;
			if (nextVal2 == libraryN) {
				count = count + 1;
			}

		}
	}
	return count;
}
    

//add holds to the holds file 
void addHolds(string command) {
	int bookCount = 0;
	int lastPlace = 0;
	int numOfCopies = 0;
	string isbn;
	string libraryN;
	//we then need to add the copy number and to find that we need to count how many (if any) are already at the library
	ifstream searchHoldings("holdings.txt");

	//we first must grab the isbn and library name then check to see if it already in the database if so this will add to the copy counter of this book
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			lastPlace = i + 1;
			break;
		}
		isbn += command[i];
	}

	//get the library name
	for (int j = lastPlace; j < command.length(); j++) {
		if (command[j] == '|') {
			break;
		}
		libraryN += command[j];
	}

	//if statement to determine if this is a new book for the database or a duplicate
	if (findHold(searchHoldings, isbn, libraryN)) {

		//add a holding to the holding file
		ofstream outputHoldings("holdings.txt", ios::app);
		outputHoldings << command << " " << " |" << " 1" << endl;
		searchHoldings.close();
	}
	else {//this book already existed in this database so we must count the copies and then put it in the database

		//create a new ifstream to go through the database to search for the number of copies
		ifstream searchForCopies("holdings.txt");

		numOfCopies = countCopies(searchForCopies, isbn, libraryN);

		ofstream outputHoldings("holdings.txt", ios::app);
		outputHoldings << command << " " << " | " <<  numOfCopies << endl;
		searchForCopies.close();
	}
}


//method for our find command, this searches the holding database and then will search the book database for the other information and display that info to the user
void findHolding(string isbnToFind) {
	ifstream findHolds("holdings.txt");
	string nextValue;
	string libraryName;
	string isbnTemp;


	string bookValue;

	//first check to see if we have the right book
	while (findHolds) {
		findHolds >> nextValue;
		if (nextValue == isbnToFind) {//we found the book so we will print out the info of the book and call a method to print out the book info
			isbnTemp = nextValue;//keep the isbn so we can print it out later and use to find book info

			findHolds >> nextValue;
			libraryName = nextValue;
			cout << libraryName << ": " << isbnTemp;

			//we then need to look into the book database to see the books information and print out the date, name, title from the other file
			ifstream bookInfo("books.txt");

			//find the same isbn that we stored in the isbnTemp variable
			while (bookInfo) {
				bookInfo >> bookValue;
				if (isbnTemp == bookValue) {//we found the same book we will then print out the info and get out of the loop since we only have to print out 1 books data
					while (bookValue != "|") {
						cout << bookValue << " ";
						bookInfo >> bookValue;
					}
					bookInfo.close();
				}
			}

			//lastly we print out the copy number from the holdings database
			findHolds >> nextValue;//pipe marker
			findHolds >> nextValue;//this value will have the copy number or if you look in the holdings.txt file it will be the number after the pipe which is the copy number

			cout << "Copy Number: " << nextValue << endl;

		}
	}

	findHolds.close();
}


int main(int argc, char* argv[]) {
	string command;//command entered
	string test;

	//create the files in case they were not already created

	ofstream holdings("holdings.txt", ios::app);
	ofstream libraries("libraries.txt", ios::app);
	ofstream books("books.txt", ios::app);


	cout << "-----Library & Book Database-----" << endl;


	//main loop for inputting commands and putting the data into the files
	while (command[0] != 'q') {

		cout << "Enter a command: ";
		getline(cin, command);//it took me a long time to remember to use getLine after looking at cin syntax online I realized that would solve the problem of getting the input broken up instead of all in one string

		//if statements for each command that can be entered
		if (command[0] == 'a') {
			//if we are adding a book
			if (command[2] == 'b') {
				addBook(command.substr(4, command.length()));//call the add method but we only need to pass it what is after the a and b
			}
			else if (command[2] == 'l') {//if we are going to add a library
				addLibrary(command.substr(4, command.length()));
			}
			else if (command[2] == 'h') {//if we are going to add a holding
				addHolds(command.substr(4, command.length()));
			}

		}
		else if (command[0] == 'l') {//if statements for list calls
			//if we are going to list all the books
			if (command[2] == 'b') {
				cout << "-----List of All Books-----" << endl;
				ifstream bookList("books.txt");
				list(bookList);
			}
			//if we are listing all the libraries
			else if (command[2] == 'l') {
				cout << "-----List of All Libraries-----" << endl;
				ifstream libraryList("libraries.txt");
				list(libraryList);
			}

		}
		else if (command[0] == 'f') {//find method
			//take the rest of the inputted string this is the isbn of the book we are looking for
			findHolding(command.substr(2, command.length()));
		}
		
	}

	holdings.close();
	books.close();
	libraries.close();


	return 0;
}