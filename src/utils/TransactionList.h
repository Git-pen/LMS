#ifndef TRANSACTION_LIST_H
#define TRANSACTION_LIST_H

#include "../entities/Transaction.h"
#include <vector>
#include <unordered_map>

class TransactionList {
private:
    struct TransactionNode {
        Transaction* data;
        TransactionNode* prev;
        TransactionNode* next;
        
        TransactionNode(Transaction* t) : data(t), prev(nullptr), next(nullptr) {}
    };
    
    TransactionNode* head;
    TransactionNode* tail;
    int count;
    
    unordered_map<string, vector<Transaction*>> userTransIndex;
    unordered_map<string, vector<Transaction*>> bookTransIndex;

public:
    TransactionList();
    ~TransactionList();
    
    void append(Transaction* trans);
    void prepend(Transaction* trans);
    vector<Transaction*> getAll();
    vector<Transaction*> getByUserID(string userID);
    vector<Transaction*> getByISBN(string isbn);
    vector<Transaction*> getRecent(int n);
    int getCount() const;
    void clear();
};

#endif
