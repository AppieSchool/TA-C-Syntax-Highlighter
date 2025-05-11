//
// Created by ruben on 22/06/24.
//

#ifndef DFATORE_STATE_H
#define DFATORE_STATE_H
#include <vector>
#include <string>
#include <tuple>
#include <map>
#include <regex>
using namespace std;

class State {
public:
    std::string name;
    std::vector<std::tuple<std::string, State*>> transitions;
    bool starting;
    bool accepting;

    State(const std::string &name, bool begin, bool end);
    State();
    void addTransition(std::tuple<std::string , State*> trans);
    void toRegex(map<string, string>& regexTransitions);
    void printTransition();

    const vector<std::tuple<std::string, State *>> &getTransitions() const;

    bool isBegin() const;

    bool isAnEnd() const;
};


#endif //DFATORE_STATE_H
