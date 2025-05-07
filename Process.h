#ifndef ENGINE_PROCESS_H
#define ENGINE_PROCESS_H
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stack>
#include "Automaten/RE.cpp"
#include "Automaten/ENFA.h"
#include "htmlConverter.h"
#include "Group.h"
#include "htmlConverter.h"

using namespace std;

class Process {
private:
    vector<Group*> groups;
    string regex;
public:
    Process() = default;
    Process(vector<Group*> groups, string inputFileName);

    const vector<Group*> &getGroups() const;

    void setGroups(const vector<Group *> &groups);

    const string &getRegex() const;

    void setRegex(const string &regex);
};


#endif //ENGINE_PROCESS_H
