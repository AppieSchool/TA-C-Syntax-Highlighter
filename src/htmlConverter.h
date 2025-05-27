 //
// Created by Jappe on 24/06/2024.
//

#ifndef TOHTML_HTMLCONVERTER_H
#define TOHTML_HTMLCONVERTER_H


#include <fstream>
#include <string>
#include <vector>
#include "Group.h"

class HTMLWriter {
public:
    HTMLWriter(const std::string &filename, const vector<Group *> &groups);

    ~HTMLWriter();

    /** Method for adding a not styled string to the main body
     *
     * @param text Line to add
     */
    void writePlainText(const std::string &text);

    /** Method for adding a styled string to the main body
     *
     * @param text Line to add
     * @param hexColorCode Text color
     * @param fontWeight font-weight:
     */
    void writeStyledText(const std::string &text, const std::string &hexColorCode, int fontWeight);

    /** Adds 1 <br> to the last line of HTML
     *
     */
    void addNewline();

    /** Saves the text that was written to the file.
     *
     */
    void saveFile();
    std::string escapeHTML(const std::string &text);

    /** Adds a new line to the Output Log field
     *
     * @param errorMessage The string that will be added to the field
     */
    void addError(const std::string &errorMessage);
private:
    std::string filename;
    std::string htmlContent;
    std::ofstream file;
    std::string errorMessage;
};


#endif //TOHTML_HTMLCONVERTER_H
