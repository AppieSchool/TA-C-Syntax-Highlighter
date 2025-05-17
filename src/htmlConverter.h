 //
// Created by Jappe on 24/06/2024.
//

#ifndef TOHTML_HTMLCONVERTER_H
#define TOHTML_HTMLCONVERTER_H


#include <fstream>
#include <string>
#include "Utils/Logger.h"

class HTMLWriter {
public:
    HTMLWriter(const std::string &filename);

    ~HTMLWriter();

    void writePlainText(const std::string &text);

    void writeStyledText(const std::string &text, const std::string &hexColorCode, int fontWeight);

    void addNewline();

    void saveFile();
    std::string escapeHTML(const std::string &text);
private:
    std::string filename;
    std::string htmlContent;
    std::ofstream file;
};


#endif //TOHTML_HTMLCONVERTER_H
