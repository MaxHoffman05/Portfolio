/*
Project 2 - Databases

Library Database using SQL and C++ or Java

By: Max Hoffman

Date: 11/13/2023 - 11/18/2023

Based on the main-OO.cpp that you gave us as an example

*/
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>

#include <mysql/jdbc.h> 

//#include <conio.h> commented out because this was visual studio dependent

using namespace std;

//used for syntax of some sql commands that I had forgotten : https://mariadb.com/kb/en/mariadb-basics/
//used for syntax for the sql library: https://docs.oracle.com/cd/B13789_01/appdev.101/b10778/reference025.htm
//used this for syntax on primary keys in SQL: https://mariadb.com/kb/en/basic-sql-statements/
//used this for the right to_string() syntax: https://cplusplus.com/reference/string/to_string/


//globals for the database parameters
string db_host;
string db_port;
string db_user;
string db_password;
string db_name;


// reads in a password without echoing it to the screen from main-OO.cpp example
string myget_passwd()
{
	string passwd;
	char ch;
	for (;;)
	{
		ch = 0;
		while (ch == 0)   // Ugh. Microsoft not following standards on getch, so need to keep reading until a new char is typed. 
			//ch = _getch();           // get char WITHIOUT echo! (should really be getch(), but MS is not following standrads again.), same down here
		if (ch == 13 || ch == 10) // if done ...
			break;           //  stop reading chars!
		cout << '*';  // dump * to screen
		passwd += ch;   // addd char to password
	}
	cin.sync(); // flush anything else in the buffer (remaining newline)
	cout << endl;  // simulate the enter that the user pressed


	// read until end of line ... so future getlines work. 
	string dummy;
	getline(cin, dummy);

	return passwd;
}

//used for printing out the tables 
void printTable(string tableN, int tableNum) {
	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* conn = driver->connect(db_host, db_user, db_password);
	conn->setSchema(db_name);

	string creator = "select * from " + tableN;
	sql::Statement* stmt = conn->createStatement();
	sql::ResultSet* res = stmt->executeQuery(creator);

	if (tableNum == 1) {//print the library database
		
		cout << "_____________Library Table____________" << endl;
		cout << "| Library Name | City Name | Zip Code|" << endl;
		while (res->next()) {//based off of the code you gave us make a result set with a select * statement and print out each row and column in the table
			cout << "| " << res->getString("nameL") << " | " << res->getString("citynameL") << " | " << res->getString("zipCode") << " | " << endl;
			
		}
	}
	else if (tableNum == 2) {//print out the holdings table
		cout << "_____________Holding Table____________" << endl;
		cout << "|ISBN | Library Name | Copy Count|" << endl;
		while (res->next()) {
			cout << "| " << res->getString("ISBN_H") << " | " << res->getString("LibraryName_H") << " | " << res->getString("countCopy_H") << " | " << endl;

		}
	}
	else if(tableNum == 3){//print out the book table
		cout << "______________Book Table______________" << endl;
		cout << "|ISBN | Publish Date | Book Title | Author Name|" << endl;
		while (res->next()) {
			cout << "| " << res->getString("ISBN_B") << " | " << res->getString("year_B") << " | " << res->getString("name_B") << " | " << res->getString("authorName_B") << " | " << endl;

		}
	}
	delete res;
	delete stmt;
	conn->close();

}

//lazy just made a method to print all the tables as well that calls printTable()
void printTables() {
	printTable("libraries_T", 1);
	printTable("holdings_T", 2);
	printTable("books_T", 3);
}

//method to count the number of copies for holdings
int numOfCopies(string tableN, string isbn, string name) {

	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* conn = driver->connect(db_host, db_user, db_password);
	conn->setSchema(db_name);
	string creator = "select * from holdings_T;";
	int count = 1;//count starts at 1
	sql::Statement* stmt = conn->createStatement();
	sql::ResultSet* res = stmt->executeQuery(creator);


	//loop through the whole table and add to the count when you see the correct isbn and name
	while (res->next()) {
		if (res->getString("ISBN_H") == isbn) {
			if (res->getString("LibraryName_H") == name) {
				count += 1;
			}
		}
	}
	
	
	conn->close();
	return count;
	/*
	I originally was going to do it the SQL way but after trying to debug what was going on I just descided to go the c++ route and go manually count the copies instead of having a SQL query do it
	//string creator = "select DISTINCT Count(ISBN_H AND LibraryName_H) as numHolds from " + tableN + " where LibraryName_H = '" + name + "' AND ISBN_H = '" + isbn + "' group by ISBN_H, LibraryName_H;";
	*/
}


//method used for updating a table
void updateTable(string table, string command) {
	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* conn = driver->connect(db_host, db_user, db_password);
	conn->setSchema(db_name);



	string creator;//SQL string command
	int lastPlace;//used for getting the strings inputted by the user
	int tempVal;

	//books_T variables to enter in table 
	string isbn;
	string year;
	string name;

	//libraries_T variables to enter in the table (reuse name)
	string cityN;
	string zipCode;
	
	if (table == "b") {//we are updating the book table

		//same code from Project #1 to grab each variable, there is probably a cleaner way to do this
		for (int i = 0; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;//start on the spot that is right after the space that was just found
				i = command.length();
			}
			else {
				isbn += command[i];
			}
		}
		tempVal = lastPlace;
		lastPlace = 0;
		for (int i = tempVal; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				year += command[i];
			}
		}
		tempVal = lastPlace;
		lastPlace = 0;
		for (int i = tempVal; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				name += command[i];
			}
		}
		string authorName;
		tempVal = lastPlace;
		
		for (int i = tempVal; i < command.length(); i++) {
			if (i == command.length()) {
				
				i = command.length();
			}
			else {
				authorName += command[i];
			}
		}

		//build up the SQL string to input what the user wants into the mysql table
		creator += "insert into books_T values( '" + isbn + "', '" + year + "', '" + name + "','" + authorName +"'); ";//also realized I needed the apostrophe because each field is a char not an int for year or isbn, makes this look very complicated!
		sql::Statement* stmt = conn->createStatement();
		stmt->execute(creator);

		delete stmt;
		printTable("books_T", 3);//print the table out passing the same connection and name
	}
	else if (table == "l") {
		//same code as above but now for libaries_T

		for (int i = 0; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				name += command[i];
			}
		}
		tempVal = lastPlace;
		lastPlace = 0;
		for (int i = tempVal; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				cityN += command[i];
			}
		}
		tempVal = lastPlace;
		lastPlace = 0;
		for (int i = tempVal; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				zipCode += command[i];
			}
		}

		creator += "insert into libraries_T values( '" + name + "', '" + cityN + "', '" + zipCode + "');";
		sql::Statement* stmt = conn->createStatement();
		stmt->execute(creator);

		delete stmt;
		printTable("libraries_T", 1);
	}
	else if (table == "h") {//same code as above but for the holdings table

		for (int i = 0; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;//start on the spot that is right after the space that was just found
				i = command.length();
			}
			else {
				isbn += command[i];
			}
		}
		tempVal = lastPlace;
		lastPlace = 0;
		for (int i = tempVal; i < command.length(); i++) {
			if (command[i] == ' ') {
				lastPlace = i + 1;
				i = command.length();
			}
			else {
				name += command[i];
			}
		}

		//get the number of copies for the book to then be entered into the database
		//call a SQL query to count the number of copies already so that value can be entered into the table with the other holding info
		string copy;
		int copyNum = numOfCopies("holdings_T", isbn, name);
		copy = to_string(copyNum);//was trying to use toString() and then found the right syntax which is to_string() from this website https://cplusplus.com/reference/string/to_string/
		//originally I was trying to use append() or other functions but they did not work

		creator += "insert into holdings_T values( '" + isbn + "', '" + name + "', '" + copy + "');";
		sql::Statement* stmt = conn->createStatement();
		stmt->execute(creator);

		delete stmt;
		printTable("holdings_T", 2);
	}
	conn->close();
}

//used for the new delete feature deletes all holdings from the holdings_T table that have the inputted isbn and Library name
void deleteHolds(string command) {
	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* conn = driver->connect(db_host, db_user, db_password);
	conn->setSchema(db_name);
	int lastPlace = 0;
	string isbn;
	int tempVal = 0;
	string name;
	
	//get the isbn and library Name
	//same code from project #1
	for (int i = 0; i < command.length(); i++) {
		if (command[i] == ' ') {
			lastPlace = i + 1;//start on the spot that is right after the space that was just found
			i = command.length();
		}
		else {
			isbn += command[i];
		}
	}
	tempVal = lastPlace;
	lastPlace = 0;
	for (int i = tempVal; i < command.length(); i++) {
		if (command[i] == ' ') {
			lastPlace = i + 1;
			i = command.length();
		}
		else {
			name += command[i];
		}
	}

	//make the SQL string to delete all the holdings with the inputted isbn and library name
	string creator = "delete from holdings_T where ISBN_H = '" + isbn + "' AND LibraryName_H = '" + name + "';";
	sql::Statement* stmt = conn->createStatement();
	stmt->execute(creator);
	cout << endl;
	cout << "Holdings Table After Deletion" << endl;
	printTable("holdings_T", 2);


	delete stmt;
	conn->close();
}


//used for the find function of the program to find all isbns matching the input in the holding table
void findHolds(string isbn) {
	sql::Driver* driver = sql::mysql::get_driver_instance();
	sql::Connection* conn = driver->connect(db_host, db_user, db_password);
	conn->setSchema(db_name);

	//SQL query to find all the matching info between 2 tables (books_T and holdings-T) given the isbn
	string creator = "select LibraryName_H, ISBN_H, year_B, authorName_B, name_B, countCopy_H from holdings_T, books_T where ISBN_B = ISBN_H and ISBN_H = '" + isbn + "';";
	sql::Statement* stmt = conn->createStatement();
	sql::ResultSet* res = stmt->executeQuery(creator);
	
	//go through the result set and print out all the info that we want for the inputted isbn
	cout << "|Library Name | ISBN | Publish Date | Title | Author Name | Copy Count|" << endl; cout.flush();
	while (res->next()) {
		cout << res->getString("LibraryName_H") << " | " << res->getString("ISBN_H") << " | " << res->getString("year_B") << " | " << res->getString("authorName_B") << " | "
			<< res->getString("name_B") << " | " << res->getString("countCopy_H") << " | " << endl;
	}
}

int main()
{

	


	// same code from the main--OO example code you gave us the only difference is that I made these strings global variables so I can use them in any method
	// also the option to now read in the database name at the bottom
	// set up the client (this machine) to use mysql
	cout << "initializing client DB subsystem ..."; cout.flush();
	//mysql_init(&mysql);
	sql::Driver * driver = sql::mysql::get_driver_instance();
	cout << "Done!" << endl << endl;
	
	// get user credentials and mysql server info
	cout << "Enter MySQL database hostname (or IP adress):";
	getline(cin, db_host); // cin >> db_host;

	cout << "Enter MySQL database port number (press enter for default):";
	getline(cin, db_port);
	if (db_port.length() > 0)
		db_host += ":" + db_port;
	
	cout << "Enter MySQL database username:";
	cin >> db_user;

	cout << "Enter MySQL database password:";
	db_password = myget_passwd();

	//project does state we need to include reading in the database name
	cout << "Enter the MySQL database name: ";
	getline(cin, db_name);
	

	try
	{
		//connect to the database
		sql::Connection* conn = driver->connect(db_host, db_user, db_password);
		conn->setSchema(db_name);

		//create the three tables
		//create the tables if regardless if they are there or not
		sql::PreparedStatement* pstmt;

		//library table
		string creator = "create table if not exists libraries_T(nameL varchar(50) PRIMARY KEY, citynameL varchar(30), zipCode varchar(10)); ";
		sql::Statement* stmt = conn->createStatement();
		stmt->execute(creator);

		//holdings table
		creator = "create table if not exists holdings_T(ISBN_H varchar(20), LibraryName_H varchar(30), countCopy_H varchar(10)); ";
		stmt = conn->createStatement();
		stmt->execute(creator);
		
		
		//book table
		creator = "create table if not exists books_T(ISBN_B varchar(20) PRIMARY KEY, year_B varchar(5), name_B varchar(100),  authorName_B varchar(20));";
		stmt = conn->createStatement();
		stmt->execute(creator);
		

		cout << "_____Project 2 - Library Database_____" << endl;
	
		//output all tables using the print method above
		printTables();
		
		string command;
		//start while loop for main loop to enter commands
		
		while (command != "q") {//same while loop from Project #1 except instead of calling methods that then edited 
			//the files for each table now I will be calling a function for each that uses SQL
			cout << "Enter a command: (q to quit) " << endl;
			getline(cin, command);

			if (command[0] == 'a') {//each of the add commands uses the same method
				//we also pass in a string to identify which table we need to add to and the database connection
				
				if (command[2] == 'b') {//add book
					updateTable("b", command.substr(4, command.length()));
				}
				else if (command[2] == 'l') {//if we are going to add a library
					updateTable("l", command.substr(4, command.length()));
				}
				else if (command[2] == 'h') {//if we are going to add a holding
					updateTable("h", command.substr(4, command.length()));
				}

			}
			else if (command[0] == 'l') {//if statements for list calls we just call the print method I made for both
				//if we are going to list all the books
				if (command[2] == 'b') {
					cout << "-----List of All Books-----" << endl;
					//call the print method
					printTable("books_T", 3);
				}
				//if we are listing all the libraries
				else if (command[2] == 'l') {
					cout << "-----List of All Libraries-----" << endl;
					printTable("libraries_T", 1);
				}

			}
			else if (command[0] == 'd') {//NEW delete method that was not in the last project
				//delete all holdings with the input given
				deleteHolds(command.substr(2, command.length()));
			}
			else if (command[0] == 'f') {//find method
				//take the rest of the inputted string this is the isbn of the book we are looking for
				findHolds(command.substr(2, command.length()));
			}			
		}

		cout.flush();
		
		delete stmt;
		conn->close();
	}
	catch (sql::SQLException sqle)
	{
		cout << "Exception in SQL: " << sqle.what() << endl;
	}
	return 0;
}