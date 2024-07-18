/*
*
* Filename: state_manager.cpp
* Author: Jennifer Assid
* Date: 8 July 2024
*
* Description: All variables/functions relating to the StateManager
*
*/

#include "state_manager.h"
#include "general.h"
#include "assignments.h"
#include "labs.h"

#include <iostream>

// Print the valid selections (adjusts based on the attached programs)
void Print_Selections(State* state)
{
    std::cout << state->Name_ << std::endl;
    std::cout << "Choose from one of the options below." << std::endl << std::endl;

    for (auto i = state->selections_.begin(); i != state->selections_.end(); i++)
    {
        auto tmp = state->SM_->States_.find(i->second);
        if (tmp == state->SM_->States_.end()) continue;

        State* tmp_state = tmp->second;

        if (tmp_state->Type_ == _Main_Menu || tmp_state->Type_ == _Exit_Application)
            std::cout << std::endl;

        std::cout << i->first << " : " << tmp_state->Name_ << " - " << tmp_state->Description_ << std::endl;
    }
}

StateManager::StateManager()
{
    // Adds all of the valid states to the State Manager
    AddState(_Main_Menu, new MainMenu());
    AddState(_Exit_Application, new ExitApplication());

    AddState(_Assignments, new Assignments());
    AddState(_WarBoats, new WarBoats_Assignment());
    AddState(_WarBoatsOO, new WarBoatsOO_Assignment());
    AddState(_Point, new Point_Assignment());
    AddState(_Table, new Table_Assignment());
    AddState(_List, new List_Assignment());
    AddState(_ListTemplate, new ListTemplate_Assignment());

    AddState(_Labs, new Labs());
}

void StateManager::Update(void)
{
    // If there has been a state change
    if (current_state_ != next_state_)
    {
        // Preserve the current state
        return_state_ = current_state_;

        // Determine if the yser wants to continue
        if (ConfirmStateChange())
        {
            // If this isn't the first state change
            if (current_state_ != Undefined)
            {
                // Clear console (for readability)
                system("cls");

                // Call the exit function for the state
                States_.at(current_state_)->Exit();
            }

            // Finish the state change
            current_state_ = next_state_;
        }

        // Get the current state
        State* temp = States_.at(current_state_);

        // Initialize the state
        temp->Initialize();
    }

    // Get the next state (from the current state's update function)
    next_state_ = States_.at(current_state_)->Update();
}

StateType StateManager::UserSelection(const std::map<std::string, StateType> valid_selections)
{
    printf("____________________________________________________________________\n");
    printf("\nType your selection: ");

    std::string user_input;
    std::map<std::string, StateType>::const_iterator valid_select;
    std::map<StateType, State*>::const_iterator state;

    while (true)
    {
        std::cin >> user_input;

        // Change the user's input to all CAPS for comparison purposes
        std::transform(user_input.begin(), user_input.end(), user_input.begin(),
            [](unsigned char c) {return std::toupper(c); });
        valid_select = valid_selections.find(user_input);
        
        // If the user has inputed a valid selection
        if (valid_select != valid_selections.end())
        {
            state = States_.find(valid_select->second);

            // If the valid selection has associated functionality
            if (state != States_.end())
                return state->first;
            else
                std::cout << "ERROR: No associated state for valid input - select different option: ";
        }
        else
            std::cout << "Input a valid selection from the list above: ";
    }
}

bool StateManager::ConfirmStateChange(void)
{
    if (return_state_ == Undefined) return true;

    std::cout << "\n*******************************************************************\n";
    std::cout << "\nContinue to " << States_.find(next_state_)->second->Name_;
    std::cout << " from " << States_.find(current_state_)->second->Name_ << "?\n\n";
    std::cout << "Type 'Y' to continue or 'N' to return: ";

    std::string user_input;

    while (true)
    {
        std::cin >> user_input;

        if (strcmp_insensitive(user_input, "Y"))
        {
            return_state_ = Undefined;
            return true;
        }
        if (strcmp_insensitive(user_input, "N"))
        {
            return_state_ = Undefined;
            system("cls");
            return false;
        }

        std::cout << "Input a valid selection (Y/N): ";
    }
}


// Compare the strings without considering the "size" of the letter
bool StateManager::strcmp_insensitive(std::string s1, std::string s2)
{
    if (s1.length() != s2.length()) return false;

    for (unsigned int i = 0; i < s1.length(); i++)
    {
        if (std::tolower(s1[i]) != std::tolower(s2[i]))
            return false;
    }

    return true;
}