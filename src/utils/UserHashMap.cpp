#include "UserHashMap.h"
#include "../Config.h"

UserHashMap::UserHashMap() : UserHashMap(INITIAL_HASH_TABLE_SIZE) {}

UserHashMap::UserHashMap(int size) : tableSize(size), count(0) {
    userIDTable = new HashNode*[tableSize];
    usernameTable = new HashNode*[tableSize];
    
    for (int i = 0; i < tableSize; i++) {
        userIDTable[i] = nullptr;
        usernameTable[i] = nullptr;
    }
}

UserHashMap::~UserHashMap() {
    clear();
    delete[] userIDTable;
    delete[] usernameTable;
}

int UserHashMap::hashFunction(string key) {
    unsigned long hash = 0;
    for (size_t i = 0; i < key.length(); i++) {
        hash = hash * 31 + key[i];
    }
    return hash % tableSize;
}

void UserHashMap::insert(User* user) {
    int idIndex = hashFunction(user->getUserID());
    HashNode* idNode = new HashNode(user->getUserID(), user);
    
    if (userIDTable[idIndex] == nullptr) {
        userIDTable[idIndex] = idNode;
    } else {
        idNode->next = userIDTable[idIndex];
        userIDTable[idIndex] = idNode;
    }
    
    int usernameIndex = hashFunction(user->getUsername());
    HashNode* usernameNode = new HashNode(user->getUsername(), user);
    
    if (usernameTable[usernameIndex] == nullptr) {
        usernameTable[usernameIndex] = usernameNode;
    } else {
        usernameNode->next = usernameTable[usernameIndex];
        usernameTable[usernameIndex] = usernameNode;
    }
    
    count++;
    
    if ((double)count / tableSize > MAX_LOAD_FACTOR) {
        resize();
    }
}

User* UserHashMap::searchByID(string userID) {
    int index = hashFunction(userID);
    HashNode* current = userIDTable[index];
    
    while (current != nullptr) {
        if (current->key == userID) {
            return current->value;
        }
        current = current->next;
    }
    
    return nullptr;
}

User* UserHashMap::searchByUsername(string username) {
    int index = hashFunction(username);
    HashNode* current = usernameTable[index];
    
    while (current != nullptr) {
        if (current->key == username) {
            return current->value;
        }
        current = current->next;
    }
    
    return nullptr;
}

bool UserHashMap::remove(string userID) {
    User* user = searchByID(userID);
    if (user == nullptr) return false;
    
    string username = user->getUsername();
    
    int idIndex = hashFunction(userID);
    HashNode* current = userIDTable[idIndex];
    HashNode* prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == userID) {
            if (prev == nullptr) {
                userIDTable[idIndex] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }
    
    int usernameIndex = hashFunction(username);
    current = usernameTable[usernameIndex];
    prev = nullptr;
    
    while (current != nullptr) {
        if (current->key == username) {
            if (prev == nullptr) {
                usernameTable[usernameIndex] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            break;
        }
        prev = current;
        current = current->next;
    }
    
    delete user;
    count--;
    return true;
}

vector<User*> UserHashMap::getAllUsers() {
    vector<User*> result;
    
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = userIDTable[i];
        while (current != nullptr) {
            result.push_back(current->value);
            current = current->next;
        }
    }
    
    return result;
}

int UserHashMap::getCount() const {
    return count;
}

bool UserHashMap::existsUsername(string username) {
    return searchByUsername(username) != nullptr;
}

void UserHashMap::clear() {
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = userIDTable[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp->value;
            delete temp;
        }
        userIDTable[i] = nullptr;
    }
    
    for (int i = 0; i < tableSize; i++) {
        HashNode* current = usernameTable[i];
        while (current != nullptr) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
        usernameTable[i] = nullptr;
    }
    
    count = 0;
}

void UserHashMap::resize() {
    int newSize = tableSize * 2;
    HashNode** newIDTable = new HashNode*[newSize];
    HashNode** newUsernameTable = new HashNode*[newSize];
    
    for (int i = 0; i < newSize; i++) {
        newIDTable[i] = nullptr;
        newUsernameTable[i] = nullptr;
    }
    
    int oldSize = tableSize;
    tableSize = newSize;
    
    for (int i = 0; i < oldSize; i++) {
        HashNode* current = userIDTable[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            
            int newIndex = hashFunction(current->key);
            current->next = newIDTable[newIndex];
            newIDTable[newIndex] = current;
            
            current = next;
        }
    }
    
    for (int i = 0; i < oldSize; i++) {
        HashNode* current = usernameTable[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            
            int newIndex = hashFunction(current->key);
            current->next = newUsernameTable[newIndex];
            newUsernameTable[newIndex] = current;
            
            current = next;
        }
    }
    
    delete[] userIDTable;
    delete[] usernameTable;
    
    userIDTable = newIDTable;
    usernameTable = newUsernameTable;
}
