#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Book {
public:
    string title;
    string author;
    int year;
    int quantity;

    Book(string t, string a, int y, int q) : title(t), author(a), year(y), quantity(q) {}

    void displayDetails() {
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Year: " << year << endl;
        cout << "Quantity: " << quantity << endl;
    }
};

class Library {
private:
    vector<Book> books;
    string fileName;

public:
    Library(string file) : fileName(file) {
        loadBooksFromFile();
    }

    void addBook(Book book) {
        books.push_back(book);
        saveBooksToFile();
    }

    void removeBook(string title) {
        for (int i = 0; i < books.size(); i++) {
            if (books[i].title == title) {
                books.erase(books.begin() + i);
                break;
            }
        }
        saveBooksToFile();
    }

    void displayBooks() {
        for (const auto& book : books) {
            book.displayDetails();
            cout << "=============================" << endl;
        }
    }

    void saveBooksToFile() {
        ofstream outFile(fileName);
        for (const auto& book : books) {
            outFile << book.title << "," << book.author << "," << book.year << "," << book.quantity << endl;
        }
        outFile.close();
    }

    void loadBooksFromFile() {
        ifstream inFile(fileName);
        if (inFile) {
            string line;
            while (getline(inFile, line)) {
                size_t pos1 = line.find(",");
                size_t pos2 = line.find(",", pos1 + 1);
                size_t pos3 = line.find(",", pos2 + 1);
                string title = line.substr(0, pos1);
                string author = line.substr(pos1 + 1, pos2 - pos1 - 1);
                int year = stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
                int quantity = stoi(line.substr(pos3 + 1));
                books.emplace_back(title, author, year, quantity);
            }
            inFile.close();
        }
    }
};

int main() {
    Library library("books.txt");
    int choice;
    do {
        cout << "Library Management System" << endl;
        cout << "1. Add Book" << endl;
        cout << "2. Remove Book" << endl;
        cout << "3. Display All Books" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string title, author;
                int year, quantity;
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                cout << "Enter Year: ";
                cin >> year;
                cout << "Enter Quantity: ";
                cin >> quantity;
                Book book(title, author, year, quantity);
                library.addBook(book);
                cout << "Book added successfully!" << endl;
                break;
            }
            case 2: {
                string title;
                cout << "Enter Book Title to remove: ";
                cin.ignore();
                getline(cin, title);
                library.removeBook(title);
                cout << "Book removed successfully!" << endl;
                break;
            }
            case 3:
                cout << "All Books in Library:" << endl;
                library.displayBooks();
                break;
            case 4:
                cout << "Exiting Library Management System. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
