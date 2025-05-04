#include <iostream>
#include <vector> // Used for dynamic array
#include <string>
#include <stdexcept>
#include <iomanip> // Used for formatting output
#include <algorithm>
using namespace std;

const string STATUS[] = {"Pending", "Approved", "Settled"};
const int MAX_RESERVATIONS = 100;

string toUpperCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Get a valid integer input
int getValidInt(const string &prompt, int min, int max)
{
    string input;
    int value;
    bool valid = false;
    while (!valid)
    {
        cout << prompt;
        getline(cin, input);

        try
        {
            if (input.empty())
            {
                throw invalid_argument("Input cannot be empty! Please enter a valid whole number.\n");
            }
            for (char c : input)
            {
                if (!isdigit(c))
                {
                    throw invalid_argument("Invalid input! Please enter a valid whole number.\n");
                }
            }
            value = stoi(input);
            if (value < min || value > max)
            {
                throw out_of_range("Input out of range! Please try again.\n");
            }
            valid = true;
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }
    return value;
}

class Reservation
{
private:
    string id, name, phoneNo, date, time, status;
    int guestCount;

public:
    Reservation() {}
    Reservation(string id, string name, string phoneNo, int guestCount, string date, string time, string status) : id(id), name(name), phoneNo(phoneNo), guestCount(guestCount), date(date), time(time), status(status) {}

    string getID() const { return id; }
    string getStatus() const { return status; }
    int getGuestCount() const { return guestCount; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    void setStatus(const string &newStatus) { status = newStatus; }

    void editReservation(int gCount, string dt, string tm)
    {
        guestCount = gCount;
        date = dt;
        time = tm;
    }

    void displayReservations() const
    {
        cout << left << setw(20) << id << setw(30) << name << setw(20) << phoneNo
             << setw(10) << guestCount << setw(15) << date << setw(15) << time
             << setw(15) << status << endl;
        cout << "========================================================================================================================\n";
    }
};

class ReservationSystem
{
private:
    vector<Reservation> reservations;
    int reservationCounter = 0;

public:
    //  Reservation System methods
    string generateID();
    void addReservation(const string &name, const string &phoneNo, int guestCount, const string &date, const string &time);
    void editReservation(const string &id);
    void cancelReservation(const string &id);
    void displayAll();
    bool hasStatus(const string &status) const;
    void displayByStatus(const string &status);
    string getStatus(const string &id) const;
    void approveReservation(const string &id);
    void settlePayment(const string &id);
    bool exists(const string &id);
    bool isEmpty() const;
};

// Implementation of generateID method
string ReservationSystem::generateID()
{
    return to_string(++reservationCounter); // Increments and returns new ID
}

// Implementation of addReservation method
void ReservationSystem::addReservation(const string &name, const string &phoneNo, int guestCount, const string &date, const string &time)
{
    string id = generateID();
    reservations.emplace_back(id, name, phoneNo, guestCount, date, time, STATUS[0]); // STATUS[0] = "Pending"
    cout << "Reservation made successfully! Reservation ID: " << id << endl;
}

void ReservationSystem::editReservation(const string &id)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id)
        {
            // Only allow editing if status is "Pending"
            if (res.getStatus() != "Pending")
            {
                cout << "Only reservations with 'Pending' status can be edited.\n";
                return;
            }

            string newDate, newTime, newGC;
            int newGuestCount;
            bool validGC = false;

            do
            {
                cout << "Enter new number of guests: ";
                getline(cin, newGC);
                bool isValid = true;
                for (char c : newGC)
                {
                    if (!isdigit(c))
                    {
                        isValid = false;
                        break;
                    }
                }
                if (!isValid || newGC.empty())
                {
                    cout << "Invalid input. Please enter a valid number.\n";
                    continue;
                }
                newGuestCount = stoi(newGC);
                if (newGuestCount < 1 || newGuestCount > 100)
                {
                    cout << "Guest Count must be between 1 and 100.\n";
                    continue;
                }
                validGC = true;
            } while (!validGC);

            do
            {
                cout << "Enter new date (MM-DD-YYYY): ";
                getline(cin, newDate);
                if (newDate.empty())
                {
                    cout << "Date cannot be empty.\n";
                }
            } while (newDate.empty());

            do
            {
                cout << "Enter new time (HH:MM AM/PM): ";
                getline(cin, newTime);
                if (newTime.empty())
                {
                    cout << "Time cannot be empty.\n";
                }
            } while (newTime.empty());

            res.editReservation(newGuestCount, newDate, newTime);
            cout << "Reservation updated successfully!\n";
            return;
        }
    }

    cout << "Reservation ID not found.\n";
}

bool ReservationSystem::hasStatus(const string &status) const
{
    for (const auto &res : reservations)
    {
        if (res.getStatus() == status)
        {
            return true;
        }
    }
    return false;
}

// Implementation of displayByStatus method
void ReservationSystem::displayByStatus(const string &status)
{
    cout << "============================================ RESERVATIONS - STATUS: " << toUpperCase(status) << " ===========================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(10) << "Guests" << setw(15) << "Date" << setw(15) << "Time"
         << setw(15) << "Status" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
    for (const auto &res : reservations)
    {
        if (res.getStatus() == status)
        {
            res.displayReservations();
        }
    }
}

string ReservationSystem::getStatus(const string &id) const
{
    for (const auto &res : reservations)
    {
        if (res.getID() == id)
        {
            return res.getStatus();
        }
    }
    return "";
}

// Implementation of displayAll method
void ReservationSystem::displayAll()
{
    cout << "\n=================================================== ALL RESERVATIONS ===================================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(10) << "Guests" << setw(15) << "Date" << setw(15) << "Time"
         << setw(15) << "Status" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------\n";
    for (const auto &res : reservations)
    {
        res.displayReservations();
    }
}

// approveReservation method - pending to approved
void ReservationSystem::approveReservation(const string &id)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getStatus() == STATUS[0]) // STATUS[0] = "Pending"
        {
            res.editReservation(res.getGuestCount(), res.getDate(), res.getTime()); // optional update
            res.setStatus(STATUS[1]);                                               // STATUS[1] = "Approved"
            return;
        }
    }
    cout << "Reservation either not found or not pending.\n";
}

// settlePayment method - approved to settled
void ReservationSystem::settlePayment(const string &id)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getStatus() == STATUS[1]) // STATUS[1] = "Approved"
        {
            res.setStatus(STATUS[2]); // STATUS[2] = "Settled"
            return;
        }
    }
    cout << "Reservation must be approved before settling payment.\n";
}

void ReservationSystem::cancelReservation(const string &id)
{
    for (auto it = reservations.begin(); it != reservations.end(); ++it)
    {
        if (it->getID() == id)
        {
            reservations.erase(it);
            return;
        }
    }
}

bool ReservationSystem::exists(const string &id)
{
    for (const auto &res : reservations)
    {
        if (res.getID() == id)
            return true;
    }
    return false;
}

bool ReservationSystem::isEmpty() const
{
    return reservations.empty();
}

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
        cout << "Select Payment Method:\n1. Maya\n2. GCash\n3. Card\nEnter your choice (Maya, GCash, Card): ";
        getline(cin, input);
        input = toUpperCase(input);

        if (input == "MAYA")
            return 1;
        else if (input == "GCASH")
            return 2;
        else if (input == "CARD")
            return 3;
        else
            cout << "Invalid payment method. Please enter one of the following: Maya, GCash, or Card.\n\n";
    }
}

ReservationSystem rs;

// Customer Menu
void customerMenu()
{
    int choice;
    bool condition = true;

    while (condition)
    {
        cout << "\n=========== CUSTOMER MENU ===========\n[1] Make reservation\n[2] Edit reservation\n[3] View Reservation\n[4] Cancel reservation\n[5] Settle Payment\n[6] Back to main menu\n";
        cout << "=====================================\n";
        choice = getValidInt("Enter choice: ", 1, 6);
        cout << "\n";

        switch (choice)
        {
        // make reservation
        case 1:
        {
            string name, phoneNo, date, time;
            bool isValidPhoneNo = false, isValidGC = false;
            int guestCount;

            cout << "\n====================== MAKE RESERVATION ======================\n";
            do
            {
                cout << "Name: ";
                getline(cin, name);
                if (name.empty())
                {
                    cout << "Input cannot be empty! Please enter a valid name.\n";
                }
            } while (name.empty());

            do
            {
                cout << "Phone Number (09XXXXXXXXX): ";
                getline(cin, phoneNo);
                if (phoneNo.empty())
                {
                    cout << "Input cannot be empty! Please enter a valid contact number.\n";
                }
                else if (phoneNo.length() != 11 || phoneNo[0] != '0' || phoneNo[1] != '9')
                {
                    cout << "Invalid Contact Number! Please enter a valid contact number.\n";
                }
                else
                {
                    isValidPhoneNo = true;
                }
            } while (!isValidPhoneNo);

            bool validNumber = false;
            do
            {
                guestCount = getValidInt("Number of Guests: ", 1, 100);
                validNumber = true;
            } while (!validNumber);

            do
            {
                cout << "Date (MM-DD-YYYY): ";
                getline(cin, date);
                if (date.empty())
                {
                    cout << "Date cannot be empty! Please enter a valid date.\n";
                }
            } while (date.empty());

            do
            {
                cout << "Time (HH:MM AM/PM): ";
                getline(cin, time);
                if (time.empty())
                {
                    cout << "Time cannot be empty! Please enter a valid time.\n";
                }
            } while (time.empty());

            rs.addReservation(name, phoneNo, guestCount, date, time);
            cout << "==============================================================\n";

            break;
        }

        // edit reservation
        case 2:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to edit.\n";
                break;
            }

            if (!rs.hasStatus(STATUS[0])) // STATUS[0] = "Pending"
            {
                cout << "No pending reservations to edit.\n";
                break;
            }

            rs.displayByStatus(STATUS[0]); // STATUS[0] = "Pending"

            string id, confirm;
            cout << "Enter Reservation ID to edit: ";
            getline(cin, id);
            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.exists(id))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            if (rs.getStatus(id) != STATUS[0]) // Check if the status is "Pending"
            {
                cout << "Only reservations with 'Pending' status can be edited.\n";
                break;
            }

            do
            {
                cout << "Are you sure you want to edit reservation ID " << id << "? (Y/N): ";
                getline(cin, confirm);
                confirm = toUpperCase(confirm);
                if (confirm == "Y")
                {
                    rs.editReservation(id);
                }
                else if (confirm == "N")
                {
                    cout << "Edit Cancelled.\n";
                }
                else if (confirm.empty())
                {
                    cout << "Confirmation cannot be empty! Please enter Y or N.\n";
                }
                else
                {
                    cout << "Invalid Input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");
            break;
        }

        // view reservation
        case 3:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to display.\n";
                break;
            }
            rs.displayAll();
            break;
        }

        // cancel reservation
        case 4:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to cancel.\n";
                break;
            }

            rs.displayAll();

            string id, confirm;
            cout << "Enter Reservation ID to cancel: ";
            getline(cin, id);

            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.exists(id))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            do
            {
                cout << "Are you sure you want to cancel reservation ID " << id << "? (Y/N): ";
                getline(cin, confirm);
                confirm = toUpperCase(confirm);
                if (confirm == "Y")
                {
                    rs.cancelReservation(id);
                    cout << "Reservation ID " << id << " has been cancelled successfully.\n";
                }
                else if (confirm == "N")
                {
                    cout << "Cancellation aborted.\n";
                }
                else if (confirm.empty())
                {
                    cout << "Confirmation cannot be empty! Please enter Y or N.\n";
                }
                else
                {
                    cout << "Invalid Input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");
            break;
        }

        // settle payment
        case 5:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to settle payment.\n";
                break;
            }

            if (!rs.hasStatus(STATUS[1])) // STATUS[1] = "Approved"
            {
                cout << "No approved reservations to settle payments.\n";
                break;
            }

            rs.displayByStatus(STATUS[1]); // STATUS[1] = "Approved"

            string id, confirm;
            cout << "Enter Reservation ID to settle payment: ";
            getline(cin, id);

            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.exists(id))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            if (rs.getStatus(id) != STATUS[1]) // Check if the status is "Approved"
            {
                cout << "Reservation ID " << id << " is either not approved yet or already settled.\n";
                break;
            }

            do
            {
                cout << "Are you sure you want to settle payment for reservation ID " << id << "? (Y/N): ";
                getline(cin, confirm);
                confirm = toUpperCase(confirm);
                if (confirm == "Y")
                {
                    rs.settlePayment(id);
                    int method = getValidPaymentMethodInput();
                    Payment *payment = Payment::getInstance();

                    switch (method)
                    {
                    case 1:
                        payment->setStrategy(new Maya());
                        break;
                    case 2:
                        payment->setStrategy(new GCash());
                        break;
                    case 3:
                        payment->setStrategy(new Card());
                        break;
                    }
                    cout << "Payment method: ";
                    payment->executePayment();
                    cout << "\n";

                    string paymentType;
                    switch (method)
                    {
                    case 1:
                        paymentType = "Maya";
                        break;
                    case 2:
                        paymentType = "GCash";
                        break;
                    case 3:
                        paymentType = "Credit / Debit Card";
                        break;

                        cout << "Payment for reservation ID " << id << " has been settled successfully!\n";
                    }
                }

                else if (confirm == "N")
                {
                    cout << "Payment settlement aborted.\n";
                }

                else if (confirm.empty())
                {
                    cout << "Confirmation cannot be empty! Please enter Y or N.\n";
                }
                else
                {
                    cout << "Invalid Input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");
            break;
        }

        case 6:
        {
            cout << "Returning to main menu...\n\n";
            condition = false;
            return;
        }
        }
    }
}

void adminMenu()
{
    int choice;
    bool condition = true;

    while (condition)
    {
        cout << "\n================ ADMIN MENU ================\n[1] View All Reservations\n[2] View and Approve Pending Reservations \n[3] Back to main menu\n";
        cout << "============================================\n";
        choice = getValidInt("Enter choice: ", 1, 3);
        cout << "\n";

        switch (choice)
        {
            // view reservations
        case 1:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to display.\n";
                break;
            }
            rs.displayAll();
            break;
        }

        // view pending & approve reservations
        case 2:
        {
            if (rs.isEmpty())
            {
                cout << "No reservations to display.\n";
                break;
            }

            if (!rs.hasStatus(STATUS[0])) // STATUS[0] = "Pending"
            {
                cout << "No pending reservations to display.\n";
                break;
            }

            rs.displayByStatus(STATUS[0]);

            string id, confirm;
            cout << "Enter Reservation ID to approve: ";
            getline(cin, id);

            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.exists(id))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            if (rs.getStatus(id) != STATUS[0]) // Check if the status is "Pending"
            {
                cout << "Reservation ID " << id << " is either approved or already settled.\n";
                break;
            }

            do
            {
                cout << "Are you sure you want to approve reservation ID " << id << "? (Y/N): ";
                getline(cin, confirm);
                confirm = toUpperCase(confirm);

                if (confirm == "Y")
                {
                    rs.approveReservation(id);
                    cout << "Reservation ID " << id << " has been approved successfully!\n";
                }

                else if (confirm == "N")
                {
                    break;
                }

                else if (confirm.empty())
                {
                    cout << "Confirmation cannot be empty! Please enter Y or N.\n";
                }

                else
                {
                    cout << "Invalid Input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");

            break;
        }

        // back to main menu
        case 3:
        {
            cout << "Returning to main menu...\n\n";
            condition = false;
            return;
        }
        }
    }
}

int main()
{
    bool condition = true;
    int choice;

    while (condition)
    {
        int choice;
        cout << "Welcome to Reserve Eat! : LOG-IN PAGE\n";
        cout << "=========== MAIN MENU ===========\n";
        cout << "[1] Customer\n[2] Admin\n[3] Exit\n";
        cout << "=================================\n";
        choice = getValidInt("Enter your choice: ", 1, 3);
        cout << "\n";

        switch (choice)
        {
        case 1:
        {
            customerMenu();
            break;
        }

        case 2:
        {
            adminMenu();
            break;
        }

        case 3:
        {
            cout << "Thank you for visiting Reserve Eat! Exiting program...\n";
            condition = false;
            break;
        }
        }
    }
    return 0;
}