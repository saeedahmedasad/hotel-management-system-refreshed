#include <iostream>
#include <fstream>
#include <sstream>
#include "structs.h"

using namespace std;

// THIS CODE IS FROM CHATGPT/GOOGLE STARTS
bool directoryExists(const string path)
{
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES) && (attrib & FILE_ATTRIBUTE_DIRECTORY);
}
bool createDirectory(const string path)
{
    if (!directoryExists(path))
    {

        return CreateDirectoryA(path.c_str(), NULL);
    }

    return true;
}
// THIS CODE IS FROM CHATGPT/GOOGLE ENDS

CurrentUser getUser(long long CNIC)
{
    CurrentUser user;
    // Open the file in read mode
    ifstream file("customers/customers.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        user.success = false;
        return user;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        // Extract Data
        getline(iss, user.first_name, ',');
        getline(iss, user.last_name, ',');
        getline(iss, user.email, ',');
        getline(iss, user.phone, ',');
        getline(iss, user.password, ',');
        getline(iss, user.hotel_name, ',');
        iss >> user.id;

        if (user.id == CNIC)
        {
            user.success = true;
            return user;
        }
    }

    // Close the file
    file.close();

    ifstream file2("owners/owners.csv");

    // Check if the file is open
    if (!file2.is_open())
    {
        user.success = false;
        return user;
    }

    // Read each line from the file
    string line2;
    while (getline(file2, line2))
    {
        istringstream iss(line2);

        // Extract Data
        getline(iss, user.first_name, ',');
        getline(iss, user.last_name, ',');
        getline(iss, user.email, ',');
        getline(iss, user.phone, ',');
        getline(iss, user.password, ',');
        getline(iss, user.hotel_name, ',');
        iss >> user.id;

        if (user.id == CNIC)
        {
            user.success = true;
            return user;
        }
    }

    // Close the file
    file2.close();

    user.success = false;
    return user;
}

bool add(Customer customer)
{
    bool created = createDirectory("customers");
    if (!created)
    {
        return false;
    }
    // Open the file in append mode
    ofstream file("customers/customers.csv", ios::app);

    // Check if the file is open
    if (!file.is_open())
    {
        return false;
    }

    // Write to the file
    file << customer.first_name << ","
         << customer.last_name << ","
         << customer.email << ","
         << customer.phone << ","
         << customer.password << ","
         << "Grand Luxury Hotel"
         << "," << customer.id << endl;

    // Close the file
    file.close();

    return true;
}

bool updateUser(Customer customer)
{
    // Open the file in read mode
    ifstream file("customers/customers.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return false;
    }

    // Create a temporary file
    ofstream tempFile("customers/temp.csv", ios::out);

    // Check if the file is open
    if (!tempFile.is_open())
    {
        return false;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        // Extract Data
        string first_name;
        string last_name;
        string email;
        string phone;
        string password;
        string hotel_name;
        long long id;

        iss >> first_name;
        iss >> last_name;
        iss >> email;
        iss >> phone;
        iss >> password;
        iss >> hotel_name;
        iss >> id;

        if (id == customer.id)
        {
            first_name = customer.first_name;
            last_name = customer.last_name;
            email = customer.email;
            phone = customer.phone;
            password = customer.password;
        }

        // Write to the file
        tempFile << first_name << ","
                 << last_name << ","
                 << email << ","
                 << phone << ","
                 << password << ","
                 << hotel_name
                 << "," << id << endl;
    }

    // Close the file
    file.close();

    // Close the temporary file
    tempFile.close();

    // Remove the original file
    remove("customers/customers.csv");

    // Rename the file
    rename("customers/temp.csv", "customers/customers.csv");

    return true;
}

bool deleteUser(long long ID)
{
    // Open the file in read mode
    ifstream file("customers/customers.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return false;
    }

    // Create a temporary file
    ofstream tempFile("customers/temp.csv", ios::out);

    // Check if the file is open
    if (!tempFile.is_open())
    {
        return false;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        // Extract Data
        string first_name;
        string last_name;
        string email;
        string phone;
        string password;
        string hotel_name;
        long long id;

        getline(iss, first_name, ',');
        getline(iss, last_name, ',');
        getline(iss, email, ',');
        getline(iss, phone, ',');
        getline(iss, password, ',');
        getline(iss, hotel_name, ',');
        iss >> id;

        if (id == ID)
        {
            continue;
        }
        else
        {
            // Write to the file
            tempFile << first_name << ","
                     << last_name << ","
                     << email << ","
                     << phone << ","
                     << password << ","
                     << hotel_name
                     << "," << id << endl;
        }
    }

    // Close the file
    file.close();

    // Close the temporary file
    tempFile.close();

    // Remove the original file
    remove("customers/customers.csv");

    // Rename the file
    rename("customers/temp.csv", "customers/customers.csv");

    return true;
}