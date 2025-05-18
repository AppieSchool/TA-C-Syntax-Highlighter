#include "src/ini_configuration.h"
#include "src/Process.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>

#include "src/Conf_Reader.h"

using namespace std;

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
            // read the configuration and create groups for processing
            pair<std::vector<Group *>, int> conf_pair = Conf_Reader::read_conf(conf);
            // break the answer into parts
            vector<Group*> groups = conf_pair.first;
            retVal = conf_pair.second;

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