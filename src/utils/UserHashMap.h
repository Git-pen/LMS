#ifndef USER_HASHMAP_H
#define USER_HASHMAP_H

#include "../entities/User.h"
#include <vector>

class UserHashMap {
private:
    struct HashNode {
        string key;
        User* value;
        HashNode* next;
        
        HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
    };
    
    int tableSize;
    HashNode** userIDTable;
    HashNode** usernameTable;
    int count;
    
    int hashFunction(string key);
    void resize();

public:
    UserHashMap();
    UserHashMap(int size);
    ~UserHashMap();
    
    void insert(User* user);
    User* searchByID(string userID);
    User* searchByUsername(string username);
    bool remove(string userID);
    vector<User*> getAllUsers();
    int getCount() const;
    bool existsUsername(string username);
    void clear();
};

#endif
