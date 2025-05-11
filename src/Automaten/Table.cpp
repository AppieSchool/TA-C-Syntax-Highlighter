//
// Created by Jappe on 22/06/2024.
//

#include <iostream>
#include "Table.h"

Table::Table(std::vector<std::string> states) {
    std::vector<std::vector<std::string>> table{};
    std::vector<std::string> bottom;
    for(int i = 0;i<states.size()-1;i++)
    {
        bottom.emplace_back(states[i]);
        std::vector<std::string> row{};
        row.emplace_back(states[i+1]);
        for(int j = 0;j<=i;j++)
        {
            row.emplace_back("-");
        }
        table.emplace_back(row);
    }

    table.emplace_back(bottom);
    this->table = table;
}

Table::~Table() {

}



void Table::print() {
    for(int i=0;i<this->table.size()-1;i++)
    {
        for(int j=0;j< this->table[i].size();j++)
        {
            std::cout << table[i][j] << '\t';
        }
        std::cout << std::endl;
    }
    std::cout << '\t';
    for(int j=0;j< this->table[table.size()-1].size();j++)
    {
        std::cout << table[table.size()-1][j] << '\t';
    }
    std::cout << std::endl;
}

std::string Table::getDistinguishability(std::string state1, std::string state2) const {
    int i1=-1;
    int i2=-1;
    for(int i = 0; i < table[table.size()-1].size(); ++i) {
        if (table[table.size()-1][i] == state1) {
            i1 = i;
        }
    }

    for(int j = 0; j < table.size()-1; ++j) {
        if (table[j][0] == state2) {
            i2 = j;
        }
    }
    return table[i2][i1+1];
}

void Table::setDistinguishable(std::string state1, std::string state2) {
    int i1=-1;
    int i2=-1;
    for(int i = 0; i < table[table.size()-1].size(); ++i) {
        if (table[table.size()-1][i] == state1) {
            i1 = i;
        }
    }

        for(int j = 0; j < table.size()-1; ++j) {
            if (table[j][0] == state2) {
                i2 = j;
            }
        }
    table[i2][i1+1] = "X";
}

std::vector<std::pair<std::string, std::string>> Table::getDistinguishables() {
    std::vector<std::pair<std::string, std::string>> pairs;
    for(int i=0;i<table.size()-1;i++)
    {
        for(int j=1;j<table[i].size();j++)
        {
            if(table[i][j] == "-")
            {
                std::pair<std::string, std::string> pair;
                pair.first = table[i][0];
                pair.second = table[table.size()-1][j-1];
                pairs.emplace_back(pair);
            }
        }
    }
    return pairs;
}
