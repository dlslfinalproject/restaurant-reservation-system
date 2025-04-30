#include <iostream>
#include <stdexcept>
using namespace std;

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
                while (condition)
                {
                    if (!isValidInteger(input))
                        throw invalid_argument("Invalid input. Please enter a valid menu option (1-4).");

                    choice = stoi(input);
                    if (choice < 1 || choice > 3)
                        throw out_of_range("Invalid menu option. Please choose from 1 to 3.");

                    string name, phoneNo, guestCount, date, time;

                    cout << "\nCUSTOMER MENU\n[1] Make reservation\n[2] Edit reservation\n[3] View Reservation\n[4] Cancel reservation\n[5] Back to main menu\n\nEnter choice: ";
                    getline(cin, input);
                    if (!isValidInteger(input))
                        throw invalid_argument("Invalid input. Please enter a valid menu option (1-5).");
                    if (choice < 1 || choice > 3)
                        throw out_of_range("Invalid menu option. Please choose from 1 to 5.");

                    if (choice == 1)
                    {
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

                    else if (choice == 2)
                    {
                        break;
                    }

                    else if (choice == 3)
                    {
                        break;
                    }

                    else if (choice == 4)
                    {
                        break;
                    }

                    else if (choice == 5)
                    {
                        break;
                    }

                    // Admin menu
                    else if (choice == 2)
                    {
                        while (condition)
                        {
                            cout << "\nADMIN MENU\n[1] View reservations\n[2] Approve reservation\n[3] Back to main menu\n\nEnter choice: ";
                            getline(cin, input);
                            if (!isValidInteger(input))
                                throw invalid_argument("Invalid input. Please enter a valid menu option (1-4).");
                            if (choice < 1 || choice > 3)
                                throw out_of_range("Invalid menu option. Please choose from 1 to 4.");
                        }
                    }

                    else if (choice == 3)
                    {
                        cout << "Exit...\n";
                        break;
                    }
                }
            }
        }

        catch (const exception &e)
        {
            cout << e.what() << "\n";
        }
    }

    return 0;
}