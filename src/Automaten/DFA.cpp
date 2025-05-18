//
// Created by nolan on 3/03/24.
//

#include "DFA.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>

#include "RE.h"
#include "State.h"

using namespace std;

vector<string> getStates(const string& input) {
    vector<string> states;
    string state;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '{' || input[i] == '}' || input[i] == ' ') {
            continue;
        } else if (input[i] == ',') {
            if (!state.empty()) {
                states.push_back(state);
                state.clear();
            }
        } else {
            state.push_back(input[i]);
        }
    }
    if (!state.empty()) {
        states.push_back(state);
    }
    std::sort(states.begin(), states.end());
    states.erase(std::unique(states.begin(), states.end()), states.end());
    return states;
}

DFA::DFA(std::string filename) {
    this->filename = filename;
    ifstream input(filename);
    if (!input.is_open()) {
        cerr << "Error: Unable to open file." << endl;
        return;
    }

    // Read the JSON content
    json j;
    input >> j;
    json json_data;
    json_data = j;

    for (const auto &symbol: json_data["alphabet"]) {
        alphabet_.push_back(symbol);
    }

    for (const auto &state: json_data["states"]) {
        if (state["starting"])
            initialState_ = state["name"];
        if (state["accepting"])
            acceptingStates_.push_back(state["name"]);
    }
    for (const auto &transition: j["transitions"]) {
        transitions_[transition["from"]][transition["input"]] = transition["to"];
    }
}



DFA::~DFA() {

}

bool DFA::accepts(std::string input) const {
    std::string currentState = initialState_;
    for(char c: input) {
        std::string symbol(1, c);

        if (std::find(alphabet_.begin(), alphabet_.end(), symbol) == alphabet_.end()) {
            return false;
        }

        if (transitions_.count(currentState) == 0) {
            std::cerr << "Missing state in transitions: " << currentState << "\n";
        }
        else if (transitions_.at(currentState).count(symbol) == 0) {
            std::cerr << "Missing symbol in state " << currentState << ": " << symbol << "\n";
        }

        currentState = transitions_.at(currentState).at(symbol);  // crash likely happens here


    }
    return std::find(acceptingStates_.begin(), acceptingStates_.end(), currentState) != acceptingStates_.end();
}


void DFA::print() const {
    json j;
    j["type"] = "DFA";
    j["alphabet"] = alphabet_;


    vector<string> states_;
    for(const auto& key: transitions_)
    {
        states_.push_back(key.first);
    }


    json states_json;
    for (const auto& state : states_) {
        json state_json;
        state_json["name"] = state;
        if(state == initialState_)
            state_json["starting"] = true;
        else
            state_json["starting"] = false;
        if(count(acceptingStates_.begin(),acceptingStates_.end(),state) > 0)
            state_json["accepting"] = true;
        else
            state_json["accepting"] = false;
        states_json.push_back(state_json);
    }
    j["states"] = states_json;


    json transitions_json;
    for (const auto& transition : transitions_) {
        const auto& from_state = transition.first;
        for (const auto& input : transition.second) {
            const auto& to_state= input.second;
            const auto& input_symbol = input.first;
            transitions_json.push_back({{"from", from_state}, {"to", to_state}, {"input", input_symbol}});
        }
    }
    j["transitions"] = transitions_json;

    cout << setw(4) << j << endl;
}

DFA::DFA(const vector<std::string> &alphabet, const string &initialState, const vector<std::string> &acceptingStates,
         const map<std::string, std::map<std::string, std::string>> &transitions) : alphabet_(alphabet),
                                                                                    initialState_(initialState),
                                                                                    acceptingStates_(acceptingStates),
                                                                                    transitions_(transitions) {}


const vector<std::string> &DFA::getAlphabet() const {
    return alphabet_;
}

const string &DFA::getInitialState() const {
    return initialState_;
}

const vector<std::string> &DFA::getAcceptingStates() const {
    return acceptingStates_;
}

const map<std::string, std::map<std::string, std::string>> &DFA::getTransitions() const {
    return transitions_;
}

DFA::DFA(DFA dfa1,DFA dfa2,bool doorsnede) {
    std::vector<std::string> alphabet1 = dfa1.getAlphabet();
    std::string initialState1 = dfa1.getInitialState();
    std::vector<std::string> acceptingStates1 = dfa1.getAcceptingStates();
    std::map<std::string, std::map<std::string, std::string>> transitions1 = dfa1.getTransitions();
    std::vector<std::string> alphabet2 = dfa2.getAlphabet();
    std::string initialState2 = dfa2.getInitialState();
    std::vector<std::string> acceptingStates2 = dfa2.getAcceptingStates();
    std::map<std::string, std::map<std::string, std::string>> transitions2 = dfa2.getTransitions();
    vector<string> states1;
    vector<string> states2;
    for(const auto& key: transitions1)
    {
        states1.push_back(key.first);
    }
    for(const auto& key: transitions2)
    {
        states2.push_back(key.first);
    }


    std::map<std::string, std::map<std::string, std::string>> newTransitions;
    vector<string> states;
    vector<string> handledStates;
    vector<string> currentStates;
    vector<string> newAcceptingStates;
    string newInitialState = "(" + initialState1 + "," + initialState2 + ")";
    currentStates.push_back(newInitialState);
    vector<pair<string, pair<std::string, std::string>>> transitions;
    states.push_back(newInitialState);


    while(!currentStates.empty())
    {
        for(const auto& state : currentStates)
        {
            if(std::count(handledStates.begin(), handledStates.end(),state) > 0)
            {
                currentStates.erase(std::remove(currentStates.begin(), currentStates.end(), state), currentStates.end());
                continue;
            }
            if(state.empty())
                continue;
            vector<string> currentState;
            currentState = getStates(state);
            for(const auto& symbol : alphabet1)
            {
                string newState1 = transitions1[currentState[0]][symbol];
                string newState2 = transitions2[currentState[1]][symbol];
                string newState = "(" + newState1 + "," + newState2 + ")";
                newTransitions[state][symbol] = newState;
                states.push_back(newState);
                currentStates.push_back(newState);
                if(std::count(acceptingStates1.begin(), acceptingStates1.end(),newState1) > 0 && std::count(acceptingStates2.begin(), acceptingStates2.end(),newState2) > 0 )
                {
                    newAcceptingStates.push_back(newState);
                }
                else if((std::count(acceptingStates1.begin(), acceptingStates1.end(),newState1) > 0 || std::count(acceptingStates2.begin(), acceptingStates2.end(),newState2) > 0) && !doorsnede)
                {
                    newAcceptingStates.push_back(newState);
                }
            }
            handledStates.push_back(currentStates[0]);
        }
    }
    this->transitions_ = newTransitions;
    this->acceptingStates_ = newAcceptingStates;
    this->initialState_ = newInitialState;
    this->alphabet_ = alphabet1;
}

vector<string> sort(vector<string> states)
{
    for (int i = 0; i < states.size(); i++) {
        for (int j = 0; j < states.size() - 1; j++) {
            string state1 = states[j];
            string state2 = states[j + 1];

            // If state is in curly brackets, extract first element for comparison
            if (state1[0] == '{') {
                state1 = state1.substr(1, state1.find(',') - 1);
            }
            if (state2[0] == '{') {
                state2 = state2.substr(1, state2.find(',') - 1);
            }

            if (state1 > state2) {
                swap(states[j], states[j + 1]);
            }
        }
    }
    return states;
}

DFA DFA::minimize() {
    // Step 1: Get all states
    std::vector<std::string> states;
    for (const auto& statePair : transitions_) {
        states.push_back(statePair.first);
    }

    states = sort(states);
    int size1 = states.size();

    Table* table1 = new Table(states);

    // Step 2: Mark distinguishable pairs based on accepting vs non-accepting
    for (int i = 0; i < size1; ++i) {
        for (int j = i + 1; j < size1; ++j) {
            bool isAccepting1 = std::find(acceptingStates_.begin(), acceptingStates_.end(), states[i]) != acceptingStates_.end();
            bool isAccepting2 = std::find(acceptingStates_.begin(), acceptingStates_.end(), states[j]) != acceptingStates_.end();
            if (isAccepting1 != isAccepting2) {
                table1->setDistinguishable(states[i], states[j]);
            }
        }
    }

    // Step 3: Propagate distinguishability
    bool changed;
    do {
        changed = false;
        for (const auto& [state1, state2] : table1->getDistinguishables()) {
            for (const auto& symbol : alphabet_) {
                std::string next1 = transitions_.at(state1).at(symbol);
                std::string next2 = transitions_.at(state2).at(symbol);
                if (next1 != next2) {
                    std::string low = std::min(next1, next2);
                    std::string high = std::max(next1, next2);
                    if (table1->getDistinguishability(low, high) == "X") {
                        table1->setDistinguishable(std::min(state1, state2), std::max(state1, state2));
                        changed = true;
                    }
                }
            }
        }
    } while (changed);

    // Step 4: Find indistinguishable (mergeable) state pairs
    std::vector<std::vector<std::string>> pairs;
    for (int i = 0; i < size1; ++i) {
        for (int j = i + 1; j < size1; ++j) {
            if (table1->getDistinguishability(states[i], states[j]) == "-") {
                pairs.push_back({states[i], states[j]});
            }
        }
    }

    // Step 5: Merge overlapping sets
    bool merged;
    do {
        merged = false;
        for (size_t i = 0; i < pairs.size(); ++i) {
            for (size_t j = i + 1; j < pairs.size(); ++j) {
                for (const auto& s : pairs[i]) {
                    if (std::find(pairs[j].begin(), pairs[j].end(), s) != pairs[j].end()) {
                        // Merge j into i
                        for (const auto& state : pairs[j]) {
                            if (std::find(pairs[i].begin(), pairs[i].end(), state) == pairs[i].end()) {
                                pairs[i].push_back(state);
                            }
                        }
                        pairs.erase(pairs.begin() + j);
                        merged = true;
                        break;
                    }
                }
                if (merged) break;
            }
        }
    } while (merged);

    // Step 6: Collect merged state sets and fill unmerged ones
    std::vector<std::set<std::string>> mergedStateSets;
    std::set<std::string> mergedSeen;
    for (const auto& group : pairs) {
        std::set<std::string> groupSet(group.begin(), group.end());
        mergedStateSets.push_back(groupSet);
        mergedSeen.insert(group.begin(), group.end());
    }
    for (const auto& state : states) {
        if (!mergedSeen.count(state)) {
            mergedStateSets.push_back({state});
        }
    }

    // Step 7: Helper for consistent merged state name
    auto makeStateName = [](const std::set<std::string>& group) {
        std::vector<std::string> sorted(group.begin(), group.end());
        std::sort(sorted.begin(), sorted.end());
        std::string name = "{";
        for (size_t i = 0; i < sorted.size(); ++i) {
            name += sorted[i];
            if (i != sorted.size() - 1) name += ", ";
        }
        name += "}";
        return name;
    };

    // Step 8: Build remapping table
    std::map<std::string, std::string> stateToMerged;
    for (const auto& group : mergedStateSets) {
        std::string mergedName = makeStateName(group);
        for (const auto& state : group) {
            stateToMerged[state] = mergedName;
        }
    }

    // Step 9: Construct new DFA components
    std::string newInitialState = stateToMerged[initialState_];
    std::vector<std::string> newAcceptingStates;
    std::map<std::string, std::map<std::string, std::string>> newTransitions;

    std::set<std::string> seenAccepting;
    for (const auto& group : mergedStateSets) {
        std::string mergedName = makeStateName(group);

        // If any state in group is accepting
        for (const auto& s : group) {
            if (std::find(acceptingStates_.begin(), acceptingStates_.end(), s) != acceptingStates_.end()) {
                if (!seenAccepting.count(mergedName)) {
                    newAcceptingStates.push_back(mergedName);
                    seenAccepting.insert(mergedName);
                }
                break;
            }
        }

        // Add transitions
        for (const auto& symbol : alphabet_) {
            std::set<std::string> targets;
            for (const auto& s : group) {
                if (transitions_.count(s) && transitions_.at(s).count(symbol)) {
                    targets.insert(stateToMerged[transitions_.at(s).at(symbol)]);
                }
            }

            if (!targets.empty()) {
                // If all targets are the same merged state, take it
                std::string target = *targets.begin();
                newTransitions[mergedName][symbol] = target;
            }
        }
    }
    table = table1;
    return DFA(alphabet_, newInitialState, newAcceptingStates, newTransitions);
}

bool DFA::operator==(DFA& other) {
    // Minimize both DFAs
    DFA dfa1 = this->minimize();
    DFA dfa2 = other.minimize();

    // Compare alphabets
    if (dfa1.getAlphabet() != dfa2.getAlphabet()) {
        return false;
    }

    std::map<std::string, std::string> mapping1to2;
    std::map<std::string, std::string> mapping2to1;

    std::queue<std::pair<std::string, std::string>> q;
    q.push({dfa1.getInitialState(), dfa2.getInitialState()});
    mapping1to2[dfa1.getInitialState()] = dfa2.getInitialState();
    mapping2to1[dfa2.getInitialState()] = dfa1.getInitialState();

    auto t1 = dfa1.getTransitions();
    auto t2 = dfa2.getTransitions();

    auto a1 = dfa1.getAcceptingStates();
    auto a2 = dfa2.getAcceptingStates();

    std::set<std::string> accepting1(a1.begin(), a1.end());
    std::set<std::string> accepting2(a2.begin(), a2.end());

    while (!q.empty()) {
        auto [s1, s2] = q.front();
        q.pop();

        bool isAccepting1 = accepting1.count(s1);
        bool isAccepting2 = accepting2.count(s2);

        if (isAccepting1 != isAccepting2) return false;

        for (const std::string& sym : dfa1.getAlphabet()) {
            std::string n1 = t1[s1][sym];
            std::string n2 = t2[s2][sym];

            if (mapping1to2.count(n1) == 0 && mapping2to1.count(n2) == 0) {
                mapping1to2[n1] = n2;
                mapping2to1[n2] = n1;
                q.push({n1, n2});
            } else if (mapping1to2[n1] != n2 || mapping2to1[n2] != n1) {
                return false;
            }
        }
    }
    return true;
}

void DFA::printTable() const {
    if(table != nullptr)
        table->print();
}

State *DFA::findName(vector<State> &states, const string &name) {
    for (State& state : states) {
        if (state.name == name) {
            return &state;
        }
    }
    return nullptr;
}


State* DFA::findName(const std::string& name) {

    std::map<std::string, State*> stateMap;
    std::vector<State> states;

    // Create all state objects
    for (const auto& statePair : transitions_) {
        const std::string& stateName = statePair.first;
        bool starting = (stateName == initialState_);
        bool accepting = (std::find(acceptingStates_.begin(), acceptingStates_.end(), stateName) != acceptingStates_.end());
        stateMap[stateName] = new State(stateName, starting, accepting);
    }

    // Populate transitions
    for (const auto& statePair : transitions_) {
        State* state = stateMap[statePair.first];
        for (const auto& transitionPair : statePair.second) {
            const std::string& input = transitionPair.first;
            const std::string& nextStateName = transitionPair.second;
            State* nextState = stateMap[nextStateName];
            state->transitions.emplace_back(input, nextState);
        }
    }

    // Collect all State objects in a vector
    for (const auto& pair : stateMap) {
        states.push_back(*pair.second);
        delete pair.second;  // Clean up dynamically allocated State objects
    }

    for (State& s : states) {
        if (s.name == name) {
            return &s;
        }
    }
    return nullptr;
}

void DFA::eliminateState(map<string, string>& regexTransitions, const string& stateToRemove) {
    map<pair<string, string>, string> newTransitions;
    set<string> stateNames;

    // Collect state names from keys
    for (const auto& entry : regexTransitions) {
        size_t comma = entry.first.find(",");
        string from = entry.first.substr(0, comma);
        string to = entry.first.substr(comma + 1);
        stateNames.insert(from);
        stateNames.insert(to);
    }

    for (const string& from : stateNames) {
        for (const string& to : stateNames) {
            if (from != stateToRemove && to != stateToRemove) {
                string key1 = from + "," + stateToRemove;
                string key2 = stateToRemove + "," + stateToRemove;
                string key3 = stateToRemove + "," + to;
                string key4 = from + "," + to;

                string regex1 = regexTransitions.count(key1) ? regexTransitions[key1] : "";
                string regex2 = regexTransitions.count(key2) ? regexTransitions[key2] : "";
                string regex3 = regexTransitions.count(key3) ? regexTransitions[key3] : "";
                string regex4 = regexTransitions.count(key4) ? regexTransitions[key4] : "";

                string combinedRegex;
                if (!regex1.empty() && !regex3.empty()) {
                    combinedRegex = concatenateRegex(regex1, concatenateRegex((!regex2.empty() ? regex2 + "*" : ""), regex3));
                    if (!regex4.empty()) {
                        combinedRegex = unionRegex(combinedRegex, regex4);
                    }
                } else if (!regex4.empty()) {
                    combinedRegex = regex4;
                }

                if (!combinedRegex.empty()) {
                    newTransitions[{from, to}] = combinedRegex;
                }
            }
        }
    }

    // Clear and update the regexTransitions map
    regexTransitions.clear();
    for (const auto& trans : newTransitions) {
        regexTransitions[trans.first.first + "," + trans.first.second] = trans.second;
    }
}




string DFA::stateEliminationMethod(vector<State>& states, State* start, State* accept) {
    map<string, string> regexTransitions;
    set<string> stateNames;

    for (State& state : states) {
        state.toRegex(regexTransitions);
        stateNames.insert(state.name);
    }

    // Create a sorted vector of state names for alphabetical order deletion
    vector<string> sortedStateNames(stateNames.begin(), stateNames.end());
    sort(sortedStateNames.begin(), sortedStateNames.end());

    for (const string& stateToRemove : sortedStateNames) {
        if (stateToRemove == start->name || stateToRemove == accept->name) {
            continue;
        }

        eliminateState(regexTransitions, stateToRemove);
    }

    string startToAccept = regexTransitions[start->name + "," + accept->name];
    string startToStart = regexTransitions[start->name + "," + start->name];
    string acceptToAccept = regexTransitions[accept->name + "," + accept->name];
    string acceptToStart = regexTransitions[accept->name + "," + start->name];

    string finalRegex;
    if (!startToStart.empty() && !acceptToAccept.empty()) {
        finalRegex = "(" + startToStart + ")*" + startToAccept + "" + acceptToAccept + "*";
    } else if (!startToStart.empty()) {
        finalRegex = "(" + startToStart + ")*(" + startToAccept + ")";
    } else if (!acceptToAccept.empty()) {
        finalRegex =  startToAccept + acceptToAccept + "*";
    } else {
        finalRegex = startToAccept;
    }

    return finalRegex;
}


string DFA::concatenateRegex(const string& r1, const string& r2) {
    if (r1.empty()) return r2;
    if (r2.empty()) return r1;
    return r1 + r2;
}

string DFA::unionRegex(const string& r1, const string& r2) {
    if (r1.empty()) return r2;
    if (r2.empty()) return r1;
    if (r1 < r2) {
        return "(" + r1 + "+" + r2 + ")";
    } else {
        return "(" + r2 + "+" + r1 + ")";
    }

}

RE DFA::toRE() {
    ifstream input(filename);
    json j;
    input >> j;
    vector<string> alphabet;
    for (const auto& letter : j["alphabet"]) {
        alphabet.emplace_back(letter);
    }

    vector<State> states;
    map<string, State*> stateMap;
    for (const auto& staat : j["states"]) {
        State state = State(staat["name"], staat["starting"], staat["accepting"]);
        states.emplace_back(state);
        stateMap[staat["name"]] = &states.back();
    }

    for (const auto& transition : j["transitions"]) {
        string nameFrom = transition["from"];
        string nameTo = transition["to"];
        string input = transition["input"];
        State* from = findName(states, nameFrom);
        if (from != nullptr) {
            State* to = findName(states, nameTo);
            if (to != nullptr) {
                tuple<string, State*> trans = {input, to};
                from->addTransition(trans);
            }
        }
    }
    State* start = nullptr;
    vector<State*> accepts;
    for (State& state : states) {
        if (state.starting) {
            start = &state;
        }
        if (state.accepting) {
            accepts.push_back(&state);
        }
    }
    std::string finalRegex;
    if (start && !accepts.empty()) {
        for (State* accept : accepts) {
            string regex = stateEliminationMethod(states, start, accept);
            if (finalRegex.empty()) {
                finalRegex = regex;
            } else {
                finalRegex = unionRegex(finalRegex, regex);
            }
        }
    }
    RE r = RE(finalRegex);
    return r;
}
