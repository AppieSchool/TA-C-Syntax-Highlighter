#include "RE.h"
#include "ENFA.h"

RE::RE(std::string re, char epsilon) {
    this->re = re;
    this->epsilon = epsilon;
}

RE::RE(std::string re) {
    this->re = re;
}

std::string RE::get_re() {
    return re;
}

ENFA RE::createBasicENFA(char c) {
    ENFA BasicENFA;
    std::shared_ptr<ENFA_state> start = make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), false, true));
    std::shared_ptr<ENFA_state> end =  make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), true, false));
    start->add_transition(std::string(1, c), end);
    BasicENFA.add_state(start);
    BasicENFA.add_state(end);
    BasicENFA.set_start(start);
    BasicENFA.add_final_state(end);
    return BasicENFA;
}

ENFA RE::applyKleeneStar(ENFA OldENFA, char epsilon) {
    ENFA KleeneStarENFA;
    std::shared_ptr<ENFA_state> start = make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), false, true));
    std::shared_ptr<ENFA_state> end = make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), true, false));

    start->add_transition(std::string(1, epsilon), OldENFA.get_start_state());
    OldENFA.get_start_state()->set_start(false);
    for (auto state : OldENFA.get_final_states()) {
        state->add_transition(std::string(1, epsilon), end);
        state->add_transition(std::string(1, epsilon), OldENFA.get_start_state());
        state->set_final(false);
        state->set_start(false);
    }

    start->add_transition(std::string(1, epsilon), end);
    KleeneStarENFA.set_start(start);
    KleeneStarENFA.add_final_state(end);
    KleeneStarENFA.add_state(start);
    KleeneStarENFA.add_state(end);

    for (auto state : OldENFA.get_states()) {
        KleeneStarENFA.add_state(state);
    }

    return KleeneStarENFA;
}

ENFA RE::createUnionENFA(ENFA Links, ENFA Rechts, char epsilon) {
    ENFA UnionENFA;
    std::shared_ptr<ENFA_state> start = make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), false, true));
    std::shared_ptr<ENFA_state> end = make_shared<ENFA_state>(ENFA_state("q" + std::to_string(stateCount++), true, false));

    start->add_transition(std::string(1, epsilon), Links.get_start_state());
    start->add_transition(std::string(1, epsilon), Rechts.get_start_state());

    for (auto state : Links.get_final_states()) {
        state->add_transition(std::string(1, epsilon), end);
        state->set_final(false);
        state->set_start(false);
    }

    for (auto state : Rechts.get_final_states()) {
        state->add_transition(std::string(1, epsilon), end);
        state->set_final(false);
        state->set_start(false);
    }

    UnionENFA.set_start(start);
    UnionENFA.add_final_state(end);
    UnionENFA.add_state(start);
    UnionENFA.add_state(end);

    for (auto state : Links.get_states()) {
        UnionENFA.add_state(state);
    }

    for (auto state : Rechts.get_states()) {
        UnionENFA.add_state(state);
    }

    return UnionENFA;
}

ENFA RE::createConcatenationENFA(ENFA Links, ENFA Rechts, char epsilon) {
    ENFA ConcatenationENFA;
    ConcatenationENFA.set_start(Links.get_start_state());

    for (auto state : Links.get_final_states()) {
        state->set_final(false);
        state->add_transition(std::string(1, epsilon), Rechts.get_start_state());
    }

    for (auto state : Rechts.get_final_states()) {
        ConcatenationENFA.add_final_state(state);
    }

    for (auto state : Links.get_states()) {
        ConcatenationENFA.add_state(state);
    }

    for (auto state : Rechts.get_states()) {
        state->set_start(false);
        ConcatenationENFA.add_state(state);
    }

    return ConcatenationENFA;
}

bool RE::isLiteral(char c) {
    return std::isalnum(c) || c == '?' || c == ',' || c == '!' || c == ':' || c == '-' || c == '_';
}

bool RE::isOperator(char c) {
    return c == '+' || c == '*' || c == '.';
}

int RE::precedence(char op) {
    if (op == '*') return 3;
    if (op == '.') return 2;
    if (op == '+') return 1;
    return 0;
}

void RE::processOperator(std::stack<ENFA>& nfaStack, std::stack<char>& opStack, char epsilon) {
    char op = opStack.top();
    opStack.pop();

    if (op == '*') {
        ENFA nfa = nfaStack.top(); nfaStack.pop();
        ENFA kleeneStar = applyKleeneStar(nfa, epsilon);
        nfaStack.push(kleeneStar);
    } else if (op == '+') {
        ENFA right = nfaStack.top(); nfaStack.pop();
        ENFA left = nfaStack.top(); nfaStack.pop();
        nfaStack.push(createUnionENFA(left, right, epsilon));
    } else if (op == '.') {
        ENFA right = nfaStack.top(); nfaStack.pop();
        ENFA left = nfaStack.top(); nfaStack.pop();
        nfaStack.push(createConcatenationENFA(left, right, epsilon));
    }
}

ENFA RE::toENFA() {
    std::stack<ENFA> nfaStack;
    std::stack<char> opStack;
    std::vector<std::string> alphabet;

    for (int i = 0; i < get_re().length(); i++) {
        char c = get_re()[i];

        if (isLiteral(c)) {
            bool found = false;
            for (const auto& alpha : alphabet) {
                if (alpha == std::string(1, c)) {
                    found = true;
                    break;
                }
            }
            if (!found && c != epsilon) {
                alphabet.push_back(std::string(1, c));
            }

            nfaStack.push(createBasicENFA(c));

            if (i + 1 < re.length() && (isLiteral(re[i + 1]) || re[i + 1] == '(')) {
                while (!opStack.empty() && precedence(opStack.top()) >= precedence('.')) {
                    processOperator(nfaStack, opStack, epsilon);
                }
                opStack.push('.');
            }
        } else if (c == '(') {
            opStack.push(c);
        } else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                processOperator(nfaStack, opStack, epsilon);
            }
            opStack.pop();
            if (i + 1 < re.length() && (isLiteral(re[i + 1]) || re[i + 1] == '(')) {
                while (!opStack.empty() && precedence(opStack.top()) >= precedence('.')) {
                    processOperator(nfaStack, opStack, epsilon);
                }
                opStack.push('.');
            }
        } else if (isOperator(c)) {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c)) {
                processOperator(nfaStack, opStack, epsilon);
            }
            opStack.push(c);

            if (c == '*' && i + 1 < re.length() && (isLiteral(re[i + 1]) || re[i + 1] == '(')) {
                while (!opStack.empty() && precedence(opStack.top()) >= precedence('.')) {
                    processOperator(nfaStack, opStack, epsilon);
                }
                opStack.push('.');
            }
        }
    }

    while (!opStack.empty()) {
        processOperator(nfaStack, opStack, epsilon);
    }
    nfaStack.top().set_eps(std::string(1, epsilon));
    nfaStack.top().set_alphabet(alphabet);
    return nfaStack.top();
}