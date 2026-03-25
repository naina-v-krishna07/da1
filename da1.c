<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define MAX_NAME 50
#define LOG_FILE "transactions.log"
#define ACCOUNTS_FILE "accounts.dat"

// Account structure
typedef struct {
    int accountNumber;
    char name[MAX_NAME];
    double balance;
    int transactionCount;
    int transactionIds[MAX_TRANSACTIONS];
} Account;

// Global arrays
Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int nextAccountNumber = 1001;

// Function prototypes
void loadAccounts();
void saveAccounts();
void loadTransactions();
void saveTransaction(int accNum, char* type, double amount);
void displayLast5Transactions(int accNum);
int findAccount(int accNum);
void deleteAccount(int accNum);
void generateAccountNumber();
int getValidInt(char* prompt, int min, int max);
double getValidDouble(char* prompt, double min);
void getValidName(char* name);
void displayMenu();
char getValidChoice();

int main() {
    loadAccounts();
    loadTransactions();
    
    printf("=== MINI BANKING SYSTEM ===\n");
    printf("Welcome to the Banking System!\n\n");
    
    int choice;
    do {
        displayMenu();
        choice = getValidInt("Enter your choice: ", 1, 10);
        
        switch(choice) {
            case 1:
                // Create Account
                {
                    char name[MAX_NAME];
                    double initialBalance;
                    
                    printf("\n--- CREATE NEW ACCOUNT ---\n");
                    getValidName(name);
                    initialBalance = getValidDouble("Enter initial balance: ", 0.0);
                    
                    if (accountCount < MAX_ACCOUNTS) {
                        accounts[accountCount].accountNumber = nextAccountNumber++;
                        strcpy(accounts[accountCount].name, name);
                        accounts[accountCount].balance = initialBalance;
                        accounts[accountCount].transactionCount = 0;
                        
                        printf("Account created successfully!\n");
                        printf("Account Number: %d\n", accounts[accountCount].accountNumber);
                        printf("Name: %s\n", accounts[accountCount].name);
                        printf("Balance: %.2f\n\n", accounts[accountCount].balance);
                        
                        accountCount++;
                        saveAccounts();
                    } else {
                        printf("Maximum account limit reached!\n\n");
                    }
                }
                break;
                
            case 2:
                // Deposit
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        double amount = getValidDouble("Enter deposit amount: ", 0.01);
                        accounts[index].balance += amount;
                        saveTransaction(accNum, "DEPOSIT", amount);
                        
                        printf("Deposit successful!\n");
                        printf("New balance: %.2f\n\n", accounts[index].balance);
                        saveAccounts();
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 3:
                // Withdraw
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        double amount = getValidDouble("Enter withdrawal amount: ", 0.01);
                        
                        if (amount <= accounts[index].balance) {
                            accounts[index].balance -= amount;
                            saveTransaction(accNum, "WITHDRAWAL", -amount);
                            
                            printf("Withdrawal successful!\n");
                            printf("New balance: %.2f\n\n", accounts[index].balance);
                            saveAccounts();
                        } else {
                            printf("Insufficient balance!\n");
                            printf("Available balance: %.2f\n\n", accounts[index].balance);
                        }
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 4:
                // Search Account
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        printf("\n--- ACCOUNT DETAILS ---\n");
                        printf("Account Number: %d\n", accounts[index].accountNumber);
                        printf("Name: %s\n", accounts[index].name);
                        printf("Balance: %.2f\n\n", accounts[index].balance);
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 5:
                // View All Accounts
                {
                    printf("\n--- ALL ACCOUNTS SUMMARY ---\n");
                    if (accountCount == 0) {
                        printf("No accounts found!\n\n");
                    } else {
                        for (int i = 0; i < accountCount; i++) {
                            printf("Acc No: %d | Name: %s | Balance: %.2f\n", 
                                   accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
                        }
                        printf("\n");
                    }
                }
                break;
                
            case 6:
                // Display Last 5 Transactions
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    displayLast5Transactions(accNum);
                }
                break;
                
            case 7:
                // Update Account Details
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        printf("\n--- UPDATE ACCOUNT ---\n");
                        printf("Current name: %s\n", accounts[index].name);
                        getValidName(accounts[index].name);
                        printf("Account updated successfully!\n\n");
                        saveAccounts();
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 8:
                // Delete Account
                {
                    int accNum = getValidInt("Enter account number to delete: ", 1001, nextAccountNumber-1);
                    deleteAccount(accNum);
                }
                break;
                
            case 9:
                printf("\nThank you for using Mini Banking System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n\n");
        }
    } while (choice != 9);
    
    saveAccounts();
    return 0;
}

// NEW FUNCTION: Delete Account
void deleteAccount(int accNum) {
    int index = findAccount(accNum);
    
    if (index == -1) {
        printf("Account %d not found!\n\n", accNum);
        return;
    }
    
    // Safety checks before deletion
    printf("\n--- ACCOUNT DELETION CONFIRMATION ---\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Name: %s\n", accounts[index].name);
    printf("Balance: %.2f\n", accounts[index].balance);
    
    if (accounts[index].balance > 0) {
        printf("\n⚠️  WARNING: Account has balance %.2f. ", accounts[index].balance);
        printf("Please withdraw all funds before deletion.\n\n");
        printf("Deletion cancelled for safety.\n\n");
        return;
    }
    
    char confirm;
    printf("\nAre you sure you want to delete this account? (y/n): ");
    scanf(" %c", &confirm);
    while (getchar() != '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        // Log the deletion
        saveTransaction(accNum, "ACCOUNT_DELETED", 0.0);
        
        // Shift all accounts after this index forward
        for (int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        
        accountCount--;
        saveAccounts();
        
        printf("✅ Account %d deleted successfully!\n\n", accNum);
    } else {
        printf("Deletion cancelled.\n\n");
    }
}

// Load accounts from file
void loadAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (file == NULL) {
        printf("No existing accounts file found. Starting fresh.\n");
        return;
    }
    
    fread(&accountCount, sizeof(int), 1, file);
    fread(&nextAccountNumber, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
    printf("Loaded %d accounts from file.\n\n", accountCount);
}

// Save accounts to file
void saveAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "wb");
    if (file != NULL) {
        fwrite(&accountCount, sizeof(int), 1, file);
        fwrite(&nextAccountNumber, sizeof(int), 1, file);
        fwrite(accounts, sizeof(Account), accountCount, file);
        fclose(file);
    }
}

// Find account by number
int findAccount(int accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

// Save transaction with timestamp
void saveTransaction(int accNum, char* type, double amount) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file != NULL) {
        time_t now;
        time(&now);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr)-1] = '\0'; // Remove newline
        
        fprintf(file, "%s | Account: %d | Type: %s | Amount: %.2f\n", 
                timeStr, accNum, type, amount);
        fclose(file);
    }
}

// Display last 5 transactions for account
void displayLast5Transactions(int accNum) {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) {
        printf("No transaction log found!\n\n");
        return;
    }
    
    char line[256];
    char recentTransactions[5][256];
    int count = 0;
    long pos = 0;
    
    // Find recent transactions by reading from end
    fseek(file, 0, SEEK_END);
    pos = ftell(file);
    
    while (pos > 0 && count < 5) {
        fseek(file, pos - 256, SEEK_SET);
        if (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "Account: ") != NULL) {
                char accStr[10];
                int lineAccNum;
                if (sscanf(line, "%*[^A]Account: %d", &lineAccNum) == 1) {
                    if (lineAccNum == accNum) {
                        strcpy(recentTransactions[count], line);
                        count++;
                    }
                }
            }
        }
        pos = ftell(file) - 1;
        if (pos < 0) break;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No transactions found for account %d\n\n", accNum);
    } else {
        printf("\n--- LAST %d TRANSACTIONS FOR ACCOUNT %d ---\n", count, accNum);
        for (int i = count - 1; i >= 0; i--) {
            printf("%s", recentTransactions[i]);
        }
        printf("\n");
    }
}

// Input validation functions
int getValidInt(char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            while (getchar() != '\n'); // Clear input buffer
            return value;
        }
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
        while (getchar() != '\n'); // Clear input buffer
    }
}

double getValidDouble(char* prompt, double min) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1 && value >= min) {
            while (getchar() != '\n');
            return value;
        }
        printf("Invalid input! Please enter a valid amount (>= %.2f).\n", min);
        while (getchar() != '\n');
    }
}

void getValidName(char* name) {
    printf("Enter account holder name: ");
    scanf("%49s", name);
    while (getchar() != '\n');
    
    // Capitalize first letter
    if (name[0] >= 'a' && name[0] <= 'z') {
        name[0] = name[0] - 32;
    }
}

void displayMenu() {
    printf("\n========== MENU ==========\n");
    printf("1.  Create New Account\n");
    printf("2.  Deposit Money\n");
    printf("3.  Withdraw Money\n");
    printf("4.  Search Account\n");
    printf("5.  View All Accounts\n");
    printf("6.  View Last 5 Transactions\n");
    printf("7.  Update Account Details\n");
    printf("8.  Delete Account\n");
    printf("9.  Exit\n");
    printf("==========================\n");
}

// Placeholder for loadTransactions (simplified)
void loadTransactions() {
    // Transaction log is append-only, no need to load
}
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_ACCOUNTS 100
#define MAX_TRANSACTIONS 1000
#define MAX_NAME 50
#define LOG_FILE "transactions.log"
#define ACCOUNTS_FILE "accounts.dat"

// Account structure
typedef struct {
    int accountNumber;
    char name[MAX_NAME];
    double balance;
    int transactionCount;
    int transactionIds[MAX_TRANSACTIONS];
} Account;

// Global arrays
Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int nextAccountNumber = 1001;

// Function prototypes
void loadAccounts();
void saveAccounts();
void loadTransactions();
void saveTransaction(int accNum, char* type, double amount);
void displayLast5Transactions(int accNum);
int findAccount(int accNum);
void deleteAccount(int accNum);
void generateAccountNumber();
int getValidInt(char* prompt, int min, int max);
double getValidDouble(char* prompt, double min);
void getValidName(char* name);
void displayMenu();
char getValidChoice();

int main() {
    loadAccounts();
    loadTransactions();
    
    printf("=== MINI BANKING SYSTEM ===\n");
    printf("Welcome to the Banking System!\n\n");
    
    int choice;
    do {
        displayMenu();
        choice = getValidInt("Enter your choice: ", 1, 10);
        
        switch(choice) {
            case 1:
                // Create Account
                {
                    char name[MAX_NAME];
                    double initialBalance;
                    
                    printf("\n--- CREATE NEW ACCOUNT ---\n");
                    getValidName(name);
                    initialBalance = getValidDouble("Enter initial balance: ", 0.0);
                    
                    if (accountCount < MAX_ACCOUNTS) {
                        accounts[accountCount].accountNumber = nextAccountNumber++;
                        strcpy(accounts[accountCount].name, name);
                        accounts[accountCount].balance = initialBalance;
                        accounts[accountCount].transactionCount = 0;
                        
                        printf("Account created successfully!\n");
                        printf("Account Number: %d\n", accounts[accountCount].accountNumber);
                        printf("Name: %s\n", accounts[accountCount].name);
                        printf("Balance: %.2f\n\n", accounts[accountCount].balance);
                        
                        accountCount++;
                        saveAccounts();
                    } else {
                        printf("Maximum account limit reached!\n\n");
                    }
                }
                break;
                
            case 2:
                // Deposit
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        double amount = getValidDouble("Enter deposit amount: ", 0.01);
                        accounts[index].balance += amount;
                        saveTransaction(accNum, "DEPOSIT", amount);
                        
                        printf("Deposit successful!\n");
                        printf("New balance: %.2f\n\n", accounts[index].balance);
                        saveAccounts();
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 3:
                // Withdraw
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        double amount = getValidDouble("Enter withdrawal amount: ", 0.01);
                        
                        if (amount <= accounts[index].balance) {
                            accounts[index].balance -= amount;
                            saveTransaction(accNum, "WITHDRAWAL", -amount);
                            
                            printf("Withdrawal successful!\n");
                            printf("New balance: %.2f\n\n", accounts[index].balance);
                            saveAccounts();
                        } else {
                            printf("Insufficient balance!\n");
                            printf("Available balance: %.2f\n\n", accounts[index].balance);
                        }
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 4:
                // Search Account
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        printf("\n--- ACCOUNT DETAILS ---\n");
                        printf("Account Number: %d\n", accounts[index].accountNumber);
                        printf("Name: %s\n", accounts[index].name);
                        printf("Balance: %.2f\n\n", accounts[index].balance);
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 5:
                // View All Accounts
                {
                    printf("\n--- ALL ACCOUNTS SUMMARY ---\n");
                    if (accountCount == 0) {
                        printf("No accounts found!\n\n");
                    } else {
                        for (int i = 0; i < accountCount; i++) {
                            printf("Acc No: %d | Name: %s | Balance: %.2f\n", 
                                   accounts[i].accountNumber, accounts[i].name, accounts[i].balance);
                        }
                        printf("\n");
                    }
                }
                break;
                
            case 6:
                // Display Last 5 Transactions
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    displayLast5Transactions(accNum);
                }
                break;
                
            case 7:
                // Update Account Details
                {
                    int accNum = getValidInt("Enter account number: ", 1001, nextAccountNumber-1);
                    int index = findAccount(accNum);
                    
                    if (index != -1) {
                        printf("\n--- UPDATE ACCOUNT ---\n");
                        printf("Current name: %s\n", accounts[index].name);
                        getValidName(accounts[index].name);
                        printf("Account updated successfully!\n\n");
                        saveAccounts();
                    } else {
                        printf("Account not found!\n\n");
                    }
                }
                break;
                
            case 8:
                // Delete Account
                {
                    int accNum = getValidInt("Enter account number to delete: ", 1001, nextAccountNumber-1);
                    deleteAccount(accNum);
                }
                break;
                
            case 9:
                printf("\nThank you for using Mini Banking System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n\n");
        }
    } while (choice != 9);
    
    saveAccounts();
    return 0;
}

// NEW FUNCTION: Delete Account
void deleteAccount(int accNum) {
    int index = findAccount(accNum);
    
    if (index == -1) {
        printf("Account %d not found!\n\n", accNum);
        return;
    }
    
    // Safety checks before deletion
    printf("\n--- ACCOUNT DELETION CONFIRMATION ---\n");
    printf("Account Number: %d\n", accounts[index].accountNumber);
    printf("Name: %s\n", accounts[index].name);
    printf("Balance: %.2f\n", accounts[index].balance);
    
    if (accounts[index].balance > 0) {
        printf("\n⚠️  WARNING: Account has balance %.2f. ", accounts[index].balance);
        printf("Please withdraw all funds before deletion.\n\n");
        printf("Deletion cancelled for safety.\n\n");
        return;
    }
    
    char confirm;
    printf("\nAre you sure you want to delete this account? (y/n): ");
    scanf(" %c", &confirm);
    while (getchar() != '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        // Log the deletion
        saveTransaction(accNum, "ACCOUNT_DELETED", 0.0);
        
        // Shift all accounts after this index forward
        for (int i = index; i < accountCount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        
        accountCount--;
        saveAccounts();
        
        printf("✅ Account %d deleted successfully!\n\n", accNum);
    } else {
        printf("Deletion cancelled.\n\n");
    }
}

// Load accounts from file
void loadAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "rb");
    if (file == NULL) {
        printf("No existing accounts file found. Starting fresh.\n");
        return;
    }
    
    fread(&accountCount, sizeof(int), 1, file);
    fread(&nextAccountNumber, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), accountCount, file);
    fclose(file);
    printf("Loaded %d accounts from file.\n\n", accountCount);
}

// Save accounts to file
void saveAccounts() {
    FILE *file = fopen(ACCOUNTS_FILE, "wb");
    if (file != NULL) {
        fwrite(&accountCount, sizeof(int), 1, file);
        fwrite(&nextAccountNumber, sizeof(int), 1, file);
        fwrite(accounts, sizeof(Account), accountCount, file);
        fclose(file);
    }
}

// Find account by number
int findAccount(int accNum) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum) {
            return i;
        }
    }
    return -1;
}

// Save transaction with timestamp
void saveTransaction(int accNum, char* type, double amount) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file != NULL) {
        time_t now;
        time(&now);
        char *timeStr = ctime(&now);
        timeStr[strlen(timeStr)-1] = '\0'; // Remove newline
        
        fprintf(file, "%s | Account: %d | Type: %s | Amount: %.2f\n", 
                timeStr, accNum, type, amount);
        fclose(file);
    }
}

// Display last 5 transactions for account
void displayLast5Transactions(int accNum) {
    FILE *file = fopen(LOG_FILE, "r");
    if (file == NULL) {
        printf("No transaction log found!\n\n");
        return;
    }
    
    char line[256];
    char recentTransactions[5][256];
    int count = 0;
    long pos = 0;
    
    // Find recent transactions by reading from end
    fseek(file, 0, SEEK_END);
    pos = ftell(file);
    
    while (pos > 0 && count < 5) {
        fseek(file, pos - 256, SEEK_SET);
        if (fgets(line, sizeof(line), file) != NULL) {
            if (strstr(line, "Account: ") != NULL) {
                char accStr[10];
                int lineAccNum;
                if (sscanf(line, "%*[^A]Account: %d", &lineAccNum) == 1) {
                    if (lineAccNum == accNum) {
                        strcpy(recentTransactions[count], line);
                        count++;
                    }
                }
            }
        }
        pos = ftell(file) - 1;
        if (pos < 0) break;
    }
    
    fclose(file);
    
    if (count == 0) {
        printf("No transactions found for account %d\n\n", accNum);
    } else {
        printf("\n--- LAST %d TRANSACTIONS FOR ACCOUNT %d ---\n", count, accNum);
        for (int i = count - 1; i >= 0; i--) {
            printf("%s", recentTransactions[i]);
        }
        printf("\n");
    }
}

// Input validation functions
int getValidInt(char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= min && value <= max) {
            while (getchar() != '\n'); // Clear input buffer
            return value;
        }
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
        while (getchar() != '\n'); // Clear input buffer
    }
}

double getValidDouble(char* prompt, double min) {
    double value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &value) == 1 && value >= min) {
            while (getchar() != '\n');
            return value;
        }
        printf("Invalid input! Please enter a valid amount (>= %.2f).\n", min);
        while (getchar() != '\n');
    }
}

void getValidName(char* name) {
    printf("Enter account holder name: ");
    scanf("%49s", name);
    while (getchar() != '\n');
    
    // Capitalize first letter
    if (name[0] >= 'a' && name[0] <= 'z') {
        name[0] = name[0] - 32;
    }
}

void displayMenu() {
    printf("\n========== MENU ==========\n");
    printf("1.  Create New Account\n");
    printf("2.  Deposit Money\n");
    printf("3.  Withdraw Money\n");
    printf("4.  Search Account\n");
    printf("5.  View All Accounts\n");
    printf("6.  View Last 5 Transactions\n");
    printf("7.  Update Account Details\n");
    printf("8.  Delete Account\n");
    printf("9.  Exit\n");
    printf("==========================\n");
}

// Placeholder for loadTransactions (simplified)
void loadTransactions() {
    // Transaction log is append-only, no need to load
}
>>>>>>> f45d9c9 (Add main.c and WASM build files for mini banking project)
