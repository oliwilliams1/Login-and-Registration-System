#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <cctype>

using json = nlohmann::json;
using namespace std;

json loadFile()
{
    ifstream file("registration.json");
    return json::parse(file);
}

json fileData = loadFile();

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
    fileData["Users"][username]["bio"] = "Empty!";
    updateFile();
}

void changeUserOrPass(string option, json* userData, string user)
{
    cout << "Enter your new " << option << "\n";
    string output;
    bool inputValid = false;
    while (!inputValid)
    {
        getline(cin, output);;
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


void profileScreen(string user, json* userPtr)
{
    bool optionValid = false;
    json userData = *userPtr;

    while (!optionValid)
    {
        cout << "\nWelcome, " << user << "!\n";
        cout << "biography: " << userData["biography"] << "\n\n";
        cout << "Options:\n";
        cout << "1: Change username\n";
        cout << "2: Change password\n";
        cout << ":3 Edit bio\n";
        cout << "4: Log out\n";
        string output;
        cin >> output;

        switch (output[0])
        {
        case '1':
            optionValid = true;
            changeUserOrPass("username", userPtr, user);

        case '2':
            optionValid = true;
            changeUserOrPass("password", userPtr, user);

        case '3':
            optionValid = true;
            changeUserOrPass("biography", userPtr, user);

        case '4':
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
        if (fileData["Users"].contains(username))
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

        if (fileData["Users"][username]["password"] == password)
        {
            validPassword = true;
            validUsername = false;
        }
        else {
            cout << "Invalid password, try again:\n";
        }
    }
    profileScreen(username, &fileData["Users"][username]);
}

void welcomeScreen()
{
    cout << "Welcome to bla h bl center\n";
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
