#include "Group.h"

#include "functions.h"

Group::Group(const string& name, const string& display_name, const string& RE, const string& color, const int fontWeight):
                name(name), display_name(display_name), RE(RE), color(color), fontWeight(fontWeight) {}

const string &Group::getName() const {
    return name;
}

void Group::setName(const string &name) {
    Group::name = name;
}

const string &Group::getRegex() const {
    return RE;
}

void Group::setRegex(const string &re) {
    RE = re;
}

const string &Group::getColor() const {
    return color;
}

void Group::setColor(const string &color) {
    Group::color = color;
}

int Group::getFontWeight() const {
    return fontWeight;
}

void Group::setFontWeight(int fontWeight) {
    Group::fontWeight = fontWeight;
}

string Group::toHTML_String() const {
    /*
    <div class="groupItem" style="color:#ff7f50; font-weight:900;">Group 0: C++ Keywords</div>
    <div class="groupItem" style="color:#6a5acd; font-weight:900;">Group 1: String Types</div>
    <div class="groupItem" style="color:#4169e1; font-weight:900;">Group 2: C++ Containers</div>
    <div class="groupItem" style="color:#3cb371; font-weight:900;">Group 3: RegEx Pattern</div>
    */
    string output = R"(<div class="groupItem" style="color:)" + this->getColor() + ";";
    const string font_weight = std::to_string(this->getFontWeight());
    output += R"(; font-weight:)" + font_weight + R"(;">)";
    output += "Group " + getTrailingDigits(this->getName()) + ": ";
    output += this->display_name + "</div>";
    return output;
}
