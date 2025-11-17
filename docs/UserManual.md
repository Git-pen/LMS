# Library Management System - User Manual

## Table of Contents
1. [Introduction](#introduction)
2. [System Requirements](#system-requirements)
3. [Installation](#installation)
4. [Getting Started](#getting-started)
5. [Administrator Guide](#administrator-guide)
6. [User Guide](#user-guide)
7. [Troubleshooting](#troubleshooting)
8. [FAQs](#faqs)

---

## 1. Introduction

The Library Management System is a comprehensive console-based application built with advanced data structures including:
- **AVL Binary Search Tree** for efficient book storage and retrieval
- **Hash Map** for O(1) user authentication
- **Doubly Linked List** for transaction history
- **Multi-Map Indices** for fast search operations

### Key Features
- Role-based access control (Administrator and User)
- Secure authentication with password hashing
- Real-time book availability tracking
- Comprehensive transaction history
- Advanced search capabilities
- File-based data persistence

---

## 2. System Requirements

### Hardware
- Minimum 512 MB RAM
- 50 MB free disk space

### Software
- Linux/Unix operating system
- G++ compiler (C++11 or later)
- Make utility

---

## 3. Installation

### Step 1: Extract Files
```bash
cd /path/to/LibraryManagementSystem
```

### Step 2: Compile
```bash
make
```

### Step 3: Run
```bash
./library_system
```

Or simply:
```bash
make run
```

### Clean Build (if needed)
```bash
make clean
make
```

---

## 4. Getting Started

### First Time Setup

When you run the system for the first time, sample data will be automatically loaded including:
- 10 sample books
- Default administrator account

### Default Administrator Credentials
```
Username: admin
Password: admin123
```

**IMPORTANT:** These credentials are hardcoded for demonstration purposes.

### Main Menu
Upon starting, you'll see:
```
1. Admin Login
2. User Login
3. Register New User
4. Exit
```

---

## 5. Administrator Guide

### 5.1 Logging In

1. Select option `1` from main menu
2. Enter username: `admin`
3. Enter password: `admin123`

### 5.2 Book Management

#### Adding a New Book
1. Navigate to: **Book Management → Add New Book**
2. Enter book details:
   - **ISBN**: Unique identifier (e.g., 978-0-123-45678-9)
   - **Title**: Book title
   - **Author**: Author name
   - **Quantity**: Number of copies
3. System adds book to BST and updates search indices

#### Removing a Book
1. Navigate to: **Book Management → Remove Book**
2. Enter ISBN of book to remove
3. Confirm action
4. **Note**: Cannot remove books that are currently borrowed

#### Updating Book Quantity
1. Navigate to: **Book Management → Update Book Quantity**
2. Enter ISBN
3. Enter new quantity
4. **Note**: New quantity must be ≥ currently borrowed copies

#### Viewing Books
- **View All Books**: Shows all books with total and available copies
- **View Available Books**: Shows only books with available copies

### 5.3 User Management

#### Viewing All Users
1. Navigate to: **User Management → View All Users**
2. Displays table with:
   - User ID
   - Username
   - Full Name
   - Borrowed books count
   - Account status

#### Viewing User Details
1. Navigate to: **User Management → View User Details**
2. Enter User ID
3. View complete profile and borrowed books

#### Deactivating/Activating Users
1. Navigate to: **User Management → Deactivate User** (or Activate)
2. Enter User ID
3. Deactivated users cannot borrow books

#### Removing Users
1. Navigate to: **User Management → Remove User**
2. Enter User ID
3. Confirm action
4. **Note**: Cannot remove users with borrowed books

### 5.4 Reports & Analytics

#### System Statistics
Displays:
- Total book titles
- Total book copies (available + borrowed)
- Total users
- Total transactions

#### All Transactions
- Shows last 20 transactions
- Displays: Transaction ID, User, Book, Type, Timestamp

#### Borrowing Report
- Lists all users with borrowed books
- Shows which books each user has borrowed

### 5.5 Search Operations

#### Search by Title
1. Enter partial or complete title
2. System searches using multi-map index (O(log n + k))
3. Returns all matching books

#### Search by Author
1. Enter author name
2. Returns all books by that author

#### Search by ISBN
1. Enter exact ISBN
2. Uses BST search (O(log n))
3. Returns specific book details

---

## 6. User Guide

### 6.1 Registration

1. Select option `3` from main menu
2. Enter registration details:
   - **Username**: Unique, will be used for login
   - **Password**: Minimum 6 characters
   - **Confirm Password**: Must match
   - **Full Name**: Your full name
   - **Email**: Email address
   - **Phone Number**: Contact number
3. Registration successful! You can now login

### 6.2 Logging In

1. Select option `2` from main menu
2. Enter your username and password
3. Access user menu

### 6.3 Browsing Books

#### View All Available Books
1. Navigate to: **Browse Books → View All Available Books**
2. Displays books with copies available
3. Shows: ISBN, Title, Author, Available Copies

#### View Book Details
1. Navigate to: **Browse Books → View Book Details**
2. Enter ISBN
3. View complete book information

### 6.4 Searching Books

Similar to admin search operations:
- Search by Title
- Search by Author  
- Search by ISBN

**Tip**: You can enter partial matches for title/author searches

### 6.5 Borrowing Books

1. Navigate to: **My Books → Borrow a Book**
2. View available books
3. Enter ISBN of book to borrow
4. System checks:
   - Book availability
   - Your borrow limit (max 5 books)
   - No duplicate borrows
5. Success! Book is borrowed and transaction recorded

**Borrow Limit**: Each user can borrow maximum 5 books simultaneously

### 6.6 Returning Books

1. Navigate to: **My Books → Return a Book**
2. View your borrowed books
3. Enter ISBN of book to return
4. System:
   - Updates book availability
   - Removes from your borrowed list
   - Records return transaction

### 6.7 Viewing History

#### My Borrowed Books
- Shows all currently borrowed books
- Displays: ISBN, Title, Author
- Shows count (e.g., 3/5 books)

#### My Transaction History
- Complete history of borrows and returns
- Shows: Transaction ID, Book, Type, Timestamp

### 6.8 Profile Management

#### View Profile
Displays:
- User ID
- Username
- Full Name
- Email and Phone
- Borrowed books count
- Account status

#### Update Contact Information
1. Navigate to: **My Profile → Update Contact Information**
2. Enter new email
3. Enter new phone number
4. Changes saved immediately

---

## 7. Troubleshooting

### Problem: "Command 'make' not found"
**Solution**: Install build-essential
```bash
sudo apt-get install build-essential
```

### Problem: Compilation errors
**Solution**: Ensure G++ supports C++11
```bash
g++ --version  # Should be 4.8 or higher
```

### Problem: "Cannot open data files"
**Solution**: Ensure `data/` directory exists
```bash
mkdir -p data
```

### Problem: "Password not hidden during input"
**Solution**: This is expected on some systems. Password is still secure.

### Problem: Forgot user password
**Solution**: 
1. Login as admin
2. Deactivate and reactivate the user (or remove if no borrowed books)
3. User can re-register with same details

### Problem: Cannot borrow books
**Possible causes**:
1. Reached borrow limit (5 books) - return some books
2. Account inactive - contact administrator
3. Book not available - check availability
4. Already borrowed this book - check "My Borrowed Books"

### Problem: Data not saving
**Solution**: Check write permissions on `data/` directory
```bash
chmod 755 data/
```

---

## 8. FAQs

**Q: How many books can I borrow?**  
A: Maximum 5 books at a time.

**Q: Can I borrow the same book twice?**  
A: No, the system prevents duplicate borrows.

**Q: What happens if I lose my password?**  
A: Contact the administrator. They can deactivate your account, and you can re-register.

**Q: Can the administrator borrow books?**  
A: No, the admin account is for management only. Create a user account to borrow.

**Q: How long can I keep borrowed books?**  
A: Currently, there's no time limit. Future versions will implement due dates.

**Q: Can I see who borrowed a specific book?**  
A: Only administrators can view this information through transaction history.

**Q: What data structures are used?**  
A:
- **Books**: AVL-balanced Binary Search Tree (O(log n) operations)
- **Users**: Hash Map with chaining (O(1) lookups)
- **Transactions**: Doubly Linked List (O(1) append, O(1) indexed lookups)
- **Search**: Multi-Map indices (O(log n + k) searches)

**Q: Is my password secure?**  
A: Passwords are hashed before storage. Never stored in plain text.

**Q: Can I change my username?**  
A: No, usernames are permanent identifiers. You can update other profile information.

**Q: What happens when the program exits?**  
A: All data is automatically saved to files in the `data/` directory.

**Q: Can multiple users access the system simultaneously?**  
A: This is a single-user console application. Only one session at a time.

**Q: How do I backup my data?**  
A: Copy the entire `data/` directory:
```bash
cp -r data/ data_backup/
```

**Q: Can I import my own book data?**  
A: Yes! Edit `data/books.txt` following the CSV format:
```
ISBN,Title,Author,Quantity,AvailableCopies
978-X-XXX-XXXXX-X,Book Title,Author Name,5,5
```

---

## Support

For additional help or to report issues:
1. Check this manual
2. Review the TestCases.md document
3. Examine the code comments in source files

---

**Version**: 1.0  
**Last Updated**: 2024  
**Built with**: C++11, Custom Data Structures (BST, HashMap, Doubly LL)
