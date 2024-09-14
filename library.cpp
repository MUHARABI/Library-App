//file name: library.cpp
//Name: Muhammed Arabi
//net ID: marab2
//CS 251 12:00 Section
//Project 1 Library Management System
//c++ file with various functions and two vectors

// You cannot use any libraries beyond the ones we have given below.
// You must use output streams, file streams, vectors, and strings.
// You are not allowed to use any other containers or objects.
// You may not use structs, pointers, or dynamic memory allocations.

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype>

using namespace std;

// Function prototypes for all the glorious function decomposition
//functions:
void printMenu();
void addItemToLibrary(vector<string>& library);
void clearLibrary(vector<string>& library);
void displayLibraryEntries(const vector<string>& library);
void listInvalidLibraryEntries(const vector<string>& library);
void loadLibraryFromFile(vector<string>& library, vector<string> &origin);
void outputLibraryToFile(vector<string>& library, vector<string> &origin);
void printCheckoutStatistics(const vector<string>& library);
void removeLibraryEntry(vector<string>& library);
void searchLibraryEntry(const vector<string>& library);


int main()
{
    string command;
    //vectors used:
    vector<string> origin; //used to keep original format of book entry
    vector<string> library; //used to add books to system
    
    cout << "Welcome to the Library Management System" << endl;
    cout << "----------------------------------------" << endl;
    cout << endl;

    do 
    {
        // Output the menu and acquire a user selection
        printMenu();
        cout << endl << "Enter a command (case does not matter): ";

        // We use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline
        getline(cin, command);
        cout << endl;

        for(char& c: command){
            c = toupper(c);
        }
        if (command == "A"){
            addItemToLibrary(library);
        }
        else if(command == "C"){
            clearLibrary(library);
        }
        else if(command == "D"){
            displayLibraryEntries(library);
        }
        else if(command == "I"){
            listInvalidLibraryEntries(library);
        }
        else if(command == "L"){
            loadLibraryFromFile(library, origin);
        }
        else if(command == "O"){
            outputLibraryToFile(library, origin);
        }
        else if(command == "P"){
            printCheckoutStatistics(library);
        }
        else if(command == "R"){
            removeLibraryEntry(library);
        }
        else if(command == "S"){
            
            searchLibraryEntry(library);
        }

        cout << endl;
    } while (!(command == "x" || command == "X"));

    return 0;
}



//main menu
void printMenu()
{
    cout << "Library Management Menu" << endl;
    cout << "-----------------------" << endl;
    cout << "A - Add Item To Library" << endl;
    cout << "C - Clear The Library Of All Entries" << endl;
    cout << "D - Display Library Entries" << endl;    
    cout << "I - List Invalid Library Entries" << endl;
    cout << "L - Load Library From File" << endl;    
    cout << "O - Output Library To File" << endl;
    cout << "P - Print Out Checkout Stats" << endl;
    cout << "R - Remove A Library Entry" << endl;      
    cout << "S - Search For A Library Entry" << endl;    
    cout << "X - Exit Program" << endl;
}
//adds user input of title, ISBN to the library
void addItemToLibrary(vector<string>& library){
    string bookTitle;
    string isbn;
    cout << "What is the book title? " << endl;
    getline(cin, bookTitle);
    // Check for commas in the book title
    if (bookTitle.find(',') != string::npos) {
        cout << "The book title cannot contain commas." << endl;
        return;
    }
    cout << "What is the 13-digit ISBN (with hyphens)? " << endl;
    getline(cin, isbn);
    // Checks if ISBN has 13 digits with hyphens
    if (isbn.length() > 18) {
        cout << "Invalid ISBN format. ISBN should have 13 hyphen-separated digits." << endl;
        return;
    }
    // Checks if ISBN format is valid 
    for (char c : isbn) {
        if (!isdigit(c) && c != '-') {
            cout << "Invalid ISBN format. ISBN should have 13 hyphen-separated digits." << endl;
            return;
        }
    }
    // Adds to the library
    library.push_back(bookTitle + " --- " + isbn + " --- In Library");
    cout << "The Following Entry Was Added" << endl << "-----------------------------" << endl;
    cout << bookTitle << " --- " << isbn << " --- In Library" << endl;;
}
//deletes all entries in library vector
void clearLibrary(vector<string>& library){
    library.clear();
    cout << "Your library is now empty." << endl;
}
//shows whats in the library
void displayLibraryEntries(const vector<string>& library) {
    if (library.empty()) {
        cout << "Your Current Library" << endl;
        cout << "--------------------" << endl;
        cout << "The library has no books." << endl;
        return;
    }
    cout << "Your Current Library" << endl;
    cout << "--------------------" << endl;
    for (const string& entry : library) {
        cout << entry << endl;
    }
}

//identifies the invalid entries based on ISBN format, or if status: "Invalid State".
void listInvalidLibraryEntries(const vector<string>& library) {
    cout << "The Following Library Entries Have Invalid Data" << endl;
    cout << "-----------------------------------------------" << endl;

    if (library.empty()) {
        cout << "The library has no books." << endl;
        return;
    }

    bool foundInvalid = false;

    for (const std::string& item : library) {
        bool hasInvalidData = false;
        std::string title = item; // Initialize title with the entire item
        std::string isbnDigits = "";

        // Check if the item contains "Invalid State" at the end (case-sensitive)
        if (item.find("Invalid State") != std::string::npos) {
            hasInvalidData = true;
            // Remove "Invalid State" from the title
            
        }
        
        // Find the position of the last '-' character to extract ISBN
        size_t lastDashPos = item.find_first_of('-')+3;
        
        if (lastDashPos != std::string::npos) {
            std::string isbnPart = item.substr(lastDashPos + 1);
            
            // Extract digits from ISBN part
            for (char c : isbnPart) {
                if (isdigit(c)) {
                    isbnDigits += c;
                }
            }
            
            // Check ISBN validity
            int actualCheckDigit = 0;

            if (isbnDigits.length() == 13) {
                isbnDigits = isbnDigits.substr(isbnDigits.length() - 13, 13); // Get the last 13 characters
                actualCheckDigit = stoi(string(1, isbnDigits[12]));
            } else {
                foundInvalid = true;
                hasInvalidData = true;
            }

            if (actualCheckDigit != 0) {  
                int sum = 0;
                for (int i = 0; i < 12; ++i) {
                    int digit = stoi(isbnDigits.substr(i, 1));
                    if (i % 2 == 0) {
                        sum += digit;
                    } else {
                        sum += digit * 3;
                    }
                }
                int remainder = sum % 10;
                int calculatedCheckDigit = 10 - remainder;
                
                if (calculatedCheckDigit == 10) {
                    calculatedCheckDigit = 0;
                }

                if (calculatedCheckDigit != actualCheckDigit) {
                    hasInvalidData = true;
                    foundInvalid = true;
                }
            }
        }
        //if invalid output invalid entries:
        if (hasInvalidData) {
            cout << item << endl;
            foundInvalid = true;
        }
    }
    //no invalid case
    if (!foundInvalid) {
        cout << "The library has no invalid entries." << endl;
    }
}


//imports entries from an external file and adds them to library:
void loadLibraryFromFile(vector<string>& library, vector<string>& origin) {
    cout << "What database to read from? " << endl;
    string filename;
    getline(cin, filename);

    ifstream infile(filename);
    if (infile.is_open()) {
        string line;
        int count = 0;

        while (getline(infile, line)) {
            count += 1;

            // Find the positions of the commas in the line
            size_t firstCommaPos = line.find(',');
            size_t secondCommaPos = line.find(',', firstCommaPos + 1);

            // Extract the bookTitle, isbn, and status
            string bookTitle = line.substr(0, firstCommaPos);
            bookTitle.erase(0, bookTitle.find_first_not_of(" \t")); // Remove leading whitespace

            string isbn = line.substr(firstCommaPos + 2, secondCommaPos - firstCommaPos - 2);
            isbn.erase(isbn.find_last_not_of(" \t") + 1); // Remove trailing whitespace

            // Check if isbn contains non-digit characters
            bool hasNonDigit = false;
            for (char c : isbn) {
                if (!isdigit(c) && c != '-') {
                    hasNonDigit = true;
                    break;
                }
            }

            string status;
            if (secondCommaPos != string::npos) {
                status = line.substr(secondCommaPos + 2);
            } else {
                status = "0";  // Default to "In Library"
            }

            // Assign the status code to the right status string
            string statusString;
            if (status == "0") {
                statusString = "In Library";
            } else if (status == "1") {
                statusString = "Checked Out";
            } else if (status == "2") {
                statusString = "On Loan";
            } else if (status == "3") {
                statusString = "Unknown State";
            } else if (status > "3") {
                origin.push_back(status);
                statusString = "Invalid State"; 
            } else {
                statusString = "In Library";
            }

            if (isbn.empty() || hasNonDigit) {
                isbn = "000-0-00-000000-0";  // Default ISBN
            }
            if (isbn == "000-0-00-000000-0" && bookTitle.find(":") != string::npos) {
                size_t colonPos = bookTitle.find(":");
                if (colonPos != string::npos) {
                    isbn = line.substr(firstCommaPos + 2, secondCommaPos - firstCommaPos - 2);
                }
            }

            // Organize and add the entry to the library vector
            string formattedEntry = bookTitle + " --- " + isbn + " --- " + statusString;
            library.push_back(formattedEntry);
        }
        cout << "Read in " << count << " lines from the file." << endl;
        infile.close();
    } else {
        cout << "Could not find the database file." << endl;
    }
}

//imports the entries loaded in from the previous function to a new file
void outputLibraryToFile(vector<string>& library, vector<string> &origin) {
    cout << "Enter the filename to save the library to: ";
    string filename;
    getline(cin, filename);

    ofstream output(filename);
    if (output.is_open()) {
        for (const string& item : library) {
            string formattedItem = item;

            // Replace status strings with corresponding status numbers
            if (formattedItem.find("In Library") != string::npos) {
                formattedItem.replace(formattedItem.find("In Library"), 10, "0");
            } else if (formattedItem.find("Checked Out") != string::npos) {
                formattedItem.replace(formattedItem.find("Checked Out"), 12, "1");
            } else if (formattedItem.find("On Loan") != string::npos) {
                formattedItem.replace(formattedItem.find("On Loan"), 8, "2");
            } else if (formattedItem.find("Unknown State") != string::npos) {
                formattedItem.replace(formattedItem.find("Unknown State"), 14, "3");
            }else if (formattedItem.find("Invalid State") != string::npos) {
                // Extract the original status number and use it
                size_t invalidStatePos = formattedItem.find("Invalid State");
                if (invalidStatePos != string::npos) {
                    // Check if there are remaining elements in origin
                    if (!origin.empty()) {
                        formattedItem.replace(invalidStatePos, 14, origin.front());
                        origin.erase(origin.begin());  // Remove the used element from origin
                    }
                }
            }



            // Replaces "---" with ","
            size_t pos = 0;
            while ((pos = formattedItem.find("---", pos)) != string::npos) {
                formattedItem.replace(pos, 4, ",");
                pos += 1;
            }

            output << formattedItem << endl;
        }
        cout << "Library saved to file." << endl;
        output.close();
    } else {
        cout << "Error: Unable to open the file for writing." << endl;
    }
}


//displays status of book
void printCheckoutStatistics(const vector<string>& library) {
    int totalBooks = library.size();
    int inLibraryCount = 0;
    int checkedOutCount = 0;
    int onLoanCount = 0;
    int unknownCount = 0;
    int otherCount = 0;

    for (const string& entry : library) {
        if (entry.find("In Library") != string::npos) {
            inLibraryCount++;
        } else if (entry.find("Checked Out") != string::npos) {
            checkedOutCount++;
        } else if (entry.find("On Loan") != string::npos) {
            onLoanCount++;
        } else if (entry.find("Unknown State") != string::npos) {
            unknownCount++;
        } else {
            otherCount++;
        }
    }

    cout << "Your Current Library's Stats" << endl;
    cout << "----------------------------" << endl;
    cout << "Total Books: " << totalBooks << endl;
    cout << "   In Library: " << inLibraryCount << endl;
    cout << "   Checked Out: " << checkedOutCount << endl;
    cout << "   On Loan: " << onLoanCount << endl;
    cout << "   Unknown: " << unknownCount << endl;
    cout << "   Other: " << otherCount << endl;
}

//removes a specific entry in library vector.
void removeLibraryEntry(vector<string>& library) {
    cout << "Would you like remove by (1) name or (2) ISBN." << endl;
    cout << "Enter the numeric choice: " << endl;
    int choice;
    cin >> choice;
    cin.ignore();

    string search;

    if (choice == 1) {
        cout << "Enter the book name: " << endl;
        getline(cin, search);
    }
    else if (choice == 2) {
        cout << "Enter the book 13-digit ISBN (with dashes): " << endl;
        getline(cin, search);
    }
    else {
        cout << "Invalid remove by choice option." << endl;
        return;
    }

    cout << "The Following Entry Was Removed From The Library" << endl;
    cout << "------------------------------------------------" << endl;

    int mostRecentIndex = -1; // Initialize to an invalid index
    bool found = false;

    for (int i = library.size() - 1; i >= 0; i--) {
        string item = library[i];
        bool hasInvalidData = false;

        if (choice == 1) {
            // Remove by name
            size_t namePos = item.find(" --- ");
            if (namePos == string::npos) {
                hasInvalidData = true;
            }
        }
        else if (choice == 2) {
            // Remove by ISBN
            size_t isbnPos = item.find(" --- ");
            size_t isbnEndPos = item.rfind(" --- ");
            if (isbnPos == string::npos || isbnEndPos == string::npos) {
                hasInvalidData = true;
            }
            else {
                string itemISBN = item.substr(isbnPos + 5, isbnEndPos - (isbnPos + 5));
                if (itemISBN == search) {
                    found = true;
                    mostRecentIndex = i; // Update the most recent index
                    cout << item << endl;
                    break;  // Remove only one entry
                }
            }
        }
        // Search by name
        if (choice == 1 && !hasInvalidData) {
            
            size_t namePos = item.find(" --- ");
            if (namePos != string::npos) {
                string itemName = item.substr(0, namePos);
                if (itemName == search) {
                    found = true;
                    mostRecentIndex = i; // Update the most recent index
                    cout << item << endl;
                    break;  // Remove only one entry
                }
            }
        }
    }

    if (found) {
        library.erase(library.begin() + mostRecentIndex); // Remove the most recently added duplicate
    }
    else {
        cout << "No matching entry found in the library." << endl;
    }
}


//search for specific book based on Name, or ISBN
void searchLibraryEntry(const vector<string>& library) {
    cout << "Would you like to search by (1) name or (2) ISBN." << endl;
    cout << "Enter the numeric choice: " << endl;
    int choice;
    cin >> choice;
    cin.ignore();

    bool found = false;
    if (choice == 1) {
        cout << "Enter the book name: " << endl;
    } else if (choice == 2) {
        cout << "Enter the book 13-digit ISBN (with dashes): " << endl;
    } else {
        cout << "Invalid search by choice option." << endl;
        return;
    }

    string search;
    getline(cin, search);

    cout << "The Following Are Your Search Results" << endl;
    cout << "-------------------------------------" << endl;

    string foundItem;
    string foundISBN;

    for (int i = library.size() - 1; i >= 0; i--) {
        string item = library[i];

        if (choice == 1) {
            // Search by name
            size_t namePos = item.find(" --- ");
            if (namePos != string::npos) {
                string itemName = item.substr(0, namePos);
                if (itemName == search) {
                    found = true;
                    foundItem = item;
                    size_t isbnPos = item.rfind(" --- ");
                    if (isbnPos != string::npos) {
                        foundISBN = item.substr(isbnPos + 5);
                    }
                    cout << item << endl;
                }
            }
        } else if (choice == 2) {
            // Search by ISBN
            size_t isbnPos = item.find(" --- ");
            size_t isbnEndPos = item.rfind(" --- ");
            if (isbnPos != string::npos) {
                string itemIsbnEND = item.substr(isbnEndPos);
                string itemISBN = item.substr(isbnPos + 5, isbnEndPos - (isbnPos + 5)); 

                
                
                if (itemISBN == search) {
                    found = true;
                    foundItem = item;
                    size_t namePos = item.find(" --- ");
                    if (namePos != string::npos) {
                        foundItem = item.substr(0, namePos);
                    }
                    cout << item << endl;
                }
            }
        }

        // Exit the loop if you found a match
        if (found) {
            break;
        }
    }

    if (!found) {
        cout << "No matching entry found in the library." << endl;
    }
}
