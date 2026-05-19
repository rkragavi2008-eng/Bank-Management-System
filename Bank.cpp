#include <iostream>
#include <fstream>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    string name;
    float balance;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accountNumber;

        cout << "Enter Name: ";
        cin.ignore();
        getline(cin, name);

        do {
            cout << "Enter Initial Balance: ";
            cin >> balance;
            if (balance < 0)
                cout << "Balance cannot be negative!\n";
        } while (balance < 0);
    }

    void showAccount() const {
        cout << "\nAccount No: " << accountNumber;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }

    int getAccNo() const { return accountNumber; }

    void deposit(float amt) {
        if (amt > 0)
            balance += amt;
        else
            cout << "Invalid deposit amount!\n";
    }

    void withdraw(float amt) {
        if (amt <= 0)
            cout << "Invalid amount!\n";
        else if (amt > balance)
            cout << "Insufficient balance!\n";
        else
            balance -= amt;
    }
};

//  Check if account exists
bool accountExists(int accNo) {
    BankAccount acc;
    ifstream file("accounts.dat", ios::binary);

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

// Create Account
void createAccount() {
    BankAccount acc;
    acc.createAccount();

    if (accountExists(acc.getAccNo())) {
        cout << "Account already exists!\n";
        return;
    }

    ofstream file("accounts.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&acc), sizeof(acc));
    file.close();

    cout << "Account Created Successfully!\n";
}

//  Display All Accounts
void displayAll() {
    BankAccount acc;
    ifstream file("accounts.dat", ios::binary);

    if (!file) {
        cout << "No records found.\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        acc.showAccount();
    }

    file.close();
}

//  Search Account
void searchAccount(int accNo) {
    BankAccount acc;
    ifstream file("accounts.dat", ios::binary);

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            acc.showAccount();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found!\n";

    file.close();
}

//  Deposit / Withdraw
void transaction(int accNo, int type) {
    BankAccount acc;
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() == accNo) {
            acc.showAccount();

            float amt;
            cout << "\nEnter amount: ";
            cin >> amt;

            if (type == 1)
                acc.deposit(amt);
            else
                acc.withdraw(amt);

        
            file.seekp(-static_cast<streamoff>(sizeof(acc)), ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(acc));

            cout << "Transaction successful!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found!\n";

    file.close();
}

//  Delete Account
void deleteAccount(int accNo) {
    BankAccount acc;
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(acc))) {
        if (acc.getAccNo() != accNo) {
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(acc));
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        cout << "Account deleted successfully!\n";
    else
        cout << "Account not found!\n";
}

//  MAIN MENU
int main() {
    int choice, accNo;

    do {
        cout << "\n===== BANK MANAGEMENT SYSTEM =====";
        cout << "\n1. Create Account";
        cout << "\n2. Display All Accounts";
        cout << "\n3. Search Account";
        cout << "\n4. Deposit";
        cout << "\n5. Withdraw";
        cout << "\n6. Delete Account";
        cout << "\n7. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2:
                displayAll();
                break;

            case 3:
                cout << "Enter Account No: ";
                cin >> accNo;
                searchAccount(accNo);
                break;

            case 4:
                cout << "Enter Account No: ";
                cin >> accNo;
                transaction(accNo, 1);
                break;

            case 5:
                cout << "Enter Account No: ";
                cin >> accNo;
                transaction(accNo, 2);
                break;

            case 6:
                cout << "Enter Account No: ";
                cin >> accNo;
                deleteAccount(accNo);
                break;

            case 7:
                cout << "Thank you!\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}