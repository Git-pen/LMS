#include "AuthManager.h"
#include "../Config.h"

AuthManager* AuthManager::instance = nullptr;

AuthManager::AuthManager() : userMap(nullptr), currentUser(nullptr), currentRole(NONE) {}

AuthManager* AuthManager::getInstance() {
    if (instance == nullptr) {
        instance = new AuthManager();
    }
    return instance;
}

void AuthManager::setUserMap(UserHashMap* map) {
    userMap = map;
}

bool AuthManager::loginAsAdmin(string username, string password) {
    if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
        currentRole = ADMIN;
        currentUser = nullptr;
        return true;
    }
    return false;
}

bool AuthManager::loginAsUser(string username, string password) {
    if (userMap == nullptr) return false;
    
    User* user = userMap->searchByUsername(username);
    
    if (user != nullptr && user->authenticate(password)) {
        if (user->isAccountActive()) {
            currentUser = user;
            currentRole = USER;
            return true;
        }
    }
    
    return false;
}

bool AuthManager::registerUser(string username, string password, string fullName, string email, string phone) {
    if (userMap == nullptr) return false;
    
    if (!isUsernameAvailable(username)) {
        return false;
    }
    
    if (!validatePassword(password)) {
        return false;
    }
    
    User* newUser = new User(username, password, fullName, email, phone);
    userMap->insert(newUser);
    
    return true;
}

void AuthManager::logout() {
    currentUser = nullptr;
    currentRole = NONE;
}

bool AuthManager::isAdmin() const {
    return currentRole == ADMIN;
}

bool AuthManager::isUser() const {
    return currentRole == USER;
}

bool AuthManager::isLoggedIn() const {
    return currentRole != NONE;
}

User* AuthManager::getCurrentUser() const {
    return currentUser;
}

string AuthManager::getCurrentUserID() const {
    if (currentRole == ADMIN) {
        return ADMIN_ID;
    } else if (currentUser != nullptr) {
        return currentUser->getUserID();
    }
    return "";
}

AuthManager::Role AuthManager::getCurrentRole() const {
    return currentRole;
}

bool AuthManager::isUsernameAvailable(string username) const {
    if (userMap == nullptr) return false;
    return !userMap->existsUsername(username);
}

bool AuthManager::validatePassword(string password) const {
    return password.length() >= MIN_PASSWORD_LENGTH;
}

bool AuthManager::validateEmail(string email) const {
    return email.find('@') != string::npos;
}
