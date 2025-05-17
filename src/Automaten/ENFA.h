#ifndef ENFA_H
#define ENFA_H

#include <map>
#include <vector>
#include <set>
#include <fstream>
#include "State.h"
#include "DFA.h"
class State;
// ENFA_state class declaration
class ENFA_state {
    std::multimap<std::string, ENFA_state *> transitions;
    std::string name;
    bool is_final;
    bool is_start;
    std::vector<ENFA_state *> e_closure;

public:
    ENFA_state();
    ENFA_state(std::string name, bool is_final, bool is_start);

    bool is_final_state();
    bool is_start_state();

    void set_name(std::string name);
    std::string get_name();

    void set_start(bool is_start);
    void set_final(bool is_final);
    void add_transition(std::string input, ENFA_state *state);

    std::multimap<std::string, ENFA_state *> get_alltransitions();
    std::vector<ENFA_state *> get_nextStates(std::string input);

    std::vector<ENFA_state *> get_e_closure();
    void set_e_closure(std::vector<ENFA_state *> e_closure);
};

class DFA_state {
private:
    bool endState = false;
    bool beginState = false;
    std::string name;
    std::map<DFA_state*, std::string> regx_transitie;
    std::map<std::string, DFA_state*> transitie;

    void SortString(std::string &str);

public:
    std::string getName();
    bool getisEndState() const;
    bool getisBeginState() const;
    void setName(std::string named);
    void setEndState(bool isEndState);
    void setBeginState(bool isBeginState);
    void addTransition(std::string inputSymbol, DFA_state *nextState);
    std::map<DFA_state*, std::string> getTransitions();
    DFA_state* volgendeState(std::string input);
};



// ENFA class declaration
class ENFA {
private:
    std::vector<ENFA_state *> states;
    std::vector<std::string> alphabet;
    ENFA_state *start_state;
    std::vector<ENFA_state *> final_states;
    std::string type = "ENFA";
    std::string eps;
    std::string file;

public:
    ENFA();
    explicit ENFA(std::string filename);
    ~ENFA();
    DFA toDFA();

    void add_state(ENFA_state *state);
    void set_alphabet(std::vector<std::string> alphabet);
    std::string get_eps();
    void set_start(ENFA_state *state);
    void add_final_state(ENFA_state *state);
    ENFA_state *get_start_state();
    std::vector<ENFA_state *> get_final_states();
    std::vector<ENFA_state *> get_states();
    void set_eps(std::string eps);


    std::set<ENFA_state*> epsilonClosure( ENFA_state* state);

    DFA_state* make_DFA_state(const std::set<ENFA_state*>& states_set);

    void printStats();

    bool accepts(const std::string& input);
};

#endif // ENFA_H
