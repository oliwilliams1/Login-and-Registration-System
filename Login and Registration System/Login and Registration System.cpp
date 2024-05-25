// Login and Registration System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
using json = nlohmann::json;
using namespace std;

json loadFile()
{
    ifstream file("registration.json");
    return json::parse(file);
}

json fileData = loadFile();

bool isInputValid(string isInputValid, string type)
{
    if (isInputValid.length() <= 3) {
        cout << type << " too short, must be atleast 5 charecters!\n";
        return false;
    }
    else {
        return true;
    }
}

int updateFile()
{
    string jsonString = fileData.dump(2);
    ofstream jsonFile("registration.json");
    jsonFile << jsonString;
    return 0;
}

int signUp()
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
    updateFile();
    return 0;
}

int logIn()
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
            cout << "Logged in!";
            validPassword = true;
            validUsername = false;
        }
        else {
            cout << "Invalid password, try again:\n";
        }
    }
    return 0;
}

int main()
{
    logIn();
}
