#ifndef BOOK_BST_H
#define BOOK_BST_H

#include "../entities/Book.h"
#include <vector>

class BookBST {
private:
    struct BookNode {
        Book* data;
        BookNode* left;
        BookNode* right;
        int height;
        
        BookNode(Book* book) : data(book), left(nullptr), right(nullptr), height(1) {}
    };
    
    BookNode* root;
    int nodeCount;
    
    // Private helper methods
    BookNode* insert(BookNode* node, Book* book);
    BookNode* search(BookNode* node, string isbn);
    BookNode* deleteNode(BookNode* node, string isbn);
    BookNode* findMin(BookNode* node);
    void inorderTraversal(BookNode* node, vector<Book*>& result);
    void destroy(BookNode* node);
    
    // AVL balancing methods
    int getHeight(BookNode* node);
    int getBalance(BookNode* node);
    BookNode* rotateRight(BookNode* y);
    BookNode* rotateLeft(BookNode* x);

public:
    BookBST();
    ~BookBST();
    
    void insert(Book* book);
    Book* search(string isbn);
    bool remove(string isbn);
    vector<Book*> getAllBooksSorted();
    int getCount() const;
    void clear();
    bool isEmpty() const;
};

#endif
