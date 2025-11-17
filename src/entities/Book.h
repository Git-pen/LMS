#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
private:
    string isbn;
    string title;
    string author;
    int quantity;
    int availableCopies;

public:
    Book();
    Book(string isbn, string title, string author, int quantity);
    
    // Getters
    string getISBN() const;
    string getTitle() const;
    string getAuthor() const;
    bool getAvailability() const;
    int getQuantity() const;
    int getAvailableCopies() const;
    
    // Setters
    void setQuantity(int qty);
    void setAvailableCopies(int copies);
    
    // Business Logic
    bool borrowBook();
    bool returnBook();
    
    // Utility
    string toString() const;
    string toFileString() const;
    static Book fromFileString(string line);
};

#endif
