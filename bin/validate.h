#include <regex>
#include <conio.h>
#include <string>

using namespace std;

// Email Validation
bool checkEmail(string email)
{
    // Regular expression pattern for email validation
    const regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");

    // Check if the email matches the pattern
    return regex_match(email, pattern);
}

// Password Validation
bool checkPasswordStrength(string password)
{
    // Regular expression pattern for password validation
    const regex pattern(R"((?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*]).{8,})");

    // Check if the password matches the pattern
    return regex_match(password, pattern);
}

// Phone Number Validation
bool checkPhoneNumber(string phone)
{
    // Regular expression pattern for phone number validation
    const regex pattern("\\b\\d{11}\\b");

    // Check if the phone number matches the pattern
    return regex_match(phone, pattern);
}

// CNIC Validation
bool checkCNIC(string cnic)
{
    // Regular expression pattern for CNIC validation
    const regex pattern("\\b\\d{13}\\b");

    // Check if the CNIC matches the pattern
    return regex_match(cnic, pattern);
}
bool isNumeric(const string &str)
{
    for (char ch : str)
    {
        if (!isdigit(ch))
        {
            return false;
        }
    }
    return true;
}
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

// Press to continue
void pressToContinue()
{
    std::cout << "\nPress any key to continue...";
    _getch();
    clearScreen();
}