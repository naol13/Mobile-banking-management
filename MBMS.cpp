#include <iostream>
#include <string>

#include <fstream> // For file handling
#include <limits>///for input vakidation
#include <vector>

using namespace std;

// Define the AccountNode structure
struct AccountNode {
    string account_holder_full_name;
    string phone_number;
    string account_id;
    int  account_number;
    double balance;
    AccountNode *next;

    AccountNode(string fullName, string phone, string id, int accNumber, double initialBalance)
        : account_holder_full_name(fullName), phone_number(phone), account_id(id), account_number(accNumber), balance(initialBalance), next(nullptr) {}
};


// Function declarations
void createAccount(AccountNode *&head);
void searchAccount(AccountNode *head);
void depositMoney(AccountNode *head);
void withdrawMoney(AccountNode *head);
bool login(AccountNode *head, int*currentAccNumber);
void editAccount(AccountNode *head);
void deleteAccount(AccountNode *&head);
void displayAllAccounts(AccountNode *head);
void saveAccountsToFile(AccountNode *head, const string &filename);
void loadAccountsFromFile(AccountNode *&head, const string &filename);

// Stub function definitions to avoid linker errors
void createAccount(AccountNode *&head) {
    cout << "createAccount function called." << endl;
}

void searchAccount(AccountNode *head) {
    cout << "searchAccount function called." << endl;
}

void depositMoney(AccountNode *head) {
    cout << "depositMoney function called." << endl;
}

void withdrawMoney(AccountNode *head) {
    cout << "withdrawMoney function called." << endl;
}

bool login(AccountNode *head, int* currentAccNumber) {
    cout << "login function called." << endl;
    return false;
}

void editAccount(AccountNode *head) {
    cout << "editAccount function called." << endl;
}

void deleteAccount(AccountNode *&head) {
    cout << "deleteAccount function called." << endl;
}

void displayAllAccounts(AccountNode *head) {
    cout << "displayAllAccounts function called." << endl;
}

void saveAccountsToFile(AccountNode *head, const string &filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }

    AccountNode *current = head;
    while (current != nullptr) {
        outFile << current->account_holder_full_name << endl;
        outFile << current->phone_number << endl;
        outFile << current->account_id << endl;
        outFile << current->account_number << endl;
        outFile << current->balance << endl;
        current = current->next;
    }
    outFile.close();
    cout << "Accounts saved to " << filename << endl;
}

// Function to load accounts from a file
void loadAccountsFromFile(AccountNode *&head, const string &filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file for reading: " << filename << endl;
        return;
    }

    string fullName, phone, id;
    int accNumber;
    double balance;

    while (getline(inFile, fullName)) {
        getline(inFile, phone);
        getline(inFile, id);
        inFile >> accNumber;
        inFile >> balance;
        inFile.ignore(); // Ignore newline character

        AccountNode *newAccount = new AccountNode(fullName, phone, id, accNumber, balance);
        newAccount->next = head;
        head = newAccount;
    }
    inFile.close();
    cout << "Accounts loaded from " << filename << endl;
}

int main()
{
    AccountNode *head = nullptr;
    int choice;
    bool isLoggedIn = false;
    int currentAccNumber = -1;
    string filename = "accounts.txt";

    // Load accounts from file at startup
    loadAccountsFromFile(head, filename);

    cout << "Welcome to Mobile Banking Management System!" << endl;

    while (true) {
        cout << "\nMenu" << endl;
        cout << "1. Create account" << endl;
        cout << "2. Log in" << endl;
        cout << "3. Deposit money" << endl;
        cout << "4. Withdraw money" << endl;
        cout << "5. Search account" << endl;
        cout << "6. Edit account" << endl;
        cout << "7. Delete account" << endl;
        cout << "8. Display all accounts" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(head);
                break;
            case 2:
                isLoggedIn = login(head, &currentAccNumber);
                break;
            case 3:
                if (!isLoggedIn) {
                    cout << "Please log in first." << endl;
                    break;
                }
                depositMoney(head);
                break;
            case 4:
                if (!isLoggedIn) {
                    cout << "Please log in first." << endl;
                    break;
                }
                withdrawMoney(head);
                break;
            case 5:
                searchAccount(head);
                break;
            case 6:
                editAccount(head);
                break;
            case 7:
                deleteAccount(head);
                break;
            case 8:
                displayAllAccounts(head);
                break;
            case 9:
                cout << "Thank you for using the mobile banking system. Goodbye!" << endl;
                // Save accounts to file before exiting
                saveAccountsToFile(head, filename);
                while (head != nullptr) {
                    AccountNode *temp = head;
                    head = head->next;
                    delete temp;
                }
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }
    
        
    return 0;
}
