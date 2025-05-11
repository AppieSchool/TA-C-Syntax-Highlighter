//
// Created by Jappe on 22/06/2024.
//

#ifndef TFA_TABLE_H
#define TFA_TABLE_H


#include <vector>

class Table {
    std::vector<std::vector<std::string>> table;
public:
    Table(std::vector<std::string> states);

    virtual ~Table();

    void print();

    std::string getDistinguishability(std::string state1, std::string state2) const;

    void setDistinguishable(std::string state1, std::string state2);

    std::vector<std::pair<std::string,std::string>> getDistinguishables();
};


#endif //TFA_TABLE_H
