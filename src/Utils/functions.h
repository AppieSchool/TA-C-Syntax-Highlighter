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


#endif //ENGINE_FUNCTIONS_H
