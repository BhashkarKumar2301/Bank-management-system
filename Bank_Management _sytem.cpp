#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accNo;
    char name[50];
    char type;
    double balance;

public:
    void createAccount() {
        cout << "\nEnter Account Number: ";
        cin >> accNo;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Account Type (S for Savings / C for Current): ";
        cin >> type;
        type = toupper(type);
        cout << "Enter Initial Deposit (>=500 for Savings and >=1000 for Current): ";
        cin >> balance;

        if ((type == 'S' && balance < 500) || (type == 'C' && balance < 1000)) {
            cout << "Insufficient initial balance for the selected account type.\n";
            balance = 0;
        }

        cout << "Account Created Successfully.\n";
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accNo;
        cout << "\nHolder Name: " << name;
        cout << "\nType: " << type;
        cout << "\nBalance: " << fixed << setprecision(2) << balance << "\n";
    }

    void modify() {
        cout << "\nModify Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Modify Account Type (S/C): ";
        cin >> type;
        type = toupper(type);
        cout << "Modify Balance Amount: ";
        cin >> balance;
    }

    void deposit(double amt) {
        balance += amt;
    }

    void withdraw(double amt) {
        if (balance >= amt)
            balance -= amt;
        else
            cout << "Insufficient Balance.\n";
    }

    int getAccNo() const {
        return accNo;
    }

    double getBalance() const {
        return balance;
    }

    char getType() const {
        return type;
    }
};

// Function Prototypes
void writeAccount();
void displayAccount(int);
void modifyAccount(int);
void deleteAccount(int);
void displayAll();
void depositWithdraw(int, int);

int main() {
    int choice;
    int num;

    do {
        cout << "\n\n\tBANK MANAGEMENT SYSTEM";
        cout << "\n\n\t1. Create New Account";
        cout << "\n\t2. Deposit Amount";
        cout << "\n\t3. Withdraw Amount";
        cout << "\n\t4. Balance Enquiry";
        cout << "\n\t5. All Account Holder List";
        cout << "\n\t6. Close an Account";
        cout << "\n\t7. Modify an Account";
        cout << "\n\t8. Exit";
        cout << "\n\n\tSelect Your Option (1-8): ";
        cin >> choice;

        switch (choice) {
        case 1:
            writeAccount();
            break;
        case 2:
            cout << "Enter Account Number: ";
            cin >> num;
            depositWithdraw(num, 1);
            break;
        case 3:
            cout << "Enter Account Number: ";
            cin >> num;
            depositWithdraw(num, 2);
            break;
        case 4:
            cout << "Enter Account Number: ";
            cin >> num;
            displayAccount(num);
            break;
        case 5:
            displayAll();
            break;
        case 6:
            cout << "Enter Account Number to Delete: ";
            cin >> num;
            deleteAccount(num);
            break;
        case 7:
            cout << "Enter Account Number to Modify: ";
            cin >> num;
            modifyAccount(num);
            break;
        case 8:
            cout << "Thanks for using Bank Management System.\n";
            break;
        default:
            cout << "Invalid Choice! Try again.\n";
        }

        cin.ignore();
        cin.get();
    } while (choice != 8);

    return 0;
}

// Function Definitions

void writeAccount() {
    Account acc;
    ofstream outFile("account.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account acc;
    bool found = false;
    ifstream inFile("account.dat", ios::binary);

    if (!inFile) {
        cout << "File could not be open!\n";
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == n) {
            acc.showAccount();
            found = true;
        }
    }

    inFile.close();
    if (!found)
        cout << "Account not found.\n";
}

void modifyAccount(int n) {
    Account acc;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    if (!file) {
        cout << "File could not be open!\n";
        return;
    }

    while (!file.eof() && !found) {
        int pos = file.tellg();
        file.read(reinterpret_cast<char*>(&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            cout << "\nEnter New Details:\n";
            acc.modify();
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "Account Updated Successfully.\n";
            found = true;
        }
    }

    file.close();
    if (!found)
        cout << "Account not found.\n";
}

void deleteAccount(int n) {
    Account acc;
    ifstream inFile("account.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    bool found = false;

    if (!inFile) {
        cout << "File could not be open!\n";
        return;
    }

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() != n)
            outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));
        else
            found = true;
    }

    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat", "account.dat");

    if (found)
        cout << "Account Deleted Successfully.\n";
    else
        cout << "Account not found.\n";
}

void displayAll() {
    Account acc;
    ifstream inFile("account.dat", ios::binary);

    if (!inFile) {
        cout << "File could not be open!\n";
        return;
    }

    cout << "\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout << "====================================================\n";
    cout << "A/C No.      NAME           Type    Balance\n";
    cout << "====================================================\n";

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        cout << setw(10) << acc.getAccNo() << "  ";
        cout << setw(15) << acc.getType() << "  ";
        cout << setw(8) << acc.getType() << "  ";
        cout << setw(10) << fixed << setprecision(2) << acc.getBalance() << "\n";
    }

    inFile.close();
}

void depositWithdraw(int n, int option) {
    Account acc;
    fstream file("account.dat", ios::binary | ios::in | ios::out);
    bool found = false;
    double amt;

    if (!file) {
        cout << "File could not be open!\n";
        return;
    }

    while (!file.eof() && !found) {
        int pos = file.tellg();
        file.read(reinterpret_cast<char*>(&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            if (option == 1) {
                cout << "\nEnter Amount to Deposit: ";
                cin >> amt;
                acc.deposit(amt);
            } else {
                cout << "\nEnter Amount to Withdraw: ";
                cin >> amt;
                acc.withdraw(amt);
            }
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));
            cout << "Transaction Successful.\n";
            found = true;
        }
    }

    file.close();
    if (!found)
        cout << "Account not found.\n";
}
