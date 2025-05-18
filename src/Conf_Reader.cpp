//
// Created by Kaede on 2025-05-11.
//

#include "Conf_Reader.h"

#include <fstream>
#include <stack>

pair<std::vector<Group *>, int> Conf_Reader::read_conf(const ini::Configuration &conf) {
    // Declaring output data
    std::vector<Group *> groups;
    int retVal = 0;
    // Trying to find the main description of the file
    // To get the number of groups
    const int n_groups = conf["General"]["NumberOfGroups"].as_int_or_die();
    // note: we can add a lot of other things here, from the background color to the style of the document text

    // For each group number try to read it
    for(int i = 0; i < n_groups; i++){
        string group_name = "groep" + to_string(i);
        string RE = conf[group_name]["RE"].as_string_or_die();
        string color = conf[group_name]["color"].as_string_or_die();
        const int font_weight = conf[group_name]["fontWeight"].as_int_or_die();
        if (!are_brackets_balanced(RE)) {
            cerr << "Error: unbalanced brackets in RE for group: " << group_name << endl;
            retVal = 1;
            continue;
        }
        auto* group = new Group(group_name, RE, color, font_weight);
        groups.push_back(group);
    }
    return {groups, retVal};
}


void Conf_Reader::merge_conf(const std::string &first_file, const std::string &second_file,
    const string &new_file_name) {
    // trying to read the first configuration
    ini::Configuration first_conf;
    try
    {
        ifstream fin(first_file);
        if (fin.peek() == istream::traits_type::eof()) {
            cout << "Ini file appears empty. Does '" << first_file << "' exist?" << endl;
            return;
        }
        fin >> first_conf;
        fin.close();
    }
    catch(ini::ParseException& ex)
    {
        cerr << "Error parsing file: " << first_file << ": " << ex.what() << endl;
        return;
    }

    // trying to read the second configuration
    ini::Configuration second_conf;
    try
    {
        ifstream fin(second_file);
        if (fin.peek() == istream::traits_type::eof()) {
            cout << "Ini file appears empty. Does '" << second_file << "' exist?" << endl;
            return;
        }
        fin >> second_conf;
        fin.close();
    }
    catch(ini::ParseException& ex)
    {
        cerr << "Error parsing file: " << second_file << ": " << ex.what() << endl;
        return;
    }

    // Passing data to a deeper function
    merge_conf(first_conf, second_conf, new_file_name);
}

void Conf_Reader::merge_conf(const ini::Configuration &first_conf, const ini::Configuration &second_conf,
                             const string &new_file_name) {
    const std::vector<Group *> first_groups = read_conf(first_conf).first;
    const std::vector<Group *> second_groups = read_conf(second_conf).first;

    // Passing data to a deeper function
    merge_conf(first_groups, second_groups, new_file_name);

    // delete the created groups to avoid leaks
    for (const auto group : first_groups) {
        delete group;
    }
    for (const auto group : second_groups) {
        delete group;
    }
}

void Conf_Reader::merge_conf(const std::vector<Group *> &first_groups, const std::vector<Group *> &second_groups,
                             const string &new_file_name) {
    int counter = 0;
    string data;

    std::vector<Group *> groups_to_save = first_groups;
    groups_to_save.insert(groups_to_save.end(), second_groups.begin(), second_groups.end());

    // go through each group and write the data into a line
    for (auto group : groups_to_save) {
        data += "[groep" + to_string(counter) + "]\n";
        data += "RE = "+group->getRegex()+"\n";
        data += "color = "+group->getColor()+"\n";
        data += "fontWeight = "+std::to_string(group->getFontWeight())+"\n";
        data += "\n";
        counter++;
    }

    // Create a master configuration data group
    data += "[General]\n";
    data += "n_groups= " +std::to_string(counter) + "\n";

    // write information to a file
    write_to_ini(data, new_file_name);
}

void Conf_Reader::save_conf(const std::vector<Group *> &groups, const std::string &new_file_name) {
    merge_conf(groups, {}, new_file_name);
}

bool Conf_Reader::write_to_ini(const std::string& data, const std::string& filename) {
    // Check for .ini extension
    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".ini") {
        std::cerr << "Error: The file must have the extension .ini\n";
        return false;
    }

    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file:" << filename << '\n';
        return false;
    }
    file << data;
    file.close();
    return true;
}
