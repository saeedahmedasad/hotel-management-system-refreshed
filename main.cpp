#include <iostream>
#include <conio.h>
#include <iomanip>
#include <Windows.h>

// Some Definitions
#define RESET "\033[0m"
#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define BG_YELLOW "\033[43m"
#define BOLD "\033[1m" /* Bold */

#include "./bin/validate.h"
#include "./bin/encryption.h"
#include "./bin/fileSystem.h"
#include "./bin/booking.h"

using namespace std;

const string HOTEL_NAME = "Mikruniche Hotel ";
struct Owner
{
    string first_name = "Saeed";
    string last_name = "Ahmed";
    long long id = 3110552418059;
    string email = "saeed@gmail.com";
    string phone = "03001111598";
    string password = "saeed1092";
    string hotel_name = HOTEL_NAME;
} owner;

// Center the text
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
    cout << color << BOLD << text << RESET << endl;
}

// Login --------------
void Login();
void SignUp();
void profile(CurrentUser);
bool logout();
void menu();
bool bookRoom();
void viewBookings();
void viewAvailableRooms();
void checkIn();
void checkOut();
void deleteBooking();
void viewRoom();
string getPassword();

int main()
{
    clearScreen();
    getBooking("204586");
    centerText(" ----------------------------------- ", BG_GREEN);
    centerText(" WELCOME TO " + HOTEL_NAME, BG_GREEN);
    centerText(" ----------------------------------- ", BG_GREEN);

    int choice;

    cout << "1. Login" << endl;
    cout << "2. Sign-up" << endl;

    cout << "Your Choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
        Login();
        break;
    case 2:
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
    if (currentUser.success)
    {
        centerText("You are already Logged In", BG_RED);
        pressToContinue();
        menu();
    }
    clearScreen();
    centerText(" Login ", BG_YELLOW);

    long long id;
    string password;
    cout << "Enter CNIC: ";
    cin >> id;
    password = getPassword();

    if (id == owner.id && password == owner.password)
    {
        currentUser.first_name = owner.first_name;
        currentUser.last_name = owner.last_name;
        currentUser.email = owner.email;
        currentUser.phone = owner.phone;
        currentUser.password = owner.password;
        currentUser.role = "owner";
        currentUser.success = true;
        currentUser.id = owner.id;
        currentUser.hotel_name = owner.hotel_name;

        clearScreen();
        centerText("Logged in Successfully", BG_GREEN);
        pressToContinue();
        menu();
    }
    currentUser = getUser(id);
    if (currentUser.success)
    {
        clearScreen();
        centerText("Logged in Successfully", BG_GREEN);
        pressToContinue();
        menu();
    }
    else
    {
        centerText("Invalid CNIC or Password", BG_RED);
        pressToContinue();
        Login();
    }
}

void SignUp()
{
    clearScreen();
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
    if (add(customer))
    {
        cout << "Your Account has been created successfully" << endl;
        profile(currentUser);
    }
    else
    {
        cout << "Error Creating Your account:( Please Sign Up Again!" << endl;
        pressToContinue();
        SignUp();
    }
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
    clearScreen();
    centerText(" Menu ", BG_YELLOW);
    if (currentUser.role == "owner")
    {
        cout << "1. View Room" << endl;
        cout << "2. View Bookings" << endl;
        cout << "3. Delete Booking" << endl;
        cout << "4. View Customers" << endl;
        cout << "5. View Profile" << endl;
        cout << "6. Logout" << endl;

        int choice;
        cout << "\nYour Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // viewRoom();
            viewRoom();
            break;
        case 2:
            // viewBookings();
            viewBookings();
            pressToContinue();
            menu();
            break;
        case 3:
            // deleteBooking();
            deleteBooking();
            break;
        case 4:
            // viewCustomers();
            clearScreen();
            cout << BG_GREEN << BOLD << left << setw(20) << "ID: " << left << setw(20) << "Full Name" << left << setw(20) << "Phone" << left << setw(20) << "Email" << RESET << endl;
            getCustomers();
            pressToContinue();
            menu();
            break;
        case 5:
            profile(currentUser);
            pressToContinue();
            menu();
            break;
        case 6:
            if (logout())
            {
                centerText("Logged Out Successfully", BG_GREEN);
                pressToContinue();
                main();
            }
            break;
        default:
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
        cout << "4. Check-In" << endl;
        cout << "5. Check-Out" << endl;
        cout << "6. View Profile" << endl;
        cout << "7. Logout" << endl;

        int choice;
        cout << "\nYour Choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            // viewAvailableRooms();
            viewAvailableRooms();
            break;
        case 2:
            // BookRoom
            bookRoom();
            menu();
            break;
        case 3:

            // All bookings of a customer
            viewBookings();
            pressToContinue();
            menu();
            break;
        case 4:
            // checkIn();
            checkIn();
            break;
        case 5:
            // checkOut();
            checkOut();
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
            centerText("Invalid Choice", BG_RED);
            pressToContinue();
            menu();
        }
    }
    main();
}

bool logout()
{
    clearScreen();
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

void viewAvailableRooms()
{

    clearScreen();
    centerText(" Available Rooms ", BG_YELLOW);
    Booking booking;
    char temp;
    while (true)
    {
        cout << "Enter Check-in Date (dd-mm-yyyy): ";
        cin >> booking.checkIn.day >> temp >> booking.checkIn.month >> temp >> booking.checkIn.year;

        if (!is_valid_date(booking.checkIn, getCurrentDate()))
        {
            cout << BG_RED << "Invalid Date, Please Enter Date that's equal to or Later Than the current Date " << RESET << endl;
            continue;
        }
        break;
    }

    while (true)
    {
        cout << "Enter Check-out Date (dd-mm-yyyy): ";
        cin >> booking.checkOut.day >> temp >> booking.checkOut.month >> temp >> booking.checkOut.year;

        if (!is_valid_date(booking.checkOut, booking.checkIn))
        {
            cout << BG_RED << "Invalid Date,Please Enter Date that's equal to or Later Than the Check-in Date " << RESET << endl;
            continue;
        }
        break;
    }

    // Getting Random Rooms and Checking their Availability
    const int SIZE = 10;
    int randomRooms[SIZE] = {0}; // Initialize array with zeros
    int generated;

    srand(time(NULL)); // Seed the random number generator

    for (int i = 0; i < SIZE; ++i)
    {
        do
        {
            generated = rand() % 100 + 1;                                                // Generate random number between 1 and 100
        } while (std::count(std::begin(randomRooms), std::end(randomRooms), generated)); // Check if the generated number is already in the array
        randomRooms[i] = generated;
    }

    cout << BG_YELLOW << setw(10) << left << "Room No" << left << setw(15) << "    Status" << RESET << endl;
    for (int i = 0; i < SIZE; i++)
    {
        if (isRoomAvailable(booking.checkIn, booking.checkOut, randomRooms[i]))
        {
            cout << left << setw(10) << randomRooms[i] << BG_GREEN << setw(15) << left << "   Available" << RESET << endl;
        }
        else
        {
            cout << left << setw(10) << randomRooms[i] << BG_RED << setw(15) << left << " Not Available" << RESET << endl;
        }
    }

    char choice;
    cout << "Do you want to book a room? (y/n):";
    cin >> choice;

    if (choice == 'y' || choice == 'Y')
    {
        int date1[] = {booking.checkIn.day, booking.checkIn.month, booking.checkIn.year};
        int date2[] = {booking.checkOut.day, booking.checkOut.month, booking.checkOut.year};
        booking.roomNumber = randomRooms[rand() % SIZE];
        booking.bookingId = generateRandomUniqueId();
        booking.totalDays = countNoOfDays(date1, date2) == 0 ? 1 : countNoOfDays(date1, date2);
        booking.totalPrice = calculateRent(booking.totalDays, 6000);
        booking.roomKey = generateRoomKey();
        booking.customerId = currentUser.id;
        cout << endl;
        centerText("Room with Following Details will be automatically Booked! Enter 'ok' to Proceed", BG_YELLOW);
        cout << endl;
        cout << BG_GREEN << left << setw(14) << "Booking ID" << left << setw(15) << "Check-in" << left << setw(15) << "Check-out" << left << setw(10) << "Room No" << left << setw(10) << "Room Key" << RESET << endl;
        string check_in_string = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
        string check_out_string = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
        cout << left << setw(14) << booking.bookingId
             << left << setw(15) << check_in_string
             << left << setw(15) << check_out_string
             << left << setw(10) << booking.roomNumber
             << left << setw(10) << booking.roomKey << RESET << endl;

        string Choice;
        cin >> Choice;

        if (Choice == "ok" || Choice == "OK" || Choice == "Ok" || Choice == "oK")
        {

            addBooking(booking);

            cout << endl;
            centerText("Room Booked Successfully", BG_GREEN);
        }
        else
        {
            centerText("Booking Cancelled", BG_RED);
        }
    }
    else
    {
        menu();
    }

    pressToContinue();
    menu();
}

bool bookRoom()
{
    char temp;
    do
    {
        clearScreen();
        centerText(" Book Room ", BG_YELLOW);
        Booking booking;

        while (true)
        {
            cout << "Enter Check-in Date (dd-mm-yyyy): ";
            cin >> booking.checkIn.day >> temp >> booking.checkIn.month >> temp >> booking.checkIn.year;

            if (!is_valid_date(booking.checkIn, getCurrentDate()))
            {
                cout << BG_RED << "Invalid Date, Please Enter Date that's equal to or Later Than the current Date " << RESET << endl;
                continue;
            }
            break;
        }

        while (true)
        {
            cout << "Enter Check-out Date (dd-mm-yyyy): ";
            cin >> booking.checkOut.day >> temp >> booking.checkOut.month >> temp >> booking.checkOut.year;

            if (!is_valid_date(booking.checkOut, booking.checkIn))
            {
                cout << BG_RED << "Invalid Date,Please Enter Date that's equal to or Later Than the Check-in Date " << RESET << endl;
                continue;
            }
            break;
        }
        do
        {
            cout << "Enter Room Number: ";
            cin >> booking.roomNumber;
            if (booking.roomNumber < 1 || booking.roomNumber > 100)
            {
                cout << BG_RED << "Invalid Room Number, Please Enter Room Number between 1 and 100 " << RESET << endl;
            }
        } while (booking.roomNumber < 1 || booking.roomNumber > 100);

        int date1[] = {booking.checkIn.day, booking.checkIn.month, booking.checkIn.year};
        int date2[] = {booking.checkOut.day, booking.checkOut.month, booking.checkOut.year};

        int totalDays = countNoOfDays(date1, date2) == 0 ? 1 : countNoOfDays(date1, date2);

        booking.bookingId = generateRandomUniqueId();
        booking.totalDays = totalDays;
        booking.totalPrice = calculateRent(booking.totalDays, 6000);
        booking.roomKey = generateRoomKey();
        booking.customerId = currentUser.id;

        if (isRoomAvailable(booking.checkIn, booking.checkOut, booking.roomNumber))
        {
            addBooking(booking);
            centerText("Room Booked Successfully", BG_GREEN);
            cout << BG_GREEN << left << setw(14) << "Booking ID" << left << setw(15) << "Check-in" << left << setw(15) << "Check-out" << left << setw(10) << "Room No" << left << setw(10) << "Room Key" << RESET << endl;
            string date = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
            string date2 = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
            cout << left << setw(14) << booking.bookingId
                 << left << setw(15) << date
                 << left << setw(15) << date2
                 << left << setw(10) << booking.roomNumber
                 << left << setw(10) << booking.roomKey << RESET << endl;
        }
        else
        {
            centerText("Room is not available for these Dates", BG_RED);
        }

        cout << "Do you want to book another room? (y/n): ";
        cin >> temp;
    } while (temp == 'y' || temp == 'Y');
    return true;
}

void viewBookings()
{
    clearScreen();
    if (currentUser.role == "owner")
    {
        centerText(" All Bookings ", BG_YELLOW);
    }
    else
    {
        centerText(" Your Bookings ", BG_YELLOW);
    }
    cout << endl;
    cout << BG_GREEN << left << setw(14) << "Booking ID" << left << setw(15) << "Check-in" << left << setw(15) << "Check-out" << left << setw(10) << "Room No" << left << setw(10) << "Room Key" << RESET << endl;
    if (currentUser.role == "owner")
    {
        printBookings();
    }
    else
    {
        printBookings(currentUser.id);
    }
}

void checkIn()
{
    clearScreen();
    if (isCheckedIn)
    {
        centerText("You are already Checked In, Please Check-out First!", BG_RED);
        pressToContinue();
        menu();
        return;
    }
    centerText(" Check-In ", BG_YELLOW);
    string bookingId;
    int roomKey;
    cout << "Enter Booking ID: ";
    cin >> bookingId;
    Booking booking = getBooking(bookingId);
    if (booking.bookingId.empty())
    {
        centerText("Invalid Booking ID", BG_RED);
        pressToContinue();
        menu();
    }

    if (is_valid_date(getCurrentDate(), booking.checkIn) == true && is_valid_date(getCurrentDate(), booking.checkOut) == false)
    {
        cout << "Enter Room Key: ";
        cin >> roomKey;

        if (booking.roomKey != roomKey)
        {
            centerText("Invalid Room Key", BG_RED);
            pressToContinue();
            menu();
        }
        else
        {
            isCheckedIn = true;
            centerText("Checked In Successfully", BG_GREEN);
            cout << "Room No: " << booking.roomNumber << endl;
            cout << "Room Key: " << booking.roomKey << endl;
            cout << "Check-out Date: " << booking.checkOut.day << "/" << booking.checkOut.month << "/" << booking.checkOut.year << endl;
            cout << "Total Stay: " << booking.totalDays << ((booking.totalDays == 1) ? " Day" : " Days") << endl;
            cout << "Total Price: " << booking.totalPrice << endl;

            pressToContinue();
            menu();
        }
    }

    if (is_valid_date(booking.checkIn, getCurrentDate()))
    {
        string checkIn = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
        string checkOut = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
        string warning = "Your Booking will start from " + checkIn + " and will end on " + checkOut + " Please Check-in on " + checkIn + " and Check-out on " + checkOut + " to avoid any inconvenience.";
        centerText(warning, BG_RED);
        pressToContinue();
        menu();
        return;
    }

    if (is_valid_date(getCurrentDate(), booking.checkOut) == true)
    {
        centerText("Your Booking has Expired", BG_RED);
        pressToContinue();
        menu();
        return;
    }

    pressToContinue();
    menu();
    return;
}

void checkOut()
{
    clearScreen();
    if (isCheckedIn)
    {
        centerText("Checked Out Successfully", BG_GREEN);
        pressToContinue();
        menu();
    }
    else
    {
        centerText("You are not Checked In any of rooms", BG_RED);
        pressToContinue();
        menu();
    }
}

void deleteBooking()
{
    clearScreen();
    centerText(" Delete Booking ", BG_YELLOW);
    string bookingId;
    cout << "Enter Booking ID: ";
    cin >> bookingId;
    if (deleteCustomerBooking(bookingId))
    {
        centerText("Booking Deleted Successfully", BG_GREEN);
        pressToContinue();
        menu();
    }
    else
    {
        centerText("Invalid Booking ID", BG_RED);
        pressToContinue();
        menu();
    }
}

void viewRoom()
{
    clearScreen();
    centerText(" View Room ", BG_YELLOW);

    int roomNo;
    cout << "Enter Room No: ";
    cin >> roomNo;

    if (roomNo < 1 || roomNo > 100)
    {
        centerText("Invalid Room Number (Enter between 1-100)", BG_RED);
        pressToContinue();
        menu();
    }
    else
    {
        cout << BG_GREEN << left << setw(15) << "Booking ID" << left << setw(15) << "Check-in" << left << setw(15) << "Check-out" << left << setw(10) << "Room No" << left << setw(10) << "Room Key" << RESET << endl;
        printAllBookingsOfARoom(roomNo);
        pressToContinue();
        menu();
    }
}