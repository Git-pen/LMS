#include "management/LibraryManager.h"
#include "management/AuthManager.h"
#include "Config.h"
#include <iostream>
#include <limits>
#include <termios.h>
#include <unistd.h>

using namespace std;

// Global managers
LibraryManager* library = nullptr;
AuthManager* auth = nullptr;

// Utility functions
void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void pressEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

string getInput(string prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

int getIntInput(string prompt) {
    string input;
    int value;
    while (true) {
        cout << prompt;
        getline(cin, input);
        try {
            value = stoi(input);
            return value;
        } catch (...) {
            cout << "Invalid input. Please enter a number.\n";
        }
    }
}

string getPasswordInput(string prompt) {
    string password;
    cout << prompt;
    
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    getline(cin, password);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    cout << "\n";
    
    return password;
}

bool confirmAction(string message) {
    string response = getInput(message + " (y/n): ");
    return (response == "y" || response == "Y" || response == "yes" || response == "Yes");
}

void displayHeader(string title) {
    cout << "\n" << string(80, '=') << "\n";
    cout << "  " << title << "\n";
    cout << string(80, '=') << "\n";
}

void displaySuccess(string message) {
    cout << "\n✓ SUCCESS: " << message << "\n";
}

void displayError(string message) {
    cout << "\n✗ ERROR: " << message << "\n";
}

// Login and Registration
bool handleAdminLogin() {
    clearScreen();
    displayHeader("ADMIN LOGIN");
    
    string username = getInput("Username: ");
    string password = getPasswordInput("Password: ");
    
    if (auth->loginAsAdmin(username, password)) {
        displaySuccess("Admin login successful!");
        pressEnter();
        return true;
    } else {
        displayError("Invalid admin credentials.");
        pressEnter();
        return false;
    }
}

bool handleUserLogin() {
    clearScreen();
    displayHeader("USER LOGIN");
    
    string username = getInput("Username: ");
    string password = getPasswordInput("Password: ");
    
    if (auth->loginAsUser(username, password)) {
        displaySuccess("Login successful! Welcome, " + auth->getCurrentUser()->getFullName() + "!");
        pressEnter();
        return true;
    } else {
        displayError("Invalid credentials or inactive account.");
        pressEnter();
        return false;
    }
}

void handleUserRegistration() {
    clearScreen();
    displayHeader("USER REGISTRATION");
    
    string username = getInput("Username: ");
    
    if (!auth->isUsernameAvailable(username)) {
        displayError("Username already taken.");
        pressEnter();
        return;
    }
    
    string password = getPasswordInput("Password (min 6 characters): ");
    
    if (!auth->validatePassword(password)) {
        displayError("Password must be at least 6 characters.");
        pressEnter();
        return;
    }
    
    string confirmPass = getPasswordInput("Confirm Password: ");
    
    if (password != confirmPass) {
        displayError("Passwords do not match.");
        pressEnter();
        return;
    }
    
    string fullName = getInput("Full Name: ");
    string email = getInput("Email: ");
    string phone = getInput("Phone Number: ");
    
    if (auth->registerUser(username, password, fullName, email, phone)) {
        displaySuccess("Registration successful! You can now login.");
        library->saveAllData();
    } else {
        displayError("Registration failed.");
    }
    
    pressEnter();
}

// Admin Interface Functions
void adminBookManagement() {
    while (true) {
        clearScreen();
        displayHeader("BOOK MANAGEMENT");
        cout << "1. Add New Book\n";
        cout << "2. Remove Book\n";
        cout << "3. Update Book Quantity\n";
        cout << "4. View All Books\n";
        cout << "5. View Available Books\n";
        cout << "6. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                displayHeader("ADD NEW BOOK");
                string isbn = getInput("ISBN: ");
                string title = getInput("Title: ");
                string author = getInput("Author: ");
                int quantity = getIntInput("Quantity: ");
                
                if (library->addBook(isbn, title, author, quantity)) {
                    displaySuccess("Book added successfully!");
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                displayHeader("REMOVE BOOK");
                string isbn = getInput("ISBN: ");
                
                if (confirmAction("Are you sure you want to remove this book?")) {
                    if (library->removeBook(isbn)) {
                        displaySuccess("Book removed successfully!");
                        library->saveAllData();
                    }
                }
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                displayHeader("UPDATE BOOK QUANTITY");
                string isbn = getInput("ISBN: ");
                int newQty = getIntInput("New Quantity: ");
                
                if (library->updateBookQuantity(isbn, newQty)) {
                    displaySuccess("Quantity updated successfully!");
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 4: {
                clearScreen();
                library->displayAllBooks();
                pressEnter();
                break;
            }
            case 5: {
                clearScreen();
                library->displayAvailableBooks();
                pressEnter();
                break;
            }
            case 6:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void adminUserManagement() {
    while (true) {
        clearScreen();
        displayHeader("USER MANAGEMENT");
        cout << "1. View All Users\n";
        cout << "2. View User Details\n";
        cout << "3. Deactivate User\n";
        cout << "4. Activate User\n";
        cout << "5. Remove User\n";
        cout << "6. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                library->displayAllUsers();
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                string userID = getInput("User ID: ");
                library->displayUserDetails(userID);
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                string userID = getInput("User ID to deactivate: ");
                if (library->deactivateUser(userID)) {
                    displaySuccess("User deactivated successfully!");
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 4: {
                clearScreen();
                string userID = getInput("User ID to activate: ");
                if (library->activateUser(userID)) {
                    displaySuccess("User activated successfully!");
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 5: {
                clearScreen();
                string userID = getInput("User ID to remove: ");
                if (confirmAction("Are you sure you want to remove this user?")) {
                    if (library->removeUser(userID)) {
                        displaySuccess("User removed successfully!");
                        library->saveAllData();
                    }
                }
                pressEnter();
                break;
            }
            case 6:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void adminReportsAnalytics() {
    while (true) {
        clearScreen();
        displayHeader("REPORTS & ANALYTICS");
        cout << "1. System Statistics\n";
        cout << "2. All Transactions\n";
        cout << "3. Borrowing Report\n";
        cout << "4. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                library->displaySystemStatistics();
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                library->displayAllTransactions();
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                library->displayBorrowingReport();
                pressEnter();
                break;
            }
            case 4:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void adminSearchOperations() {
    while (true) {
        clearScreen();
        displayHeader("SEARCH OPERATIONS");
        cout << "1. Search Book by Title\n";
        cout << "2. Search Book by Author\n";
        cout << "3. Search Book by ISBN\n";
        cout << "4. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                string query = getInput("Enter title: ");
                vector<Book*> results = library->searchBooks(query, "title");
                
                if (results.empty()) {
                    cout << "No books found.\n";
                } else {
                    cout << "\nSearch Results:\n";
                    for (Book* book : results) {
                        cout << "\n" << book->toString() << "\n";
                        cout << string(60, '-') << "\n";
                    }
                }
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                string query = getInput("Enter author: ");
                vector<Book*> results = library->searchBooks(query, "author");
                
                if (results.empty()) {
                    cout << "No books found.\n";
                } else {
                    cout << "\nSearch Results:\n";
                    for (Book* book : results) {
                        cout << "\n" << book->toString() << "\n";
                        cout << string(60, '-') << "\n";
                    }
                }
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                string isbn = getInput("Enter ISBN: ");
                library->displayBookDetails(isbn);
                pressEnter();
                break;
            }
            case 4:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void adminInterface() {
    while (true) {
        clearScreen();
        displayHeader("ADMINISTRATOR MENU");
        cout << "1. Book Management\n";
        cout << "2. User Management\n";
        cout << "3. Reports & Analytics\n";
        cout << "4. Search Operations\n";
        cout << "5. Logout\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1:
                adminBookManagement();
                break;
            case 2:
                adminUserManagement();
                break;
            case 3:
                adminReportsAnalytics();
                break;
            case 4:
                adminSearchOperations();
                break;
            case 5:
                library->saveAllData();
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

// User Interface Functions
void userBrowseBooks() {
    while (true) {
        clearScreen();
        displayHeader("BROWSE BOOKS");
        cout << "1. View All Available Books\n";
        cout << "2. View All Books\n";
        cout << "3. View Book Details\n";
        cout << "4. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                library->displayAvailableBooks();
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                library->displayAllBooks();
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                string isbn = getInput("Enter ISBN: ");
                library->displayBookDetails(isbn);
                pressEnter();
                break;
            }
            case 4:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void userSearchBooks() {
    while (true) {
        clearScreen();
        displayHeader("SEARCH BOOKS");
        cout << "1. Search by Title\n";
        cout << "2. Search by Author\n";
        cout << "3. Search by ISBN\n";
        cout << "4. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                string query = getInput("Enter title: ");
                vector<Book*> results = library->searchBooks(query, "title");
                
                if (results.empty()) {
                    cout << "No books found.\n";
                } else {
                    cout << "\nSearch Results:\n";
                    for (Book* book : results) {
                        cout << "\n" << book->toString() << "\n";
                        cout << string(60, '-') << "\n";
                    }
                }
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                string query = getInput("Enter author: ");
                vector<Book*> results = library->searchBooks(query, "author");
                
                if (results.empty()) {
                    cout << "No books found.\n";
                } else {
                    cout << "\nSearch Results:\n";
                    for (Book* book : results) {
                        cout << "\n" << book->toString() << "\n";
                        cout << string(60, '-') << "\n";
                    }
                }
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                string isbn = getInput("Enter ISBN: ");
                library->displayBookDetails(isbn);
                pressEnter();
                break;
            }
            case 4:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void userMyBooks() {
    while (true) {
        clearScreen();
        displayHeader("MY BOOKS");
        cout << "1. Borrow a Book\n";
        cout << "2. Return a Book\n";
        cout << "3. My Borrowed Books\n";
        cout << "4. My Transaction History\n";
        cout << "5. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                library->displayAvailableBooks();
                string isbn = getInput("\nEnter ISBN to borrow: ");
                
                if (library->borrowBook(isbn)) {
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                library->displayMyBorrowedBooks();
                string isbn = getInput("\nEnter ISBN to return: ");
                
                if (library->returnBook(isbn)) {
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 3: {
                clearScreen();
                library->displayMyBorrowedBooks();
                pressEnter();
                break;
            }
            case 4: {
                clearScreen();
                library->displayMyTransactionHistory();
                pressEnter();
                break;
            }
            case 5:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void userMyProfile() {
    while (true) {
        clearScreen();
        displayHeader("MY PROFILE");
        cout << "1. View Profile\n";
        cout << "2. Update Contact Information\n";
        cout << "3. Back to Main Menu\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1: {
                clearScreen();
                library->displayMyProfile();
                pressEnter();
                break;
            }
            case 2: {
                clearScreen();
                displayHeader("UPDATE CONTACT");
                string email = getInput("New Email: ");
                string phone = getInput("New Phone: ");
                
                if (library->updateMyContact(email, phone)) {
                    displaySuccess("Contact information updated!");
                    library->saveAllData();
                }
                pressEnter();
                break;
            }
            case 3:
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

void userInterface() {
    while (true) {
        clearScreen();
        displayHeader("USER MENU");
        User* currentUser = auth->getCurrentUser();
        cout << "Welcome, " << currentUser->getFullName() << "!\n";
        cout << "Books Borrowed: " << currentUser->getBorrowedCount() 
             << "/" << MAX_BORROW_LIMIT << "\n\n";
        
        cout << "1. Browse Books\n";
        cout << "2. Search Books\n";
        cout << "3. My Books\n";
        cout << "4. My Profile\n";
        cout << "5. Logout\n";
        
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1:
                userBrowseBooks();
                break;
            case 2:
                userSearchBooks();
                break;
            case 3:
                userMyBooks();
                break;
            case 4:
                userMyProfile();
                break;
            case 5:
                library->saveAllData();
                return;
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
}

// Main Login Screen
void displayLoginScreen() {
    clearScreen();
    cout << "\n";
    cout << "╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                                                            ║\n";
    cout << "║          LIBRARY MANAGEMENT SYSTEM                         ║\n";
    cout << "║                                                            ║\n";
    cout << "║          Built with Custom Data Structures:                ║\n";
    cout << "║          • AVL Binary Search Tree (Books)                  ║\n";
    cout << "║          • Hash Map (Users)                                ║\n";
    cout << "║          • Doubly Linked List (Transactions)               ║\n";
    cout << "║          • Multi-Map Indices (Fast Search)                 ║\n";
    cout << "║                                                            ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "1. Admin Login\n";
    cout << "2. User Login\n";
    cout << "3. Register New User\n";
    cout << "4. Exit\n";
}

int main() {
    library = LibraryManager::getInstance();
    auth = AuthManager::getInstance();
    
    library->setAuthManager(auth);
    auth->setUserMap(library->getUserMap());
    
    cout << "Loading data...\n";
    if (!library->loadAllData()) {
        cout << "No existing data found. Initializing with sample data...\n";
        library->initializeSampleData();
        cout << "Sample data loaded!\n";
        cout << "\nDefault Admin Credentials:\n";
        cout << "  Username: admin\n";
        cout << "  Password: admin123\n";
        pressEnter();
    } else {
        cout << "Data loaded successfully!\n";
        pressEnter();
    }
    
    while (true) {
        displayLoginScreen();
        int choice = getIntInput("\nEnter choice: ");
        
        switch (choice) {
            case 1:
                if (handleAdminLogin()) {
                    adminInterface();
                    auth->logout();
                }
                break;
                
            case 2:
                if (handleUserLogin()) {
                    userInterface();
                    auth->logout();
                }
                break;
                
            case 3:
                handleUserRegistration();
                break;
                
            case 4:
                library->saveAllData();
                cout << "\nSaving data and exiting...\n";
                cout << "Thank you for using Library Management System!\n";
                return 0;
                
            default:
                displayError("Invalid choice.");
                pressEnter();
        }
    }
    
    return 0;
}
