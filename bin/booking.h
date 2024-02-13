#include <random>
#include <string>

using namespace std;

struct Booking
{
    string bookingId;
    Date checkIn;
    Date checkOut;
    int roomNumber;
    int roomKey;
    int totalDays;
    int totalPrice;
    long long customerId;
};

string checkInBookingId = "";
bool isCheckedIn = false;

string generateRandomUniqueId()
{
    string uniqueId = "";

    srand((unsigned)time(0));

    for (int i = 0; i < 6; i++)
    {
        uniqueId += to_string(1 + (rand() % 9));
    }

    return uniqueId;
}

Date getCurrentDate()
{
    Date date;
    // current date/time based on current system
    time_t now = time(0);

    tm *ltm = localtime(&now);

    // print various components of tm structure.
    date.year = 1900 + ltm->tm_year;
    date.month = 1 + ltm->tm_mon;
    date.day = ltm->tm_mday;
    return date;
}

bool is_valid_date(Date date, Date currentDate)
{
    if (date.day < 0 || date.day > 31)
    {
        return false;
    }
    if (date.month < 0 || date.month > 12)
    {
        return false;
    }
    if (date.year < 0)
    {
        return false;
    }

    if (date.year < currentDate.year)
    {
        return false;
    }
    else
    {
        if (date.month < currentDate.month)
        {
            return false;
        }
        else
        {
            if ((date.month == currentDate.month) && (date.day < currentDate.day))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return false;
}

const int monthDays[12] = {31, 59, 90, 120, 151, 181, 212, 243,
                           273, 304, 334, 365};
int countLeapYearDays(int d[])
{
    int years = d[2];
    if (d[1] <= 2)
        years--;
    return ((years / 4) - (years / 100) + (years / 400));
}
int countNoOfDays(int date1[], int date2[])
{
    long int dayCount1 = (date1[2] * 365);
    dayCount1 += monthDays[date1[1]];
    dayCount1 += date1[0];
    dayCount1 += countLeapYearDays(date1);
    long int dayCount2 = (date2[2] * 365);
    dayCount2 += monthDays[date2[1]];
    dayCount2 += date2[0];
    dayCount2 += countLeapYearDays(date2);
    return (abs(dayCount1 - dayCount2));
}

double calculateRent(int days, double price = 1000.0)
{
    return days * price;
}

int generateRoomKey()
{
    srand((unsigned)time(NULL));

    int random = 1000 + (rand() % 9999);

    return random;
}

bool addBooking(Booking booking)
{
    bool created = createDirectory("bookings");
    if (!created)
    {
        return false;
    }
    // Open the file in append mode
    ofstream file("bookings/bookings.csv", ios::app);

    // Check if the file is open
    if (!file.is_open())
    {
        return false;
    }

    // Write to the file for the booking
    file << booking.bookingId << ","

         << booking.checkIn.day << "/"
         << booking.checkIn.month << "/"
         << booking.checkIn.year << ","

         << booking.checkOut.day << "/"
         << booking.checkOut.month << "/"
         << booking.checkOut.year << ","
         << booking.roomNumber << ","
         << booking.roomKey << ","
         << booking.totalDays << ","
         << booking.totalPrice << ","
         << booking.customerId << "\n";

    // Close the file
    file.close();

    return true;
}

bool isRoomAvailable(Date check_in, Date check_out, int room_no)
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return true;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        if (booking.roomNumber == room_no)
        {
            if (is_valid_date(check_in, booking.checkIn) == true && is_valid_date(check_in, booking.checkOut) == false)
            {
                return false;
            }
            else if (is_valid_date(check_out, booking.checkIn) == false && is_valid_date(check_out, booking.checkIn) == true)
            {
                return false;
            }
        }
    }

    file.close();

    return true;
}

void printBookings(long long customerId)
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        if (booking.customerId == customerId)
        {
            string checkIn = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
            string checkOut = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
            if (is_valid_date(booking.checkIn, getCurrentDate()))
            {
                cout << left << setw(14) << booking.bookingId
                     << left << setw(15) << checkIn
                     << left << setw(15) << checkOut
                     << left << setw(10) << booking.roomNumber
                     << left << setw(10) << booking.roomKey << RESET << endl;
            }
            else
            {
                cout << left << setw(14) << booking.bookingId
                     << left << setw(15) << checkIn
                     << left << setw(15) << checkOut
                     << left << setw(10) << booking.roomNumber
                     << left << setw(10) << BG_RED << " (Expired)" << RESET << endl;
            }
        }
    }

    file.close();
}

void printBookings()
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        string checkIn = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
        string checkOut = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
        if (is_valid_date(booking.checkIn, getCurrentDate()))
        {
            cout << left << setw(14) << booking.bookingId
                 << left << setw(15) << checkIn
                 << left << setw(15) << checkOut
                 << left << setw(10) << booking.roomNumber
                 << left << setw(10) << booking.roomKey << RESET << endl;
        }
        else
        {
            cout << left << setw(14) << booking.bookingId
                 << left << setw(15) << checkIn
                 << left << setw(15) << checkOut
                 << left << setw(10) << booking.roomNumber
                 << left << setw(10) << BG_RED << " (Expired)" << RESET << endl;
        }
    }
    file.close();
}

Booking getBooking(string bookingId)
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return Booking();
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        if (booking.bookingId == bookingId && currentUser.id == booking.customerId)
        {
            return booking;
        }
    }

    file.close();

    return Booking();
}

bool deleteCustomerBooking(string bookingId)
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");
    bool found = false;

    // Check if the file is open
    if (!file.is_open())
    {
        return false;
    }

    // Open the temporary file in write mode
    ofstream tempFile("bookings/temp.csv");

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

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        if (booking.bookingId != bookingId)
        {
            tempFile << booking.bookingId << ","
                     << booking.checkIn.day << "/"
                     << booking.checkIn.month << "/"
                     << booking.checkIn.year << ","
                     << booking.checkOut.day << "/"
                     << booking.checkOut.month << "/"
                     << booking.checkOut.year << ","
                     << booking.roomNumber << ","
                     << booking.roomKey << ","
                     << booking.totalDays << ","
                     << booking.totalPrice << ","
                     << booking.customerId << endl;
        }
        else
        {
            found = true;
        }
    }

    // Close the file
    file.close();

    // Close the temporary file
    tempFile.close();

    // Remove the original file
    remove("bookings/bookings.csv");

    // Rename the file
    rename("bookings/temp.csv", "bookings/bookings.csv");

    if (found)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void printAllBookingsOfARoom(int roomNo)
{
    // Open the file in read mode
    ifstream file("bookings/bookings.csv");

    // Check if the file is open
    if (!file.is_open())
    {
        return;
    }

    // Read each line from the file
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);

        Booking booking;
        char delimiter = ',';
        getline(iss, booking.bookingId, ',');
        iss >> booking.checkIn.day >> delimiter >> booking.checkIn.month >> delimiter >> booking.checkIn.year;
        iss.ignore();
        iss >> booking.checkOut.day >> delimiter >> booking.checkOut.month >> delimiter >> booking.checkOut.year;
        iss.ignore();
        iss >> booking.roomNumber;
        iss.ignore();
        iss >> booking.roomKey;
        iss.ignore();
        iss >> booking.totalDays;
        iss.ignore();
        iss >> booking.totalPrice;
        iss.ignore();
        iss >> booking.customerId;

        if (booking.roomNumber == roomNo)
        {
            string checkIn = to_string(booking.checkIn.day) + "/" + to_string(booking.checkIn.month) + "/" + to_string(booking.checkIn.year);
            string checkOut = to_string(booking.checkOut.day) + "/" + to_string(booking.checkOut.month) + "/" + to_string(booking.checkOut.year);
            cout << left << setw(15) << booking.bookingId
                 << left << setw(15) << checkIn
                 << left << setw(15) << checkOut
                 << left << setw(10) << booking.roomNumber
                 << left << setw(10) << booking.roomKey << RESET << endl;
        }
    }

    file.close();
}