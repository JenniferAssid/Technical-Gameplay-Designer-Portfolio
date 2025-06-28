/*
*
* Filename: general.h
* Author: Jennifer Assid
* Date: 8 July 2024
*
* Description: Handles all states needed to run the basic framework
*
*/

#pragma once
#include "state_manager.h"
#include <vector>

// Establish all of the main states for the application
class MainMenu : public State
{
public:
    MainMenu();

    StateType Update(void) override;
};

class Assignments : public State
{
public:
    Assignments();

    StateType Update(void) override;
};

class ExitApplication : public State
{
public:
    ExitApplication() : State(_Exit_Application, "Exit Application", "Close the application.") {}

    void Initialize(void) override;
    StateType Update(void) override;
};


typedef void (*FUNC_PTR)(void); // Function pointer for direct calling

// Set up the base submenu class to run secondary tests associated with a larger "idea" (i.e., test functions for an assignment)
class SubMenu : public State
{
public:
    // No default constructor (to make sure that any class created has relevant information)
    SubMenu() = delete;
    SubMenu(StateType type,
        std::string name,
        std::string description, 
        bool assignment = false) : State(type,
                                name,
                                description), 
        assignments_(assignment) 
    {}

    ~SubMenu() override
    {
        programs_.clear();
    }

    void Initialize(void) override { Print_Programs(); }
    StateType Update(void) override;

protected:
    std::vector<std::tuple<std::string, std::string, FUNC_PTR>> programs_;
    bool assignments_ = false;

protected:
    // Add a 'program' (executable function) for the user to selection
    inline void AddProgram(std::string name, std::string description, FUNC_PTR function)
    {
        programs_.push_back(std::tuple<std::string, std::string, FUNC_PTR>{name, description, function});
    }

    void Print_Programs();

    void RunProgram(FUNC_PTR);
    FUNC_PTR UserSelection();
};