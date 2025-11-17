#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "BookBST.h"
#include "UserHashMap.h"
#include "TransactionList.h"
#include <string>
#include <vector>

class FileHandler {
public:
    static bool fileExists(string filename);
    static bool createFile(string filename);
    static bool createDirectory(string dirname);
    static vector<string> readLines(string filename);
    static bool writeLines(string filename, vector<string> lines);
    static bool appendLine(string filename, string line);
    
    static bool saveBooks(BookBST* bookTree, string filename);
    static bool loadBooks(BookBST* bookTree, string filename);
    static bool saveUsers(UserHashMap* userMap, string filename);
    static bool loadUsers(UserHashMap* userMap, string filename);
    static bool saveTransactions(TransactionList* transList, string filename);
    static bool loadTransactions(TransactionList* transList, string filename);
    
    static vector<string> split(string str, char delimiter);
    static string trim(string str);
};

#endif
