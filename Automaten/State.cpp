//
// Created by ruben on 22/06/24.
//


#include "State.h"

State::State(const std::string &name, bool begin, bool end) : name(name), starting(begin), accepting(end) {}
State::State() {}
void State::addTransition(std::tuple<std::string, State*> trans) {
    transitions.push_back(trans);
}
void State::printTransition() {
    for (const auto& trans : transitions) {
        std::cout << "From: " << name << std::endl;
        std::cout << "To: " << std::get<1>(trans)->name << std::endl;
        std::cout << "Input: " << std::get<0>(trans) << std::endl << std::endl;
    }
}
void State::toRegex(map<string, string>& regexTransitions) {
    for (auto& transition : transitions) {
        string input = get<0>(transition);
        State* toState = get<1>(transition);
        string key = name + "," + toState->name;
        if (regexTransitions.find(key) == regexTransitions.end()) {
            regexTransitions[key] = input;
        } else {
            if (regexTransitions[key]<input) {
                regexTransitions[key] = "(" + regexTransitions[key] + "+" + input + ")";
            } else {
                regexTransitions[key] = "(" + input + "+" + regexTransitions[key] + ")";
            }
        }
    }
}

bool State::isBegin() const {
    return starting;
}

bool State::isAnEnd() const {
    return accepting;
}

const vector<std::tuple<std::string, State *>> &State::getTransitions() const {
    return transitions;
}
