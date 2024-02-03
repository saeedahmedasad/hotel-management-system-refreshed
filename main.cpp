#include <iostream>
#include <conio.h>
#include <iomanip>
#include <Windows.h>

#include "./bin/validate.h"
#include "./bin/encryption.h"
#include "./bin/fileSystem.h"

using namespace std;

const string HOTEL_NAME = "Phooli Kothi";

// Center the text
#define RESET "\033[0m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
void centerText(const std::string &text, string color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);

    int consoleWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
    int padding = (consoleWidth - static_cast<int>(text.length())) / 2;

    // Move the cursor to the desired position
    COORD cursorPos;
    cursorPos.X = static_cast<SHORT>(padding);
    cursorPos.Y = consoleInfo.dwCursorPosition.Y;
    SetConsoleCursorPosition(hConsole, cursorPos);

    // Output the centered text
    cout << color << "\033[1m" << text << RESET << endl;
}

// Login --------------
void Login();
void SignUp();
void profile(CurrentUser);
bool logout();
void menu();
string getPassword();

int main()
{
    clearScreen();

    centerText(" ----------------------------------- ", BG_GREEN);
    centerText(" WELCOME TO HOTEL MANAGEMENT SYSTEM  ", BG_GREEN);
    centerText(" ----------------------------------- ", BG_GREEN);

    int choice;

    cout << "1. Login" << endl;
    cout << "2. Sign-up" << endl;

    cout << "Your Choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        clearScreen();
        Login();
        break;
    case 2:
        clearScreen();
        SignUp();
        break;
    }
    return 0;
}

// Function Definitions
string getPassword()
{
    cout << "Password: ";
    char c;
    string password;
    while ((c = getch()) != '\r')
    {

        if (c == '\b' && !password.empty())
        {
            password.pop_back();
            cout << "\b \b";
        }
        else
        {
            password += c;
            cout << "*";
        }
    }

    return password;
}

void Login()
{
    centerText(" Login ", BG_YELLOW);

    string username, password;
    cout << "Enter CNIC: ";
    cin >> username;
    password = getPassword();

    cout << "Username: " << username << endl;
    cout << "Password: " << password << endl;
}

void SignUp()
{
    centerText(" Sign-up ", BG_YELLOW);
    Customer customer;
    cout << "Enter First Name: ";
    cin >> customer.first_name;
    cout << "Enter Last Name: ";
    cin >> customer.last_name;
    cout << "Enter CNIC: ";
    while (true)
    {
        if (!(cin >> customer.id))
        {
            cin.clear();                                         // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << BG_RED << "Invalid CNIC, Enter Again: " << RESET;
            continue;
        }

        cin.ignore(); // Ignore the newline character
        if (!checkCNIC(to_string(customer.id)) || !isNumeric(to_string(customer.id)))
        {
            cout << BG_RED << "Invalid CNIC, Enter Again: " << RESET;
            customer.id = 0;
            continue;
        }
        else
        {

            break;
        }
    }
    cout << "Enter Email: ";
    while (true)
    {
        cin >> customer.email;
        cin.ignore();
        if (!checkEmail(customer.email))
            cout << BG_RED << "Invalid Email, Enter Again: " << RESET;
        else
            break;
    }

    cout << "Enter Phone: ";
    while (true)
    {
        cin >> customer.phone;
        cin.ignore();
        if (!checkPhoneNumber(customer.phone) || !isNumeric(customer.phone))
            cout << BG_RED << "Invalid Phone Number, Enter Again: " << RESET;
        else
            break;
    }

    string password;
    string confirmPassword;

    while (true)
    {
        password = getPassword();
        if (!checkPasswordStrength(password))
        {
            cout << BG_RED << "\nPassword is too weak" << RESET << endl;
            continue;
        }

        cout << "\nConfirm ";
        confirmPassword = getPassword();

        if (password != confirmPassword)
        {
            cout << BG_RED << "\nPasswords do not match :( Please Enter Again " << RESET << endl;
            continue;
        }

        customer.password = encrypt(password);
        break;
    }

    currentUser.first_name = customer.first_name;
    currentUser.last_name = customer.last_name;
    currentUser.id = customer.id;
    currentUser.phone = customer.phone;
    currentUser.password = customer.password;
    currentUser.email = customer.email;
    currentUser.role = "customer";
    currentUser.success = true;
    currentUser.hotel_name = HOTEL_NAME;

    clearScreen();
    cout << "Your Account has been created successfully" << endl;
    profile(currentUser);

    pressToContinue();
    Login();
}
void hidePassword(string password)
{
    for (int i = 0; i < password.size(); i++)
    {
        cout << "*";
    }
}
void profile(CurrentUser customer)
{
    clearScreen();
    centerText(" Your Profile ", BG_YELLOW);

    cout << setw(20) << left << "First Name: "
         << customer.first_name << endl;
    cout << setw(20) << left << "Last Name: "
         << customer.last_name << endl;
    cout << setw(20) << left << "CNIC: "
         << customer.id << endl;
    cout << setw(20) << left << "Email: "
         << customer.email << endl;
    cout << setw(20) << left << "Phone: "
         << customer.phone << endl;
    cout << setw(20) << left << "Password: ";
    hidePassword(customer.password);
    cout << endl;

    pressToContinue();
    menu();
}

void menu()
{
    centerText(" Menu ", BG_YELLOW);
    if (currentUser.role == "owner")
    {
        cout << "1. View Room" << endl;
        cout << "2. View Bookings" << endl;
        cout << "3. Update Room" << endl;
        cout << "4. Delete Room" << endl;
        cout << "5. View Customers" << endl;
        cout << "6. View Profile" << endl;
        cout << "7. Logout" << endl;

        int choice;
        cout << "\nYour Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // viewRooms();
            break;
        case 2:
            // viewBookings();
            break;
        case 3:
            // updateRoom();
            break;
        case 4:
            // deleteRoom();
            break;
        case 5:
            // viewCustomers();
            break;
        case 6:
            profile(currentUser);
            pressToContinue();
            menu();
            break;
        case 7:
            if (logout())
            {
                centerText("Logged Out Successfully", BG_GREEN);
                pressToContinue();
                main();
            }
            break;
        default:
        {
            centerText("Invalid Choice", BG_RED);
            pressToContinue();
            menu();
        }
        }
        else
        {
            cout << "1. View Available Rooms" << endl;
            cout << "2. Book Room" << endl;
            cout << "3. View Bookings" << endl;
            cout << "4. View Profile" << endl;
            cout << "5. Logout" << endl;

            int choice;
            cout << "\nYour Choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                // viewAvailableRooms();
                break;
            case 2:

                // bookRoom();
                break;
            case 3:
                // viewBookings();
                break;
            case 4:
                profile(currentUser);
                pressToContinue();
                menu();
                break;
            case 5:
                logout();
                pressToContinue();
                menu();
                break;
            default:
            {
                centerText("Invalid Choice", BG_RED);
                pressToContinue();
                menu();
            }
            }
        }
    }

    bool logout()
    {
        currentUser.success = false;
        currentUser.first_name = "";
        currentUser.last_name = "";
        currentUser.email = "";
        currentUser.phone = "";
        currentUser.password = "";
        currentUser.id = 0;
        currentUser.role = "";
        currentUser.hotel_name = "";
        return true;
    }
