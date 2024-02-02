#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "./bin/validate.h"
#include "./bin/encryption.h"
#include "./bin/fileSystem.h"

using namespace std;

//! Clear the Screen
void clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

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
string getPassword();

int main()
{
    clearScreen();
    long long id = 4545;
    bool success = deleteUser(id);
    if (success)
        cout << "Deleted Successfully" << endl;
    else
        cout << "Failed to delete" << endl;

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
        centerText(" Sign-up ", BG_YELLOW);
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
        cin >> customer.id;
        cin.ignore();
        if (!checkCNIC(to_string(customer.id)) || !isNumeric(to_string(customer.id)))
        {
            cout << BG_RED << "Invalid CNIC, Enter Again: " << RESET;
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

    clearScreen();
    centerText(" Signed Up Successfully ", BG_GREEN);
    cout << "First Name: " << customer.first_name << endl;
    cout << "Last Name: " << customer.last_name << endl;
    cout << "CNIC: " << customer.id << endl;
    cout << "Email: " << customer.email << endl;
    cout << "Phone: " << customer.phone << endl;
    cout << "Password: " << customer.password << endl;

    pressToContinue();
    clearScreen();
    Login();
}
