#include "Transaction.h"
#include "../Config.h"
#include <sstream>
#include <iomanip>
#include <ctime>

int Transaction::transactionCounter = 1;

Transaction::Transaction() 
    : transactionID(""), userID(""), isbn(""), type(""), 
      timestamp(""), userName(""), bookTitle("") {}

Transaction::Transaction(string userID, string isbn, string type, string userName, string bookTitle)
    : userID(userID), isbn(isbn), type(type), userName(userName), bookTitle(bookTitle) {
    transactionID = generateID();
    timestamp = generateTimestamp();
}

string Transaction::getTransactionID() const { return transactionID; }
string Transaction::getUserID() const { return userID; }
string Transaction::getISBN() const { return isbn; }
string Transaction::getType() const { return type; }
string Transaction::getTimestamp() const { return timestamp; }
string Transaction::getUserName() const { return userName; }
string Transaction::getBookTitle() const { return bookTitle; }

void Transaction::setTransactionID(string id) { transactionID = id; }

string Transaction::toString() const {
    stringstream ss;
    ss << "Transaction ID: " << transactionID << "\n"
       << "User: " << userName << " (" << userID << ")\n"
       << "Book: " << bookTitle << " (" << isbn << ")\n"
       << "Type: " << type << "\n"
       << "Timestamp: " << timestamp;
    return ss.str();
}

string Transaction::toFileString() const {
    stringstream ss;
    ss << transactionID << CSV_DELIMITER 
       << userID << CSV_DELIMITER 
       << isbn << CSV_DELIMITER 
       << type << CSV_DELIMITER 
       << timestamp << CSV_DELIMITER 
       << userName << CSV_DELIMITER 
       << bookTitle;
    return ss.str();
}

Transaction Transaction::fromFileString(string line) {
    stringstream ss(line);
    string transID, userID, isbn, type, timestamp, userName, bookTitle;
    
    getline(ss, transID, CSV_DELIMITER);
    getline(ss, userID, CSV_DELIMITER);
    getline(ss, isbn, CSV_DELIMITER);
    getline(ss, type, CSV_DELIMITER);
    getline(ss, timestamp, CSV_DELIMITER);
    getline(ss, userName, CSV_DELIMITER);
    getline(ss, bookTitle, CSV_DELIMITER);
    
    Transaction trans;
    trans.transactionID = transID;
    trans.userID = userID;
    trans.isbn = isbn;
    trans.type = type;
    trans.timestamp = timestamp;
    trans.userName = userName;
    trans.bookTitle = bookTitle;
    
    // Update counter
    if (transID.length() > 1 && transID[0] == 'T') {
        int num = stoi(transID.substr(1));
        if (num >= transactionCounter) {
            transactionCounter = num + 1;
        }
    }
    
    return trans;
}

string Transaction::generateTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    
    stringstream ss;
    ss << (1900 + ltm->tm_year) << "-"
       << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-"
       << setfill('0') << setw(2) << ltm->tm_mday << " "
       << setfill('0') << setw(2) << ltm->tm_hour << ":"
       << setfill('0') << setw(2) << ltm->tm_min << ":"
       << setfill('0') << setw(2) << ltm->tm_sec;
    
    return ss.str();
}

string Transaction::generateID() {
    stringstream ss;
    ss << "T" << setfill('0') << setw(4) << transactionCounter++;
    return ss.str();
}
