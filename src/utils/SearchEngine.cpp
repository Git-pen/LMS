#include "SearchEngine.h"
#include <cctype>
#include <sstream>

SearchEngine::SearchEngine() : bookTree(nullptr) {}

void SearchEngine::setBookTree(BookBST* tree) {
    bookTree = tree;
}

string SearchEngine::normalize(string str) {
    string result = "";
    for (char c : str) {
        result += tolower(c);
    }
    
    while (!result.empty() && result[0] == ' ') {
        result = result.substr(1);
    }
    while (!result.empty() && result[result.length() - 1] == ' ') {
        result = result.substr(0, result.length() - 1);
    }
    
    return result;
}

vector<string> SearchEngine::tokenize(string str) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    
    while (ss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

void SearchEngine::buildIndices() {
    if (bookTree == nullptr) return;
    
    titleIndex.clear();
    authorIndex.clear();
    
    vector<Book*> allBooks = bookTree->getAllBooksSorted();
    for (Book* book : allBooks) {
        addBookToIndex(book);
    }
}

void SearchEngine::addBookToIndex(Book* book) {
    string lowerTitle = normalize(book->getTitle());
    string lowerAuthor = normalize(book->getAuthor());
    
    titleIndex.insert({lowerTitle, book});
    vector<string> titleWords = tokenize(lowerTitle);
    for (const string& word : titleWords) {
        titleIndex.insert({word, book});
    }
    
    authorIndex.insert({lowerAuthor, book});
    vector<string> authorWords = tokenize(lowerAuthor);
    for (const string& word : authorWords) {
        authorIndex.insert({word, book});
    }
}

void SearchEngine::removeBookFromIndex(Book* book) {
    string lowerTitle = normalize(book->getTitle());
    string lowerAuthor = normalize(book->getAuthor());
    
    auto titleRange = titleIndex.equal_range(lowerTitle);
    for (auto it = titleRange.first; it != titleRange.second; ) {
        if (it->second->getISBN() == book->getISBN()) {
            it = titleIndex.erase(it);
        } else {
            ++it;
        }
    }
    
    vector<string> titleWords = tokenize(lowerTitle);
    for (const string& word : titleWords) {
        auto wordRange = titleIndex.equal_range(word);
        for (auto it = wordRange.first; it != wordRange.second; ) {
            if (it->second->getISBN() == book->getISBN()) {
                it = titleIndex.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    auto authorRange = authorIndex.equal_range(lowerAuthor);
    for (auto it = authorRange.first; it != authorRange.second; ) {
        if (it->second->getISBN() == book->getISBN()) {
            it = authorIndex.erase(it);
        } else {
            ++it;
        }
    }
    
    vector<string> authorWords = tokenize(lowerAuthor);
    for (const string& word : authorWords) {
        auto wordRange = authorIndex.equal_range(word);
        for (auto it = wordRange.first; it != wordRange.second; ) {
            if (it->second->getISBN() == book->getISBN()) {
                it = authorIndex.erase(it);
            } else {
                ++it;
            }
        }
    }
}

vector<Book*> SearchEngine::searchByTitle(string title) {
    string normalized = normalize(title);
    vector<Book*> results;
    
    auto range = titleIndex.equal_range(normalized);
    for (auto it = range.first; it != range.second; ++it) {
        results.push_back(it->second);
    }
    
    sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());
    
    return results;
}

vector<Book*> SearchEngine::searchByAuthor(string author) {
    string normalized = normalize(author);
    vector<Book*> results;
    
    auto range = authorIndex.equal_range(normalized);
    for (auto it = range.first; it != range.second; ++it) {
        results.push_back(it->second);
    }
    
    sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());
    
    return results;
}

vector<Book*> SearchEngine::searchByKeyword(string keyword) {
    vector<Book*> titleResults = searchByTitle(keyword);
    vector<Book*> authorResults = searchByAuthor(keyword);
    
    vector<Book*> combined;
    combined.insert(combined.end(), titleResults.begin(), titleResults.end());
    combined.insert(combined.end(), authorResults.begin(), authorResults.end());
    
    sort(combined.begin(), combined.end());
    combined.erase(unique(combined.begin(), combined.end()), combined.end());
    
    return combined;
}

Book* SearchEngine::searchByISBN(string isbn) {
    if (bookTree == nullptr) return nullptr;
    return bookTree->search(isbn);
}

vector<Book*> SearchEngine::searchAvailableBooks() {
    if (bookTree == nullptr) return vector<Book*>();
    
    vector<Book*> allBooks = bookTree->getAllBooksSorted();
    vector<Book*> available;
    
    for (Book* book : allBooks) {
        if (book->getAvailability()) {
            available.push_back(book);
        }
    }
    
    return available;
}

void SearchEngine::rebuildIndices() {
    buildIndices();
}

void SearchEngine::clear() {
    titleIndex.clear();
    authorIndex.clear();
}
