// This program is for the second assignment of the OOP module, for the Membership specification
// Written by Luca J. Ricagni
// Student number: 200894968

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <ctime>
#include <chrono>

using namespace std;

void EnterToContinue();
void RunTests();
string GetMusicPreferences();

enum class MusicPreference
{
    Pop = 1,
    Rock,
    Rap,
    Classical,
    Indie,
    Metal,
    Jazz,
    Blues
} musicPreference;

enum class MembershipType
{
    FreeType,
    SilverType,
    GoldType
} membershipType;

enum class PaymentType
{
    Card,
    Paypal,
    DirectDebit,
    NA
} paymentType;

class Date 
{
    int year = 1900;
    int month = 1;
    int day = 1;

public:
    // Constructor
    Date(string date) 
    {
        this->year = stoi(date.substr(0, 4));
        this->month = stoi(date.substr(5, 2));
        this->day = stoi(date.substr(8, 2));
    }

    Date() {}

    static const int maxValidYear = 9999;
    static const int minValidYear = 1900;

    static bool IsLeap(int year)
    {
        // Return true if year is a multiple pf 4 and not multiple of 100 OR year is multiple of 400. 
        return (((year % 4 == 0) &&
            (year % 100 != 0)) ||
            (year % 400 == 0));
    }

    static bool IsValidDate(string date)
    {
        if (date.length() != 10)
        {
            return false;
        }

        int y = stoi(date.substr(0, 4));
        int m = stoi(date.substr(5, 2));
        int d = stoi(date.substr(8, 2));

        // Check if year is within limits 
        if (y > maxValidYear ||
            y < minValidYear)
            return false;
        if (m < 1 || m > 12)
            return false;
        if (d < 1 || d > 31)
            return false;

        // Handle February with leap year check
        if (m == 2)
        {
            if (IsLeap(y))
                return (d <= 29);
            else
                return (d <= 28);
        }

        // Months of April, June, Sept and Nov must have number of days less than or equal to 30. 
        if (m == 4 || m == 6 ||
            m == 9 || m == 11)
            return (d <= 30);

        return true;
    }

    static bool isFutureDate(string date)
    {
        std::time_t t = std::time(0);   // get time now
        std::tm* now = localtime(&t);

        string currentDate = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday);

        if (date > currentDate)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    string GetDateAsString()
    {
        string tempMonth = to_string(month);
        if (tempMonth.length() < 2)
        {
            tempMonth = "0" + tempMonth;
        }
        string tempDay = to_string(day);
        if (tempDay.length() < 2)
        {
            tempDay = "0" + tempDay;
        }

        return to_string(year) + "-" + tempMonth + "-" + tempDay;
    }

    bool operator<(const Date& d) 
    {
        if (this->year == d.year)
        {
            if (this->month == d.month)
            {
                if (this->day == d.day)
                {
                    return false;
                }
                else
                {
                    return this->day < d.day;
                }
            }
            else
            {
                return this->month < d.month;
            }
        }
        else
        {
            return this->year < d.year;
        }
    }

    bool operator>(const Date& d) 
    {
        if (this->year == d.year)
        {
            if (this->month == d.month)
            {
                if (this->day == d.day)
                {
                    return false;
                }
                else
                {
                    return this->day > d.day;
                }
            }
            else
            {
                return this->month > d.month;
            }
        }
        else
        {
            return this->year > d.year;
        }
    }

    bool operator==(const Date& d) 
    {
        if (this->year == d.year && this->month == d.month && this->day == d.day)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

// Abstract base class User
class User 
{
protected:
    string userName;
    string name;
    string lastName;
    Date dateOfBirth;
    string gender;
    Date dateOfRegistry;
    vector<int> musicPrefs;
    vector<string> friends;

public:
    // Constructor
    User(string userName, string name, string lastName, string gender, string dateOfBirth, string musicPrefs, string dateOfRegistry = "")
    {
        this->userName = userName;
        this->name = name;
        this->lastName = lastName;
        this->gender = gender;
        this->dateOfBirth = Date(dateOfBirth);

        string token;
        istringstream ss(musicPrefs);
        musicPrefs.clear();
        while (getline(ss, token, ':')) 
        {
            this->musicPrefs.push_back(stoi(token));
        }

        if (dateOfRegistry == "")
        {
            std::time_t t = std::time(0);   // Get time now
            std::tm* now = localtime(&t);

            this->dateOfRegistry = Date(to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday));
        }
        else
        {
            this->dateOfRegistry = Date(dateOfRegistry);
        }
    }

    string GetUserName()
    {
        return userName;
    }

    vector<string> GetFriends()
    {
        return friends;
    }

    string GetDateOfBirthAsString()
    {
        return dateOfBirth.GetDateAsString();
    }

    string GetDateOfRegistryAsString()
    {
        return dateOfRegistry.GetDateAsString();
    }

    Date* GetDateOfRegistry()
    {
        return &dateOfRegistry;
    }

    virtual string GetMembershipType() = 0; // Denotes abstract base class

    virtual string DisplayInfo() const = 0; // Denotes abstract base class

    virtual string SaveInfo() const = 0; // Denotes abstract base class

    virtual string GetMonthlyFeeAsString() = 0; // Denotes abstract base class

    virtual string GetPaymentType() = 0; // Denotes abstract base class

    bool AddFriend(string friendToAdd) 
    {
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (*it == friendToAdd)
            {
                // Already a friend
                return false;
            }
        }

        friends.push_back(friendToAdd);
        return true;
    }

    bool RemoveFriend(string friendToRemove)
    {
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (*it == friendToRemove)
            {
                // Remove friend
                friends.erase(it);
                return true;
            }
        }

        // Didn't find friend
        return false;
    }
};

// Free membership
class Free : public User 
{
private:
    int monthlyFee;
public:
    // Constructor
    Free(string userName, string name, string lastName, string gender, string dateOfBirth, string musicPrefs, string dateOfRegistry = "")
        : User(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry)
    {
        this->monthlyFee = 0;
    }

    string DisplayInfo() const override
    {
        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append("/");
            }

            switch (*it)
            {
                case 1:
                    musicPrefsTemp.append("Pop");
                    break;
                case 2:
                    musicPrefsTemp.append("Rock");
                    break;
                case 3:
                    musicPrefsTemp.append("Rap");
                    break;
                case 4:
                    musicPrefsTemp.append("Classical");
                    break;
                case 5:
                    musicPrefsTemp.append("Indie");
                    break;
                case 6:
                    musicPrefsTemp.append("Metal");
                    break;
                case 7:
                    musicPrefsTemp.append("Jazz");
                    break;
                case 8:
                    musicPrefsTemp.append("Blues");
                    break;
                default:
                    cout << "Not a valid music preference" << endl;
                    break;
            }
            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }
        return "Free Membership - UserName: " + userName + ", " + "Name: " + name + ", " + "Last name: " + lastName + ", " + "Gender: " + gender + ", " + "Date of birth: " + ((User*)this)->GetDateOfBirthAsString() 
            + ", " + "Music Preferences: " + musicPrefsTemp + ", " + "Friends list: " + friendsList + ", " + "Date of Registry: " + ((User*)this)->GetDateOfRegistryAsString();
    }

    string SaveInfo() const override
    {
        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append(":");
            }

            musicPrefsTemp.append(to_string(*it));

            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }

        return "Free," + userName + "," + name + "," + lastName + "," + gender + "," + ((User*)this)->GetDateOfBirthAsString() + "," + musicPrefsTemp + "," + friendsList + "," + ((User*)this)->GetDateOfRegistryAsString();
    }

    string GetMembershipType() override
    {
        return "Free";
    }

    string GetMonthlyFeeAsString()
    {
        return "0";
    }

    string GetPaymentType() override
    {
        return "NA";
    }
};


// Silver membership
class Silver : public User 
{
private:
    int monthlyFee;
    int payType;
    Date memberSinceDate;
public:
    // Constructor
    Silver(string userName, string name, string lastName, string gender, string dateOfBirth, string musicPrefs, string dateOfRegistry = "", PaymentType payType = PaymentType::Card, string memberSinceDate = "")
        : User(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry)
    {
        this->monthlyFee = 20;
        this->payType = (int)payType;

        if (memberSinceDate == "")
        {
            std::time_t t = std::time(0);   // Get time now
            std::tm* now = localtime(&t);
            this->memberSinceDate = Date(to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday));
        }
        else
        {
            this->memberSinceDate = Date(memberSinceDate);
        }
    }

    string GetMemberSinceDate()
    {
        return memberSinceDate.GetDateAsString();
    }

    Date* GetDateOfMembership()
    {
        return &memberSinceDate;
    }

    string DisplayInfo() const override
    {
        string payTypeTemp;
        if (payType == 0)
        {
            payTypeTemp = "Card";
        }
        else if (payType == 1)
        {
            payTypeTemp = "Paypal";
        }
        else if (payType == 2)
        {
            payTypeTemp = "Direct Debit";
        }

        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append("/");
            }

            switch (*it)
            {
            case 1:
                musicPrefsTemp.append("Pop");
                break;
            case 2:
                musicPrefsTemp.append("Rock");
                break;
            case 3:
                musicPrefsTemp.append("Rap");
                break;
            case 4:
                musicPrefsTemp.append("Classical");
                break;
            case 5:
                musicPrefsTemp.append("Indie");
                break;
            case 6:
                musicPrefsTemp.append("Metal");
                break;
            case 7:
                musicPrefsTemp.append("Jazz");
                break;
            case 8:
                musicPrefsTemp.append("Blues");
                break;
            default:
                cout << "Not a valid music preference" << endl;
                break;
            }
            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }

        return "Silver Membership - UserName: " + userName + ", " + "Name: " + name + ", " + "Last name: " + lastName + ", " + "Gender: " + gender + ", " + "Date of birth: " + ((User*)this)->GetDateOfBirthAsString() 
            + ", " + "Music Preferences: " + musicPrefsTemp + ", " + "Friends list: " + friendsList + ", " + "Date of Registry: " + ((User*)this)->GetDateOfRegistryAsString() + ", " + "Payment Type: " 
            + payTypeTemp + ", " + "Member since: " + ((Silver*)this)->GetMemberSinceDate();
    }

    string SaveInfo() const override
    {
        string payTypeTemp;
        if (payType == 0)
        {
            payTypeTemp = "0";
        }
        else if (payType == 1)
        {
            payTypeTemp = "1";
        }
        else if (payType == 2)
        {
            payTypeTemp = "2";
        }

        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append(":");
            }

            musicPrefsTemp.append(to_string(*it));

            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }

        return "Silver," + userName + "," + name + "," + lastName + "," + gender + "," + ((User*)this)->GetDateOfBirthAsString() + "," + musicPrefsTemp + "," + friendsList + "," 
            + ((User*)this)->GetDateOfRegistryAsString() + "," + payTypeTemp + "," + ((Silver*)this)->GetMemberSinceDate();
    }

    string GetMembershipType() override
    {
        return "Silver";
    }

    string GetMonthlyFeeAsString()
    {
        return "20";
    }

    string GetPaymentType() override
    {
        if (payType == 0)
        {
            return "Card";
        }
        else if (payType == 1)
        {
            return "Paypal";
        }
        else if (payType == 2)
        {
            return "DirectDebit";
        }
        else
        {
            return "NA";
        }
    }
};


// Gold membership
class Gold : public User
{
private:
    int monthlyFee;
    int payType;
    Date memberSinceDate;
public:
    // Constructor
    Gold(string userName, string name, string lastName, string gender, string dateOfBirth, string musicPrefs, string dateOfRegistry = "", PaymentType payType = PaymentType::Card, string memberSinceDate = "")
        : User(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry)
    {
        this->monthlyFee = 40;
        this->payType = (int)payType;

        if (memberSinceDate == "")
        {
            std::time_t t = std::time(0);   // Get time now
            std::tm* now = localtime(&t);
            this->memberSinceDate = Date(to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-" + to_string(now->tm_mday));
        }
        else
        {
            this->memberSinceDate = Date(memberSinceDate);
        }
    }

    string GetMemberSinceDate()
    {
        return memberSinceDate.GetDateAsString();
    }

    Date* GetDateOfMembership()
    {
        return &memberSinceDate;
    }

    string DisplayInfo() const override
    {
        string payTypeTemp;
        if (payType == 0)
        {
            payTypeTemp = "Card";
        }
        else if (payType == 1)
        {
            payTypeTemp = "Paypal";
        }
        else if (payType == 2)
        {
            payTypeTemp = "Direct Debit";
        }

        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append("/");
            }

            switch (*it)
            {
            case 1:
                musicPrefsTemp.append("Pop");
                break;
            case 2:
                musicPrefsTemp.append("Rock");
                break;
            case 3:
                musicPrefsTemp.append("Rap");
                break;
            case 4:
                musicPrefsTemp.append("Classical");
                break;
            case 5:
                musicPrefsTemp.append("Indie");
                break;
            case 6:
                musicPrefsTemp.append("Metal");
                break;
            case 7:
                musicPrefsTemp.append("Jazz");
                break;
            case 8:
                musicPrefsTemp.append("Blues");
                break;
            default:
                cout << "Not a valid music preference" << endl;
                break;
            }
            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }

        return "Gold Membership - UserName: " + userName + ", " + "Name: " + name + ", " + "Last name: " + lastName + ", " + "Gender: " + gender + ", " + "Date of birth: " + ((User*)this)->GetDateOfBirthAsString() 
            + ", " + "Music Preferences: " + musicPrefsTemp + ", " + "Friends list: " + friendsList + ", " + "Date of Registry: " + ((User*)this)->GetDateOfRegistryAsString() + ", " + "Payment Type: " + payTypeTemp 
            + ", " + "Member since: " + ((Gold*)this)->GetMemberSinceDate();
    }

    string SaveInfo() const override
    {
        string payTypeTemp;
        if (payType == 0)
        {
            payTypeTemp = "0";
        }
        else if (payType == 1)
        {
            payTypeTemp = "1";
        }
        else if (payType == 2)
        {
            payTypeTemp = "2";
        }

        string musicPrefsTemp;
        bool first = true;
        for (auto it = musicPrefs.cbegin(); it != musicPrefs.cend(); ++it)
        {
            if (!first)
            {
                musicPrefsTemp.append(":");
            }

            musicPrefsTemp.append(to_string(*it));

            first = false;
        }

        string friendsList;
        first = true;
        for (auto it = friends.cbegin(); it != friends.cend(); ++it)
        {
            if (!first)
            {
                friendsList.append(":");
            }

            friendsList.append(*it);

            first = false;
        }

        return "Gold," + userName + "," + name + "," + lastName + "," + gender + "," + ((User*)this)->GetDateOfBirthAsString() + "," + musicPrefsTemp + "," + friendsList + "," + ((User*)this)->GetDateOfRegistryAsString()
            + "," + payTypeTemp + "," + ((Silver*)this)->GetMemberSinceDate();
    }

    string GetMembershipType() override
    {
        return "Gold";
    }

    string GetMonthlyFeeAsString() override
    {
        return "40";
    }

    string GetPaymentType() override
    {
        if (payType == 0)
        {
            return "Card";
        }
        else if (payType == 1)
        {
            return "Paypal";
        }
        else if (payType == 2)
        {
            return "DirectDebit";
        }
        else
        {
            return "NA";
        }
    }
};

// User inventory object
class UserInventory 
{
private:
    // Dictionary containing all users
    map<string, User*> dictionaryOfUsers;

public:
    // Destructor
    ~UserInventory()
    {
        for (map<string, User*>::const_iterator it = dictionaryOfUsers.begin(); it != dictionaryOfUsers.end(); ++it)
        {
            delete it->second;
        }
    }

    // Search users by types
    bool SearchUsers(char searchMenuSelection, string userType = "", string usernameToSearchFor = "", string lowerDateBound = "", string upperDateBound = "")
    {
        bool found = false;
        if (searchMenuSelection == '1')
        {
            if (userType == "Free")
            {
                cout << "Displaying Free members:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMembershipType() == "Free")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (userType == "Silver")
            {
                cout << "Displaying Silver members:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMembershipType() == "Silver")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (userType == "Gold")
            {
                cout << "Displaying Gold members:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMembershipType() == "Gold")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
        }
        else if (searchMenuSelection == '2')
        {
            if (GetUser(usernameToSearchFor) == NULL)
            {
                cout << "User does not exist" << endl;
            }
            else
            {
                auto it = dictionaryOfUsers.find(usernameToSearchFor);

                cout << it->second->DisplayInfo() << endl;;
                found = true;
            }
        }
        else if (searchMenuSelection == '3')
        {
            Date tempLowerDate = Date(lowerDateBound);
            Date tempUpperDate = Date(upperDateBound);
            cout << "Users in registry range: " + lowerDateBound + " to " + upperDateBound << endl;

            for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
            {
                if (*(it->second->GetDateOfRegistry()) < tempUpperDate && (*(it->second->GetDateOfRegistry()) > tempLowerDate))
                {
                    cout << it->second->DisplayInfo() << endl;
                    found = true;
                }
            }
        }

        return found;
    }

    // Search member types by details
    bool SearchMembers(char searchMenuSelection, string monthlyFeeToSearchFor = "", string paymentTypeToSearchFor = "", string lowerDateBound = "", string upperDateBound = "")
    {
        bool found = false;
        if (searchMenuSelection == '1')
        {
            if (monthlyFeeToSearchFor == "0")
            {
                cout << "Displaying members with a monthly fee of 0 Pounds:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMonthlyFeeAsString() == "0")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (monthlyFeeToSearchFor == "20")
            {
                cout << "Displaying members with a monthly fee of 20 Pounds:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMonthlyFeeAsString() == "20")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (monthlyFeeToSearchFor == "40")
            {
                cout << "Displaying members with a monthly fee of 40 Pounds:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetMonthlyFeeAsString() == "40")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
        }
        else if (searchMenuSelection == '2')
        {
            if (paymentTypeToSearchFor == "Card")
            {
                cout << "Displaying members with payment type of Card:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetPaymentType() == "Card")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (paymentTypeToSearchFor == "Paypal")
            {
                cout << "Displaying members with payment type of Paypal:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetPaymentType() == "Paypal")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
            if (paymentTypeToSearchFor == "DirectDebit")
            {
                cout << "Displaying members with payment type of DirectDebit:" << endl;
                for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
                {
                    if (it->second->GetPaymentType() == "DirectDebit")
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
        }
        else if (searchMenuSelection == '3')
        {
            Date tempLowerDate = Date(lowerDateBound);
            Date tempUpperDate = Date(upperDateBound);
            cout << "Users with member since date in range: " + lowerDateBound + " to " + upperDateBound << ":" << endl;

            for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
            {
                if (it->second->GetMembershipType() == "Silver")
                {
                    if (*((Silver*)(it->second))->GetDateOfMembership() < tempUpperDate && *((Silver*)(it->second))->GetDateOfMembership() > tempLowerDate)
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
                else if (it->second->GetMembershipType() == "Gold")
                {
                    if (*((Gold*)(it->second))->GetDateOfMembership() < tempUpperDate && *((Gold*)(it->second))->GetDateOfMembership() > tempLowerDate)
                    {
                        cout << it->second->DisplayInfo() << endl;
                        found = true;
                    }
                }
            }
        }
        return found;
    }

    // Display all members
    void DisplayAllMembers()
    {
        cout << "USER LIST" << "\n";
        cout << "-------------------------------------------------------------------------------------------------" << endl;
        for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
        {       
            cout << it->second->DisplayInfo() << endl;
        }

        cout << "-------------------------------------------------------------------------------------------------" << endl;
        
    }

    void DisplayInformation(string username)
    {
        auto it = dictionaryOfUsers.find(username);

        cout << it->second->DisplayInfo() << endl;;
    }

    void ResetDictionaryOfUsers()
    {
        for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
        {
            delete it->second;
        }

        dictionaryOfUsers.clear();
    }

    // Add user
    bool AddUser(MembershipType memType, string userName, string name, string lastName, string gender, string dateOfBirth, string musicPrefs, string dateOfRegistry = "", PaymentType payType = PaymentType::NA, string memberSinceDate = "")
    {
        if (memType == MembershipType::FreeType)
        {
            if (GetUser(userName) == NULL)
            {
                Free* tempPtr = new Free(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry);
                dictionaryOfUsers[userName] = tempPtr;
                return true;
            }
            else
            {
                cout << "User " + userName + " already exists" << endl;
            }
        }
        else if (memType == MembershipType::SilverType)
        {
            if (GetUser(userName) == NULL)
            {
                Silver* tempPtr = new Silver(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry, payType, memberSinceDate);
                dictionaryOfUsers[userName] = tempPtr;
                return true;
            }
            else
            {
                cout << "User " + userName + " already exists" << endl;
            }
        }
        else if (memType == MembershipType::GoldType)
        {
            if (GetUser(userName) == NULL)
            {
                Gold* tempPtr = new Gold(userName, name, lastName, gender, dateOfBirth, musicPrefs, dateOfRegistry, payType, memberSinceDate);
                dictionaryOfUsers[userName] = tempPtr;
                return true;
            }
            else
            {
                cout << "User " + userName + " already exists" << endl;
            }
        }
        else
        {
            cout << "Problem adding a User, Invalid Type" << endl;
        }
        return false;
    }

    User* GetUser(string username)
    {
        map<string, User*>::iterator it = dictionaryOfUsers.find(username);

        if (it != dictionaryOfUsers.end()) {
            return dictionaryOfUsers.find(username)->second;
        }
        return NULL;    
    }

    // Remove user
    void RemoveUser(string username)
    {
        User* ptr = GetUser(username);
        User* ptrFriend;
        if (ptr != NULL)
        {
            // Remove friend references to this user from other users
            auto friendsList = ptr->GetFriends();
            for (auto it = friendsList.cbegin(); it != friendsList.cend(); ++it)
            {
                ptrFriend = GetUser(*it);
                ptrFriend->RemoveFriend(username);
            }
        
            delete ptr;
            dictionaryOfUsers.erase(username);
        }
    }

    // Save users on exit
    void SaveUsers(string fileName)
    {
        ofstream myfile;
        myfile.open(fileName);

        for (auto it = dictionaryOfUsers.cbegin(); it != dictionaryOfUsers.cend(); ++it)
        {
            myfile << it->second->SaveInfo() << endl;
        }

        myfile.close();
    }

    // Read users from file
    void ReadUsers(string fileName)
    {
        ifstream inputfile;
        inputfile.open(fileName);
        string line;

        if (inputfile.is_open())
        {      
            string token;
            vector <string> stringList;
            while (getline(inputfile, line))
            {               
                istringstream ss(line);
                stringList.clear();

                while (getline(ss, token, ','))
                {
                    stringList.push_back(token);
                }

                if (stringList[0] == "Free")
                {
                    AddUser(MembershipType::FreeType, stringList[1], stringList[2], stringList[3], stringList[4], stringList[5], stringList[6], stringList[8]);
                }
                else if (stringList[0] == "Silver")
                {
                    AddUser(MembershipType::SilverType, stringList[1], stringList[2], stringList[3], stringList[4], stringList[5], stringList[6], stringList[8], (PaymentType)stoi(stringList[9]), stringList[10]);
                }
                else if (stringList[0] == "Gold")
                {
                    AddUser(MembershipType::GoldType, stringList[1], stringList[2], stringList[3], stringList[4], stringList[5], stringList[6],stringList[8], (PaymentType)stoi(stringList[9]), stringList[10]);
                }
            }

            // Reset to first line in file
            inputfile.clear();
            inputfile.seekg(0);

            // Read in friends after all users have been added so that friend relationships can be set
            vector <string> friendsList;
            while (getline(inputfile, line))
            {
                istringstream ss(line);
                stringList.clear();

                while (getline(ss, token, ','))
                {
                    stringList.push_back(token);
                }

                friendsList.clear();

                istringstream ssFriends(stringList[7]);

                while (getline(ssFriends, token, ':'))
                {
                    friendsList.push_back(token);
                }
                AddFriends(stringList[1], friendsList);
            }
            inputfile.close();
        }
    }

    void AddFriends(string userName, vector<string> friendsToAdd)
    {
        User *user = GetUser(userName);
        if (user == NULL)
        {
            return;
        }

        User* usersFriend;
        for (auto it = friendsToAdd.cbegin(); it != friendsToAdd.cend(); ++it)
        {
            usersFriend = GetUser(*it);
            if (usersFriend != NULL)
            {
                // Add bi-directional friendship
                user->AddFriend(*it);
                usersFriend->AddFriend(userName);
            }
        }
    }

    void RemoveFriends(string userName, vector<string> friendsToRemove)
    {
        User* user = GetUser(userName);
        if (user == NULL)
        {
            return;
        }

        User* usersFriend;
        for (auto it = friendsToRemove.cbegin(); it != friendsToRemove.cend(); ++it)
        {
            usersFriend = GetUser(*it);
            if (usersFriend != NULL)
            {
                // Remove bi-directional friendship
                user->RemoveFriend(*it);
                usersFriend->RemoveFriend(userName);
            }
        }
    }
};

int main()
{
    // Declare filename to save to
    string filename = "SavedUsers.txt";

    // Create an instance of User Inventory
    UserInventory membershipSystem;

    // Read file and fill dictionary with saved users
    membershipSystem.ReadUsers("SavedUsers.txt");

    while (true)
    {
        cout << "\x1b[2J\x1b[H" << flush; // Clear the terminal
        // Main Menu
        cout <<  "                        MAIN MENU                           " << endl;
        cout << "-------------------------------------------------------------" << endl;
        cout << "1. Search users by type, username or registry range" << endl;
        cout << "2. Search member subtypes by details" << endl;
        cout << "3. Display all members" << endl;
        cout << "4. Add user" << endl;
        cout << "5. Remove user" << endl;
        cout << "6. Update friends" << endl;
        cout << "7. Run test functions" << endl;
        cout << "8. Add sample users" << endl;
        cout << "9. Save users to file and exit" << endl;
        cout << "-------------------------------------------------------------" << endl;

    
        char mainMenuSelection;
        cout << "Enter menu selection(1-9):" << endl;
        cin >> mainMenuSelection;

        if (mainMenuSelection == '1')
        {
            cout << "\x1b[2J\x1b[H" << flush; // Clear the terminal

            // Search Users by Type, Username or registry range 
            cout << "              SEARCH MENU              " << endl;
            cout << "---------------------------------------" << endl;
            cout << "1. Search users by type" << endl;
            cout << "2. Search users by username" << endl;
            cout << "3. Search users by registry range" << endl;
            cout << "---------------------------------------" << endl;

            char searchMenuSelection;
            cout << "Enter menu selection(1-3):" << endl;
            while (true)
            {
                cin >> searchMenuSelection;
                if (searchMenuSelection == '1' || searchMenuSelection == '2' || searchMenuSelection == '3')
                {
                    break;
                }
                else
                {
                    cout << "Error: Not a valid search type, please try again" << endl;
                }
            }

            if (searchMenuSelection == '1')
            {
                // Search users by type
                cout << "Please indicate which membership type you'd like to search for: " << endl;
                cout << "1. Free" << endl;
                cout << "2. Silver" << endl;
                cout << "3. Gold" << endl;
                string userTypeToSearchFor;
                while (true)
                {
                    cin >> userTypeToSearchFor;

                    if (userTypeToSearchFor == "1" || userTypeToSearchFor == "2" || userTypeToSearchFor == "3")
                    {
                        if (userTypeToSearchFor == "1")
                        {
                            userTypeToSearchFor = "Free";
                        }
                        else if (userTypeToSearchFor == "2")
                        {
                            userTypeToSearchFor = "Silver";
                        }
                        else if (userTypeToSearchFor == "3")
                        {
                            userTypeToSearchFor = "Gold";
                        }
                        membershipSystem.SearchUsers(searchMenuSelection, userTypeToSearchFor);
                        break;
                    }
                    else
                    {
                        cout << "Error: Not a valid user type, please try again" << endl;
                    }
                }
            }
            else if (searchMenuSelection == '2')
            {
                // Search users by username
                string usernameToSearchFor;
                cout << "Enter a username to search for:" << endl;
                cin >> usernameToSearchFor;

                membershipSystem.SearchUsers(searchMenuSelection, "", usernameToSearchFor);
            }
            else if (searchMenuSelection == '3')
            {
                // Search users by registry range
                string fromDateStr;
                string toDateStr;
                while (true)
                {
                    cout << "Enter from-date in the form YYYY-MM-DD: " << endl;
                    while (true)
                    {
                        cin >> fromDateStr;

                        // Check date
                        if (Date::IsValidDate(fromDateStr))
                        {
                            break;
                        }
                        else
                        {
                            cout << "Invalid Date, please try again" << endl;
                        }
                    }

                    cout << "Enter to-date in the form YYYY-MM-DD: " << endl;
                    while (true)
                    {
                        cin >> toDateStr;

                        // Check date
                        if (Date::IsValidDate(toDateStr))
                        {
                            break;
                        }
                        else
                        {
                            cout << "Invalid Date, please try again" << endl;
                        }
                    }

                    Date fromDate = Date(fromDateStr);
                    Date toDate = Date(toDateStr);

                    if (toDate < fromDate)
                    {
                        cout << "Invalid dates - from date cannot be after to date" << endl;
                    }
                    else
                    {
                        membershipSystem.SearchUsers(searchMenuSelection, "", "", fromDateStr, toDateStr);
                        break;
                    }
                }
            }
            else
            {
                // Ask for another input from the user
                cout << "Invalid entry! Please try again." << endl;
            }
        }
        else if (mainMenuSelection == '2')
        {
            // Search member subtypes by details
            cout << "\x1b[2J\x1b[H" << flush; // Clear the terminal

            // Search Users by Type, Username or registry range 
            cout << "              SEARCH MENU              " << endl;
            cout << "---------------------------------------" << endl;
            cout << "1. Search members by monthly fee" << endl;
            cout << "2. Search members by payment type" << endl;
            cout << "3. Search members by membership date range" << endl;
            cout << "---------------------------------------" << endl;
            cout << "Enter menu selection(1-3):" << endl;
            char searchMenuSelection;
            while (true)
            {
                cin >> searchMenuSelection;
                if (searchMenuSelection == '1' || searchMenuSelection == '2' || searchMenuSelection == '3')
                {
                    break;
                }
                else
                {
                    cout << "Error: Not a valid search type, please try again" << endl;
                }
            }
                
            if (searchMenuSelection == '1')
            {
                // Search users by type
                string monthlyFeeToSearchFor;
                cout << "Enter a monthly fee amount to search for:" << endl;
                cout << "1. 0 GBP" << endl;
                cout << "2. 20 GBP" << endl;
                cout << "3. 40 GBP" << endl;
                cin >> monthlyFeeToSearchFor;
                while (true)
                {
                    if (monthlyFeeToSearchFor == "1" || monthlyFeeToSearchFor == "2" || monthlyFeeToSearchFor == "3")
                    {
                        if (monthlyFeeToSearchFor == "1")
                        {
                            monthlyFeeToSearchFor = "0";
                        }
                        else if (monthlyFeeToSearchFor == "2")
                        {
                            monthlyFeeToSearchFor = "20";
                        }
                        else if (monthlyFeeToSearchFor == "3")
                        {
                            monthlyFeeToSearchFor = "40";
                        }
                        membershipSystem.SearchMembers(searchMenuSelection, monthlyFeeToSearchFor);
                        break;
                    }
                    else
                    {
                        cout << "Error: Not a valid monthly fee, please try again" << endl;
                    }
                }
            }
            else if (searchMenuSelection == '2')
            {
                // Search users by username
                string paymentTypeToSearchFor;
                cout << "Please indicate which payment type to search for: " << endl;
                cout << "1. Card" << endl;
                cout << "2. Paypal" << endl;
                cout << "3. DirectDebit" << endl;
                cin >> paymentTypeToSearchFor;

                while (true)
                {
                    if (paymentTypeToSearchFor == "1" || paymentTypeToSearchFor == "2" || paymentTypeToSearchFor == "3")
                    {
                        if (paymentTypeToSearchFor == "1")
                        {
                            paymentTypeToSearchFor = "Card";
                        }
                        else if (paymentTypeToSearchFor == "2")
                        {
                            paymentTypeToSearchFor = "Paypal";
                        }
                        else if (paymentTypeToSearchFor == "3")
                        {
                            paymentTypeToSearchFor = "DirectDebit";
                        }
                        membershipSystem.SearchMembers(searchMenuSelection, "", paymentTypeToSearchFor);
                        break;
                    }
                    else
                    {
                        cout << "Error: Not a valid payment type" << endl;
                    }
                }
            }
            else if (searchMenuSelection == '3')
            {
                // Search users by registry range
                string fromDateStr;
                string toDateStr;
                while (true)
                {
                    cout << "Enter from-date in the form YYYY-MM-DD: " << endl;
                    while (true)
                    {
                        cin >> fromDateStr;

                        //check date
                        if (Date::IsValidDate(fromDateStr))
                        {
                            break;
                        }
                        else
                        {
                            cout << "Invalid Date, please try again" << endl;
                        }
                    }

                    cout << "Enter to-date in the form YYYY-MM-DD: " << endl;
                    while (true)
                    {
                        cin >> toDateStr;

                        //check date
                        if (Date::IsValidDate(toDateStr))
                        {
                            break;
                        }
                        else
                        {
                            cout << "Invalid Date, please try again" << endl;
                        }
                    }

                    Date fromDate = Date(fromDateStr);
                    Date toDate = Date(toDateStr);

                    if (toDate < fromDate)
                    {
                        cout << "Invalid dates - from date cannot be after to date" << endl;
                    }
                    else
                    {
                        membershipSystem.SearchMembers(searchMenuSelection, "", "", fromDateStr, toDateStr);
                        break;
                    }
                }
            }
            else
            {
                // Ask for another input from the user
                cout << "Invalid entry! Please try again." << endl;
            }
        }
        else if (mainMenuSelection == '3')
        {
            // Display all members
            membershipSystem.DisplayAllMembers();
        }
        else if (mainMenuSelection == '4')
        {
            // Add user
            cout << "\x1b[2J\x1b[H" << flush; // Clear the terminal
            string desiredUsername;
            cout << "Please input a unique username: " << endl;

            string membershipTypeInput;
            string name;
            string lastName;
            string dateOfBirth;
            string gender;
            string musicPrefs;
            string paymentTypeInput;

            bool passedUsername;
            while (true)
            {
                passedUsername = true;
                cin >> desiredUsername;
                for (int i = 0; i < (int)(desiredUsername.length()); i++)
                {
                    if (isalnum(desiredUsername[i]) == false)
                    {
                        cout << "Invalid name - must only contain alphanumeric characters, please try again" << endl;
                        passedUsername = false;
                        break;
                    }
                }

                if (passedUsername == true)
                {
                    if (membershipSystem.GetUser(desiredUsername) == NULL)
                    {
                        break;
                    }
                    else
                    {
                        cout << "Username is already taken, please input a different username: " << endl;
                    }
                }
            }

            auto membershipType = MembershipType::FreeType;
            cout << "Please indicate which membership type you'd like: " << endl;
            cout << "1. Free" << endl;
            cout << "2. Silver" << endl;
            cout << "3. Gold" << endl;
            while (true)
            {
                cin >> membershipTypeInput;

                if (membershipTypeInput == "1" || membershipTypeInput == "2" || membershipTypeInput == "3")
                {
                    membershipType = (MembershipType)(stoi(membershipTypeInput) - 1);
                    break;
                }
                else
                {
                    cout << "Invalid membership type, please try again" << endl;
                }
            }

            cout << "Please enter your first name: " << endl;
            bool passedName = true;
            while (true)
            {
                passedName = true;
                cin >> name;
                for (int i = 0; i < (int)(name.length()); i++)
                {
                    if (isalpha(name[i]) == false)
                    {
                        cout << "Invalid name - must only contain alphabetic characters, please try again" << endl;
                        passedName = false;
                        break;
                    }
                }

                if (passedName == true)
                {
                    break;
                }
            }

            cout << "Please enter your last name: " << endl;
            bool passedLastName = true;
            while (true)
            {
                passedLastName = true;
                cin >> lastName;
                for (int i = 0; i < (int)(lastName.length()); i++)
                {
                    if (isalpha(lastName[i]) == false)
                    {
                        cout << "Invalid name - must only contain alphabetic characters, please try again" << endl;
                        passedLastName = false;
                        break;
                    }
                }

                if (passedLastName == true)
                {
                    break;
                }
            }

            while (true)
            {
                cout << "Please enter your date of birth (YYYY-MM-DD): " << endl;
                cin >> dateOfBirth;

                //check date
                if (Date::IsValidDate(dateOfBirth) == true && Date::isFutureDate(dateOfBirth) == false)
                {
                    break;
                }
                else
                {
                    cout << "Invalid Date, please try again" << endl;
                }
            }

            cout << "Please indicate your gender: " << endl;
            cout << "1. Male" << endl;
            cout << "2. Female" << endl;
            cout << "3. Other" << endl;

            while (true)
            {
                cin >> gender;

                if (gender == "1" || gender == "2" || gender == "3")
                {
                    if (gender == "1")
                    {
                        gender = "Male";
                    }
                    else if (gender == "2")
                    {
                        gender = "Female";
                    }
                    else if (gender == "3")
                    {
                        gender = "Other";
                    }
                    break;
                }
                else
                {
                    cout << "Invalid entry, please try again" << endl;
                }

            }

            musicPrefs = GetMusicPreferences();
                            
            auto paymentType = PaymentType::Card;

            if (membershipTypeInput == "Silver" || membershipTypeInput == "Gold")
            {
                cout << "Please indicate which payment type you'd like: " << endl;
                cout << "1. Card" << endl;
                cout << "2. Paypal" << endl;
                cout << "3. DirectDebit" << endl;
                cin >> paymentTypeInput;

                while (true)
                {
                    if (paymentTypeInput == "1" || paymentTypeInput == "2" || paymentTypeInput == "3")
                    {
                        if (paymentTypeInput == "1")
                        {
                            paymentTypeInput = "Card";
                        }
                        else if (paymentTypeInput == "2")
                        {
                            paymentTypeInput = "Paypal";
                        }
                        else if (paymentTypeInput == "3")
                        {
                            paymentTypeInput = "DirectDebit";
                        }
                        break;
                    }
                    else
                    {
                        cout << "Invalid payment type, please try again" << endl;
                    }
                }
            }
            else
            {
                paymentTypeInput = "NA";
            }
            
            membershipSystem.AddUser(membershipType, desiredUsername, name, lastName, gender, dateOfBirth, musicPrefs, "", paymentType, "");
            cout << "User added successfully!" << endl;
        }
        else if (mainMenuSelection == '5')
        {
            // Remove user
            string userToBeRemoved;
            cout << "Please enter the username of the user you would like to remove:" << endl;
            cin >> userToBeRemoved;

            if (membershipSystem.GetUser(userToBeRemoved) != NULL) // Check if user is present in system
            {
                membershipSystem.RemoveUser(userToBeRemoved);
                cout << "User has been successfully removed" << endl;
            }
            else
            {
                cout << "User with that username is not present and therefore cannot be removed" << endl;
            }
        }
        else if (mainMenuSelection == '6')
        {
            string menuSelection;

            cout << "\x1b[2J\x1b[H" << flush; // Clear the terminal

            cout << "Please indicate what you would like to do:" << endl;
            cout << "1. Add friends" << endl;
            cout << "2. Remove friends" << endl;

            while (true)
            {
                cin >> menuSelection;
                if (menuSelection == "1" || menuSelection == "2")
                {
                    break;
                }
                else
                {
                    cout << "Invalid menu selection, please try again" << endl;
                }
            }

            if (menuSelection == "1")
            {
                // Add friends
                cout << "Please enter the username of the user you'd like to add friends to" << endl;
                string usernameToAddFriendsTo;
                while (true)
                {
                    cin >> usernameToAddFriendsTo;
                    if (membershipSystem.GetUser(usernameToAddFriendsTo) != NULL)
                    {
                        break;
                    }
                    else
                    {
                        cout << "User does not exist, please enter a valid username" << endl;
                    }
                }
                
                vector<string> friendsToAdd;
                string usernameToAddAsFriend;
                bool finshedAddingUsers = false;
                while (true)
                {
                    cout << "Please give the username of the friend you'd like to add" << endl;
                    cin >> usernameToAddAsFriend;
                    if (membershipSystem.GetUser(usernameToAddAsFriend) != NULL)
                    {
                        friendsToAdd.push_back(usernameToAddAsFriend);
                    }
                    else
                    {
                        cout << "User does not exist, please enter a valid username" << endl;
                    }

                    cout << "Do you wish to add another user as a friend?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;
                    string addAnother;
                    while (true)
                    {
                        cin >> addAnother;
                        if (addAnother == "1")
                        {
                            break;
                        }
                        else if (addAnother == "2")
                        {
                            finshedAddingUsers = true;
                            break;
                        }
                        else
                        {
                            cout << "Invalid menu selection, please try again" << endl;
                        }
                    }

                    if (finshedAddingUsers)
                    {
                        break;
                    }
                }
                if ((int)(friendsToAdd.size()) > 0)
                {
                    membershipSystem.AddFriends(usernameToAddFriendsTo, friendsToAdd);
                    cout << "Friend(s) added successfully" << endl;
                }
            }
            else if (menuSelection == "2")
            {
                // Remove friends
                cout << "Please enter the username of the user you'd like to remove friends from" << endl;
                string usernameToRemoveFriendsFrom;
                while (true)
                {
                    cin >> usernameToRemoveFriendsFrom;
                    if (membershipSystem.GetUser(usernameToRemoveFriendsFrom) != NULL)
                    {
                        break;
                    }
                    else
                    {
                        cout << "User does not exist, please enter a valid username" << endl;
                    }
                }

                vector<string> friendsToRemove;
                string usernameToRemoveFromFriend;
                bool finshedAddingUsers = false;
                while (true)
                {
                    cout << "Please give the username of the friend you'd like to remove" << endl;
                    cin >> usernameToRemoveFromFriend;
                    if (membershipSystem.GetUser(usernameToRemoveFromFriend) != NULL)
                    {
                        friendsToRemove.push_back(usernameToRemoveFromFriend);
                    }
                    else
                    {
                        cout << "User does not exist, please enter a valid username" << endl;
                    }

                    cout << "Do you wish to remove another user as a friend?" << endl;
                    cout << "1. Yes" << endl;
                    cout << "2. No" << endl;
                    string addAnother;
                    while (true)
                    {
                        cin >> addAnother;
                        if (addAnother == "1")
                        {
                            break;
                        }
                        else if (addAnother == "2")
                        {
                            finshedAddingUsers = true;
                            break;
                        }
                        else
                        {
                            cout << "Invalid menu selection, please try again" << endl;
                        }
                    }

                    if (finshedAddingUsers)
                    {
                        break;
                    }
                }

                if ((int)(friendsToRemove.size()) > 0)
                {
                    membershipSystem.RemoveFriends(usernameToRemoveFriendsFrom, friendsToRemove);
                    cout << "Friend(s) removed successfully" << endl;
                }
            }
        }
        else if (mainMenuSelection == '7')
        {
            // Run test functions
            RunTests();
        }
        else if (mainMenuSelection == '8')
        {
            // Seed membershipSystem with dummy users
            cout << "Adding sample users..." << endl;
            string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
            membershipSystem.AddUser(MembershipType::FreeType, "RedSled01", "Emily", "Brown", "Female", "1990-01-19", musicPrefs, "2019-08-15");
            musicPrefs = to_string((int)MusicPreference::Jazz) + ":" + to_string((int)MusicPreference::Blues);
            membershipSystem.AddUser(MembershipType::FreeType, "KiwiWillow", "Robert", "Smith", "Male", "2005-09-01", musicPrefs, "2018-12-04");
            musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Rap);
            membershipSystem.AddUser(MembershipType::FreeType, "ElementZap", "Maria", "Garcia", "Female", "2000-12-31", musicPrefs, "2017-02-22");
            musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Rock);
            membershipSystem.AddUser(MembershipType::FreeType, "FearBoost", "James", "Johnson", "Male", "2089-03-05", musicPrefs, "2016-09-18");

            musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Rock) + ":" + to_string((int)MusicPreference::Jazz) + ":" + to_string((int)MusicPreference::Metal);
            membershipSystem.AddUser(MembershipType::SilverType, "PsychBradley", "Bradley", "Hunt", "Male", "1992-06-19", musicPrefs, "2009-04-25", PaymentType::DirectDebit), "2010-04-25";
            musicPrefs = to_string((int)MusicPreference::Pop);
            membershipSystem.AddUser(MembershipType::SilverType, "Ruccimark", "Ryan", "Williams", "Male", "1970-12-12", musicPrefs, "2019-07-25", PaymentType::DirectDebit, "2020-07-25");
            musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Classical) + ":" + to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Blues);
            membershipSystem.AddUser(MembershipType::SilverType, "Cybenzel", "Laura", "Jones", "Female", "2001-08-27", musicPrefs, "2013-01-11", PaymentType::Paypal, "2015-03-29");
            musicPrefs = to_string((int)MusicPreference::Metal) + ":" + to_string((int)MusicPreference::Rap);
            membershipSystem.AddUser(MembershipType::SilverType, "ShinZap", "Nicole", "Rodriguez", "Female", "1996-03-02", musicPrefs, "2014-06-16", PaymentType::Card, "2019-07-25");

            musicPrefs = to_string((int)MusicPreference::Rock) + ":" + to_string((int)MusicPreference::Rap);
            membershipSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Anderson", "Female", "1999-09-22", musicPrefs, "2015-04-25", PaymentType::Paypal, "2016-08-06");
            musicPrefs = to_string((int)MusicPreference::Blues) + ":" + to_string((int)MusicPreference::Classical) + ":" + to_string((int)MusicPreference::Rap);
            membershipSystem.AddUser(MembershipType::GoldType, "JoshSizzlin", "Joshua", "Nova", "Male", "1965-12-04", musicPrefs, "2020-10-05", PaymentType::Card, "2020-11-30");
            musicPrefs = to_string((int)MusicPreference::Classical) + ":" + to_string((int)MusicPreference::Blues);
            membershipSystem.AddUser(MembershipType::GoldType, "Uberling", "Jennifer", "Moore", "Female", "1978-04-13", musicPrefs, "2018-11-30", PaymentType::Card, "2019-05-10");
            musicPrefs = to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Rock);
            membershipSystem.AddUser(MembershipType::GoldType, "Memexport", "Kenneth", "White", "Male", "1963-01-09", musicPrefs, "2019-08-21", PaymentType::DirectDebit, "2020-08-21");

            cout << "Finished" << endl;
        }
        else if (mainMenuSelection == '9')
        {
            // Save Users to file and exit
            cout << "Saving Users......." << endl;
            membershipSystem.SaveUsers("SavedUsers.txt");
            cout << "Users saved!" << endl;
            return 1;
        }
        else 
        {
            // Ask for another input from the user
            cout << "Invalid entry! Please try again." << endl;
        }
        EnterToContinue();
    }
}

string GetMusicPreferences()
{
    cout << "Please indicate your music preferences (1-8), or 0 to stop adding genres:" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;
    cout << "1. Pop" << endl;
    cout << "2. Rock" << endl;
    cout << "3. Rap" << endl;
    cout << "4. Classical" << endl;
    cout << "5. Indie" << endl;
    cout << "6. Metal" << endl;
    cout << "7. Jazz" << endl;
    cout << "8. Blues" << endl;
    cout << "0. Finished adding genres" << endl;
    cout << "-----------------------------------------------------------------------------" << endl;

    bool alreadyUsed[8] = { false };
    string musicPrefs;
    bool first = true;
    char musicPrefInputChar;
    int musicPrefInputInt;
    while (true)
    {
        cin >> musicPrefInputChar;
        musicPrefInputInt = musicPrefInputChar - 48;

        if (musicPrefInputInt == 0)
        {
            return musicPrefs;
        }

        if (alreadyUsed[musicPrefInputInt - 1] == false)
        {
            if (!first)
            {
                musicPrefs.append(":");
            }
            first = false;

            switch (musicPrefInputInt)
            {
                case 1:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("1");
                    break;
                case 2:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("2");
                    break;
                case 3:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("3");
                    break;
                case 4:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("4");
                    break;
                case 5:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("5");
                    break;
                case 6:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("6");
                    break;
                case 7:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("7");
                    break;
                case 8:
                    alreadyUsed[musicPrefInputInt - 1] = true;
                    musicPrefs.append("8");
                    break;
                default:
                    cout << "Invalid input, please try again" << endl;
                    break;
            }
            cout << "Please enter another music preference" << endl;
        }
        else
        {
            cout << "Genre has already been used, please try again" << endl;
        }
    }
}

void EnterToContinue() {
    cout << "Press enter to continue";
    cin.clear(); // Clears the stream
    cin.ignore(50, '\n'); // Ignores next line(enter)
    cin.get(); //“cin>>” expects at least one char input, just enter doesn’t work
    std::cout << "\x1b[2J\x1b[H" << std::flush; // Clear the terminal
}

void DisplayUsersTest()
{
    UserInventory testSystem;

    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    testSystem.DisplayAllMembers();
}

bool InsertUsersTest() 
{
    UserInventory testSystem;

    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    testSystem.SaveUsers("Save&LoadTest.txt");

    // Reset dictionary
    testSystem.ResetDictionaryOfUsers();

    // Load in previously saved users
    testSystem.ReadUsers("Save&LoadTest.txt");

    // Check users are present in dictionary
    if (testSystem.GetUser("Bob98") == NULL)
    {
        return false;
    }

    if (testSystem.GetUser("James85") == NULL)
    {
        return false;
    }

    if (testSystem.GetUser("AdamTheMan") == NULL)
    {
        return false;
    }

    // Internal object test
    User* temp = testSystem.GetUser("Crystal02");
    if (temp == NULL)
    {
        return false;
    }
    if (temp->SaveInfo() != "Gold,Crystal02,Mille,Doe,Female,2002-12-08,1:5:4,,2015-04-25,2,2020-12-03")
    {
        return false;
    }

    // Invalid user
    if (testSystem.GetUser("fgfgfgfg") != NULL)
    {
        return false;
    }

    return true;
}

bool DeleteUsersTest()
{
    UserInventory testSystem;

    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    testSystem.RemoveUser("Bob98");
    testSystem.RemoveUser("James85");
    testSystem.RemoveUser("AdamTheMan");
    testSystem.RemoveUser("fgfgfgfg");

    // Check users are present in dictionary
    if (testSystem.GetUser("Bob98") != NULL)
    {
        return false;
    }

    if (testSystem.GetUser("James85") != NULL)
    {
        return false;
    }

    if (testSystem.GetUser("AdamTheMan") != NULL)
    {
        return false;
    }

    if (testSystem.GetUser("Crystal02") == NULL)
    {
        return false;
    }

    return true;
}

bool InvalidDateTest()
{
    string d1 = "dfdfgdf"; // Bad date
    if (Date::IsValidDate(d1) == true)
    {
        return false;
    }

    string d2 = "2020-14-10"; // Bad month
    if (Date::IsValidDate(d2) == true)
    {
        return false;
    }

    string d3 = "20202-12-10"; // Bad year
    if (Date::IsValidDate(d3) == true)
    {
        return false;
    }

    string d4 = "2020-12-55"; // Bad day
    if (Date::IsValidDate(d4) == true)
    {
        return false;
    }

    string d5 = "2019-02-29"; // Not a leap year
    if (Date::IsValidDate(d5) == true)
    {
        return false;
    }

    string d6 = "2020-02-28"; // Leap year
    if (Date::IsValidDate(d6) == false)
    {
        return false;
    }

    return true;
}

bool FutureDateTest()
{
    string d1 = "2055-02-10"; // Future date
    if (Date::isFutureDate(d1) == false)
    {
        return false;
    }

    string d2 = "2019-02-10"; // Past date
    if (Date::isFutureDate(d2) == true)
    {
        return false;
    }

    return true;
}

bool AddFriendsTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    // Add friends
    vector<string> friendsList;
    friendsList.push_back("AdamTheMan");

    // Add bi-directional friend relationship (eg if AdamTheMan is a friend of Bob98 then assume the opposite is also true)
    testSystem.AddFriends("Bob98", friendsList);

    User *userBob = testSystem.GetUser("Bob98");
    vector<string> bobsFriends = userBob->GetFriends();
    bool foundAdam = false;
    for (auto it = bobsFriends.cbegin(); it != bobsFriends.cend(); ++it)
    {
        if (*it == "AdamTheMan")
        {
            foundAdam = true;
        }
    }

    User* userAdam = testSystem.GetUser("AdamTheMan");
    vector<string> adamsFriends = userAdam->GetFriends();
    bool foundBob = false;
    for (auto it = adamsFriends.cbegin(); it != adamsFriends.cend(); ++it)
    {
        if (*it == "Bob98")
        {
            foundBob = true;
        }
    }

    return foundBob && foundAdam;
}

bool RemoveFriendsTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    // Add friends
    vector<string> friendsList;
    friendsList.push_back("AdamTheMan");
    friendsList.push_back("Crystal02");
    friendsList.push_back("James85");

    // Add bi-directional friend relationship (eg if AdamTheMan is a friend of Bob98 then assume the opposite is also true)
    testSystem.AddFriends("Bob98", friendsList);

    // Remove friends from bob
    vector<string> friendsToRemoveList;
    friendsToRemoveList.push_back("AdamTheMan");
    friendsToRemoveList.push_back("Crystal02");
    testSystem.RemoveFriends("Bob98", friendsToRemoveList);

    User* userBob = testSystem.GetUser("Bob98");
    vector<string> bobsFriends = userBob->GetFriends();
    bool foundAdam = false;
    bool foundCrystal = false;
    bool foundJames = false;
    for (auto it = bobsFriends.cbegin(); it != bobsFriends.cend(); ++it)
    {
        if (*it == "AdamTheMan")
        {
            foundAdam = true;
        }
        else if (*it == "Crystal02")
        {
            foundCrystal = true;
        }
        else if (*it == "James85")
        {
            foundJames = true;
        }
    }

    return !foundAdam && !foundCrystal && foundJames;
}

bool SaveAndLoadUsersWithFriendsTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);
    musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Indie) + ":" + to_string((int)MusicPreference::Classical);
    testSystem.AddUser(MembershipType::GoldType, "Crystal02", "Mille", "Doe", "Female", "2002-12-08", musicPrefs, "2015-04-25", PaymentType::DirectDebit, "2020-12-03");

    // Add friends
    vector<string> friendsList;
    friendsList.push_back("AdamTheMan");
    friendsList.push_back("James85");

    // Add bidirectional friend
    testSystem.AddFriends("Bob98", friendsList);

    testSystem.SaveUsers("SaveAndLoadUsersWithFriendsTest.txt");

    // Reset dictionary
    testSystem.ResetDictionaryOfUsers();

    // Load in previously saved users
    testSystem.ReadUsers("SaveAndLoadUsersWithFriendsTest.txt");

    User* userBob = testSystem.GetUser("Bob98");
    vector<string> bobsFriends = userBob->GetFriends();
    bool foundAdamFromBob = false;
    bool foundJamesFromBob = false;
    for (auto it = bobsFriends.cbegin(); it != bobsFriends.cend(); ++it)
    {
        if (*it == "AdamTheMan")
        {
            foundAdamFromBob = true;
        } 
        else if (*it == "James85")
        {
            foundJamesFromBob = true;
        }
    }

    User* userAdam = testSystem.GetUser("AdamTheMan");
    vector<string> adamsFriends = userAdam->GetFriends();
    bool foundBobFromAdam = false;
    for (auto it = adamsFriends.cbegin(); it != adamsFriends.cend(); ++it)
    {
        if (*it == "Bob98")
        {
            foundBobFromAdam = true;
        }
    }

    User* userJames = testSystem.GetUser("James85");
    vector<string> jamesFriends = userJames->GetFriends();
    bool foundBobFromJames = false;
    for (auto it = jamesFriends.cbegin(); it != jamesFriends.cend(); ++it)
    {
        if (*it == "Bob98")
        {
            foundBobFromJames = true;
        }
    }

    return foundAdamFromBob && foundJamesFromBob && foundBobFromAdam && foundBobFromJames;
}

bool SearchByTypeTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);

    cout << endl;
    cout << "Running SearchByTypeTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchUsers('1', "Free");
    bool found2 = testSystem.SearchUsers('1', "Gold");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

bool SearchByUsernameTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs);

    cout << endl;
    cout << "Running SearchByUsernameTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchUsers('2', "", "Bob98");
    bool found2 = testSystem.SearchUsers('2', "", "ShouldNotExist");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

bool SearchByRegistryDateTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs, "2019-01-01");

    cout << endl;
    cout << "Running SearchByRegistryDateTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchUsers('3', "", "", "2020-01-01", "2020-12-01");
    bool found2 = testSystem.SearchUsers('3', "", "", "1980-01-01", "1990-12-01");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

bool SearchByMonthlyFeeTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs, "2019-01-01");

    cout << endl;
    cout << "Running SearchByMonthlyFeeTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchMembers('1', "0");
    bool found2 = testSystem.SearchMembers('1', "40");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

bool SearchByPaymentTypeTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card);
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs, "2019-01-01");

    cout << endl;
    cout << "Running SearchByPaymentTypeTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchMembers('2', "", "Card");
    bool found2 = testSystem.SearchMembers('2', "", "Paypal");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

bool SearchByMemberSinceDateTest()
{
    UserInventory testSystem;
    string musicPrefs = to_string((int)MusicPreference::Pop) + ":" + to_string((int)MusicPreference::Metal);
    testSystem.AddUser(MembershipType::FreeType, "Bob98", "Bob", "Smith", "Male", "1998-02-24", musicPrefs, "2020-11-11");
    musicPrefs = to_string((int)MusicPreference::Rap) + ":" + to_string((int)MusicPreference::Rock);
    testSystem.AddUser(MembershipType::SilverType, "James85", "James", "Black", "Male", "1985-05-14", musicPrefs, "2010-04-25", PaymentType::Card, "2020-01-01");
    musicPrefs = to_string((int)MusicPreference::Indie);
    testSystem.AddUser(MembershipType::FreeType, "AdamTheMan", "Adam", "Brown", "Male", "2000-08-29", musicPrefs, "2019-01-01");

    cout << endl;
    cout << "Running SearchByMemberSinceDateTest..." << endl;
    cout << "------------------------------------" << endl;
    bool found = testSystem.SearchMembers('3', "", "", "2019-01-01", "2020-12-01");
    bool found2 = testSystem.SearchMembers('3', "", "", "1980-01-01", "1990-12-01");
    cout << "------------------------------------" << endl;

    return found && !found2;
}

void RunTests()
{
    int passedCount = 0;
    int failedCount = 0;
    cout << "Running Tests..." << endl;
    cout << "------------------------------------" << endl;

    DisplayUsersTest();

    if (InsertUsersTest())
    {
        cout << "InsertUsersTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "InsertUsersTest Failed!!!" << endl;
        failedCount++;
    }

    if (DeleteUsersTest())
    {
        cout << "DeleteUsersTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "DeleteUsersTest Failed!!!" << endl;
        failedCount++;
    }

    if (InvalidDateTest())
    {
        cout << "InvalidDateTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "InvalidDateTest Failed!!!" << endl;
        failedCount++;
    }

    if (FutureDateTest())
    {
        cout << "FutureDateTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "FutureDateTest Failed!!!" << endl;
        failedCount++;
    }

    if (AddFriendsTest())
    {
        cout << "AddFriendsTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "AddFriendsTest Failed!!!" << endl;
        failedCount++;
    }

    if (RemoveFriendsTest())
    {
        cout << "RemoveFriendsTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "RemoveFriendsTest Failed!!!" << endl;
        failedCount++;
    }

    if (SaveAndLoadUsersWithFriendsTest())
    {
        cout << "SaveAndLoadUsersWithFriendsTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SaveAndLoadUsersWithFriendsTest Failed!!!" << endl;
        failedCount++;
    }

    if (SearchByTypeTest())
    {
        cout << "SearchByTypeTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByTypeTest Failed!!!" << endl;
        failedCount++;
    }

    if (SearchByUsernameTest())
    {
        cout << "SearchByUsernameTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByUsernameTest Failed!!!" << endl;
        failedCount++;
    }
    
    if (SearchByRegistryDateTest())
    {
        cout << "SearchByRegistryDateTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByRegistryDateTest Failed!!!" << endl;
        failedCount++;
    }

    if (SearchByMonthlyFeeTest())
    {
        cout << "SearchByMonthlyFeeTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByMonthlyFeeTest Failed!!!" << endl;
        failedCount++;
    }

    if (SearchByPaymentTypeTest())
    {
        cout << "SearchByPaymentTypeTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByPaymentTypeTest Failed!!!" << endl;
        failedCount++;
    }

    if (SearchByMemberSinceDateTest())
    {
        cout << "SearchByMemberSinceDateTest Passed" << endl;
        passedCount++;
    }
    else
    {
        cout << "SearchByMemberSinceDateTest Failed!!!" << endl;
        failedCount++;
    }
    cout << endl;
    cout << "------------------------------------" << endl;

    cout << "Tests passed: " << passedCount << endl;
    cout << "Tests failed: " << failedCount << endl;
    cout << endl;
}