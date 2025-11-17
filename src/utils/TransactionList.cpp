#include "TransactionList.h"

TransactionList::TransactionList() : head(nullptr), tail(nullptr), count(0) {}

TransactionList::~TransactionList() {
    clear();
}

void TransactionList::append(Transaction* trans) {
    TransactionNode* newNode = new TransactionNode(trans);
    
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    
    count++;
    
    userTransIndex[trans->getUserID()].push_back(trans);
    bookTransIndex[trans->getISBN()].push_back(trans);
}

void TransactionList::prepend(Transaction* trans) {
    TransactionNode* newNode = new TransactionNode(trans);
    
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    
    count++;
    
    userTransIndex[trans->getUserID()].push_back(trans);
    bookTransIndex[trans->getISBN()].push_back(trans);
}

vector<Transaction*> TransactionList::getAll() {
    vector<Transaction*> result;
    TransactionNode* current = head;
    
    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }
    
    return result;
}

vector<Transaction*> TransactionList::getByUserID(string userID) {
    if (userTransIndex.find(userID) != userTransIndex.end()) {
        return userTransIndex[userID];
    }
    return vector<Transaction*>();
}

vector<Transaction*> TransactionList::getByISBN(string isbn) {
    if (bookTransIndex.find(isbn) != bookTransIndex.end()) {
        return bookTransIndex[isbn];
    }
    return vector<Transaction*>();
}

vector<Transaction*> TransactionList::getRecent(int n) {
    vector<Transaction*> result;
    TransactionNode* current = tail;
    
    int counter = 0;
    while (current != nullptr && counter < n) {
        result.push_back(current->data);
        current = current->prev;
        counter++;
    }
    
    return result;
}

int TransactionList::getCount() const {
    return count;
}

void TransactionList::clear() {
    TransactionNode* current = head;
    while (current != nullptr) {
        TransactionNode* temp = current;
        current = current->next;
        delete temp->data;
        delete temp;
    }
    
    head = tail = nullptr;
    count = 0;
    userTransIndex.clear();
    bookTransIndex.clear();
}
