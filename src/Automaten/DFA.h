//
// Created by nolan on 3/03/24.
//

#ifndef UNTITLED2_DFA_H
#define UNTITLED2_DFA_H

#include <string>
#include <vector>

#include "json.hpp"
#include "Table.h"


using json = nlohmann::json;
using namespace std;

class RE;
class State;
class DFA {
    std::vector<std::string> alphabet_;
    std::string initialState_;
    std::vector<std::string> acceptingStates_;
    std::map<std::string, std::map<std::string, std::string>> transitions_;
    Table* table = nullptr;
    std::string filename;
public:
    DFA(std::string filename);

    DFA(const std::vector<std::string> &alphabet, const std::string &initialState,
        const std::vector<std::string> &acceptingStates,
        const std::map<std::string, std::map<std::string, std::string>> &transitions);

    virtual ~DFA();

    bool accepts(std::string input) const;

    void print() const;

    const std::vector<std::string> &getAlphabet() const;

    const std::string &getInitialState() const;

    const std::vector<std::string> &getAcceptingStates() const;

    const std::map<std::string, std::map<std::string, std::string>> &getTransitions() const;

    DFA(DFA dfa1,DFA dfa2,bool doorsnede);

    DFA minimize();

    void printTable() const;

    bool operator==(DFA& other);

    State* findName(vector<State>& states, const string& name);

    RE toRE();

    string stateEliminationMethod(vector<State> &states, State *start, State *accept);

    State *findName(const string &name);

    string concatenateRegex(const string &r1, const string &r2);

    string unionRegex(const string &r1, const string &r2);

    void eliminateState(map<std::string, std::string> &regexTransitions, const string &stateToRemove);
};


#endif //UNTITLED2_DFA_H
