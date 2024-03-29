#include <string>

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
};

// Structure for the Customer
struct Customer
{
    string first_name;
    string last_name;
    string email;
    string phone;
    string password;
    long long id; // ID of the customer
};

// Current User
struct CurrentUser
{
    string first_name;
    string last_name;
    string email;
    string phone;
    string password;
    long long id; // ID of the customer
    string hotel_name;
    string role;
    bool success;
};

CurrentUser currentUser; // keeping track of logged in user