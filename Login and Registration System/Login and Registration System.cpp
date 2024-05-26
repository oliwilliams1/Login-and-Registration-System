#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <cctype>

using json = nlohmann::json;
using namespace std;

json makeFile()
{
    json j;
    j["Users"]["snout"]["password"] = "snout";
    j["Users"]["snout"]["biography"] = "Empty!";
    string jsonString = j.dump(2);
    ofstream file("registration.json", ios::out);
    file << jsonString;

    return json::parse(jsonString);
}

json loadFile()
{
    ifstream file("registration.json");
    if (!file.is_open()) {
        return makeFile();
    }
    return json::parse(file);
}

json fileData = loadFile();

void clearFile()
{
    json j;
    j["Users"]["snout"]["password"] = "snout";
    j["Users"]["snout"]["biography"] = "Empty!";
    string jsonString = j.dump(2);
    ofstream file("registration.json", ios::out);
    file << jsonString;
    fileData = j; // reload
}

string stringUpper(string string)
{
    string[0] = toupper(string[0]);
    return string;
}

bool isInputValid(string input, string type)
{
    if (input.length() < 5) {
        cout << "\n" << type << " too short, must be atleast 5 charecters!\n";
        return false;
    }
    if (type == "Username") {
        if (fileData["Users"].count(input) != 0)
        {
            cout << "\nUsername already taken, please try again.\n";
            return false;
        }
    }
    return true;
}

void updateFile()
{
    string jsonString = fileData.dump(2);
    ofstream jsonFile("registration.json");
    jsonFile << jsonString;
}

void signUp()
{
    bool usernameValid = false;
    bool passwordValid = false;
    string username;
    string password;
    while (!usernameValid)  
    {
        cout << "Enter username:\n";
        cin >> username;
        usernameValid = isInputValid(username, "Username");
    }
    while (!passwordValid)
    {
        cout << "Enter password:\n";
        cin >> password;
        passwordValid = isInputValid(password, "Password");
    }
    fileData["Users"][username]["password"] = password;
    fileData["Users"][username]["biography"] = "Empty!";
    updateFile();
}

void changeUserData(string option, json* userData, string user)
{
    cout << "Enter your new " << option << "\n";
    string output;
    bool inputValid = false;
    while (!inputValid)
    {
        getline(cin, output);
        inputValid = isInputValid(output, stringUpper(option));
    }
    if (option == "username")
    {
        fileData["Users"][output] = *userData;
        fileData["Users"].erase(user);
    }
    else {
        fileData["Users"][user][option] = output;
    }
    
    updateFile(); // update the file
}

void profileScreen(string user, json* userPtr, bool isAdmin)
{
    bool optionValid = false;
    json userData = *userPtr;

    while (!optionValid)
    {
        if (isAdmin) {
            cout << "\n" << user << "'s profile\n";
        }
        else {
            cout << "\nWelcome, " << user << "!\n";
        }

        cout << "biography: " << userData["biography"] << "\n\n";
        cout << "Options:\n";
        cout << "1: Change username\n";
        cout << "2: Change password\n";
        cout << "3: Edit bio\n";
        cout << "4: " << (isAdmin ? "Exit" : "Log out") << "\n";
        string output;
        cin >> output;

        switch (output[0])
        {
        case '1':
            optionValid = true;
            changeUserData("username", userPtr, user);
            break;

        case '2':
            optionValid = true;
            changeUserData("password", userPtr, user);
            break;

        case '3':
            optionValid = true;
            changeUserData("biography", userPtr, user);
            break;

        case '4':
            optionValid = true;
            break;
        }
    }
}

void printAllUsers(string operation)
{
    bool isPass = (operation == "passwords");
    bool isDel = (operation == "delete");
    if (isPass) {
        cout << "User : Pass\n";
    }
    else {
        cout << "\nInput the index of a user to " << operation << ":\n";
    }
    int iteration = 0;
    for (json::iterator i = fileData["Users"].begin(); i != fileData["Users"].end(); ++i)
    {
        if (isPass) {
            cout << i.key() << " : " << i.value()["password"] << '\n';
        }
        else {
            cout << iteration << ": " << i.key() << '\n';
        }
        iteration++;
    }

    if (!isPass) {
        string option;
        cin >> option;
        int selectedInt = option[0] - '0';
        iteration = 0;
        string toDel;
        for (json::iterator i = fileData["Users"].begin(); i != fileData["Users"].end(); ++i)
        {
            if (iteration == selectedInt) {
                if (operation == "modify") {
                    string user = i.key();
                    profileScreen(user, &fileData["Users"][user], true);
                }
                if (operation == "delete") {
                    toDel = i.key();
                }
            }
            iteration++;
        }
        fileData["Users"].erase(toDel);
        updateFile();
    }
}

void adminWindow()
{
    string output;
    bool optionValid = false;
    while (!optionValid)
    {
        cout << "\nReady to hack and things!!?\n";
        cout << "1: Edit profiles\n";
        cout << "2: Delete profiles\n";
        cout << "3: View all user's passwords\n";
        cout << "4: Delete JSON file\n";
        cout << "5: Log out\n";
        cin >> output;
        switch (output[0])
        {
        case '1':
            optionValid = true;
            printAllUsers("modify");
            break;

        case '2':
            optionValid = true;
            printAllUsers("delete");
            break;

        case '3':
            optionValid = true;
            printAllUsers("passwords");
            break;

        case '4':
            optionValid = true;
            clearFile();
            break;

        case '5':
            optionValid = true;
        }
    }
}

void logIn()
{
    bool validUsername = false;
    bool validPassword = false;
    string username;
    string password;

    cout << "Enter username:\n";
    while (!validUsername)
    {
        cin >> username;
        if (fileData["Users"].contains(username) || username == "admin")
        {
            validUsername = true;
        }
        else {
            cout << "Invalid username, try again:\n";
        }
    }
    cout << "Enter password:\n";
    while (!validPassword)
    {
        cin >> password;

        if (fileData["Users"][username]["password"] == password || password == "admin")
        {
            validPassword = true;
            validUsername = false;
        }
        else {
            cout << "Invalid password, try again:\n";
        }
    }
    if (username == "admin" && password == "admin") {
        adminWindow();
    }
    else {
        profileScreen(username, &fileData["Users"][username], false);
    }
}

void welcomeScreen()
{
    cout << "\nWelcome to bla h bl center\n";
    cout << "1: Sign Up\n";
    cout << "2: Log In\n";
    cout << "Input option:\n";
}

void startingScreen()
{
    while (true)
    {
        welcomeScreen();
        string option;
        bool validOption = false;
        while (!validOption)
        {
            cin >> option;
            switch (option[0]) {
            case '1':
                signUp();
                validOption = true;
                break;

            case '2':
                logIn();
                validOption = true;
                break;

            default:
                cout << "Invalid option!\n\n";
                welcomeScreen();
                break;
            }
        }
    }
}

int main()
{
    startingScreen();
}