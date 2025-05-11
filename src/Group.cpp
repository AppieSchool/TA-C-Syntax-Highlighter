#include "Group.h"

Group::Group(const string& name, const string& RE, const string& color, int fontWeight):
                name(name), RE(RE), color(color), fontWeight(fontWeight) {}

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
