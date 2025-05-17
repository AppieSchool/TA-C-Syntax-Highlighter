#include "ENFA.h"

#include <iostream>
#include <queue>

#include "json.hpp"
using json = nlohmann::json;

// ENFA_state implementations
ENFA_state::ENFA_state() {}

ENFA_state::ENFA_state(std::string name, bool is_final, bool is_start)
        : name(std::move(name)), is_final(is_final), is_start(is_start) {}

bool ENFA_state::is_final_state() { return is_final; }
bool ENFA_state::is_start_state() { return is_start; }
void ENFA_state::set_name(std::string name) { this->name = std::move(name); }
std::string ENFA_state::get_name() { return name; }
void ENFA_state::set_start(bool is_start) { this->is_start = is_start; }
void ENFA_state::set_final(bool is_final) { this->is_final = is_final; }

void ENFA_state::add_transition(std::string input, std::shared_ptr<ENFA_state> state) {
    transitions.insert({input, state});
}

std::multimap<std::string, std::shared_ptr<ENFA_state>> ENFA_state::get_alltransitions() {
    return transitions;
}

std::vector<std::shared_ptr<ENFA_state>> ENFA_state::get_e_closure() {
    return e_closure;
}

void ENFA_state::set_e_closure(std::vector<std::shared_ptr<ENFA_state>> closure) {
    e_closure = std::move(closure);
}

std::vector<std::shared_ptr<ENFA_state>> ENFA_state::get_nextStates(std::string input) {
    std::vector<std::shared_ptr<ENFA_state>> result;
    for (auto& [key, state] : transitions) {
        if (key == input && state) {
            result.push_back(state);
        }
    }
    return result;
}

// DFA_state implementations
std::string DFA_state::getName() {
    return name;
}

bool DFA_state::getisEndState() const {
    return endState;
}

bool DFA_state::getisBeginState() const {
    return beginState;
}

void DFA_state::setName(std::string named) {
    name = named;
}

void DFA_state::setEndState(bool isEndState) {
    endState = isEndState;
}

void DFA_state::setBeginState(bool isBeginState) {
    beginState = isBeginState;
}

void DFA_state::addTransition(std::string inputSymbol, std::shared_ptr<DFA_state> nextState) {
    transitie[inputSymbol] = nextState;

    if (regx_transitie.count(nextState) > 0) {
        std::string inputstring = regx_transitie[nextState] + "+" + inputSymbol;
        SortString(inputstring);
        regx_transitie[nextState] = inputstring;
    } else {
        regx_transitie[nextState] = inputSymbol;
    }
}

std::map<std::string, std::shared_ptr<DFA_state>> DFA_state::getSymbolTransitions() const {
    return transitie;
}

void DFA_state::SortString(std::string &str) {
    std::vector<char> chars;
    for (char c : str) {
        if (c != '+') {
            chars.push_back(c);
        }
    }

    std::sort(chars.begin(), chars.end());

    str = "";
    for (size_t i = 0; i < chars.size(); ++i) {
        str += chars[i];
        if (i != chars.size() - 1) {
            str += "+";
        }
    }
}

std::map<std::shared_ptr<DFA_state>, std::string> DFA_state::getTransitions() {
    return regx_transitie;
}

std::shared_ptr<DFA_state> DFA_state::volgendeState(std::string input) {
    auto it = transitie.find(input);
    if (it != transitie.end()) {
        return it->second;
    }
    return nullptr;
}


// ENFA function implementations
ENFA::ENFA() {}


ENFA::ENFA(std::string filename) : file(filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    json j;
    input >> j;

    for (const auto &symbol : j["alphabet"]) {
        alphabet.push_back(symbol);
    }

    eps = j["eps"];

    std::unordered_map<std::string, std::shared_ptr<ENFA_state>> state_map;
    for (const auto &state_json : j["states"]) {
        std::string name = state_json["name"];
        bool accepting = state_json["accepting"];
        bool starting = state_json["starting"];

        std::shared_ptr<ENFA_state> state = make_shared<ENFA_state>(ENFA_state(name, accepting, starting));
        states.push_back(state);
        state_map[name] = state;

        if (starting) {
            start_state = state;
        }
    }


    for (const auto &transition : j["transitions"]) {
        std::string from_name = transition["from"];
        std::string to_name = transition["to"];
        std::string input = transition["input"];

        std::shared_ptr<ENFA_state> from_state = state_map[from_name];
        std::shared_ptr<ENFA_state> to_state = state_map[to_name];

        from_state->add_transition(input, to_state);
    }
}




void ENFA::add_state(std::shared_ptr<ENFA_state> state) {
    states.push_back(state);
}

void ENFA::set_alphabet(std::vector<std::string> alphabet) {
    this->alphabet = alphabet;
}

std::string ENFA::get_eps() {
    return eps;
}

void ENFA::set_start(std::shared_ptr<ENFA_state> state) {
    start_state = state;
}

void ENFA::add_final_state(std::shared_ptr<ENFA_state> state) {
    final_states.push_back(state);
}

std::shared_ptr<ENFA_state> ENFA::get_start_state() {
    return start_state;
}

std::vector<std::shared_ptr<ENFA_state>> ENFA::get_final_states() {
    return final_states;
}

std::vector<std::shared_ptr<ENFA_state>> ENFA::get_states() {
    return states;
}

void ENFA::set_eps(std::string eps) {
    this->eps = eps;
}

void ENFA::printStats() {
    std::cout << "no_of_states=" << this->states.size() << std::endl;
    int eps_counter = 0;

    for (std::shared_ptr<ENFA_state> state : states) {
        for (auto transition : state->get_alltransitions()) {
            if (transition.first == eps) {
                eps_counter++;
            }
        }
    }

    std::cout << "no_of_transitions[" << eps << "]=" << eps_counter << std::endl;

    // Sort the alphabet vector in ascending order
    std::sort(this->alphabet.begin(), this->alphabet.end());

    // Now iterate over the sorted alphabet vector
    for (const std::string& alpha : this->alphabet) {
        int counter = 0;
        for (std::shared_ptr<ENFA_state> state : states) {
            for (auto transition : state->get_alltransitions()) {
                if (transition.first == alpha) {
                    counter++;
                }
            }
        }
        std::cout << "no_of_transitions[" << alpha << "]=" << counter << std::endl;
    }

    std::map<int, int> degree_map;

    for (std::shared_ptr<ENFA_state> state : states) {
        int degree = state->get_alltransitions().size();
        degree_map[degree]++;
    }

    for (auto degree : degree_map) {
        std::cout << "degree[" << degree.first << "]=" << degree.second << std::endl;
    }
}

bool ENFA::accepts(const std::string& inputString) {
    std::set<std::shared_ptr<ENFA_state>> currentState = epsilonClosure(start_state);
    for (char input : inputString) {
        std::string inputStr = std::string(1, input);
        std::set<std::shared_ptr<ENFA_state>> newStates;
        for (std::shared_ptr<ENFA_state> state : currentState) {
            std::vector<std::shared_ptr<ENFA_state>> nextStates = state->get_nextStates(inputStr);
            for (std::shared_ptr<ENFA_state> nextState : nextStates) {
                std::set<std::shared_ptr<ENFA_state>> epsilonClosureSet = epsilonClosure(nextState);
                newStates.insert(epsilonClosureSet.begin(), epsilonClosureSet.end());
            }
        }
        currentState = newStates;
    }
    for (std::shared_ptr<ENFA_state> state : currentState) {
        if (state->is_final_state()) {
            return true;
        }
    }
    return false;
}

DFA ENFA::toDFA() {
    std::vector<std::shared_ptr<DFA_state>> dfaStates;

    std::map<std::set<std::shared_ptr<ENFA_state>>, std::shared_ptr<DFA_state>> stateMap;
    std::queue<std::set<std::shared_ptr<ENFA_state>>> stateQueue;

    std::set<std::shared_ptr<ENFA_state>> startState = epsilonClosure(start_state);
    stateQueue.push(startState);

    while (!stateQueue.empty()) {
        std::set<std::shared_ptr<ENFA_state>> nfaStates = stateQueue.front();
        stateQueue.pop();

        std::shared_ptr<DFA_state> dfaState;
        if (stateMap.count(nfaStates) == 0) {
            dfaState = make_DFA_state(nfaStates);
            stateMap[nfaStates] = dfaState;
            dfaStates.push_back(dfaState);
        } else {
            dfaState = stateMap[nfaStates];
        }

        for (const std::string &symbol: alphabet) {
            std::set<std::shared_ptr<ENFA_state>> newNFAStatesSet;
            for (std::shared_ptr<ENFA_state> nfaState: nfaStates) {
                std::vector<std::shared_ptr<ENFA_state>> nextStates = nfaState->get_nextStates(symbol);
                for (std::shared_ptr<ENFA_state> nextState: nextStates) {
                    std::set<std::shared_ptr<ENFA_state>> epsilonClosureSet = epsilonClosure(nextState);
                    newNFAStatesSet.insert(epsilonClosureSet.begin(), epsilonClosureSet.end());
                }
            }

            std::shared_ptr<DFA_state> newDFAState;
            if (stateMap.count(newNFAStatesSet) == 0) {
                newDFAState = make_DFA_state(newNFAStatesSet);
                stateMap[newNFAStatesSet] = newDFAState;
                dfaStates.push_back(newDFAState);
                stateQueue.push(newNFAStatesSet);
            } else {
                newDFAState = stateMap[newNFAStatesSet];
            }

            dfaState->addTransition(symbol, newDFAState);
        }
    }

    auto beginstate_DFA = stateMap[startState];
    beginstate_DFA->setBeginState(true);

    // Prepare data for the DFA constructor
    std::vector<std::string> acceptingStateNames;
    std::map<std::string, std::map<std::string, std::string>> transitionMap;
    std::string initialStateName = beginstate_DFA->getName(); // from earlier

    for (const auto &dfaState: dfaStates) {
        std::string stateName = dfaState->getName();
        if (dfaState->getisEndState()) {
            acceptingStateNames.push_back(stateName);
        }

        for (auto &pair: dfaState->getSymbolTransitions()) {
            const auto &input = pair.first;
            const auto &nextState = pair.second;
            transitionMap[stateName][input] = nextState->getName();
        }
    }
    return DFA(alphabet, initialStateName, acceptingStateNames, transitionMap);
}

std::set<std::shared_ptr<ENFA_state>> ENFA::epsilonClosure(std::shared_ptr<ENFA_state> state) {
    std::set<std::shared_ptr<ENFA_state>> closure;
    std::queue<std::shared_ptr<ENFA_state>> queue;

    queue.push(state);
    closure.insert(state);

    while (!queue.empty()) {
        std::shared_ptr<ENFA_state> current = queue.front();
        queue.pop();
        std::vector<std::shared_ptr<ENFA_state>> nextStates = current->get_nextStates(eps);
        for (std::shared_ptr<ENFA_state> nextState : nextStates) {
            if (closure.find(nextState) == closure.end()) {
                closure.insert(nextState);
                queue.push(nextState);
            }
        }
    }

    return closure;
}


std::shared_ptr<DFA_state> ENFA::make_DFA_state(const std::set<std::shared_ptr<ENFA_state>>& states_set) {
    auto newDFAstate = make_shared<DFA_state>(DFA_state());
    std::set<string> DFA_state_names;
    if (states_set.empty()){
        newDFAstate->setName("{}");
        return newDFAstate;
    }

    for (std::shared_ptr<ENFA_state> state : states_set){
        if(state->is_final_state()){
            newDFAstate->setEndState(true);
        }
        DFA_state_names.insert(state->get_name());
    }

    string name_state =  "{";
    for (const string& name : DFA_state_names){
        name_state += name + ",";
    }

    if (!DFA_state_names.empty()) {
        name_state.pop_back();
    }
    name_state += "}";
    newDFAstate->setName(name_state);
    return newDFAstate;
}
