#include "ini_configuration.h"
#include "Process.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <stack>

using namespace std;

bool areBracketsBalanced(const string& str) {
    stack<char> s;
    for (char ch : str) {
        if (ch == '(' || ch == '{' || ch == '[') {
            s.push(ch);
        } else if (ch == ')' || ch == '}' || ch == ']') {
            if (s.empty()) {
                return false;
            }
            char top = s.top();
            s.pop();
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '[')) {
                return false;
            }
        }
    }
    return s.empty();
}
int main(int argc, char const* argv[])
{
    int retVal = 0;
    try
    {
        vector<string> args = vector<string>(argv+1, argv+argc);
        if (args.empty()) {
            ifstream fileIn("filelist.txt");
            if (!fileIn.is_open()) {
                cerr << "Failed to open 'filelist.txt'" << endl;
                return 1;
            }
            string filelistName;
            while (getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
            fileIn.close();
            if (args.empty()) {
                cerr << "No files found in 'filelist.txt'" << endl;
                return 1;
            }
        }
        for(const string& fileName : args)
        {
            ini::Configuration conf;
            try
            {
                ifstream fin(fileName);
                if (fin.peek() == istream::traits_type::eof()) {
                    cout << "Ini file appears empty. Does '" << fileName << "' exist?" << endl;
                    continue;
                }
                fin >> conf;
                fin.close();
            }
            catch(ini::ParseException& ex)
            {
                cerr << "Error parsing file: " << fileName << ": " << ex.what() << endl;
                retVal = 1;
                continue;
            }

            vector<Group*> groups = {};
            for(int i = 0; i < 4; i++){
                string groupName = "groep" + to_string(i);
                string RE = conf[groupName]["RE"].as_string_or_die();
                string color = conf[groupName]["color"].as_string_or_die();
                int fontWeight = conf[groupName]["fontWeight"].as_int_or_die();
                if (!areBracketsBalanced(RE)) {
                    cerr << "Error: unbalanced brackets in RE for group: " << groupName << endl;
                    retVal = 1;
                    continue;
                }

                Group* group = new Group(groupName, RE, color, fontWeight);
                groups.push_back(group);
            }
            Process process(groups, "input.txt");
        }
    }
    catch(const std::bad_alloc &exception)
    {
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}