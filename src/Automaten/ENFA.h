#ifndef ENFA_H
#define ENFA_H

#include <map>
#include <vector>
#include <set>
#include <fstream>
#include "State.h"
class State;
class DFA;
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

    //
    /*
    State* make_DFA_state(const std::set<ENFA_state*>& states);
    */

    void printStats();

    bool accepts(const std::string& input);
};

#endif // ENFA_H
