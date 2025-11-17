#include "LibraryManager.h"
#include "../Config.h"
#include <iostream>
#include <iomanip>

LibraryManager* LibraryManager::instance = nullptr;

LibraryManager::LibraryManager() : authManager(nullptr) {
    initializeDataStructures();
}

LibraryManager::~LibraryManager() {
    delete bookTree;
    delete userMap;
    delete transactionList;
    delete searchEngine;
}

void LibraryManager::initializeDataStructures() {
    bookTree = new BookBST();
    userMap = new UserHashMap();
    transactionList = new TransactionList();
    searchEngine = new SearchEngine();
    searchEngine->setBookTree(bookTree);
}

LibraryManager* LibraryManager::getInstance() {
    if (instance == nullptr) {
        instance = new LibraryManager();
    }
    return instance;
}

void LibraryManager::setAuthManager(AuthManager* auth) {
    authManager = auth;
}

// ============ ADMIN OPERATIONS - BOOK MANAGEMENT ============

bool LibraryManager::addBook(string isbn, string title, string author, int quantity) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    Book* existing = bookTree->search(isbn);
    if (existing != nullptr) {
        cout << "Error: Book with ISBN " << isbn << " already exists.\n";
        return false;
    }
    
    Book* newBook = new Book(isbn, title, author, quantity);
    bookTree->insert(newBook);
    searchEngine->addBookToIndex(newBook);
    
    return true;
}

bool LibraryManager::removeBook(string isbn) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return false;
    }
    
    if (book->getAvailableCopies() < book->getQuantity()) {
        cout << "Error: Cannot remove book. Some copies are currently borrowed.\n";
        return false;
    }
    
    searchEngine->removeBookFromIndex(book);
    return bookTree->remove(isbn);
}

bool LibraryManager::updateBookDetails(string isbn, string newTitle, string newAuthor) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return false;
    }
    
    searchEngine->removeBookFromIndex(book);
    
    Book* updatedBook = new Book(isbn, newTitle, newAuthor, book->getQuantity());
    updatedBook->setAvailableCopies(book->getAvailableCopies());
    
    bookTree->remove(isbn);
    bookTree->insert(updatedBook);
    searchEngine->addBookToIndex(updatedBook);
    
    return true;
}

bool LibraryManager::updateBookQuantity(string isbn, int newQuantity) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return false;
    }
    
    int borrowed = book->getQuantity() - book->getAvailableCopies();
    if (newQuantity < borrowed) {
        cout << "Error: Cannot set quantity below borrowed copies (" << borrowed << ").\n";
        return false;
    }
    
    book->setQuantity(newQuantity);
    book->setAvailableCopies(newQuantity - borrowed);
    
    return true;
}

// ============ ADMIN OPERATIONS - USER MANAGEMENT ============

vector<User*> LibraryManager::getAllUsers() {
    if (!authManager || !authManager->isAdmin()) {
        return vector<User*>();
    }
    return userMap->getAllUsers();
}

void LibraryManager::displayAllUsers() {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return;
    }
    
    vector<User*> users = userMap->getAllUsers();
    
    if (users.empty()) {
        cout << "No users registered.\n";
        return;
    }
    
    cout << "\n" << string(100, '=') << "\n";
    cout << left << setw(10) << "User ID"
         << setw(20) << "Username"
         << setw(25) << "Full Name"
         << setw(15) << "Borrowed"
         << setw(10) << "Status" << "\n";
    cout << string(100, '=') << "\n";
    
    for (User* user : users) {
        cout << left << setw(10) << user->getUserID()
             << setw(20) << user->getUsername()
             << setw(25) << user->getFullName()
             << setw(15) << (to_string(user->getBorrowedCount()) + "/" + to_string(MAX_BORROW_LIMIT))
             << setw(10) << (user->isAccountActive() ? "Active" : "Inactive") << "\n";
    }
    
    cout << string(100, '=') << "\n";
    cout << "Total Users: " << users.size() << "\n";
}

bool LibraryManager::removeUser(string userID) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    if (user->getBorrowedCount() > 0) {
        cout << "Error: Cannot remove user with borrowed books.\n";
        return false;
    }
    
    return userMap->remove(userID);
}

bool LibraryManager::deactivateUser(string userID) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    user->setActive(false);
    return true;
}

bool LibraryManager::activateUser(string userID) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return false;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    user->setActive(true);
    return true;
}

void LibraryManager::displayUserDetails(string userID) {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return;
    }
    
    User* user = userMap->searchByID(userID);
    if (user == nullptr) {
        cout << "Error: User not found.\n";
        return;
    }
    
    cout << "\n" << string(60, '=') << "\n";
    cout << user->toString() << "\n";
    cout << string(60, '=') << "\n";
    
    set<string> borrowedISBNs = user->getBorrowedISBNs();
    if (!borrowedISBNs.empty()) {
        cout << "\nBorrowed Books:\n";
        for (const string& isbn : borrowedISBNs) {
            Book* book = bookTree->search(isbn);
            if (book != nullptr) {
                cout << "  - " << book->getTitle() << " (" << isbn << ")\n";
            }
        }
    }
}

// ============ ADMIN OPERATIONS - REPORTS & STATISTICS ============

void LibraryManager::displayAllTransactions() {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return;
    }
    
    vector<Transaction*> transactions = transactionList->getRecent(RECENT_TRANSACTIONS_COUNT);
    
    if (transactions.empty()) {
        cout << "No transactions recorded.\n";
        return;
    }
    
    cout << "\n" << string(120, '=') << "\n";
    cout << "RECENT TRANSACTIONS (Last " << RECENT_TRANSACTIONS_COUNT << ")\n";
    cout << string(120, '=') << "\n";
    
    cout << left << setw(12) << "Trans ID"
         << setw(15) << "User"
         << setw(35) << "Book"
         << setw(10) << "Type"
         << setw(20) << "Timestamp" << "\n";
    cout << string(120, '=') << "\n";
    
    for (Transaction* trans : transactions) {
        cout << left << setw(12) << trans->getTransactionID()
             << setw(15) << trans->getUserName()
             << setw(35) << trans->getBookTitle().substr(0, 32)
             << setw(10) << trans->getType()
             << setw(20) << trans->getTimestamp() << "\n";
    }
    
    cout << string(120, '=') << "\n";
}

void LibraryManager::displaySystemStatistics() {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return;
    }
    
    int totalBooks = bookTree->getCount();
    int availableBooks = 0;
    int borrowedBooks = 0;
    
    vector<Book*> allBooks = bookTree->getAllBooksSorted();
    for (Book* book : allBooks) {
        availableBooks += book->getAvailableCopies();
        borrowedBooks += (book->getQuantity() - book->getAvailableCopies());
    }
    
    int totalUsers = userMap->getCount();
    int totalTransactions = transactionList->getCount();
    
    cout << "\n" << string(60, '=') << "\n";
    cout << "LIBRARY SYSTEM STATISTICS\n";
    cout << string(60, '=') << "\n";
    cout << "Total Book Titles: " << totalBooks << "\n";
    cout << "Total Book Copies: " << (availableBooks + borrowedBooks) << "\n";
    cout << "  - Available: " << availableBooks << "\n";
    cout << "  - Borrowed: " << borrowedBooks << "\n";
    cout << "Total Users: " << totalUsers << "\n";
    cout << "Total Transactions: " << totalTransactions << "\n";
    cout << string(60, '=') << "\n";
}

void LibraryManager::displayBorrowingReport() {
    if (!authManager || !authManager->isAdmin()) {
        cout << "Access Denied: Admin privileges required.\n";
        return;
    }
    
    vector<User*> users = userMap->getAllUsers();
    
    cout << "\n" << string(80, '=') << "\n";
    cout << "BORROWING REPORT\n";
    cout << string(80, '=') << "\n";
    
    for (User* user : users) {
        if (user->getBorrowedCount() > 0) {
            cout << user->getFullName() << " (" << user->getUserID() << "): "
                 << user->getBorrowedCount() << " books\n";
            
            set<string> borrowedISBNs = user->getBorrowedISBNs();
            for (const string& isbn : borrowedISBNs) {
                Book* book = bookTree->search(isbn);
                if (book != nullptr) {
                    cout << "  - " << book->getTitle() << "\n";
                }
            }
            cout << "\n";
        }
    }
    
    cout << string(80, '=') << "\n";
}

// ============ USER OPERATIONS - BROWSE & SEARCH ============

void LibraryManager::displayAvailableBooks() {
    vector<Book*> books = searchEngine->searchAvailableBooks();
    
    if (books.empty()) {
        cout << "No available books.\n";
        return;
    }
    
    cout << "\n" << string(120, '=') << "\n";
    cout << "AVAILABLE BOOKS\n";
    cout << string(120, '=') << "\n";
    
    cout << left << setw(20) << "ISBN"
         << setw(40) << "Title"
         << setw(30) << "Author"
         << setw(10) << "Available" << "\n";
    cout << string(120, '=') << "\n";
    
    for (Book* book : books) {
        cout << left << setw(20) << book->getISBN()
             << setw(40) << book->getTitle().substr(0, 37)
             << setw(30) << book->getAuthor().substr(0, 27)
             << setw(10) << book->getAvailableCopies() << "\n";
    }
    
    cout << string(120, '=') << "\n";
    cout << "Total Available Books: " << books.size() << "\n";
}

void LibraryManager::displayAllBooks() {
    vector<Book*> books = bookTree->getAllBooksSorted();
    
    if (books.empty()) {
        cout << "No books in library.\n";
        return;
    }
    
    cout << "\n" << string(130, '=') << "\n";
    cout << "ALL BOOKS\n";
    cout << string(130, '=') << "\n";
    
    cout << left << setw(20) << "ISBN"
         << setw(40) << "Title"
         << setw(30) << "Author"
         << setw(10) << "Total"
         << setw(10) << "Available" << "\n";
    cout << string(130, '=') << "\n";
    
    for (Book* book : books) {
        cout << left << setw(20) << book->getISBN()
             << setw(40) << book->getTitle().substr(0, 37)
             << setw(30) << book->getAuthor().substr(0, 27)
             << setw(10) << book->getQuantity()
             << setw(10) << book->getAvailableCopies() << "\n";
    }
    
    cout << string(130, '=') << "\n";
    cout << "Total Books: " << books.size() << "\n";
}

vector<Book*> LibraryManager::searchBooks(string query, string type) {
    if (type == "title") {
        return searchEngine->searchByTitle(query);
    } else if (type == "author") {
        return searchEngine->searchByAuthor(query);
    } else if (type == "isbn") {
        Book* book = searchEngine->searchByISBN(query);
        if (book != nullptr) {
            return vector<Book*>{book};
        }
        return vector<Book*>();
    }
    return searchEngine->searchByKeyword(query);
}

void LibraryManager::displayBookDetails(string isbn) {
    Book* book = bookTree->search(isbn);
    
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return;
    }
    
    cout << "\n" << string(60, '=') << "\n";
    cout << book->toString() << "\n";
    cout << string(60, '=') << "\n";
}

// ============ USER OPERATIONS - BORROW & RETURN ============

bool LibraryManager::borrowBook(string isbn) {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return false;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    if (!currentUser->canBorrow()) {
        if (currentUser->getBorrowedCount() >= MAX_BORROW_LIMIT) {
            cout << "Error: You have reached the borrowing limit (" << MAX_BORROW_LIMIT << " books).\n";
        } else {
            cout << "Error: Your account is inactive.\n";
        }
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return false;
    }
    
    if (!book->getAvailability()) {
        cout << "Error: Book is not available.\n";
        return false;
    }
    
    if (currentUser->hasBorrowedBook(isbn)) {
        cout << "Error: You have already borrowed this book.\n";
        return false;
    }
    
    book->borrowBook();
    currentUser->addBorrowedBook(isbn);
    
    Transaction* trans = new Transaction(
        currentUser->getUserID(),
        isbn,
        "BORROW",
        currentUser->getFullName(),
        book->getTitle()
    );
    transactionList->append(trans);
    
    cout << "Success: Book borrowed successfully!\n";
    return true;
}

bool LibraryManager::returnBook(string isbn) {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return false;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    if (!currentUser->hasBorrowedBook(isbn)) {
        cout << "Error: You have not borrowed this book.\n";
        return false;
    }
    
    Book* book = bookTree->search(isbn);
    if (book == nullptr) {
        cout << "Error: Book not found.\n";
        return false;
    }
    
    book->returnBook();
    currentUser->removeBorrowedBook(isbn);
    
    Transaction* trans = new Transaction(
        currentUser->getUserID(),
        isbn,
        "RETURN",
        currentUser->getFullName(),
        book->getTitle()
    );
    transactionList->append(trans);
    
    cout << "Success: Book returned successfully!\n";
    return true;
}

void LibraryManager::displayMyBorrowedBooks() {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return;
    }
    
    set<string> borrowedISBNs = currentUser->getBorrowedISBNs();
    
    if (borrowedISBNs.empty()) {
        cout << "You have no borrowed books.\n";
        return;
    }
    
    cout << "\n" << string(100, '=') << "\n";
    cout << "MY BORROWED BOOKS\n";
    cout << string(100, '=') << "\n";
    
    cout << left << setw(20) << "ISBN"
         << setw(50) << "Title"
         << setw(30) << "Author" << "\n";
    cout << string(100, '=') << "\n";
    
    for (const string& isbn : borrowedISBNs) {
        Book* book = bookTree->search(isbn);
        if (book != nullptr) {
            cout << left << setw(20) << book->getISBN()
                 << setw(50) << book->getTitle().substr(0, 47)
                 << setw(30) << book->getAuthor().substr(0, 27) << "\n";
        }
    }
    
    cout << string(100, '=') << "\n";
    cout << "Total: " << borrowedISBNs.size() << "/" << MAX_BORROW_LIMIT << " books\n";
}

void LibraryManager::displayMyTransactionHistory() {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return;
    }
    
    vector<Transaction*> transactions = transactionList->getByUserID(currentUser->getUserID());
    
    if (transactions.empty()) {
        cout << "No transaction history.\n";
        return;
    }
    
    cout << "\n" << string(120, '=') << "\n";
    cout << "MY TRANSACTION HISTORY\n";
    cout << string(120, '=') << "\n";
    
    cout << left << setw(12) << "Trans ID"
         << setw(40) << "Book"
         << setw(10) << "Type"
         << setw(20) << "Timestamp" << "\n";
    cout << string(120, '=') << "\n";
    
    for (Transaction* trans : transactions) {
        cout << left << setw(12) << trans->getTransactionID()
             << setw(40) << trans->getBookTitle().substr(0, 37)
             << setw(10) << trans->getType()
             << setw(20) << trans->getTimestamp() << "\n";
    }
    
    cout << string(120, '=') << "\n";
    cout << "Total Transactions: " << transactions.size() << "\n";
}

// ============ USER OPERATIONS - PROFILE ============

void LibraryManager::displayMyProfile() {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return;
    }
    
    cout << "\n" << string(60, '=') << "\n";
    cout << currentUser->toString() << "\n";
    cout << string(60, '=') << "\n";
}

bool LibraryManager::updateMyContact(string email, string phone) {
    if (!authManager || !authManager->isUser()) {
        cout << "Access Denied: Please login as user.\n";
        return false;
    }
    
    User* currentUser = authManager->getCurrentUser();
    if (currentUser == nullptr) {
        cout << "Error: User not found.\n";
        return false;
    }
    
    currentUser->updateContact(email, phone);
    return true;
}

// ============ DATA PERSISTENCE ============

bool LibraryManager::saveAllData() {
    FileHandler::createDirectory(DATA_DIR);
    
    bool success = true;
    success &= FileHandler::saveBooks(bookTree, BOOKS_FILE);
    success &= FileHandler::saveUsers(userMap, USERS_FILE);
    success &= FileHandler::saveTransactions(transactionList, TRANSACTIONS_FILE);
    
    return success;
}

bool LibraryManager::loadAllData() {
    bool booksLoaded = FileHandler::loadBooks(bookTree, BOOKS_FILE);
    bool usersLoaded = FileHandler::loadUsers(userMap, USERS_FILE);
    bool transLoaded = FileHandler::loadTransactions(transactionList, TRANSACTIONS_FILE);
    
    if (booksLoaded) {
        searchEngine->buildIndices();
    }
    
    return booksLoaded || usersLoaded || transLoaded;
}

void LibraryManager::initializeSampleData() {
    addBook("978-0-13-468599-1", "The C++ Programming Language", "Bjarne Stroustrup", 5);
    addBook("978-0-321-56384-2", "Effective C++", "Scott Meyers", 3);
    addBook("978-0-262-03384-8", "Introduction to Algorithms", "Thomas Cormen", 4);
    addBook("978-0-201-35088-5", "Data Structures and Algorithms", "Alfred Aho", 3);
    addBook("978-0-672-32692-7", "Data Structures Using C++", "D.S. Malik", 4);
    addBook("978-0-201-63361-0", "Design Patterns", "Gang of Four", 2);
    addBook("978-0-596-00927-5", "Head First Design Patterns", "Eric Freeman", 3);
    addBook("978-0-201-89683-1", "The Art of Computer Programming Vol 1", "Donald Knuth", 2);
    addBook("978-0-13-110362-7", "The C Programming Language", "Brian Kernighan", 5);
    addBook("978-0-134-68599-4", "Clean Code", "Robert Martin", 4);
    
    saveAllData();
}

// ============ UTILITY ============

int LibraryManager::getTotalBooks() {
    return bookTree->getCount();
}

int LibraryManager::getTotalAvailableBooks() {
    int count = 0;
    vector<Book*> books = bookTree->getAllBooksSorted();
    for (Book* book : books) {
        if (book->getAvailability()) {
            count++;
        }
    }
    return count;
}

int LibraryManager::getTotalUsers() {
    return userMap->getCount();
}

int LibraryManager::getTotalTransactions() {
    return transactionList->getCount();
}
