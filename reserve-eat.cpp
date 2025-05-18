// Author: Zurinee Irish M. Belo, Katherine Anne S. Liwanag, Jane Allyson L. Paray, and Jhenelle K.Alonzo

/* This program is a restaurant reservation system that allows users (customers and admin) to make reservations and approve reservations. The program applied the four pillars of OOP:
Encapsulation, Inheritance, Polymorphism, and Abstraction. It also implements the Singleton and Strategy Pattern for the Payment class to ensure that only one
instance of the payment method is created and used throughout the program. In generating reports, file handling is used for storing and retrieving settled reservation data. */

#include <iostream>  // Used for input/output operations
#include <vector>    // Used for dynamic array
#include <string>    // Used for string manipulation
#include <stdexcept> // Used for exception handling
#include <iomanip>   // Used for formatting output
#include <algorithm> // Used for various algorithmic operations
#include <fstream>   // Used for file operations
#include <ctime>     // Used for time-related functions
#include <regex>     // Used for regular expressions and pattern matching within strings
using namespace std; // Standard namespace

const string STATUS[] = {"Pending", "Approved", "Settled", "Rejected"}; // 0, 1, 2, 3
const int MAX_RESERVATIONS = 100;                                       // Maximum number of reservations

// Struct to hold user information
struct User
{
    string username;
    string password;
};

vector<User> users; // Vector to hold user data

bool userExists(const string &username);                               // Function to check if a user exists
bool authenticateUser(const string &username, const string &password); // Function to authenticate a user
void registerUser(const string &username, const string &password);     // Function to register a new user
void customerMenu(const string &username);                             // Function to display customer menu
void adminMenu();                                                      // Function to display admin menu

// Function to check if a user exists in the system
bool userExists(const string &username)
{
    for (const auto &user : users)
    {
        if (user.username == username)
        {
            return true;
        }
    }
    return false;
}

// Function to authenticate a user and return true if the username and password match
bool authenticateUser(const string &username, const string &password)
{
    for (const auto &user : users)
    {
        if (user.username == username && user.password == password)
        {
            return true;
        }
    }
    return false;
}

// Function to register a new user
void registerUser(const string &username, const string &password)
{
    users.push_back({username, password});
}

// Forward declaration of menu functions for customer and admin
void customerMenu(const string &username);
void adminMenu();

// Function to convert a string to uppercase
string toUpperCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// Function to convert a string to lowercase
string toLowerCase(const string &str)
{
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
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

// Function to check if a string contains only digits
bool isAllDigits(const string &str)
{
    for (char c : str)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

// Function to check if a date is valid
bool isValidDate(const string &date)
{
    if (date.length() != 10 || date[2] != '-' || date[5] != '-')
        return false;

    string mm = date.substr(0, 2);
    string dd = date.substr(3, 2);
    string yyyy = date.substr(6, 4);

    if (!isAllDigits(mm) || !isAllDigits(dd) || !isAllDigits(yyyy))
        return false;

    int month = stoi(mm);
    int day = stoi(dd);
    int year = stoi(yyyy);

    // Get current year
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    int currentYear = now->tm_year + 1900;

    // Disallow years before the current year
    if (year < currentYear)
        return false;

    if (month < 1 || month > 12)
        return false;

    if (day < 1 || day > 31)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
        daysInMonth[1] = 29;

    if (day > daysInMonth[month - 1])
        return false;

    return true;
}

// Function to check if a time is valid
bool isValidTime(const string &time)
{
    if (time.length() != 8 || time[2] != ':' || time[5] != ' ')
        return false;

    string upperTime = toUpperCase(time);

    if ((upperTime[6] != 'A' && upperTime[6] != 'P') || upperTime[7] != 'M')
        return false;

    string hh = upperTime.substr(0, 2);
    string mm = upperTime.substr(3, 2);

    int hour, minute;
    try
    {
        hour = stoi(hh);
        minute = stoi(mm);
    }
    catch (...)
    {
        return false;
    }

    if (hour < 1 || hour > 12)
        return false;
    if (minute < 0 || minute > 59)
        return false;

    return true;
}

string addTwoHours(const string &startTime)
{
    int hour = stoi(startTime.substr(0, 2));
    int minute = stoi(startTime.substr(3, 2));
    string meridian = startTime.substr(6, 2); // "AM" or "PM"

    hour += 2;
    if (hour > 12)
    {
        hour -= 12;
        meridian = (meridian == "AM") ? "PM" : "AM";
    }
    else if (hour == 12)
    {
        meridian = (meridian == "AM") ? "PM" : "AM";
    }

    // Format back to HH:MM AM/PM
    stringstream ss;
    ss << setw(2) << setfill('0') << hour << ":"
       << setw(2) << setfill('0') << minute << " "
       << meridian;
    return ss.str();
}

// Class to represent a reservation
class Reservation
{
private:
    string id, username, name, phoneNo, date, startTime, endTime, status;
    int tablesReserved;

public:
    Reservation() : tablesReserved(0) {}
    Reservation(string id, string username, string name, string phoneNo, int tablesReserved, string date, string startTime, string endTime, string status) : id(id), username(username), name(name), phoneNo(phoneNo), tablesReserved(tablesReserved), date(date), startTime(startTime), endTime(endTime), status(status) {}

    string getID() const { return id; }
    string getUsername() const { return username; }
    string getName() const { return name; }
    string getPhoneNo() const { return phoneNo; }
    string getStatus() const { return status; }
    int getTablesReserved() const { return tablesReserved; }
    string getDate() const { return date; }
    string getStartTime() const { return startTime; }
    string getEndTime() const { return endTime; }
    void setStatus(const string &newStatus) { status = newStatus; }

    void editReservation(int tReserved, string dt, string stm, string etm)
    {
        tablesReserved = tReserved;
        date = dt;
        startTime = stm;
        endTime = etm;
    }

    void displayReservations() const
    {
        cout << left << setw(20) << id << setw(30) << name << setw(20) << phoneNo
             << setw(20) << tablesReserved << setw(15) << date << setw(15) << startTime
             << setw(15) << endTime << setw(15) << status << endl;
        cout << "==============================================================================================================================================================\n";
    }
};

// Class to represent the reservation system
class ReservationSystem
{
private:
    vector<Reservation> reservations;
    int reservationCounter = 0;

public:
    //  Reservation System methods
    string generateID();
    void addReservationSilent(const string &username, const string &name, const string &phoneNo, int tablesReserved, const string &date, const string &time);
    void initializeSampleReservations();
    void logToFile(const string &logEntry);
    void addReservation(const string &username, const string &name, const string &phoneNo, int tablesReserved, const string &date, const string &time);
    int getAvailableTables(const string &date, const string &startTime, const string &endTime) const;
    void editReservation(const string &id, const string &username);
    void rejectReservation(const string &id);
    void cancelReservation(const string &id);
    void displayAll();
    bool hasStatus(const string &status) const;
    bool hasUserReservationWithStatus(const string &status, const string &username) const;
    void displayByStatus(const string &status);
    void displayUserReservations(const string &username);
    void displayUserReservationByStatus(const string &status, const string &username);
    string getStatus(const string &id) const;
    void approveReservation(const string &id);
    void settlePayment(const string &id, const string &paymentType);
    bool exists(const string &id);
    bool existsForUser(const string &id, const string &username) const;
    bool isEmpty() const;
    bool isUserReservationEmpty(const string &username) const;
};

// Add a reservation without user interaction
void ReservationSystem::addReservationSilent(const string &username, const string &name, const string &phoneNo, int tablesReserved, const string &date, const string &startTime)
{
    string id = generateID();
    string endTime = addTwoHours(startTime); // Assuming you have this function
    reservations.emplace_back(id, username, name, phoneNo, tablesReserved, date, startTime, endTime, STATUS[0]);
}

// Initialize sample reservations
void ReservationSystem::initializeSampleReservations()
{
    addReservationSilent("zurineeirish@gmail.com", "Zurinee Belo", "09868366562", 2, "09-30-2025", "09:00 PM");
    addReservationSilent("jhenelle@gmail.com", "Jhenelle Alonzo", "09926639727", 1, "05-26-2025", "09:00 PM");
    addReservationSilent("janeallyson@gmail.com", "Jane Paray", "09171234567", 1, "06-15-2025", "07:00 PM");
    addReservationSilent("katherineanne@gmail.com", "Katherine Liwanag", "09171234567", 1, "06-15-2025", "07:00 PM");
}

// Implementation of generateID method
string ReservationSystem::generateID()
{
    // Load counter from file
    ifstream inFile("counter.txt");
    if (inFile.is_open())
    {
        inFile >> reservationCounter;
        inFile.close();
    }

    reservationCounter++; // Increment counter

    // Save updated counter to file
    ofstream outFile("counter.txt");
    if (outFile.is_open())
    {
        outFile << reservationCounter;
        outFile.close();
    }

    return to_string(reservationCounter);
}

// Implementation of recording logs to file
void ReservationSystem::logToFile(const string &logEntry)
{
    ofstream log("reservation_log.txt", ios::app); // Append mode
    if (log.is_open())
    {
        time_t now = time(0);
        string timestamp = ctime(&now);
        timestamp.pop_back(); // Remove trailing newline
        log << "[" << timestamp << "] " << logEntry << "\n";
        log.close();
    }
}

// Adds a reservation to the system
void ReservationSystem::addReservation(const string &username, const string &name, const string &phoneNo, int tablesReserved, const string &date, const string &startTime)
{
    string endTime = addTwoHours(startTime);
    string id = generateID();
    reservations.emplace_back(id, username, name, phoneNo, tablesReserved, date, startTime, endTime, STATUS[0]);
    cout << "Reservation made successfully! Reservation ID: " << id << endl;
}

int ReservationSystem::getAvailableTables(const string &date, const string &startTime, const string &endTime) const
{
    int totalTables = 10;
    int bookedTables = 0;

    for (const auto &res : reservations)
    {
        if (res.getDate() == date)
        {
            // Check for time overlap
            if (!(endTime <= res.getStartTime() || startTime >= res.getEndTime()))
            {
                bookedTables += res.getTablesReserved();
            }
        }
    }

    return totalTables - bookedTables;
}

// Enables the user to edit their reservation
void ReservationSystem::editReservation(const string &id, const string &username)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getUsername() == username)
        {
            // Only allow editing if status is "Pending"
            if (res.getStatus() != "Pending")
            {
                cout << "Only reservations with 'Pending' status can be edited.\n";
                return;
            }

            string newDate, newStartTime, newTR;
            int newTablesReserved = res.getTablesReserved();
            bool validTR = false;

            do
            {
                cout << "Enter new date (MM-DD-YYYY): ";
                getline(cin, newDate);
                if (newDate.empty())
                {
                    cout << "Date cannot be empty! Please enter a valid date.\n";
                }
                else if (!isValidDate(newDate))
                {
                    cout << "Invalid date format or value! Please follow MM-DD-YYYY.\n";
                    newDate.clear(); // Clear invalid input to retry
                }
            } while (newDate.empty() || !isValidDate(newDate));

            if (newDate.empty())
                newDate = res.getDate();

            do
            {
                cout << "Enter new time (HH:MM AM/PM): ";
                getline(cin, newStartTime);
                if (newStartTime.empty())
                {
                    cout << "Time cannot be empty! Please enter a valid time.\n";
                }
                else if (!isValidTime(newStartTime))
                {
                    cout << "Invalid time format or value! Please follow HH:MM AM/PM.\n";
                    newStartTime.clear(); // Clear invalid input to retry
                }
            } while (newStartTime.empty() || !isValidTime(newStartTime));
            if (newStartTime.empty())
                newStartTime = res.getStartTime();

            string newEndTime = addTwoHours(newStartTime);
            int availableTablesForNewTime = getAvailableTables(newDate, newStartTime, newEndTime) + res.getTablesReserved();

            do
            {
                cout << "Enter new number of tables: ";
                getline(cin, newTR);
                if (newTR.empty())
                {
                    validTR = true; // Keep the existing value
                }
                else
                {
                    bool isValidNum = true;
                    for (char c : newTR)
                    {
                        if (!isdigit(c))
                        {
                            isValidNum = false;
                            break;
                        }
                    }
                    if (!isValidNum)
                    {
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }
                    int tempTables = stoi(newTR);
                    if (tempTables > 0 && tempTables <= 10 && tempTables <= availableTablesForNewTime)
                    {
                        newTablesReserved = tempTables;
                        validTR = true;
                    }
                    else
                    {
                        cout << "Invalid number of tables. Please enter a value between 1 and " << min(10, availableTablesForNewTime) << ".\n";
                    }
                }
            } while (!validTR);

            res.editReservation(newTablesReserved, newDate, newStartTime, newEndTime);
            cout << "Reservation updated successfully!\n";
            return;
        }
    }

    cout << "Reservation ID not found.\n";
}

// Identifies if a reservation with a specific status exists
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

// Identifies if a user has a reservation with a specific status
bool ReservationSystem::hasUserReservationWithStatus(const string &status, const string &username) const
{
    for (const auto &res : reservations)
    {
        if (res.getStatus() == status && res.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}

// Displays all reservations with a specific status
void ReservationSystem::displayByStatus(const string &status)
{
    cout << "ALL " << toUpperCase(status) << " RESERVATIONS" << endl;
    cout << "==============================================================================================================================================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(20) << "Reserved Table" << setw(15) << "Date" << setw(15) << "Start Time"
         << setw(15) << "End Time" << setw(15) << "Status" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    for (const auto &res : reservations)
    {
        if (res.getStatus() == status)
        {
            res.displayReservations();
        }
    }
}

// Displays all reservations for a specific user
void ReservationSystem::displayUserReservations(const string &username)
{
    cout << "User: " << username << endl;
    cout << "======================================================================== RESERVATIONS ========================================================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(20) << "Reserved Table" << setw(15) << "Date" << setw(15) << "Start Time"
         << setw(15) << "End Time" << setw(15) << "Status" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    for (const auto &res : reservations)
    {
        if (res.getUsername() == username)
            res.displayReservations();
    }
}

// Displays all reservations for a specific user with a specific status
void ReservationSystem::displayUserReservationByStatus(const string &status, const string &username)
{
    cout << "User: " << username << endl;
    cout << "ALL " << toUpperCase(status) << " RESERVATIONS" << endl;
    cout << "==============================================================================================================================================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(20) << "Reserved Table" << setw(15) << "Date" << setw(15) << "Start Time"
         << setw(15) << "End Time" << setw(15) << "Status" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    for (const auto &res : reservations)
    {
        if (res.getUsername() == username && res.getStatus() == status)
        {
            res.displayReservations();
        }
    }
}

// Retrieves the status of a reservation by ID
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

// Displays all reservations in the system
void ReservationSystem::displayAll()
{
    cout << "\n====================================================================== ALL RESERVATIONS ======================================================================\n";
    cout << left << setw(20) << "Reservation ID" << setw(30) << "Name" << setw(20) << "Phone Number"
         << setw(20) << "Reserved Table" << setw(15) << "Date" << setw(15) << "Start Time"
         << setw(15) << "End Time" << setw(15) << "Status" << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------\n";
    for (const auto &res : reservations)
    {
        res.displayReservations();
    }
}

// Enables the admin to approve a reservation
void ReservationSystem::approveReservation(const string &id)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getStatus() == STATUS[0]) // STATUS[0] = "Pending"
        {
            res.editReservation(res.getTablesReserved(), res.getDate(), res.getStartTime(), res.getEndTime()); // optional update
            res.setStatus(STATUS[1]);                                                                          // STATUS[1] = "Approved"
            return;
        }
    }
    cout << "Reservation either not found or not pending.\n";
}

// Enables the admin to reject a reservation
void ReservationSystem::rejectReservation(const string &id)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getStatus() == STATUS[0]) // STATUS[0] = "Pending"
        {
            res.setStatus(STATUS[3]); // STATUS[3] = "Rejected"
            return;
        }
    }
    cout << "Reservation either not found or not pending.\n";
}

// Enables the user to settle payment for a reservation
void ReservationSystem::settlePayment(const string &id, const string &paymentType)
{
    for (auto &res : reservations)
    {
        if (res.getID() == id && res.getStatus() == STATUS[1]) // STATUS[1] = "Approved"
        {
            res.setStatus(STATUS[2]); // STATUS[2] = "Settled"

            // Log the settled reservation
            ofstream logFile("settled_reservations.txt", ios::app);
            if (logFile.is_open())
            {
                time_t now = time(0);
                string dt = ctime(&now);
                dt.pop_back(); // remove newline

                logFile << "RESERVATION ID: " << res.getID()
                        << " | Name: " << res.getName()
                        << " | Phone: " << res.getPhoneNo()
                        << " | Reserved Table: " << res.getTablesReserved()
                        << " | Date: " << res.getDate()
                        << " | Start Time: " << res.getStartTime()
                        << " | End Time: " << res.getEndTime()
                        << " | Status: Settled"
                        << " | Payment Method: " << paymentType
                        << " | Settled At: " << dt << endl;
                logFile.close();
            }

            return;
        }
    }

    cout << "Reservation must be approved before settling payment.\n";
}

// Enables the user to cancel a reservation
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

// Checks if a reservation with a specific ID exists
bool ReservationSystem::exists(const string &id)
{
    for (const auto &res : reservations)
    {
        if (res.getID() == id)
            return true;
    }
    return false;
}

// Checks if a reservation with a specific ID exists for a specific user
bool ReservationSystem::existsForUser(const string &id, const string &username) const
{
    for (const auto &res : reservations)
    {
        if (res.getID() == id && res.getUsername() == username)
            return true;
    }
    return false;
}

// Checks if the reservation system is empty
bool ReservationSystem::isEmpty() const
{
    return reservations.empty();
}

// Checks if a user has any reservations
bool ReservationSystem::isUserReservationEmpty(const string &username) const
{
    for (const auto &res : reservations)
    {
        if (res.getUsername() == username)
        {
            return false;
        }
    }
    return true;
}

// Class to represent the payment method strategy
class PaymentMethod
{
public:
    virtual void paymentMethod() = 0;
    virtual ~PaymentMethod() {}
};

// Derived classes for different payment methods
class Maya : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        string accountNo, authCode;
        regex accRegex("^09\\d{9}$");
        regex authRegex("^\\d{6}$");
        cout << "Enter Maya Account Number (09XXXXXXXXX): ";
        getline(cin, accountNo);
        while (!regex_match(accountNo, accRegex))
        {
            cout << "Invalid account number! Please try again: ";
            getline(cin, accountNo);
        }

        cout << "Enter Maya Authentication Code (6 digits): ";
        getline(cin, authCode);
        while (!regex_match(authCode, authRegex))
        {
            cout << "Invalid authentication code! Please try again: ";
            getline(cin, authCode);
        }

        cout << "Payment successful via Maya.\n";
    }
};

// Derived class for GCash payment method
class GCash : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        string accountNo, authCode;
        regex accRegex("^09\\d{9}$");
        regex authRegex("^\\d{6}$");
        cout << "Enter GCash Account Number (09XXXXXXXXX): ";
        getline(cin, accountNo);
        while (!regex_match(accountNo, accRegex))
        {
            cout << "Invalid account number! Please try again: ";
            getline(cin, accountNo);
        }

        cout << "Enter GCash Authentication Code (6 digits): ";
        getline(cin, authCode);
        while (!regex_match(authCode, authRegex))
        {
            cout << "Invalid authentication code! Please try again: ";
            getline(cin, authCode);
        }

        cout << "Payment successful via GCash.\n";
    }
};

// Derived class for Card payment method
class Card : public PaymentMethod
{
public:
    void paymentMethod() override
    {
        string cardNo, name, expiry, cvv;
        regex cardRegex("^\\d{16}$");
        regex expiryRegex("^(0[1-9]|1[0-2])/\\d{4}$");
        regex cvvRegex("^\\d{3}$");

        cout << "Enter Card Number (16 digits): ";
        getline(cin, cardNo);
        while (!regex_match(cardNo, cardRegex))
        {
            cout << "Invalid card number! Please try again: ";
            getline(cin, cardNo);
        }

        cout << "Enter Cardholder's Name: ";
        getline(cin, name);
        while (name.empty())
        {
            cout << "Name cannot be empty! Please try again: ";
            getline(cin, name);
        }

        cout << "Enter Expiry Date (MM/YYYY): ";
        getline(cin, expiry);
        while (!regex_match(expiry, expiryRegex))
        {
            cout << "Invalid expiry date! Please try again: ";
            getline(cin, expiry);
        }

        cout << "Enter CVV (3 digits): ";
        getline(cin, cvv);
        while (!regex_match(cvv, cvvRegex))
        {
            cout << "Invalid CVV! Please try again: ";
            getline(cin, cvv);
        }

        cout << "Payment successful via Credit/Debit Card.\n";
    }
};

// Singleton class for payment processing
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

Payment *Payment::instance = nullptr; // Singleton instance

// Function to get a valid payment method input
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
            cout << "Invalid payment method! Please enter one of the following: Maya, GCash, or Card.\n\n";
    }
}

ReservationSystem rs; // Global instance of ReservationSystem

// Customer Menu
void customerMenu(const string &username)
{
    int choice;
    bool condition = true;

    while (condition)
    {
        cout << "\n=========== CUSTOMER MENU ===========\n[1] Make reservation\n[2] Edit reservation\n[3] View Reservation\n[4] Cancel reservation\n[5] Settle Payment\n[6] Log out\n";
        cout << "=====================================\n";
        choice = getValidInt("Enter choice: ", 1, 6);
        cout << "\n";

        switch (choice)
        {
        // Make reservation
        case 1:
        {
            string name, phoneNo, date, startTime;
            bool isValidPhoneNo = false, isValidGC = false;
            int tablesNeeded;

            cout << "\n=========================== MAKE RESERVATION ===========================\n";
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

            do
            {
                cout << "Date (MM-DD-YYYY): ";
                getline(cin, date);
                if (date.empty())
                {
                    cout << "Date cannot be empty! Please enter a valid date.\n";
                }
                else if (!isValidDate(date))
                {
                    cout << "Invalid date format or value! Please follow MM-DD-YYYY.\n";
                    date.clear(); // Clear invalid input to retry
                }
            } while (date.empty() || !isValidDate(date));

            do
            {
                cout << "Start Time (HH:MM AM/PM): ";
                getline(cin, startTime);
                if (startTime.empty())
                {
                    cout << "Time cannot be empty! Please enter a valid time.\n";
                }
                else if (!isValidTime(startTime))
                {
                    cout << "Invalid time format or value! Please follow HH:MM AM/PM.\n";
                    startTime.clear(); // Clear invalid input to retry
                }
            } while (startTime.empty() || !isValidTime(startTime));

            int availableTables = rs.getAvailableTables(date, startTime, addTwoHours(startTime)); // We need to implement this

            cout << "\nAvailable tables for " << date << " at " << startTime << " - " << addTwoHours(startTime) << ": " << availableTables << " / 10\n";

            if (availableTables <= 0)
            {
                cout << "Sorry, there are no tables available at this time. Please try a different time or date.\n";
                break;
            }

            bool validNumber = false;
            do
            {
                tablesNeeded = getValidInt("Number of Tables to reserve: ", 1, min(10, availableTables));
                validNumber = true;
            } while (!validNumber);

            cout << "========================================================================\n";
            rs.addReservation(username, name, phoneNo, tablesNeeded, date, startTime);

            break;
        }

        // Edit reservation
        case 2:
        {
            if (rs.isUserReservationEmpty(username))
            {
                cout << "No reservations to edit.\n";
                break;
            }

            if (!rs.hasUserReservationWithStatus(STATUS[0], username)) // STATUS[0] = "Pending"
            {
                cout << "No pending reservations to edit.\n";
                break;
            }

            rs.displayUserReservationByStatus(STATUS[0], username); // STATUS[0] = "Pending"

            string id, confirm;
            cout << "Enter Reservation ID to edit: ";
            getline(cin, id);
            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.existsForUser(id, username))
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
                    rs.editReservation(id, username);
                }
                else if (confirm == "N")
                {
                    cout << "Edit cancelled.\n";
                }
                else if (confirm.empty())
                {
                    cout << "Confirmation cannot be empty! Please enter Y or N.\n";
                }
                else
                {
                    cout << "Invalid input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");
            break;
        }

        // View reservation
        case 3:
        {
            if (rs.isUserReservationEmpty(username))
            {
                cout << "No reservations to display.\n";
                break;
            }
            rs.displayUserReservations(username);
            break;
        }

        // Cancel reservation
        case 4:
        {
            if (rs.isUserReservationEmpty(username))
            {
                cout << "No reservations to cancel.\n";
                break;
            }
            bool hasDisplay = false;

            if (rs.hasUserReservationWithStatus(STATUS[0], username))
            {
                rs.displayUserReservationByStatus(STATUS[0], username);
                cout << "\n";
                hasDisplay = true;
            }

            if (rs.hasUserReservationWithStatus(STATUS[1], username))
            {
                rs.displayUserReservationByStatus(STATUS[1], username);
                hasDisplay = true;
            }

            if (!hasDisplay)
            {
                cout << "No cancellable reservations found.\n";
                break;
            }

            string id, confirm;
            cout << "Enter Reservation ID to cancel: ";
            getline(cin, id);

            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.existsForUser(id, username))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            if (rs.getStatus(id) == STATUS[2] || rs.getStatus(id) == STATUS[3]) // Check if the status is "Pending"
            {
                cout << "Settled or Rejected Reservation cannot be cancelled.\n";
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
                    cout << "Invalid input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");

            break;
        }

        // Settle payment
        case 5:
        {
            if (rs.isUserReservationEmpty(username))
            {
                cout << "No reservations to settle payment.\n";
                break;
            }

            if (!rs.hasUserReservationWithStatus(STATUS[1], username)) // STATUS[1] = "Approved"
            {
                cout << "No approved reservations to settle payments.\n";
                break;
            }

            rs.displayUserReservationByStatus(STATUS[1], username); // STATUS[1] = "Approved"

            string id, confirm;
            cout << "Enter Reservation ID to settle payment: ";
            getline(cin, id);

            if (id.empty())
            {
                cout << "Reservation ID cannot be empty! Please enter a valid ID.\n";
                break;
            }

            if (!rs.existsForUser(id, username))
            {
                cout << "Reservation with ID " << id << " does not exist.\n";
                break;
            }

            if (rs.getStatus(id) != STATUS[1]) // Check if the status is "Approved"
            {
                cout << "Reservation ID " << id << " is either rejected, not approved yet, or already settled.\n";
                break;
            }

            do
            {
                cout << "Are you sure you want to settle payment for reservation ID " << id << "? (Y/N): ";
                getline(cin, confirm);
                confirm = toUpperCase(confirm);
                if (confirm == "Y")
                {
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
                    cout << "Payment Processing...\n";
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
                    rs.settlePayment(id, paymentType);
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
                    cout << "Invalid input! Please enter Y or N only.\n";
                }

            } while (confirm != "Y" && confirm != "N");
            break;
        }

        case 6:
        {
            cout << "Logging out...\n\n";
            condition = false;
            return;
        }
        }
    }
}

// Admin menu
void adminMenu()
{
    int choice;
    bool condition = true;

    while (condition)
    {
        cout << "\n================ ADMIN MENU ================\n[1] View All Reservations\n[2] Review Reservations \n[3] Log out\n";
        cout << "============================================\n";
        choice = getValidInt("Enter choice: ", 1, 3);
        cout << "\n";

        switch (choice)
        {
            // View reservations
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

        // View pending & approve reservations
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

            string id, action, confirm;
            condition = true;
            cout << "Enter Reservation ID to review: ";
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
                cout << "Reservation ID " << id << " status is already updated.\n";
                break;
            }

            do
            {
                cout << "Approve or reject reservation ID " << id << "? : "; // Enter "Approve" or "Reject"
                getline(cin, action);
                action = toUpperCase(action);

                if (action.empty())
                {
                    cout << "Review option cannot be empty! Please enter Approve or Reject.\n";
                }
                else if (action == "APPROVE" || action == "REJECT")
                {
                    do
                    {
                        cout << "Are you sure you want to " << toLowerCase(action) << " reservation ID " << id << "? (Y/N): ";
                        getline(cin, confirm);
                        confirm = toUpperCase(confirm);

                        if (confirm == "Y")
                        {
                            if (action == "APPROVE")
                            {
                                rs.approveReservation(id);
                                cout << "Reservation ID " << id << " has been approved successfully!\n";
                            }
                            else
                            {
                                rs.rejectReservation(id);
                                cout << "Reservation ID " << id << " has been rejected.\n";
                            }
                            break;
                        }
                        else if (confirm == "N")
                        {
                            cout << action << " cancelled.\n";
                            break;
                        }
                        else
                        {
                            cout << "Invalid input! Please enter Y or N only.\n";
                        }
                    } while (confirm != "Y" && confirm != "N");

                    break;
                }
                else
                {
                    cout << "Invalid input! Please enter Approve or Reject only.\n";
                }

            } while (condition);
            break;
        }

        // Back to main menu
        case 3:
        {
            cout << "Logging out...\n\n";
            condition = false;
            return;
        }
        }
    }
}

int main()
{
    rs.initializeSampleReservations(); // Initialize sample reservations
    bool condition = true;
    int choice;

    // Main menu
    while (condition)
    {
        int choice;
        cout << "Welcome to Reserve Eat!\n";
        cout << "=========== MAIN MENU ===========\n";
        cout << "[1] Customer\n[2] Admin\n[3] Exit\n";
        cout << "=================================\n";
        choice = getValidInt("Enter your choice: ", 1, 3);
        cout << "\n";

        switch (choice)
        {
        case 1:
        {
            string username, password;
            bool isAuthenticated = false;
            cout << "========== CUSTOMER LOG IN ==========" << endl;
            do
            {
                cout << "Username: ";
                getline(cin, username);
                username = toUpperCase(username);
                if (username.empty())
                {
                    cout << "Input cannot be empty! Please enter a valid username.\n";
                }
            } while (username.empty());

            do
            {
                cout << "Password: ";
                getline(cin, password);
                if (password.empty())
                {
                    cout << "Input cannot be empty! Please enter a valid password.\n";
                }
                else if (password.length() < 8)
                {
                    cout << "Invalid input! Password must be at least 8 characters long.\n";
                }
            } while (password.empty() || password.length() < 8);
            cout << "=====================================" << endl;

            if (userExists(username))
            {
                // User exists, so authenticate the password
                if (authenticateUser(username, password))
                {
                    cout << "Login successful!\n";
                    customerMenu(username); // Proceed to customer menu
                }
                else
                {
                    cout << "Incorrect password! Please try again.\n";
                }
            }
            else
            {
                // User doesn't exist, register new user
                cout << "New user registered successfully!\n";
                registerUser(username, password);
                customerMenu(username); // Proceed to customer menu after registration
            }
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