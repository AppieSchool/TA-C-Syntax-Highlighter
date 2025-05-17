#include "ENFA.h"

#include <iostream>
#include <queue>

#include "json.hpp"
using json = nlohmann::json;

// ENFA_state function implementations
ENFA_state::ENFA_state() {}


ENFA_state::ENFA_state(std::string name, bool is_final, bool is_start) {
    this->name = name;
    this->is_final = is_final;
    this->is_start = is_start;
}



bool ENFA_state::is_final_state() {
    return is_final;
}

bool ENFA_state::is_start_state() {
    return is_start;
}

void ENFA_state::set_name(std::string name) {
    this->name = name;
}

std::string ENFA_state::get_name() {
    return name;
}

void ENFA_state::set_start(bool is_start) {
    this->is_start = is_start;
}

void ENFA_state::set_final(bool is_final) {
    this->is_final = is_final;
}

void ENFA_state::add_transition(std::string input, ENFA_state *state) {
    transitions.insert(std::make_pair(input, state));
}

std::multimap<std::string, ENFA_state *> ENFA_state::get_alltransitions() {
    if (this == nullptr) {
        throw std::runtime_error("Attempted to call get_alltransitions on a null ENFA_state pointer");
    }
    return transitions;
}

std::vector<ENFA_state *> ENFA_state::get_e_closure() {
    return e_closure;
}

void ENFA_state::set_e_closure(std::vector<ENFA_state *> e_closure) {
    this->e_closure = e_closure;
}

std::vector<ENFA_state *> ENFA_state::get_nextStates(std::string input) {
    std::vector<ENFA_state *> result;
    for (auto transition : get_alltransitions()) {
        if(transition.first == input) {
            ENFA_state* next_state = transition.second;
            if(next_state != nullptr) {
                result.push_back(next_state);
            }
        }
    }
    return result;
}


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

void DFA_state::addTransition(std::string inputSymbol, DFA_state *nextState) {
    transitie[inputSymbol] = nextState;

    if (regx_transitie.count(nextState) > 0) {
        std::string inputstring = regx_transitie[nextState] + "+" + inputSymbol;
        SortString(inputstring);
        regx_transitie[nextState] = inputstring;
    } else {
        regx_transitie[nextState] = inputSymbol;
    }
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

std::map<DFA_state*, std::string> DFA_state::getTransitions() {
    return regx_transitie;
}

DFA_state* DFA_state::volgendeState(std::string input) {
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

    std::unordered_map<std::string, ENFA_state*> state_map;
    for (const auto &state_json : j["states"]) {
        std::string name = state_json["name"];
        bool accepting = state_json["accepting"];
        bool starting = state_json["starting"];

        ENFA_state* state = new ENFA_state(name, accepting, starting);
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

        ENFA_state* from_state = state_map[from_name];
        ENFA_state* to_state = state_map[to_name];

        from_state->add_transition(input, to_state);
    }
}




void ENFA::add_state(ENFA_state *state) {
    states.push_back(state);
}

void ENFA::set_alphabet(std::vector<std::string> alphabet) {
    this->alphabet = alphabet;
}

std::string ENFA::get_eps() {
    return eps;
}

void ENFA::set_start(ENFA_state *state) {
    start_state = state;
}

void ENFA::add_final_state(ENFA_state *state) {
    final_states.push_back(state);
}

ENFA_state* ENFA::get_start_state() {
    return start_state;
}

std::vector<ENFA_state *> ENFA::get_final_states() {
    return final_states;
}

std::vector<ENFA_state *> ENFA::get_states() {
    return states;
}

void ENFA::set_eps(std::string eps) {
    this->eps = eps;
}

void ENFA::printStats() {
    std::cout << "no_of_states=" << this->states.size() << std::endl;
    int eps_counter = 0;

    for (ENFA_state* state : states) {
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
        for (ENFA_state* state : states) {
            for (auto transition : state->get_alltransitions()) {
                if (transition.first == alpha) {
                    counter++;
                }
            }
        }
        std::cout << "no_of_transitions[" << alpha << "]=" << counter << std::endl;
    }

    std::map<int, int> degree_map;

    for (ENFA_state* state : states) {
        int degree = state->get_alltransitions().size();
        degree_map[degree]++;
    }

    for (auto degree : degree_map) {
        std::cout << "degree[" << degree.first << "]=" << degree.second << std::endl;
    }
}

bool ENFA::accepts(const std::string& inputString) {
    std::set<ENFA_state*> currentState = epsilonClosure(start_state);
    for (char input : inputString) {
        std::string inputStr = std::string(1, input);
        std::set<ENFA_state*> newStates;
        for (ENFA_state* state : currentState) {
            std::vector<ENFA_state*> nextStates = state->get_nextStates(inputStr);
            for (ENFA_state* nextState : nextStates) {
                std::set<ENFA_state*> epsilonClosureSet = epsilonClosure(nextState);
                newStates.insert(epsilonClosureSet.begin(), epsilonClosureSet.end());
            }
        }
        currentState = newStates;
    }
    for (ENFA_state* state : currentState) {
        if (state->is_final_state()) {
            return true;
        }
    }
    return false;
}

DFA ENFA::toDFA() {
    json DFA_JSON;
    DFA_JSON["type"] = "DFA";
    DFA_JSON["alphabet"] = this->alphabet;

    DFA_JSON["states"] = json::array();
    DFA_JSON["transitions"] = json::array();

    std::vector<DFA_state*> dfaStates;


    std::map<std::set<ENFA_state*>, DFA_state*> stateMap;
    queue<std::set<ENFA_state*>> stateQueue;

    std::set<ENFA_state*> startState = epsilonClosure(start_state);
    stateQueue.push(startState);

    while (!stateQueue.empty()) {
        std::set<ENFA_state*> nfaStates = stateQueue.front();
        stateQueue.pop();

        DFA_state* dfaState;
        if (stateMap.count(nfaStates) == 0) {
            dfaState = make_DFA_state(nfaStates);
            stateMap[nfaStates] = dfaState;
            dfaStates.push_back(dfaState);
        } else {
            dfaState = stateMap[nfaStates];
        }

        for (const std::string& symbol : alphabet) {
            std::set<ENFA_state*> newNFAStatesSet;
            for (ENFA_state* nfaState : nfaStates) {
                std::vector<ENFA_state*> nextStates = nfaState->get_nextStates(symbol);
                for (ENFA_state* nextState : nextStates) {
                    std::set<ENFA_state*> epsilonClosureSet = epsilonClosure(nextState);
                    newNFAStatesSet.insert(epsilonClosureSet.begin(), epsilonClosureSet.end());
                }
            }

            DFA_state* newDFAState;
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

    auto *beginstate_DFA = stateMap[startState];
    beginstate_DFA->setBeginState(true);

    for (int i = 0; i < dfaStates.size(); i++) {
        json state;
        state["name"] = dfaStates[i]->getName();
        state["accepting"] = dfaStates[i]->getisEndState();
        state["starting"] = dfaStates[i]->getisBeginState();
        DFA_JSON["states"].push_back(state);
    }

    for (int i = 0; i < states.size(); ++i) {
        for (int j = 0; j < alphabet.size(); ++j) {
            char input = alphabet[j][0];
            json transition;
            string input_string = string(1, input);
            transition["from"] = dfaStates[i]->getName();
            transition["to"] = dfaStates[i]->volgendeState(input_string)->getName();
            transition["input"] = alphabet[j];
            DFA_JSON["transitions"].push_back(transition);
        }
    }

    for (DFA_state* state : dfaStates) {
        delete state;
    }

    return DFA(DFA_JSON);
}

std::set<ENFA_state *> ENFA::epsilonClosure(ENFA_state* state) {
    std::set<ENFA_state *> closure;
    std::queue<ENFA_state *> queue;

    queue.push(state);
    closure.insert(state);

    while (!queue.empty()) {
        ENFA_state* current = queue.front();
        queue.pop();
        std::vector<ENFA_state *> nextStates = current->get_nextStates(eps);
        for (ENFA_state* nextState : nextStates) {
            if (closure.find(nextState) == closure.end()) {
                closure.insert(nextState);
                queue.push(nextState);
            }
        }
    }

    return closure;
}


DFA_state* ENFA::make_DFA_state(const std::set<ENFA_state*>& states_set) {
    auto * newDFAstate = new DFA_state();
    std::set<string> DFA_state_names;
    if (states_set.empty()){
        newDFAstate->setName("{}");
        return newDFAstate;
    }

    for (ENFA_state* state : states_set){
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
