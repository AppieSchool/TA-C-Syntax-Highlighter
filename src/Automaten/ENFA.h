#ifndef ENFA_H
#define ENFA_H

#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <memory>
#include <string>
#include "State.h"
#include "DFA.h"

class ENFA_state {
    std::multimap<std::string, std::shared_ptr<ENFA_state>> transitions;
    std::string name;
    bool is_final;
    bool is_start;
    std::vector<std::shared_ptr<ENFA_state>> e_closure;

public:
    ENFA_state();
    ENFA_state(std::string name, bool is_final, bool is_start);

    bool is_final_state();
    bool is_start_state();

    void set_name(std::string name);
    std::string get_name();

    void set_start(bool is_start);
    void set_final(bool is_final);
    void add_transition(std::string input, std::shared_ptr<ENFA_state> state);

    std::multimap<std::string, std::shared_ptr<ENFA_state>> get_alltransitions();
    std::vector<std::shared_ptr<ENFA_state>> get_nextStates(std::string input);

    std::vector<std::shared_ptr<ENFA_state>> get_e_closure();
    void set_e_closure(std::vector<std::shared_ptr<ENFA_state>> e_closure);
};

class DFA_state {
private:
    bool endState = false;
    bool beginState = false;
    std::string name;
    std::map<std::shared_ptr<DFA_state>, std::string> regx_transitie;
    std::map<std::string, std::shared_ptr<DFA_state>> transitie;
    void SortString(std::string &str);

public:
    std::string getName();
    bool getisEndState() const;
    bool getisBeginState() const;
    void setName(std::string named);
    void setEndState(bool isEndState);
    void setBeginState(bool isBeginState);
    void addTransition(std::string inputSymbol, std::shared_ptr<DFA_state> nextState);
    std::map<std::string, std::shared_ptr<DFA_state>> getSymbolTransitions() const;
    std::map<std::shared_ptr<DFA_state>, std::string> getTransitions();
    std::shared_ptr<DFA_state> volgendeState(std::string input);
};



class ENFA {
private:
    std::vector<std::shared_ptr<ENFA_state>> states;
    std::vector<std::shared_ptr<ENFA_state>> final_states;
    std::shared_ptr<ENFA_state> start_state;
    std::vector<std::string> alphabet;
    std::string type = "ENFA";
    std::string eps;
    std::string file;

public:
    ENFA();
    explicit ENFA(std::string filename);
    DFA toDFA();

    void add_state(std::shared_ptr<ENFA_state> state);
    void set_alphabet(std::vector<std::string> alphabet);
    std::string get_eps();
    void set_start(std::shared_ptr<ENFA_state> state);
    void add_final_state(std::shared_ptr<ENFA_state> state);
    std::shared_ptr<ENFA_state> get_start_state();
    std::vector<std::shared_ptr<ENFA_state>> get_final_states();
    std::vector<std::shared_ptr<ENFA_state>> get_states();
    void set_eps(std::string eps);

    std::set<std::shared_ptr<ENFA_state>> epsilonClosure( std::shared_ptr<ENFA_state> state);

    std::shared_ptr<DFA_state> make_DFA_state(const std::set<std::shared_ptr<ENFA_state>>& states_set);

    void printStats();

    bool accepts(const std::string& input);
};

#endif // ENFA_H
