#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

class Transaction {
private:
    string transactionID;
    string userID;
    string isbn;
    string type;
    string timestamp;
    string userName;
    string bookTitle;
    
    static int transactionCounter;

public:
    Transaction();
    Transaction(string userID, string isbn, string type, string userName, string bookTitle);
    
    // Getters
    string getTransactionID() const;
    string getUserID() const;
    string getISBN() const;
    string getType() const;
    string getTimestamp() const;
    string getUserName() const;
    string getBookTitle() const;
    
    // Utility
    string toString() const;
    string toFileString() const;
    static Transaction fromFileString(string line);
    static string generateTimestamp();
    static string generateID();
    void setTransactionID(string id);
};

#endif
