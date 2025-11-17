#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include "../entities/User.h"
#include "../utils/UserHashMap.h"
#include <string>

class AuthManager {
public:
    enum Role { NONE, ADMIN, USER };

private:
    static AuthManager* instance;
    UserHashMap* userMap;
    User* currentUser;
    Role currentRole;
    
    AuthManager();

public:
    static AuthManager* getInstance();
    void setUserMap(UserHashMap* map);
    
    bool loginAsAdmin(string username, string password);
    bool loginAsUser(string username, string password);
    bool registerUser(string username, string password, string fullName, string email, string phone);
    void logout();
    
    bool isAdmin() const;
    bool isUser() const;
    bool isLoggedIn() const;
    User* getCurrentUser() const;
    string getCurrentUserID() const;
    Role getCurrentRole() const;
    
    bool isUsernameAvailable(string username) const;
    bool validatePassword(string password) const;
    bool validateEmail(string email) const;
};

#endif
