#include "User.h"
#include "../Config.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

int User::userCounter = 1;

User::User() : userID(""), username(""), password(""), fullName(""), 
               email(""), phoneNumber(""), borrowedCount(0), isActive(true) {}

User::User(string username, string password, string fullName, string email, string phone)
    : username(username), password(hashPassword(password)), fullName(fullName), 
      email(email), phoneNumber(phone), borrowedCount(0), isActive(true) {
    userID = generateUserID();
}

bool User::authenticate(string password) const {
    return this->password == hashPassword(password);
}

string User::hashPassword(string password) {
    unsigned long hash = 5381;
    for (char c : password) {
        hash = ((hash << 5) + hash) + c;
    }
    return to_string(hash);
}

string User::getUserID() const { return userID; }
string User::getUsername() const { return username; }
string User::getFullName() const { return fullName; }
string User::getEmail() const { return email; }
string User::getPhoneNumber() const { return phoneNumber; }
int User::getBorrowedCount() const { return borrowedCount; }
set<string> User::getBorrowedISBNs() const { return borrowedISBNs; }
bool User::isAccountActive() const { return isActive; }

void User::setActive(bool status) { isActive = status; }
void User::updateContact(string email, string phone) {
    this->email = email;
    this->phoneNumber = phone;
}
void User::setUserID(string id) { userID = id; }

bool User::canBorrow() const {
    return isActive && borrowedCount < MAX_BORROW_LIMIT;
}

void User::addBorrowedBook(string isbn) {
    if (borrowedISBNs.find(isbn) == borrowedISBNs.end()) {
        borrowedISBNs.insert(isbn);
        borrowedCount++;
    }
}

void User::removeBorrowedBook(string isbn) {
    if (borrowedISBNs.find(isbn) != borrowedISBNs.end()) {
        borrowedISBNs.erase(isbn);
        borrowedCount--;
    }
}

bool User::hasBorrowedBook(string isbn) const {
    return borrowedISBNs.find(isbn) != borrowedISBNs.end();
}

string User::toString() const {
    stringstream ss;
    ss << "User ID: " << userID << "\n"
       << "Username: " << username << "\n"
       << "Full Name: " << fullName << "\n"
       << "Email: " << email << "\n"
       << "Phone: " << phoneNumber << "\n"
       << "Borrowed Books: " << borrowedCount << "/" << MAX_BORROW_LIMIT << "\n"
       << "Status: " << (isActive ? "Active" : "Inactive");
    return ss.str();
}

string User::toFileString() const {
    stringstream ss;
    ss << userID << CSV_DELIMITER 
       << username << CSV_DELIMITER 
       << password << CSV_DELIMITER 
       << fullName << CSV_DELIMITER 
       << email << CSV_DELIMITER 
       << phoneNumber << CSV_DELIMITER 
       << borrowedCount << CSV_DELIMITER;
    
    // Add borrowed ISBNs
    int count = 0;
    for (const string& isbn : borrowedISBNs) {
        if (count > 0) ss << LIST_DELIMITER;
        ss << isbn;
        count++;
    }
    
    ss << CSV_DELIMITER << (isActive ? "1" : "0");
    
    return ss.str();
}

User User::fromFileString(string line) {
    stringstream ss(line);
    string userID, username, password, fullName, email, phone, countStr, isbnList, activeStr;
    
    getline(ss, userID, CSV_DELIMITER);
    getline(ss, username, CSV_DELIMITER);
    getline(ss, password, CSV_DELIMITER);
    getline(ss, fullName, CSV_DELIMITER);
    getline(ss, email, CSV_DELIMITER);
    getline(ss, phone, CSV_DELIMITER);
    getline(ss, countStr, CSV_DELIMITER);
    getline(ss, isbnList, CSV_DELIMITER);
    getline(ss, activeStr, CSV_DELIMITER);
    
    User user;
    user.userID = userID;
    user.username = username;
    user.password = password;
    user.fullName = fullName;
    user.email = email;
    user.phoneNumber = phone;
    user.borrowedCount = stoi(countStr);
    user.isActive = (activeStr == "1");
    
    // Parse borrowed ISBNs
    if (!isbnList.empty()) {
        stringstream isbnStream(isbnList);
        string isbn;
        while (getline(isbnStream, isbn, LIST_DELIMITER)) {
            user.borrowedISBNs.insert(isbn);
        }
    }
    
    // Update counter
    if (userID.length() > 1 && userID[0] == 'U') {
        int num = stoi(userID.substr(1));
        if (num >= userCounter) {
            userCounter = num + 1;
        }
    }
    
    return user;
}

string User::generateUserID() {
    stringstream ss;
    ss << "U" << setfill('0') << setw(3) << userCounter++;
    return ss.str();
}
