/*
*
* Filename: state_manager.h
* Author: Jennifer Assid
* Date: 8 July 2024
*
* Description: All variables/functions relating to the StateManager
*
*/
#pragma once

#include <string>
#include <algorithm>
#include <cctype>
#include <map>

// All valid state types
typedef enum
{
    Undefined = -1,
    _Main_Menu, _Exit_Application,

    _Assignments,
        _Splitter,
        _Sparsevector,
        _Sparsevector_CPP,
        _Lazycopy,

    _Labs
} StateType;

class StateManager;

// The States that the Manager handles ('menus' for the user to interact wtih)
class State
{
public:
    State() = delete;
    State(StateType type, std::string name, std::string description) :
        Type_(type),
        Name_(name),
        Description_(description)
    {}

    virtual ~State() { selections_.clear(); }

    inline void AddSelection(std::string input, StateType type)
    {
        // make the "key" all uppercase (for consistency)
        std::transform(input.begin(), input.end(), input.begin(),
            [](unsigned char c) {return std::toupper(c); });
        selections_.insert(std::pair<std::string, StateType>{input, type});
    }

    virtual void Initialize(void) { Print_Selections(this); }
    virtual StateType Update(void) { return Undefined; }
    virtual void Exit(void) {}

    StateType Type_;
    std::string Name_;
    std::string Description_;

    StateManager* SM_;

protected:

    std::map<std::string, StateType> selections_;

    friend void Print_Selections(State *state);
};

class StateManager
{
public:
    StateManager();
    ~StateManager() { States_.clear(); }

    inline void AddState(StateType type, State* state) 
    { 
        state->SM_ = this;
        States_.insert(std::pair<StateType, State*>(type, state)); 
    }

    void Update(void);

    inline void RestartState(void) 
    { 
        // Clear the console (for easy readability)
        system("cls");

        // Re-initialize the 
        States_.find(current_state_)->second->Initialize(); 
    }
    
    // Change the state based off of the user selection
    StateType UserSelection(const std::map<std::string, StateType> valid_selections);

    bool Program_Running_ = true;
    std::map<StateType, State*> States_;

private:
    bool ConfirmStateChange(void);

    bool strcmp_insensitive(std::string s1, std::string s2);
    
private:
    StateType return_state_ = Undefined;
    StateType current_state_ = Undefined;
    StateType next_state_ = _Main_Menu;
};