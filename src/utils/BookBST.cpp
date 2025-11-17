#include "BookBST.h"
#include <algorithm>

BookBST::BookBST() : root(nullptr), nodeCount(0) {}

BookBST::~BookBST() {
    clear();
}

void BookBST::insert(Book* book) {
    root = insert(root, book);
}

BookBST::BookNode* BookBST::insert(BookNode* node, Book* book) {
    if (node == nullptr) {
        nodeCount++;
        return new BookNode(book);
    }
    
    if (book->getISBN() < node->data->getISBN()) {
        node->left = insert(node->left, book);
    } else if (book->getISBN() > node->data->getISBN()) {
        node->right = insert(node->right, book);
    } else {
        return node;
    }
    
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    int balance = getBalance(node);
    
    // Left-Left Case
    if (balance > 1 && book->getISBN() < node->left->data->getISBN()) {
        return rotateRight(node);
    }
    
    // Right-Right Case
    if (balance < -1 && book->getISBN() > node->right->data->getISBN()) {
        return rotateLeft(node);
    }
    
    // Left-Right Case
    if (balance > 1 && book->getISBN() > node->left->data->getISBN()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right-Left Case
    if (balance < -1 && book->getISBN() < node->right->data->getISBN()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

Book* BookBST::search(string isbn) {
    BookNode* result = search(root, isbn);
    return result ? result->data : nullptr;
}

BookBST::BookNode* BookBST::search(BookNode* node, string isbn) {
    if (node == nullptr || node->data->getISBN() == isbn) {
        return node;
    }
    
    if (isbn < node->data->getISBN()) {
        return search(node->left, isbn);
    }
    
    return search(node->right, isbn);
}

bool BookBST::remove(string isbn) {
    int beforeCount = nodeCount;
    root = deleteNode(root, isbn);
    return nodeCount < beforeCount;
}

BookBST::BookNode* BookBST::deleteNode(BookNode* node, string isbn) {
    if (node == nullptr) {
        return node;
    }
    
    if (isbn < node->data->getISBN()) {
        node->left = deleteNode(node->left, isbn);
    } else if (isbn > node->data->getISBN()) {
        node->right = deleteNode(node->right, isbn);
    } else {
        if (node->left == nullptr || node->right == nullptr) {
            BookNode* temp = node->left ? node->left : node->right;
            
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            } else {
                *node = *temp;
            }
            
            delete temp->data;
            delete temp;
            nodeCount--;
        } else {
            BookNode* temp = findMin(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data->getISBN());
        }
    }
    
    if (node == nullptr) {
        return node;
    }
    
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    
    int balance = getBalance(node);
    
    // Left-Left Case
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    
    // Left-Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right-Right Case
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    // Right-Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

BookBST::BookNode* BookBST::findMin(BookNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

vector<Book*> BookBST::getAllBooksSorted() {
    vector<Book*> result;
    inorderTraversal(root, result);
    return result;
}

void BookBST::inorderTraversal(BookNode* node, vector<Book*>& result) {
    if (node == nullptr) return;
    
    inorderTraversal(node->left, result);
    result.push_back(node->data);
    inorderTraversal(node->right, result);
}

int BookBST::getHeight(BookNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int BookBST::getBalance(BookNode* node) {
    if (node == nullptr) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

BookBST::BookNode* BookBST::rotateRight(BookNode* y) {
    BookNode* x = y->left;
    BookNode* T2 = x->right;
    
    x->right = y;
    y->left = T2;
    
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    
    return x;
}

BookBST::BookNode* BookBST::rotateLeft(BookNode* x) {
    BookNode* y = x->right;
    BookNode* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    
    return y;
}

int BookBST::getCount() const {
    return nodeCount;
}

void BookBST::clear() {
    destroy(root);
    root = nullptr;
    nodeCount = 0;
}

void BookBST::destroy(BookNode* node) {
    if (node != nullptr) {
        destroy(node->left);
        destroy(node->right);
        delete node->data;
        delete node;
    }
}

bool BookBST::isEmpty() const {
    return root == nullptr;
}
