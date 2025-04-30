#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
using namespace std;

const string STATUS[] = {"Pending", "Approved", "Settled"};
const int MAX_RESERVATIONS = 100; 

class Reservation
{
private:
    string id, name, phoneNo, date, time, status;
    int guestCount;

public:
    Reservation() {}
    Reservation(string id, string name, string phoneNo, int guestCount, string date, string time, string status) : name(name), phoneNo(phoneNo), guestCount(guestCount), date(date), time(time), status(status) {}

    string getID() const { return id; }

    void editReservation(int gCount, string dt, string tm)
    {
        guestCount = gCount;
        date = dt;
        time = tm;
    }

    void displayReservations() const
    {
        cout << left << setw(15) << id << setw(15) << name << setw(15) << phoneNo << setw(10) << guestCount << setw(10) << date << setw(10) << time << setw(10) << "status" << endl;
    }
};

class ReservationSystem {
    private:
        vector<Reservation> reservations;
        int reservationCounter = 0;
    
    public:
        string generateID();
        void addReservation(...);
        void editReservation(const string &id);
        void cancelReservation(const string &id);
        void displayAll();
        void displayByStatus(const string &status);
        void approveReservation(const string &id);
        void settlePayment(const string &id);
        bool exists(const string &id);
    };

class PaymentMethod
{
public:
    virtual void paymentMethod() = 0;
    virtual ~PaymentMethod() {}
};

class Maya : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        cout << "Maya";
    }
};

class GCash : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        cout << "GCash";
    }
};

class Card : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        cout << "Credit / Debit Card";
    }
};

class Payment
{
private:
    static Payment *instance;
    PaymentMethod *strategy;

    Payment() : strategy(nullptr) {}
    ~Payment() { delete strategy; }

public:
    static Payment *getInstance()
    {
        if (!instance)
            instance = new Payment();
        return instance;
    }

    void setStrategy(PaymentMethod *method)
    {
        if (strategy != nullptr)
            delete strategy;
        strategy = method;
    }

    void executePayment()
    {
        if (strategy)
            return strategy->paymentMethod();
        else
            cout << "No payment method selected.";
    }
};

Payment *Payment::instance = nullptr;


int getValidPaymentMethodInput()
{
    string input;
    while (true)
    {
        cout << "Select Payment Method:\n1. Cash\n2. GCash\n3. Card\nEnter your choice (Cash, GCash, Card): ";
        getline(cin, input);
        input = toUpperCase(input);

        if (input == "CASH")
            return 1;
        else if (input == "GCASH")
            return 2;
        else if (input == "CARD")
            return 3;
        else
            cout << "Invalid payment method. Please enter one of the following: Cash, GCash, or Card.\n\n";
    }
}

bool isValidInteger(const string &input)
{
    if (input.empty())
        return false;
    for (char c : input)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

void customerMenu()
{
    string input;
    int choice;
    bool condition = true;

    while (condition)
    {
        cout << "\nCUSTOMER MENU\n[1] Make reservation\n[2] Edit reservation\n[3] View Reservation\n[4] Cancel reservation\n[5] Back to main menu\n\nEnter choice: ";
        getline(cin, input);

        try
        {
            if (!isValidInteger(input))
                throw invalid_argument("Invalid input. Please enter a valid menu option (1-5).");

            choice = stoi(input);
            if (choice < 1 || choice > 5)
                throw out_of_range("Invalid menu option. Please choose from 1 to 5.");
        }
        catch (const exception &e)
        {
            cout << e.what() << "\n";
        }

        // make reservation
        if (choice == 1)
        {
            string name, phoneNo, guestCount, date, time;
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter phone number: ";
            getline(cin, phoneNo);
            cout << "Enter number of guests: ";
            getline(cin, guestCount);
            cout << "Enter date (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Enter time (HH:MM): ";
            getline(cin, time);
            cout << "Reservation made successfully!\n";
            break;
        }

        // edit reservation
        else if (choice == 2)
        {
            break;
        }

        // view reservation
        else if (choice == 3)
        {
            cout << left << setw(15) << "ID" << setw(15) << "Name" << setw(15) << "Phone No" << setw(10) << "Guests" << setw(10) << "Date" << setw(10) << "Time" << setw(10) << "Status"<< endl;
            break;
        }

        // cancel reservation
        else if (choice == 4)
        {
            cout << left << setw(15) << "ID" << setw(15) << "Name" << setw(15) << "Phone No" << setw(10) << "Guests" << setw(10) << "Date" << setw(10) << "Time" << setw(10) << "Status"<< endl;
            break;
        }

        // back to main menu
        else if (choice == 5)
        {
            return;
        }
    }
}

void adminMenu()
{
    string input;
    int choice;
    bool condition = true;
    while (condition)
    {
        cout << "\nADMIN MENU\n[1] View reservations\n[2] Approve reservation\n[3] Back to main menu\n\nEnter choice: ";
        getline(cin, input);
        try
        {
            if (!isValidInteger(input))
                throw invalid_argument("Invalid input. Please enter a valid menu option (1-3).");

            choice = stoi(input);
            if (choice < 1 || choice > 3)
                throw out_of_range("Invalid menu option. Please choose from 1 to 3.");
        }
        catch (const exception &e)
        {
            cout << e.what() << "\n";
        }

        // view reservations
        if (choice == 1)
        {
            cout << left << setw(15) << "ID" << setw(15) << "Name" << setw(15) << "Phone No" << setw(10) << "Guests" << setw(10) << "Date" << setw(10) << "Time" << setw(10) << "Status"<< endl;
            break;
        }

        // view pending & approve reservations
        else if (choice == 2)
        {
            cout << left << setw(15) << "ID" << setw(15) << "Name" << setw(15) << "Phone No" << setw(10) << "Guests" << setw(10) << "Date" << setw(10) << "Time" << setw(10) << "Status"<< endl;
            break;
        }

        // back to main menu
        else if (choice == 3)
        {
            return;
        }
    }
}

int main()
{
    bool condition = true;
    int choice;

    while (condition)
    {
        string input;
        cout << "Welcome to Reserve Eat! : Restaurant Reservation System\n"
             << endl;

        // insert intro here

        cout << "MAIN MENU\n[1] Customer\n[2] Admin\n[3] Exit\n\nEnter choice: ";
        getline(cin, input);

        try
        {
            if (!isValidInteger(input))
                throw invalid_argument("Invalid input. Please enter a valid menu option (1-4).");

            choice = stoi(input);
            if (choice < 1 || choice > 3)
                throw out_of_range("Invalid menu option. Please choose from 1 to 3.");

            // Customer menu
            if (choice == 1)
            {
                customerMenu();
            }

            else if (choice == 2)
            {
                adminMenu();
            }

            else if (choice == 3)
            {
                cout << "Exit...";
                condition = false;
            }
        }
        catch (const exception &e)
        {
            cout << e.what() << "\n";
        }
    }
    return 0;
}