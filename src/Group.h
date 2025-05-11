#ifndef TOGTA_GROUP_H
#define TOGTA_GROUP_H

#include <string>

using namespace std;

class Group {
public:
    string name;
    string RE;
    string color;
    int fontWeight;

    Group(const string& name, const string& RE, const string& color, int fontWeight);

    const string &getName() const;
    void setName(const string &name);

    const string &getRegex() const;
    void setRegex(const string &re);

    const string &getColor() const;
    void setColor(const string &color);

    int getFontWeight() const;
    void setFontWeight(int fontWeight);

};


#endif //TOGTA_GROUP_H
