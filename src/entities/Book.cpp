#include "Book.h"
#include "../Config.h"
#include <sstream>
#include <iomanip>

Book::Book() : isbn(""), title(""), author(""), quantity(0), availableCopies(0) {}

Book::Book(string isbn, string title, string author, int quantity) 
    : isbn(isbn), title(title), author(author), quantity(quantity), availableCopies(quantity) {}

string Book::getISBN() const { return isbn; }
string Book::getTitle() const { return title; }
string Book::getAuthor() const { return author; }
bool Book::getAvailability() const { return availableCopies > 0; }
int Book::getQuantity() const { return quantity; }
int Book::getAvailableCopies() const { return availableCopies; }

void Book::setQuantity(int qty) { 
    quantity = qty; 
    if (availableCopies > quantity) {
        availableCopies = quantity;
    }
}

void Book::setAvailableCopies(int copies) { 
    if (copies <= quantity && copies >= 0) {
        availableCopies = copies; 
    }
}

bool Book::borrowBook() {
    if (availableCopies > 0) {
        availableCopies--;
        return true;
    }
    return false;
}

bool Book::returnBook() {
    if (availableCopies < quantity) {
        availableCopies++;
        return true;
    }
    return false;
}

string Book::toString() const {
    stringstream ss;
    ss << "ISBN: " << isbn << "\n"
       << "Title: " << title << "\n"
       << "Author: " << author << "\n"
       << "Quantity: " << quantity << "\n"
       << "Available: " << availableCopies << "\n"
       << "Status: " << (availableCopies > 0 ? "Available" : "Not Available");
    return ss.str();
}

string Book::toFileString() const {
    stringstream ss;
    ss << isbn << CSV_DELIMITER 
       << title << CSV_DELIMITER 
       << author << CSV_DELIMITER 
       << quantity << CSV_DELIMITER 
       << availableCopies;
    return ss.str();
}

Book Book::fromFileString(string line) {
    stringstream ss(line);
    string isbn, title, author, qtyStr, availStr;
    
    getline(ss, isbn, CSV_DELIMITER);
    getline(ss, title, CSV_DELIMITER);
    getline(ss, author, CSV_DELIMITER);
    getline(ss, qtyStr, CSV_DELIMITER);
    getline(ss, availStr, CSV_DELIMITER);
    
    Book book(isbn, title, author, stoi(qtyStr));
    book.setAvailableCopies(stoi(availStr));
    
    return book;
}
