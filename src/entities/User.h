#ifndef USER_H
#define USER_H

#include <string>
#include <set>
using namespace std;

class User {
private:
    string userID;
    string username;
    string password;
    string fullName;
    string email;
    string phoneNumber;
    set<string> borrowedISBNs;
    int borrowedCount;
    bool isActive;
    
    static int userCounter;

public:
    User();
    User(string username, string password, string fullName, string email, string phone);
    
    // Authentication
    bool authenticate(string password) const;
    static string hashPassword(string password);
    
    // Getters
    string getUserID() const;
    string getUsername() const;
    string getFullName() const;
    string getEmail() const;
    string getPhoneNumber() const;
    int getBorrowedCount() const;
    set<string> getBorrowedISBNs() const;
    bool isAccountActive() const;
    
    // Setters
    void setActive(bool status);
    void updateContact(string email, string phone);
    void setUserID(string id);
    
    // Business Logic
    bool canBorrow() const;
    void addBorrowedBook(string isbn);
    void removeBorrowedBook(string isbn);
    bool hasBorrowedBook(string isbn) const;
    
    // Utility
    string toString() const;
    string toFileString() const;
    static User fromFileString(string line);
    static string generateUserID();
};

#endif
