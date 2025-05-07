#ifndef RETOENFA_RE_H
#define RETOENFA_RE_H

#include <string>
#include <stack>
#include <vector>
#include "ENFA.h"

class RE {
    std::string re;
    char epsilon;
    int stateCount = 0;

public:
    RE(std::string re, char epsilon);
    RE(std::string re);

    std::string get_re();

    ENFA createBasicENFA(char c);
    ENFA applyKleeneStar(ENFA OldENFA, char epsilon);
    ENFA createUnionENFA(ENFA Links, ENFA Rechts, char epsilon);
    ENFA createConcatenationENFA(ENFA Links, ENFA Rechts, char epsilon);

    bool isLiteral(char c);
    bool isOperator(char c);
    int precedence(char op);

    void processOperator(std::stack<ENFA>& nfaStack, std::stack<char>& opStack, char epsilon);
    ENFA toENFA();
};

#endif