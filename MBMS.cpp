#include <iostream>// 
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iomanip> // For fixed and setprecision
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>
#include <sys/stat.h> // For checking file existence
#include <limits> // For numeric_limits
#include <conio.h> // For _getch() to hide password input
#include <algorithm> // For all_of

using namespace std;

// Function to clear the console screen
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Admin class to manage a single admin account
class Admin {
private:
    static bool initialized;
    static string username;
    static string password;
    static string adminFile;

public:
    // Initialize admin account
    static bool initialize(const string& adminUsername, const string& adminPassword) {
        // Check if admin.dat file exists directly using fstream
        ifstream checkFile("admin.dat");
        if (checkFile.good()) {
            checkFile.close();
            cout << "Admin file already exists. Cannot create a new admin." << endl;
            return false; // Admin already exists
        }
        checkFile.close();

        username = adminUsername;
        password = adminPassword;
        initialized = true;
        saveAdminToFile();
        return true;
    }

    // Verify admin credentials
    static bool login(const string& inputUsername, const string& inputPassword) {
        if (!initialized) {
            loadAdminFromFile();
        }

        return (initialized && inputUsername == username && inputPassword == password);
    }

    // Change admin password
    static bool changePassword(const string& oldPassword, const string& newPassword) {
        if (!initialized) {
            loadAdminFromFile();
        }

        if (initialized && oldPassword == password) {
            password = newPassword;
            saveAdminToFile();
            return true;
        }
        return false;
    }

    // Save admin credentials to file
    static void saveAdminToFile() {
        ofstream file("admin.dat");
        if (file.is_open()) {
            file << username << endl;
            file << password << endl;
            file.close();
            cout << "Admin data saved to file successfully." << endl;
        } else {
            cout << "Error: Could not open admin.dat for writing." << endl;
        }
    }

    // Load admin credentials from file
    static bool loadAdminFromFile() {
        ifstream file("admin.dat");
        if (file.is_open()) {
            getline(file, username);
            getline(file, password);
            file.close();
            initialized = true;
            return true;
        }
        return false;
    }

    // Check if admin exists
    static bool isInitialized() {
        if (!initialized) {
            // Try to load from file
            initialized = loadAdminFromFile();
        }
        return initialized;
    }

    // Reset initialization status - used for testing if admin file exists
    static void resetInitialization() {
        initialized = false;
    }
};

// Initialize static members
bool Admin::initialized = false;
string Admin::username = "";
string Admin::password = "";
string Admin::adminFile = "admin.dat";



struct Transaction {
    string type; // "Deposit" or "Withdrawal"
    double amount;
    string timestamp;
    string description; // Added description field for service charges

    // Constructor for regular transactions
    Transaction(const string& t, double a, const string& time)
        : type(t), amount(a), timestamp(time), description(t) {}

    // Constructor for service charge transactions with different parameter names
    Transaction(const string& desc, double a, const string& time, bool isService)
        : type("Service"), amount(a), timestamp(time), description(desc) {
        (void)isService; // Mark parameter as unused to suppress warning
    }
};

struct AccountNode {
    string account_holder_full_name;
    string phone_number;
    string account_id;
    string account_type; // New field for account type
    string sex; // New field for sex
    string status; // New field for status (student/servant)
    int age; // New field for age
    double balance;
    time_t zero_balance_start_time; // Track zero balance start time for savings accounts
    time_t last_interest_time; // Track last interest application time for saving accounts
    time_t account_creation_time; // Track account creation time for zero balance rule
    vector<double> daily_balances; // Track daily balances for interest calculation
    vector<Transaction> transactions;
    AccountNode* next;

    AccountNode(const string& fullName, const string& phone, const string& id, const string& accountTypeVal, const string& sexVal, const string& statusVal, int ageVal, double initialBalance)
        : account_holder_full_name(fullName), phone_number(phone), account_id(id), account_type(accountTypeVal), sex(sexVal), status(statusVal), age(ageVal), balance(initialBalance), zero_balance_start_time(0), last_interest_time(0), account_creation_time(time(0)), next(nullptr) {}
};

int lastAccountID = 0;
string bankServiceAccountID = "BANK-SERVICE-001"; // Special ID for the bank service account

bool fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

string intToString(int num) {
    return to_string(num);
}

string getCurrentTimestamp() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << 1 + ltm->tm_mon << "-"
       << ltm->tm_mday << " "
       << ltm->tm_hour << ":"
       << ltm->tm_min << ":"
       << ltm->tm_sec;
    return ss.str();
}

int findHighestAccountID(AccountNode* head) {
    int highestID = 0;
    AccountNode* current = head;
    while (current != nullptr) {
        try {
            int currentID = stoi(current->account_id);
            if (currentID > highestID) {
                highestID = currentID;
            }
        } catch (...) {
            // Skip invalid IDs
        }
        current = current->next;
    }
    return highestID;
}


void createAccount(AccountNode*& head, const string& filename) {
    (void)filename; // Mark parameter as unused to suppress warning
     cout << "createAccount called" << endl;
    string fullName, phone, id, accountType, sex, status;
    int age;
    double initialBalance;

    // Load existing accounts from file


    cout << "Current lastAccountID before increment: " << lastAccountID << endl;
    lastAccountID++;  // Increment the ID
    id = intToString(lastAccountID);
    cout << "New account ID will be: " << id << endl;

    // Get full name
    cout << "Enter account holder full name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, fullName);
    while (fullName.empty()) {
        cout << "Name cannot be empty. Please enter again: ";
        getline(cin, fullName);
    }
    cout << "Full name entered: " << fullName << endl;

    // Get phone number
    cout << "Enter phone number: ";
    getline(cin, phone);
    while (phone.length() != 10 || !all_of(phone.begin(), phone.end(), ::isdigit)) {
        cout << "Invalid phone number. Please enter a valid 10-digit phone number: ";
        getline(cin, phone);
    }
    cout << "Phone number entered: " << phone << endl;

    // Get status
    cout << "Enter status (student/servant): ";
    getline(cin, status);
    while (status != "student" && status != "servant") {
        cout << "Invalid status. Please enter either 'student' or 'servant': ";
        getline(cin, status);
    }
    cout << "Status entered: " << status << endl;

    // Get account type
    cout << "Select account type:" << endl;
    cout << "1. Saving account" << endl;
    if (status != "servant") {
        cout << "2. Education account" << endl;
    }
    cout << "3. Women's account" << endl;
    cout << "4. Children's account" << endl;
    cout << "5. Closed account" << endl;
    cout << "6. Without interest account" << endl;
    cout << "Enter choice: ";
    int typeChoice;
    while (!(cin >> typeChoice) || typeChoice < 1 || typeChoice > 6) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a valid option: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Handle servant restrictions for Education account
    if (typeChoice == 2 && status == "servant") {
        cout << "Education accounts are not available for servants." << endl;
        cout << "Please select a different account type (1, 3-6): ";
        while (!(cin >> typeChoice) || typeChoice < 1 || typeChoice > 6 || typeChoice == 2) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a valid option (1, 3-6): ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    switch (typeChoice) {
        case 1:
            accountType = "Saving account";
            break;
        case 2:
            accountType = "Education account";
            break;
        case 3:
            accountType = "Women's account";
            break;
        case 4:
            accountType = "Children's account";
            break;
        case 5:
            accountType = "Closed account";
            break;
        case 6:
            accountType = "Without interest account";
            break;
    }
    cout << "Account type selected: " << accountType << endl;

    // Get sex
    cout << "Enter sex (male/female): ";
    getline(cin, sex);
    while (sex != "male" && sex != "female") {
        cout << "Invalid sex. Please enter either 'male' or 'female': ";
        getline(cin, sex);
    }
    cout << "Sex entered: " << sex << endl;

    // Validate Women's account is only for females
    if (accountType == "Women's account" && sex != "female") {
        cout << "Error: Only female users can open a Women's account." << endl;
        cout << "Please select a different account type." << endl;
        return;
    }

    // Get age
    cout << "Enter age: ";
    while (!(cin >> age) || age <= 0 || age > 120) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid age. Please enter a valid age (1-120): ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Age entered: " << age << endl;

    // Validate age for Children's account
    if (accountType == "Children's account" && age >= 18) {
        cout << "Error: Age must be under 18 for Children's account." << endl;
        cout << "Please select a different account type for adults." << endl;
        return;
    }

    // Validate age for Education account
    if (accountType == "Education account" && (age < 6 || age > 25)) {
        cout << "Error: Age must be between 6 and 25 for Education account." << endl;
        cout << "Please select a different account type." << endl;
        return;
    }

    // Get initial balance
    cout << "Enter initial balance: ";
    while (!(cin >> initialBalance) || initialBalance < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid balance. Please enter a non-negative amount: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Initial balance entered: " << initialBalance << endl;

    // Validate initial balance based on account type
    if (accountType == "Saving account" && initialBalance < 50) {
        cout << "Error: Initial balance for Saving account must be at least 50 Birr." << endl;
        return;
    } else if (accountType == "Education account" && initialBalance < 10) {
        cout << "Error: Initial balance for Education account must be at least 10 Birr." << endl;
        return;
    } else if (accountType == "Women's account" && initialBalance < 25) {
        cout << "Error: Initial balance for Women's account must be at least 25 Birr." << endl;
        return;
    } else if (accountType == "Children's account" && initialBalance < 25) {
        cout << "Error: Initial balance for Children's account must be at least 25 Birr." << endl;
        return;
    } else if (accountType == "Closed account" && initialBalance < 200) {
        cout << "Error: Initial balance for Closed account must be at least 200 Birr." << endl;
        return;
    } else if (accountType == "Without interest account" && initialBalance < 50) {
        cout << "Error: Initial balance for Without interest account must be at least 50 Birr." << endl;
        return;
    }

    cout << "Initial balance of " << initialBalance << " Birr accepted for " << accountType << "." << endl;

    AccountNode* newAccount = new AccountNode(fullName, phone, id, accountType, sex, status, age, initialBalance);
    newAccount->next = head;
    head = newAccount;

    cout << "Account created successfully! Your account ID is: " << id << endl;

}
AccountNode* findAccountByID(AccountNode* head, const string& accountID);


bool login(AccountNode* head, string* currentAccountID) {
    string inputID;
    cout << "Enter your account ID: ";
    cin >> inputID;

    AccountNode* account = findAccountByID(head, inputID);
    if (account) {
        cout << "Login successful!" << endl;
        cout << "Welcome, " << account->account_holder_full_name << "!" << endl;
        cout << "Current balance: ********" << endl;
        *currentAccountID = inputID;
        return true;
    } else {
        cout << "Invalid account ID. Please try again." << endl;
        return false;
    } 

}

void depositMoney(AccountNode* head, const string& filename, const string& currentAccountID) {


}


void withdrawMoney(AccountNode* head, const string& filename, const string& currentAccountID) {


}


void searchAccount(AccountNode* head) {
}

void editAccount(AccountNode* head) {

}

void deleteAccount(AccountNode*& head) {


}

void displayAllAccounts(AccountNode* head, bool isAdmin) {


}


void viewTransactions(AccountNode* head) {

}

void transferMoney(AccountNode* head, const string& filename) {


}



// Forward declarations
AccountNode* findAccountByID(AccountNode* head, const string& accountID);
AccountNode* getOrCreateServiceAccount(AccountNode*& head);

// Forward declaration of global variables
extern string bankServiceAccountID;

// Forward declaration of applyServiceCharge function
void applyServiceCharge(AccountNode* head, AccountNode* account, double chargeAmount, const string& chargeReason);

// Function to get current timestamp or find the bank service account
AccountNode* getOrCreateServiceAccount(AccountNode*& head) {

}

void loadAccountsFromFile(AccountNode*& head, const string& filename) {


}

void saveAccountsToFile(AccountNode* head, const string& filename) {

}

// New function to save all files (currently only accounts file)
void saveAllFiles(AccountNode* head, const string& filename) {
}



AccountNode* findAccountByID(AccountNode* head, const string& accountID) {

}


void applyMonthlyInterest(AccountNode* head) {

}


// Apply service charge and record it to the service account
void applyServiceCharge(AccountNode* head, AccountNode* account, double chargeAmount, const string& chargeReason) {

}





// Function to pause the screen and wait for user input
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

bool isSameDay(const string& timestamp1, const string& timestamp2) {
    // Compare only the date part (YYYY-MM-DD)
    return timestamp1.substr(0, 10) == timestamp2.substr(0, 10);
}

double getTodayTransferOutTotal(AccountNode* account) {

}

// Function to view service charges collected
void viewServiceCharges(AccountNode* head) {

}



int main() {
    AccountNode* head = nullptr;
    int choice;
    bool isLoggedIn = false;
    bool isAdmin = false;
    string currentAccountID = "";
    string filename = "accounts.csv";

    loadAccountsFromFile(head, filename);

    // Initialize the bank service account
    AccountNode* serviceAccount = getOrCreateServiceAccount(head);
    if (serviceAccount) {
        cout << "Bank service account initialized with ID: " << bankServiceAccountID << endl;
    } else {
        cout << "Failed to initialize bank service account!" << endl;
    }

    // Check if admin account exists by checking if admin.dat file exists
    struct stat buffer;
    bool adminFileExists = (stat("admin.dat", &buffer) == 0);

    // Reset the initialization status to ensure fresh check
    Admin::resetInitialization();

    if (!adminFileExists) {
        cout << "No admin account found. Please register an admin account using option 12." << endl;
    }

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
        cout << "9. View transactions" << endl;
        cout << "10. Transfer money" << endl;
        cout << "11. Admin Login" << endl;
        cout << "12. Register Admin" << endl;
        cout << "13. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount(head, filename);
                saveAllFiles(head, filename);
                break;
            case 2:
                isLoggedIn = login(head, &currentAccountID);
                break;
            case 3:
                if (!isLoggedIn) {
                    cout << "Please log in first." << endl;
                    break;
                }
                depositMoney(head, filename, currentAccountID);
                saveAllFiles(head, filename);
                // After deposit, log out to allow other account login
                isLoggedIn = false;
                currentAccountID = "";
                break;
            case 4:
                withdrawMoney(head, filename, currentAccountID);
                saveAllFiles(head, filename);
                // After withdraw, log out to allow other account login
                isLoggedIn = false;
                currentAccountID = "";
                break;
            case 5:
                searchAccount(head);
                break;
            case 6:
                editAccount(head);
                saveAllFiles(head, filename);
                break;
            case 7:
                deleteAccount(head);
                saveAllFiles(head, filename);
                break;
            case 8:
            displayAllAccounts(head, false);
            break;
            case 9:
                if (!isLoggedIn) {
                    cout << "Please log in first." << endl;
                    break;
                }
                viewTransactions(head);
                break;
            case 10:
                transferMoney(head, filename);
                saveAllFiles(head, filename);
                break;
            case 11:
                // Admin login
                {
                    string adminUsername, adminPassword;
                    cout << "Enter admin username: ";
                    cin >> adminUsername;

                    // Hide password with asterisks
                    cout << "Enter admin password: ";
                    adminPassword = ""; // Clear password
                    char ch;
                    while ((ch = _getch()) != '\r') { // Read until Enter key
                        if (ch == '\b') { // Handle backspace
                            if (!adminPassword.empty()) {
                                adminPassword.pop_back();
                                cout << "\b \b"; // Erase last * from console
                            }
                        } else {
                            adminPassword.push_back(ch);
                            cout << '*'; // Show * instead of actual character
                        }
                    }
                    cout << endl; // Add newline after password input

                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (Admin::login(adminUsername, adminPassword)) {
                        isAdmin = true;
                        cout << "Admin login successful!" << endl;
                        pauseScreen();

                        // Admin menu loop
                        bool adminMenuActive = true;
                        while (adminMenuActive) {
                            clearScreen();
                            cout << "===== Admin Menu =====" << endl;
                            cout << "1. View Service Charges" << endl;
                            cout << "2. Apply Monthly Interest to All Accounts" << endl;
                            cout << "3. Change Admin Password" << endl;
                            cout << "4. Display All Accounts" << endl;
                            cout << "0. Return to Main Menu" << endl;
                            cout << "=====================" << endl;
                            cout << "Enter your choice: ";

                            int adminChoice;
                            cin >> adminChoice;
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            switch (adminChoice) {
                                case 1:
                                    viewServiceCharges(head);
                                    pauseScreen();
                                    break;
                                case 2:
                                    // Apply monthly interest for all accounts by their account type interest rate
                                    applyMonthlyInterest(head);
                                    saveAllFiles(head, filename);
                                    cout << "Monthly interest applied to all applicable accounts." << endl;
                                    break;
                                case 3:
                                    // Change admin password
                                    {
                                        string oldPassword, newPassword, confirmPassword;

                                        // Hide current password with asterisks
                                        cout << "Enter current admin password: ";
                                        oldPassword = ""; // Clear password
                                        char ch;
                                        while ((ch = _getch()) != '\r') { // Read until Enter key
                                            if (ch == '\b') { // Handle backspace
                                                if (!oldPassword.empty()) {
                                                    oldPassword.pop_back();
                                                    cout << "\b \b"; // Erase last * from console
                                                }
                                            } else {
                                                oldPassword.push_back(ch);
                                                cout << '*'; // Show * instead of actual character
                                            }
                                        }
                                        cout << endl; // Add newline after password input

                                        // Hide new password with asterisks
                                        cout << "Enter new admin password: ";
                                        newPassword = ""; // Clear password
                                        while ((ch = _getch()) != '\r') { // Read until Enter key
                                            if (ch == '\b') { // Handle backspace
                                                if (!newPassword.empty()) {
                                                    newPassword.pop_back();
                                                    cout << "\b \b"; // Erase last * from console
                                                }
                                            } else {
                                                newPassword.push_back(ch);
                                                cout << '*'; // Show * instead of actual character
                                            }
                                        }
                                        cout << endl; // Add newline after password input

                                        // Hide confirm password with asterisks
                                        cout << "Confirm new admin password: ";
                                        confirmPassword = ""; // Clear password
                                        while ((ch = _getch()) != '\r') { // Read until Enter key
                                            if (ch == '\b') { // Handle backspace
                                                if (!confirmPassword.empty()) {
                                                    confirmPassword.pop_back();
                                                    cout << "\b \b"; // Erase last * from console
                                                }
                                            } else {
                                                confirmPassword.push_back(ch);
                                                cout << '*'; // Show * instead of actual character
                                            }
                                        }
                                        cout << endl; // Add newline after password input

                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                        if (newPassword != confirmPassword) {
                                            cout << "New passwords do not match." << endl;
                                        } else if (Admin::changePassword(oldPassword, newPassword)) {
                                            cout << "Admin password changed successfully." << endl;
                                        } else {
                                            cout << "Failed to change admin password. Incorrect current password." << endl;
                                        }
                                        pauseScreen();
                                    }
                                    break;
                                case 4:
            displayAllAccounts(head, isAdmin);
            pauseScreen();
                                    break;
                                case 0:
                                    adminMenuActive = false;
                                    break;
                                default:
                                    cout << "Invalid choice. Please try again." << endl;
                                    pauseScreen();
                            }
                        }
                    } else {
                        cout << "Invalid admin credentials." << endl;
                        pauseScreen();
                    }
                }
                break;

            case 12:
                // Register Admin
                {
                    // Check if admin.dat file exists directly using fstream
                    ifstream checkFile("admin.dat");
                    bool adminFileExists = checkFile.good();

                    if (adminFileExists) {
                        checkFile.close();
                        cout << "Admin file found." << endl;
                        cout << "Admin account already exists. Cannot create another admin." << endl;
                    } else {
                        string adminUsername, adminPassword, confirmPassword;

                        cout << "===== Admin Registration =====" << endl;
                        cout << "Enter admin username: ";
                        cin >> adminUsername;

                        // Hide password with asterisks
                        cout << "Enter admin password: ";
                        adminPassword = ""; // Clear password
                        char ch;
                        while ((ch = _getch()) != '\r') { // Read until Enter key
                            if (ch == '\b') { // Handle backspace
                                if (!adminPassword.empty()) {
                                    adminPassword.pop_back();
                                    cout << "\b \b"; // Erase last * from console
                                }
                            } else {
                                adminPassword.push_back(ch);
                                cout << '*'; // Show * instead of actual character
                            }
                        }
                        cout << endl; // Add newline after password input

                        // Hide confirm password with asterisks
                        cout << "Confirm admin password: ";
                        confirmPassword = ""; // Clear password
                        while ((ch = _getch()) != '\r') { // Read until Enter key
                            if (ch == '\b') { // Handle backspace
                                if (!confirmPassword.empty()) {
                                    confirmPassword.pop_back();
                                    cout << "\b \b"; // Erase last * from console
                                }
                            } else {
                                confirmPassword.push_back(ch);
                                cout << '*'; // Show * instead of actual character
                            }
                        }
                        cout << endl; // Add newline after password input

                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (adminPassword != confirmPassword) {
                            cout << "Passwords do not match. Admin registration failed." << endl;
                        } else {
                            if (Admin::initialize(adminUsername, adminPassword)) {
                                cout << "Admin account created successfully!" << endl;
                                cout << "Username: " << adminUsername << endl;
                                cout << "Please remember your password." << endl;
                            } else {
                                cout << "Failed to create admin account." << endl;
                            }
                        }
                    }
                    pauseScreen();
                }
                break;

            case 13:
                cout << "Thank you for using the mobile banking system. Goodbye!" << endl;
                saveAllFiles(head, filename);
                while (head != nullptr) {
                    AccountNode* temp = head;
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
