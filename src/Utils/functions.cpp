//
// Created by abdir on 18/05/2025.
//
#include "functions.h"


bool are_brackets_balanced(const std::string& str) {
    // Creates a stack in which we will store all found brackets
    std::stack<char> brackets_stack;
    for (char character : str) {
        // If the bracket is opening, then add it to the stack.
        if (character == '(' || character == '{' || character == '[') {
            brackets_stack.push(character);
            // If the bracket is closing then
        } else if (character == ')' || character == '}' || character == ']') {
            // Check if the stack is empty
            if (brackets_stack.empty()) {
                return false;
            }
            // Get the last bracket
            const char top = brackets_stack.top();
            brackets_stack.pop();
            // If the closing bracket is the same type as the resulting last bracket,
            // then everything is ok.
            if ((character == ')' && top != '(') ||
                (character == '}' && top != '{') ||
                (character == ']' && top != '[')) {
                return false;
            }
        }
    }
    return brackets_stack.empty();
}

std::tuple<bool, int> bracket_line(const std::vector<std::vector<std::string>>& lines) {
    std::stack<std::pair<char, int>> brackets_stack;

    for (int i = 0; i < lines.size(); ++i) {
        const auto& words = lines[i];
        for (const auto& word : words) {
            for (char character : word) {
                if (character == '{' || character == '(') {
                    brackets_stack.push({character, i + 1});  // Store char + line number
                } else if (character == '}' || character == ')') {
                    if (brackets_stack.empty()) {
                        return {false, i + 1};  // Too many closing brackets
                    }

                    auto [open, line_opened] = brackets_stack.top();
                    brackets_stack.pop();

                    if ((character == '}' && open != '{') || (character == ')' && open != '(')) {
                        return {false, i + 1};  // Mismatched bracket
                    }
                }
            }
        }
    }

    // If any unmatched opening brackets remain
    if (!brackets_stack.empty()) {
        return {false, brackets_stack.top().second};  // Report line where unclosed bracket was opened
    }

    return {true, 0};  // Everything matched
}



std::vector<std::string> tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;

    for (char c : line) {
        if (std::isalnum(c) || c == '_' || c == ':') {
            token += c;  // build identifier or keyword
        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            if (!std::isspace(c)) {
                tokens.push_back(std::string(1, c));  // treat punctuation as separate token
            }
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

