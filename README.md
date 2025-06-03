# Mobile Banking Management System

## Description
This is a console-based Mobile Banking Management System implemented in C++. It allows users to create and manage different types of bank accounts, perform transactions such as deposits, withdrawals, and transfers, and view transaction history. The system also includes an admin interface for managing service charges, applying monthly interest, and changing admin credentials.

## Features
- Create various types of accounts (Saving, Education, Women's, Children's, Closed, Without interest)
- User login and authentication
- Deposit and withdraw money with account-specific rules and service charges
- Search, edit, and delete accounts
- Display all accounts with balance visibility control
- View transaction history for accounts
- Transfer money between accounts with transfer fees and daily limits
- Admin functionalities:
  - Register and login as admin
  - View collected service charges
  - Apply monthly interest to eligible accounts
  - Change admin password

## Algorithm

### Account Creation
1. Prompt user for personal details (name, phone, status, sex, age).
2. Validate inputs and enforce account-type specific rules (e.g., age limits, minimum initial balance).
3. Assign a unique account ID.
4. Create a new account node and add it to the linked list.
5. Save account data to file.

### User Login
1. Prompt user for account ID.
2. Search linked list for matching account.
3. If found, allow access to account functions; otherwise, deny login.

### Deposit Money
1. Verify user is logged in.
2. Prompt for deposit amount.
3. Validate amount is positive.
4. Add amount to account balance.
5. Record transaction with timestamp.
6. Save updated data to file.

### Withdraw Money
1. Verify user is logged in.
2. Prompt for withdrawal amount.
3. Validate amount and check minimum balance requirements.
4. Calculate and apply service charges based on account type.
5. Deduct amount and charges from balance.
6. Record transaction with timestamp.
7. Save updated data to file.

### Transfer Money
1. Prompt for source and destination account IDs.
2. Validate accounts and user permissions.
3. Prompt for transfer amount.
4. Check daily transfer limits and minimum balance.
5. Calculate transfer fee and apply service charge.
6. Update balances of both accounts.
7. Record transactions for both accounts.
8. Save updated data to file.

### Admin Functions
- Register admin account if none exists.
- Admin login with hidden password input.
- View total service charges collected.
- Apply monthly interest to eligible accounts based on account type.
- Change admin password securely.

## Installation
1. Ensure you have a C++ compiler installed (e.g., g++).
2. Compile the source code:
   ```
   g++ MBMS.cpp -o MBMS
   ```
3. Run the executable:
   ```
   ./MBMS
   ```

## Usage
- Run the program and follow the on-screen menu options.
- Create an account before logging in.
- Use your account ID to log in and perform transactions.
- Admin users can register using option 12 and login using option 11.
- Admin menu provides additional management features.

## File Structure
- `MBMS.cpp`: Main source code file containing the entire system implementation.
- `accounts.csv`: Data file storing account information (created after running the program).
- `admin.dat`: Data file storing admin credentials (created after admin registration).
- `output/`: Directory for any output files (e.g., reports or logs).

## Admin Account
- Admin account must be registered before using admin features.
- Admin credentials are stored securely in `admin.dat`.
- Admin can view service charges, apply monthly interest, and change password.

## Notes
- The system enforces account-specific rules such as minimum balances and age restrictions.
- Service charges and transfer fees are automatically applied based on account types.
- Password inputs for admin are hidden for security.
- The system uses file storage to persist data between sessions.

## Authors
    -**NAME**                       

- **NATNAEL TSEDEKE**…………………………….**7783/23**
- **ROBEL WENDOWESEN** ……………………………**0752/23**
- **NAOL SISAY**…………………………………….…....**0590/23**
- **MASTEWAL DEMIREW**……………………………**7197/23**
- **SARA DEJENE**........................................................**7293/21**
- **NATNAEL BRHANU**.........................................**6169/23**

