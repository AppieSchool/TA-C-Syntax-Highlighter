//
// Created by abdir on 18/05/2025.
//

#ifndef ENGINE_FUNCTIONS_H
#define ENGINE_FUNCTIONS_H
#include <string>
#include <stack>
#include <tuple>
#include <vector>
/**
 *  Method-function for checking the balance of brackets in a string.
 * @param str The string that will be checked for brackets
 * @return  True if the brackets are balanced, false otherwise.
 */
bool are_brackets_balanced(const std::string& str);

/**
 *  Method-function for checking the balance of brackets in a vector of strings.
 * @param lines The vector of strings that will be checked for brackets
 * @return  True if the brackets are balanced, false otherwise with the line number
 */
std::tuple<bool, int> bracket_line(const std::vector<std::vector<std::string>>& lines);

/**
 * Method-function for normalizing words.
 * @param line The line that will be normalized
 * @return The normalized words
 */
std::vector<std::string> tokenizeLine(const std::string& line);

/** Check if a string is a control string
 * (lines by type of cycles, classes)
 *
 * @param line Split line into words, no spaces
 * @return True if the line is a control line
 */
bool isControlStructure(const std::vector<std::string>& line);

/** Check for semicolons in non-structural strings
 * Outputs found errors to Logger::log(Log Level::WARNING)
 *
 * @param lines Vector of broken lines into words, without spaces
 */
void checkSemicolons(const std::vector<std::vector<std::string>>& lines);

std::string getTrailingDigits(const std::string& input);


#endif //ENGINE_FUNCTIONS_H
