#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "BookBST.h"
#include <map>
#include <vector>
#include <algorithm>

class SearchEngine {
private:
    multimap<string, Book*> titleIndex;
    multimap<string, Book*> authorIndex;
    BookBST* bookTree;
    
    string normalize(string str);
    vector<string> tokenize(string str);

public:
    SearchEngine();
    void setBookTree(BookBST* tree);
    void buildIndices();
    void addBookToIndex(Book* book);
    void removeBookFromIndex(Book* book);
    
    vector<Book*> searchByTitle(string title);
    vector<Book*> searchByAuthor(string author);
    vector<Book*> searchByKeyword(string keyword);
    Book* searchByISBN(string isbn);
    vector<Book*> searchAvailableBooks();
    
    void rebuildIndices();
    void clear();
};

#endif
