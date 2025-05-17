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

vector<string> getStates(const string input)
{
    vector<string> states;
    string state;
    for(const auto symbol : input)
    {
        if(symbol == '{')
        {

        }
        else if(symbol == '}')
        {
            states.push_back(state);
        }
        else if(symbol == ',')
        {
            states.push_back(state);
            state = "";
        }
        else
        {
            state.push_back(symbol);
        }
    }
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
    vector<string> states;
    for(const auto& statePair: transitions_) {
        states.push_back(statePair.first);
    }
    int size1 = states.size();

    states = sort(states);

    Table* table1 = new Table(states);

    for (int i = 0; i < size1; i++)
    {
        for (int j = i + 1; j < size1; j++)
        {
            string state1 = states[i];
            string state2 = states[j];
            bool isAccepting1 = find(acceptingStates_.begin(), acceptingStates_.end(), state1) != acceptingStates_.end();
            bool isAccepting2 = find(acceptingStates_.begin(), acceptingStates_.end(), state2) != acceptingStates_.end();
            if (isAccepting1 != isAccepting2)
            {
                table1->setDistinguishable(state1,state2);
            }
        }
    }

    bool changed;
    do {
        changed = false;
        vector<pair<string, string>> dis = table1->getDistinguishables();
        for (int i = 0; i < dis.size(); ++i)
        {
            string state1 = dis[i].first;
            string state2 = dis[i].second;
            for(basic_string<char> c : alphabet_)
            {
                string next1 = transitions_.at(state1).at(c);
                string next2 = transitions_.at(state2).at(c);
                if(next1 < next2)
                {
                    if(table1->getDistinguishability(next1,next2) == "X")
                    {
                        changed = true;
                        table1->setDistinguishable(min(state1,state2),max(state1,state2));
                    }
                }
                else if(next1 > next2)
                {
                    if(table1->getDistinguishability(next2,next1) == "X")
                    {
                        changed = true;
                        table1->setDistinguishable(min(state1,state2),max(state1,state2));
                    }
                }
            }
        }
    } while (changed);


    vector<pair<string, string>> dis = table1->getDistinguishables();
    if(!dis.empty())
    {
        vector<vector<string>> pairs{};
        vector<vector<string>> merge{};
        for(int i =0;i<dis.size();i++)
        {
            vector<string> pair;
            pair.emplace_back(dis[i].first);
            pair.emplace_back(dis[i].second);
            pairs.emplace_back(pair);
        }
        bool merged;
        do {
            merged = false;
            for (int i = 0; i < pairs.size() - 1; i++) {
                pairs[i] = sort(pairs[i]);
                for (int j = 0; j < pairs[i].size(); j++) {
                    if (count(pairs[i + 1].begin(), pairs[i + 1].end(), pairs[i][j]) > 0) {
                        for (const auto& state : pairs[i + 1]) {
                            if (find(pairs[i].begin(), pairs[i].end(), state) == pairs[i].end()) {
                                pairs[i].emplace_back(state);
                                merged = true;
                            }
                        }
                    }
                    if (merged)
                        break;
                }
                if (merged)
                    break;
            }

            for (int i = 0; i < pairs.size() - 1; i++) {
                pairs[i] = sort(pairs[i]);
                pairs[i + 1] = sort(pairs[i + 1]);

                if (pairs[i] == pairs[i + 1]) {
                    pairs.erase(pairs.begin() + i + 1);
                    merged = true;
                } else {
                    bool subset = true;
                    if (pairs[i].size() < pairs[i + 1].size()) {
                        for (const auto& state : pairs[i]) {
                            if (find(pairs[i + 1].begin(), pairs[i + 1].end(), state) == pairs[i + 1].end()) {
                                subset = false;
                                break;
                            }
                        }
                        if (subset) {
                            pairs.erase(pairs.begin() + i);
                            merged = true;
                        }
                    } else if (pairs[i].size() > pairs[i + 1].size()) {
                        for (const auto& state : pairs[i + 1]) {
                            if (find(pairs[i].begin(), pairs[i].end(), state) == pairs[i].end()) {
                                subset = false;
                                break;
                            }
                        }
                        if (subset) {
                            pairs.erase(pairs.begin() + i + 1);
                            merged = true;
                        }
                    }
                }
            }
        } while (merged);


        vector<string> mergedStates;
        for(int i=0;i<pairs.size();i++)
        {
            string newState = "{";
            for(int j=0;j<pairs[i].size()-1;j++)
            {
                newState += pairs[i][j] + ",";
            }
            newState += pairs[i][pairs[i].size()-1] + "}";
            mergedStates.emplace_back(newState);
        }

        vector<string> newStates;

        for(const auto & mergedState : mergedStates)
        {
            newStates.emplace_back(mergedState);
            vector<string> state = getStates(mergedState);
            for(auto & j : states)
            {
                if(std::count(state.begin(), state.end(),j) == 0)
                    newStates.emplace_back(j);
            }
        }

        vector<string> newAlphabet = alphabet_;
        string newInitialState = initialState_;
        vector<string> newAcceptingStates;
        map<string, map<string, string>> newTransitions;

        // Determine the new initial state
        for (const auto& mergedState : mergedStates) {
            vector<string> state = getStates(mergedState);
            if (std::count(state.begin(), state.end(), initialState_) > 0) {
                newInitialState = mergedState;
                break;
            }
        }

        // Determine the new accepting states
        for (const auto& mergedState : mergedStates) {
            vector<string> state = getStates(mergedState);
            for (const auto& acceptingState : acceptingStates_) {
                if (std::count(state.begin(), state.end(), acceptingState) > 0) {
                    newAcceptingStates.push_back(mergedState);
                    break;
                }
            }
        }

        // Determine the new transitions
        for (const auto& mergedState : mergedStates) {
            vector<string> state = getStates(mergedState);
            for (const auto& symbol : alphabet_) {
                vector<string> nextStates;
                for (const auto& s : state) {
                    nextStates.push_back(transitions_.at(s).at(symbol));
                }
                sort(nextStates.begin(), nextStates.end());
                string newNextState = "{";
                for (int i = 0; i < nextStates.size() - 1; i++) {
                    newNextState += nextStates[i] + ",";
                }
                newNextState += nextStates[nextStates.size() - 1] + "}";
                newTransitions[mergedState][symbol] = newNextState;
            }
        }

        table = table1;

        return DFA(newAlphabet, newInitialState, newAcceptingStates, newTransitions);
    }
    else
    {
        return DFA(alphabet_,initialState_,acceptingStates_,transitions_);
    }
}

bool DFA::operator==( DFA& other)  {
    // Minimize both DFAs
    DFA minDFA1 = this->minimize();
    DFA minDFA2 = other.minimize();

    // Compare alphabets
    if (minDFA1.getAlphabet() != minDFA2.getAlphabet()) {
        return false;
    }

    // Compare initial states
    if (minDFA1.getInitialState() != minDFA2.getInitialState()) {
        return false;
    }

    // Compare accepting states
    vector<string> acceptingStates1 = minDFA1.getAcceptingStates();
    vector<string> acceptingStates2 = minDFA2.getAcceptingStates();
    sort(acceptingStates1.begin(), acceptingStates1.end());
    sort(acceptingStates2.begin(), acceptingStates2.end());
    if (acceptingStates1 != acceptingStates2) {
        return false;
    }

    // Compare transitions
    auto transitions1 = minDFA1.getTransitions();
    auto transitions2 = minDFA2.getTransitions();
    if (transitions1.size() != transitions2.size()) {
        return false;
    }

    for (const auto& state1 : transitions1) {
        const string& stateName1 = state1.first;
        if (transitions2.find(stateName1) == transitions2.end()) {
            return false;
        }

        const auto& stateTransitions1 = state1.second;
        const auto& stateTransitions2 = transitions2.at(stateName1);
        if (stateTransitions1 != stateTransitions2) {
            return false;
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
