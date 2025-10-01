#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
using namespace std;

struct User {
    int accountNum;
    double balance;
    string accountName, bday, contactNumber, pin;
};

struct Node {
    User user;
    Node *next;
    Node(User x) : user(x), next(nullptr) {}
};

class Account {
private:
    Node *head, *currentlyLoggedIn;
public:
    Account() {
        head = nullptr;
        currentlyLoggedIn = nullptr;
    }
    ~Account() {
        Node *p, *t;
        p = head;

        while (p != nullptr) {
            t = p;
            p = p->next;
            delete t;
        }

        head = nullptr;
    }
    void balanceInquiry();
    int getBalance();
    void withdraw(double amount);
    void deposit(double amount);
    void fundTransfer(int accountNum, double amount);
    void changePin(string pin);
    void signUp(User x);
    bool logIn(int accountNum, string pin);
    void save();
    void retrieve();
    string encrypt(string pin);
    string decrypt(string pin);
    void displayTester();
};

int logInSignUp();
int transactionMenu();


//main
int main() {
    Account acc;
    acc.retrieve();
    User y;
    string pinTemp;

    while (true) {
        switch(logInSignUp()) {
        case 1:
            system("cls");
            cout << "Account number: ";
            cin >> y.accountNum;
            cout << "Pin: ";
            cin.ignore();
            getline(cin, y.pin);

            if (acc.logIn(y.accountNum, y.pin)) {
                while (true) {
                    switch(transactionMenu()) {
                        int amount;
                    case 1:
                        system("cls");
                        acc.balanceInquiry();
                        break;
                    case 2:
                        system("cls");
                        cout << "Enter amount: ";
                        cin >> amount;
                        acc.withdraw(amount);
                        break;
                    case 3:
                        system("cls");
                        cout << "Enter amount: ";
                        cin >> amount;
                        acc.deposit(amount);
                        break;
                    case 4:
                        system("cls");
                        cout << "Enter account number: ";
                        cin >> y.accountNum;
                        cout << "Enter amount to transfer: ";
                        cin >> amount;
                        acc.fundTransfer(y.accountNum, amount);
                        break;
                    case 5:
                        cout << "Enter new pin: ";
                        cin.ignore();
                        getline(cin, y.pin);
                        acc.changePin(y.pin);
                        break;
                    case 6:
                        acc.save();
                        exit(0);
                    default:
                        cout << "Invalid input" << endl;
                        system("pause");
                    }
                }
            }
            break;
        case 2:
            system("cls");
            cout << "Account number: ";
            cin >> y.accountNum;
            cout << "Account name: ";
            cin.ignore();
            getline(cin, y.accountName);
            cout << "Birthday: ";
            getline(cin, y.bday);
            cout << "Contact number: ";
            getline(cin, y.contactNumber);
            cout << "Initial deposit (5000 minimum): ";
            cin >> y.balance;
            if (y.balance < 5000) {
                cout << "\n\nThat's less than 5000" << endl;
                system("pause");
                break;
            }
            cout << "Pin code: ";
            cin.ignore();
            getline(cin, pinTemp);
            y.pin = acc.encrypt(pinTemp);
            acc.signUp(y);
            acc.save();

            cout << "Account registered" << endl;
            system("pause");
            break;
        case 3:
            acc.save();
            exit(0);
        case 4:
            acc.displayTester(); //temporary account checker for debugging
            break;
        default:
            cout << "Invalid input" << endl;
            system("pause");
        }
    }
    return 0;
}

//functions
int logInSignUp() {
    system("cls");
    int userChoice;

    cout << "1.) Log in " << endl;
    cout << "2.) Sign up" << endl;
    cout << "3.) Exit" << endl;
    cout << "Select: ";
    cin >> userChoice;

    return userChoice;
}

int transactionMenu() {
    system("cls");
    int userChoice;

    cout << "1.) Balance Inquiry" << endl;
    cout << "2.) Withdraw" << endl;
    cout << "3.) Deposit" << endl;
    cout << "4.) Fund Transfer" << endl;
    cout << "5.) Change Pin Code" << endl;
    cout << "6.) Exit" << endl;
    cout << "Select: ";
    cin >> userChoice;

    return userChoice;
}

string Account::encrypt(string pin) {
    int v = 120;

    for (int i = 0; i < pin.size(); i++) {
        pin[i] = pin[i] + v;
    }

    return pin;
}

string Account::decrypt(string pin) {
    int v = 120;

    for (int i = 0; i < pin.size(); i++) {
        pin[i] = pin[i] - v;
    }

    return pin;
}

void Account::signUp(User x) {
    Node *p, *q, *newNode;
    q = p = head;
    newNode = new Node(x);

    while (p != nullptr) {
        q = p;
        p = p->next;
    }

    if (p == head) {
        head = newNode;
    } else {
        q->next = newNode;
    }
}

bool Account::logIn(int accountNum, string pin) {
    Node *p;
    p = head;

    while (p != nullptr && accountNum != p->user.accountNum) {
        p = p->next;
    }

    if (p == nullptr) {
        cout << "Account does not exist" << endl;
        system("pause");
        return false;
    }

    if (pin != decrypt(p->user.pin)) {
        cout << "Wrong pin" << endl;
        system("pause");
        return false;
    }

    currentlyLoggedIn = p;


    cout << "Logged in successful" << endl;
    system("pause");

    return true;
}

void Account::save() {
    Node *p;
    p = head;
    ofstream outFile("list.txt");

    if (!outFile) {
        cout << "File error" << endl;
        system("pause");
        return;
    }

    while (p != nullptr) {
        outFile << p->user.accountNum << "," << p->user.accountName << "," << p->user.bday << "," << p->user.contactNumber << "," << p->user.balance << "," << p->user.pin << endl;

        p = p->next;
    }

    outFile.close();
}

void Account::retrieve() {
    User y;
    ifstream inFile("list.txt");

    if (!inFile) {
        cout << "File error" << endl;
        system("pause");
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string balance, accountNum;
        getline(ss, accountNum, ',');
        getline(ss, y.accountName, ',');
        getline(ss, y.bday, ',');
        getline(ss, y.contactNumber, ',');
        getline(ss, balance, ',');
        getline(ss, y.pin, ',');

        y.balance = stoi(balance);
        y.accountNum = stoi(accountNum);

        signUp(y);
    }

    inFile.close();

    cout << "Data retrieved successfully" << endl;
    system("pause");
}

void Account::displayTester() { //TEMPORARY FOR DEBUGGING
    Node *p;
    p = head;

    while (p != nullptr) {
        cout << p->user.accountNum << "," << p->user.accountName << "," << p->user.bday << "," << p->user.contactNumber << "," << p->user.balance << "," << p->user.pin << endl;

        p = p->next;
    }

    system("pause");
}

int Account::getBalance() {
    return currentlyLoggedIn->user.balance;
}

void Account::balanceInquiry() {
    cout << "Balance: " << getBalance() << endl;
    system("pause");
}

void Account::withdraw(double amount) {
    if (amount > getBalance()) {
        cout << "Not enough balance" << endl;
        system("pause");
    } else {
        currentlyLoggedIn->user.balance -= amount;
        cout << "Successful Transaction" << endl;
        system("pause");
    }
}
void Account::deposit(double amount) {
    if (amount < 0) {
        cout << "Invalid" << endl;
        system("pause");
    } else {
        currentlyLoggedIn->user.balance += amount;
        cout << "Successful Transaction" << endl;
        system("pause");
    }
}

void Account::fundTransfer(int accountNum, double amount) {
    Node *p;
    p = head;

    if (amount > getBalance()) {
        cout << "Not enough balance" << endl;
        system("pause");
        return;
    }

    while (p != nullptr && accountNum != p->user.accountNum) {
        p = p->next;
    }

    if (p == nullptr) {
        cout << "Account does not exist" << endl;
        system("pause");
        return;
    }

    p->user.balance += amount;
    currentlyLoggedIn->user.balance -= amount;

    cout << "Fund transefered successfully" << endl;
    system("pause");
}

void Account::changePin(string pin) {
    currentlyLoggedIn->user.pin = encrypt(pin);

    cout << "Pin updated" << endl;
    system("pause");
}
