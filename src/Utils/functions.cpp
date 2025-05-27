//
// Created by abdir on 18/05/2025.
//
#include "functions.h"

#include <iostream>
#include <set>

#include "Logger.h"


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

bool isControlStructure(const std::vector<std::string> &line) {
    static const std::set<std::string> controlKeywords = {
        "if", "else", "for", "while", "switch", "do",
        "class", "struct", "namespace", "try", "catch" , "#"
    };

    if (line.empty()) return false;
    // The simplest heuristic:
    std::string first = line[0];
    // Check for functions (can start with type and end with parenthesis)
    for (size_t i = 0; i < line.size(); ++i) {
        if (line[i] == "(") {
            // There is a bracket and no semicolon - possibly a function definition
            for (size_t j = i; j < line.size(); ++j) {
                if (line[j] == ")") {
                    if (j + 1 < line.size() && (line[j + 1] == "{" || line[j + 1] == "const")) {
                        return true;
                    }
                }
            }
        }
    }
    // Control structures
    if (controlKeywords.count(first)) return true;
    return false;
}

bool endsWithSemicolonOrBlock(const std::vector<std::string>& line) {
    const std::string& lastToken = line.back();

    // Check for list initialization without semicolon:
    // look for closing curly brace without semicolon after
    if (lastToken == "}") {
        if (line.size() >= 2 && line[line.size() - 2] != ";") {
            return false; // `... }` but without `;`
        }
        return true;
    }

    if (lastToken == ";" || lastToken == "{") {
        return true;
    }

    return false;
}

void checkSemicolons(const std::vector<std::vector<std::string>> &lines) {
    for (size_t lineNum = 0; lineNum < lines.size(); ++lineNum) {
        const auto& line = lines[lineNum];
        // do not check empty lines
        if (line.empty()) continue;
        // do not check structures
        if (isControlStructure(line)) continue;
        const std::string& lastToken = line.back();
        // Check the last element of a string
        if (!endsWithSemicolonOrBlock(line)) {
            Logger::log(LogLevel::WARNING, "Warning: line " + std::to_string(lineNum + 1) +" may be missing a semicolon.");
        }
    }
}

std::string getTrailingDigits(const std::string& input) {
    std::string result;
    // Go from the end of the line to the beginning
    for (int i = static_cast<int>(input.size()) - 1; i >= 0; --i) {
        if (std::isdigit(input[i])) {
            result = input[i] + result;  // Add a digit to the beginning of the result
        } else {
            break;  // Stop if you encounter a non-digit
        }
    }
    return result;
}

