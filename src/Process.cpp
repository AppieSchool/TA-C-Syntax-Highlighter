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
    cout << "[INFO] Starting Process constructor..." << endl;

    ifstream inputFile(inputFileName);
    if (!inputFile) {
        cerr << "[ERROR] Unable to open input file: " << inputFileName << endl;
        return;
    } else {
        cout << "[INFO] Successfully opened input file: " << inputFileName << endl;
    }

    HTMLWriter output("output.html");
    cout << "[INFO] Initialized HTMLWriter for output.html" << endl;

    vector<ENFA> enfas;
    cout << "[INFO] Processing groups..." << endl;
    for (auto group : getGroups()) {
        cout << "[INFO] Processing group: " << group->getName() << endl;

        if (group->getRegex().empty()) {
            cerr << "[WARNING] No regex for group " << group->getName() << endl;
            continue;
        }

        cout << "[INFO] Converting regex to ENFA for group: " << group->getName() << endl;
        RE GroupRegex(group->getRegex());
        ENFA GroupNFA = GroupRegex.toENFA();
        enfas.push_back(GroupNFA);
    }

    cout << "[INFO] Reading input file and tokenizing words..." << endl;
    string line;
    vector<vector<string>> lines;
    while (getline(inputFile, line)) {
        cout << "[DEBUG] Read line: " << line << endl;

        vector<string> words;
        istringstream stream(line);
        string word;
        while (stream >> word) {
            cout << "[DEBUG] Tokenized word: " << word << endl;
            words.push_back(word);
        }
        lines.push_back(words);
    }
    inputFile.close();
    cout << "[INFO] Finished reading input file." << endl;

    cout << "[INFO] Processing words..." << endl;
    for (const auto& line : lines) {
        for (const auto& word : line) {
            cout << "[DEBUG] Checking word: " << word << endl;
            bool isAccepted = false;
            Group* g = nullptr;

            for (size_t index = 0; index < enfas.size(); index++) {
                g = getGroups()[index];

                cout << "[DEBUG] Checking if ENFA accepts word: " << word << " for group " << g->getName() << endl;
                if (enfas[index].accepts(word)) {
                    cout << "[INFO] Word '" << word << "' accepted by group: " << g->getName() << endl;
                    isAccepted = true;
                    break;
                }
            }

            if (g) {
                if (isAccepted) {
                    cout << "[SUCCESS] Writing styled text: " << word << endl;
                    output.writeStyledText(word, g->getColor(), g->getFontWeight());
                } else {
                    cout << "[INFO] Writing plain text: " << word << endl;
                    output.writePlainText(word);
                }
            } else {
                cerr << "[ERROR] No valid group found for word: " << word << endl;
            }
        }
        output.addNewline();
    }

    cout << "[INFO] Saving HTML file..." << endl;
    output.saveFile();
    cout << "[SUCCESS] HTML file 'output.html' saved successfully!" << endl;
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
