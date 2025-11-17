#include "FileHandler.h"
#include "../Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #define mkdir(dir, mode) _mkdir(dir)
    #define access _access
    #define F_OK 0
#else
    #include <sys/stat.h>
    #include <unistd.h>
#endif

bool FileHandler::fileExists(string filename) {
    #ifdef _WIN32
        return (_access(filename.c_str(), F_OK) == 0);
    #else
        struct stat buffer;
        return (stat(filename.c_str(), &buffer) == 0);
    #endif
}

bool FileHandler::createFile(string filename) {
    ofstream file(filename);
    if (file.is_open()) {
        file.close();
        return true;
    }
    return false;
}

bool FileHandler::createDirectory(string dirname) {
    #ifdef _WIN32
        return _mkdir(dirname.c_str()) == 0 || fileExists(dirname);
    #else
        return mkdir(dirname.c_str(), 0777) == 0 || fileExists(dirname);
    #endif
}

vector<string> FileHandler::readLines(string filename) {
    vector<string> lines;
    ifstream file(filename);
    
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty() && line != "\r") {
                lines.push_back(line);
            }
        }
        file.close();
    }
    
    return lines;
}

bool FileHandler::writeLines(string filename, vector<string> lines) {
    ofstream file(filename);
    
    if (file.is_open()) {
        for (const string& line : lines) {
            file << line << "\n";
        }
        file.close();
        return true;
    }
    
    return false;
}

bool FileHandler::appendLine(string filename, string line) {
    ofstream file(filename, ios::app);
    
    if (file.is_open()) {
        file << line << "\n";
        file.close();
        return true;
    }
    
    return false;
}

bool FileHandler::saveBooks(BookBST* bookTree, string filename) {
    vector<Book*> books = bookTree->getAllBooksSorted();
    vector<string> lines;
    
    lines.push_back("ISBN,Title,Author,Quantity,AvailableCopies");
    
    for (Book* book : books) {
        lines.push_back(book->toFileString());
    }
    
    return writeLines(filename, lines);
}

bool FileHandler::loadBooks(BookBST* bookTree, string filename) {
    if (!fileExists(filename)) {
        return false;
    }
    
    vector<string> lines = readLines(filename);
    
    if (lines.empty()) return false;
    
    for (size_t i = 1; i < lines.size(); i++) {
        try {
            Book* book = new Book(Book::fromFileString(lines[i]));
            bookTree->insert(book);
        } catch (...) {
            continue;
        }
    }
    
    return true;
}

bool FileHandler::saveUsers(UserHashMap* userMap, string filename) {
    vector<User*> users = userMap->getAllUsers();
    vector<string> lines;
    
    lines.push_back("UserID,Username,Password,FullName,Email,Phone,BorrowedCount,BorrowedISBNs,IsActive");
    
    for (User* user : users) {
        lines.push_back(user->toFileString());
    }
    
    return writeLines(filename, lines);
}

bool FileHandler::loadUsers(UserHashMap* userMap, string filename) {
    if (!fileExists(filename)) {
        return false;
    }
    
    vector<string> lines = readLines(filename);
    
    if (lines.empty()) return false;
    
    for (size_t i = 1; i < lines.size(); i++) {
        try {
            User* user = new User(User::fromFileString(lines[i]));
            userMap->insert(user);
        } catch (...) {
            continue;
        }
    }
    
    return true;
}

bool FileHandler::saveTransactions(TransactionList* transList, string filename) {
    vector<Transaction*> transactions = transList->getAll();
    vector<string> lines;
    
    lines.push_back("TransactionID,UserID,ISBN,Type,Timestamp,UserName,BookTitle");
    
    for (Transaction* trans : transactions) {
        lines.push_back(trans->toFileString());
    }
    
    return writeLines(filename, lines);
}

bool FileHandler::loadTransactions(TransactionList* transList, string filename) {
    if (!fileExists(filename)) {
        return false;
    }
    
    vector<string> lines = readLines(filename);
    
    if (lines.empty()) return false;
    
    for (size_t i = 1; i < lines.size(); i++) {
        try {
            Transaction* trans = new Transaction(Transaction::fromFileString(lines[i]));
            transList->append(trans);
        } catch (...) {
            continue;
        }
    }
    
    return true;
}

vector<string> FileHandler::split(string str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

string FileHandler::trim(string str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}
