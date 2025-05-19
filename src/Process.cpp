#include "Process.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

#include "htmlConverter.h"
#include "RE.h"

using namespace std;

Process::Process(vector<Group *> groups, string inputFileName) : groups(std::move(groups)) {
    Logger::log(LogLevel::INFO, "Starting Process constructor...");

    ifstream inputFile(inputFileName);
    if (!inputFile) {
        Logger::log(LogLevel::ERROR, "Unable to open input file: " + inputFileName);
        return;
    } else {
        Logger::log(LogLevel::INFO, "Successfully opened input file: " + inputFileName);
    }

    // TODO: give in the groups so that the Sidebar is correct in the HTML
    HTMLWriter output("output.html");
    Logger::log(LogLevel::INFO, "Initialized HTMLWriter for output.html");

    vector<DFA> dfas;
    Logger::log(LogLevel::INFO, "Processing groups...");
    for (auto group : getGroups()) {
        Logger::log(LogLevel::INFO, "Processing group: " + group->getName());

        if (group->getRegex().empty()) {
            Logger::log(LogLevel::WARNING, "No regex for group: " + group->getName());
            continue;
        }

        Logger::log(LogLevel::INFO, "Converting regex to minimalized DFA for group: " + group->getName());
        RE GroupRegex(group->getRegex());
        ENFA GroupNFA = GroupRegex.toENFA();
        DFA GroupDFA = GroupNFA.toDFA();
        GroupDFA.minimize();
        dfas.push_back(GroupDFA);
    }

    Logger::log(LogLevel::INFO, "Reading input file and tokenizing words...");
    std::string line;
    std::vector<std::vector<std::string>> lines;

    while (getline(inputFile, line)) {
        Logger::log(LogLevel::DEBUG, "Read line: " + line);

        std::vector<std::string> words = tokenizeLine(line);  // Tokenize current line
        lines.push_back(words);  // Always push a vector (even if empty)
    }

    inputFile.close();
    Logger::log(LogLevel::INFO, "Finished reading input file.");

    // Check for balanced brackets
    auto [balanced, lineNumber] = bracket_line(lines);
    if (!balanced) {
        Logger::log(LogLevel::INFO, "Unbalanced brackets found in line: " + to_string(lineNumber));
        output.addError("Unbalanced brackets found in line: " + to_string(lineNumber));
    } else {
        Logger::log(LogLevel::INFO, "All brackets are balanced.");
    }

    // TODO: check for semicolons

    Logger::log(LogLevel::INFO, "Processing words...");
    for (const auto& line : lines) {
        for (const auto& word : line) {
            Logger::log(LogLevel::DEBUG, "Checking word: " + word);
            bool isAccepted = false;
            Group* g = nullptr;

            for (size_t index = 0; index < dfas.size(); index++) {
                g = getGroups()[index];

                Logger::log(LogLevel::DEBUG, "Checking if DFA accepts word: " + word + " for group: " + g->getName());
                if (dfas[index].accepts(word)) {
                    Logger::log(LogLevel::INFO, "Word: '" + word + "' accepted by group: " + g->getName());
                    isAccepted = true;
                    break;
                }else if(g->getName() == "groep2") {
                    Logger::log(LogLevel::DEBUG, "Word: '" + word + "' not accepted by group: " + g->getName());
                    auto [suggestion, close] = dfas[index].isCloseToAcceptingState(word, 3);
                    if (close) {
                        output.addError("Wrong word: " + word + " Did you mean: "  + suggestion + "?");
                    }
                }
            }

            if (g) {
                if (isAccepted) {
                    Logger::log(LogLevel::SUCCESS, "Writing styled text: " + word);
                    output.writeStyledText(word, g->getColor(), g->getFontWeight());
                } else {
                    Logger::log(LogLevel::INFO, "Writing plain text: " + word);
                    output.writePlainText(word);
                }
            } else {
                Logger::log(LogLevel::WARNING, "No valid group found for word: " + word);
            }
        }
        output.addNewline();
    }

    Logger::log(LogLevel::INFO, "Saving HTML file...");
    output.saveFile();
    Logger::log(LogLevel::SUCCESS, "HTML file 'output.html' saved successfully!");
}

const vector<Group *> &Process::getGroups() const {
    return groups;
}

void Process::setGroups(const vector<Group *> &groups) {
    this->groups = groups;
}

const string &Process::getRegex() const {
    return regex;
}

void Process::setRegex(const string &regex) {
    Process::regex = regex;
}
