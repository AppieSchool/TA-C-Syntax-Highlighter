#ifndef ENGINE_PROCESS_H
#define ENGINE_PROCESS_H
#include <vector>
#include <string>

#include "ENFA.h"
#include "Utils/Logger.h"
#include "Group.h"
#include "Utils/functions.h"

class Process {
private:
    vector<Group*> groups;
    string regex;
public:
    Process() = default;

    Process(vector<Group *> groups, const string &inputFileName);

    const vector<Group*> &getGroups() const;

    void setGroups(const vector<Group *> &groups);

    const string &getRegex() const;

    void setRegex(const string &regex);
};


#endif //ENGINE_PROCESS_H
