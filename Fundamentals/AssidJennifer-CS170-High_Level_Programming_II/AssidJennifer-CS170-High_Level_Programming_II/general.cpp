/*
*
* Filename: general.cpp
* Author: Jennifer Assid
* Date: 8 July 2024
*
* Description: Handles all states needed to run the basic framework
*
*/

#include "general.h"

#include <iostream>
#include <tuple>

// Add various user selections to their associated menu
MainMenu::MainMenu() : State(_Main_Menu, 
    "Main Menu", 
    "Base menu to access all contained content.")
{
    AddSelection("X", _Exit_Application);
    AddSelection("A", _Assignments);
    AddSelection("L", _Labs);
}

StateType MainMenu::Update()
{
    if (SM_ == nullptr) return Undefined;

    return SM_->UserSelection(selections_);
}

Assignments::Assignments() : State(_Assignments,
    "Assignments",
    "Run assignment programs.")
{
    AddSelection("0", _WarBoats);
    AddSelection("1", _WarBoatsOO);
    AddSelection("2", _Point);
    AddSelection("3", _Table);
    AddSelection("4", _List);
    AddSelection("5", _ListTemplate);
    AddSelection("M", _Main_Menu);
}

StateType Assignments::Update()
{
    if (SM_ == nullptr) return Undefined;

    return SM_->UserSelection(selections_);
}

void ExitApplication::Initialize()
{
    std::cout << "Thank you for checking out Jennifer Assid's work for CS170: High-Level Programmming Language II" << std::endl;
}

StateType ExitApplication::Update()
{
    SM_->Program_Running_ = false;

    return Type_;
}

StateType SubMenu::Update()
{
    if (SM_ == nullptr) return Undefined;

    FUNC_PTR tmp = UserSelection();

    // If the user has noted to return - return to the previous menu
    if (tmp == nullptr)
    {
        if (assignments_ == true)
            return _Assignments;
        else
            return _Main_Menu;
    }

    RunProgram(tmp);

    return Type_;
}

void SubMenu::Print_Programs()
{
    std::cout << Name_ << std::endl;
    std::cout << "Choose from one of the options below." << std::endl << std::endl;

    for (unsigned int i = 0; i < programs_.size(); i++)
    {
        std::cout << i << " : " << std::get<0>(programs_[i]) << " - " << std::get<1>(programs_[i]) << std::endl;
    }

    std::cout << std::endl << "R : Return - Go to previous menu." << std::endl;
}

void SubMenu::RunProgram(FUNC_PTR func)
{
    func();

    std::cout << "Program completed - returning to " << Name_ << std::endl << std::endl;

    // Wait for user input to continue
    system("pause");

    SM_->RestartState();
}

FUNC_PTR SubMenu::UserSelection()
{
    printf("____________________________________________________________________\n");
    printf("\nType your selection: ");

    std::string user_input;
    unsigned int i_user_input;

    while (true)
    {
        std::cin >> user_input;

        // Return to the previous menu
        if (user_input == "r" || user_input == "R")
            return nullptr;

        try
        {
            // Convert the user input to an int to parse
            i_user_input = std::stoi(user_input);

            // If the input is valid - return the associated function
            if (i_user_input >= 0 && i_user_input < programs_.size())
                return std::get<2>(programs_[i_user_input]);
            else
                std::cout << "Input a valid selection from the list above: ";
        }
        catch (std::exception e)
        {
            // Catch any 'unacceptable' input
            std::cout << "Input a valid selection from the list above: ";
            continue;
        }
    }

    return nullptr;
}