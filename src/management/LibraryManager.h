#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include "../utils/BookBST.h"
#include "../utils/UserHashMap.h"
#include "../utils/TransactionList.h"
#include "../utils/SearchEngine.h"
#include "../utils/FileHandler.h"
#include "AuthManager.h"
#include <string>
#include <vector>

class LibraryManager {
private:
    static LibraryManager* instance;
    BookBST* bookTree;
    UserHashMap* userMap;
    TransactionList* transactionList;
    SearchEngine* searchEngine;
    AuthManager* authManager;
    
    LibraryManager();
    void initializeDataStructures();

public:
    static LibraryManager* getInstance();
    ~LibraryManager();
    void setAuthManager(AuthManager* auth);
    
    // Admin Operations - Book Management
    bool addBook(string isbn, string title, string author, int quantity);
    bool removeBook(string isbn);
    bool updateBookDetails(string isbn, string newTitle, string newAuthor);
    bool updateBookQuantity(string isbn, int newQuantity);
    
    // Admin Operations - User Management
    vector<User*> getAllUsers();
    void displayAllUsers();
    bool removeUser(string userID);
    bool deactivateUser(string userID);
    bool activateUser(string userID);
    void displayUserDetails(string userID);
    
    // Admin Operations - Reports & Statistics
    void displayAllTransactions();
    void displaySystemStatistics();
    void displayBorrowingReport();
    
    // User Operations - Browse & Search
    void displayAvailableBooks();
    void displayAllBooks();
    vector<Book*> searchBooks(string query, string type);
    void displayBookDetails(string isbn);
    
    // User Operations - Borrow & Return
    bool borrowBook(string isbn);
    bool returnBook(string isbn);
    void displayMyBorrowedBooks();
    void displayMyTransactionHistory();
    
    // User Operations - Profile
    void displayMyProfile();
    bool updateMyContact(string email, string phone);
    
    // Data Persistence
    bool saveAllData();
    bool loadAllData();
    void initializeSampleData();
    
    // Utility
    int getTotalBooks();
    int getTotalAvailableBooks();
    int getTotalUsers();
    int getTotalTransactions();
    
    BookBST* getBookTree() { return bookTree; }
    UserHashMap* getUserMap() { return userMap; }
};

#endif
