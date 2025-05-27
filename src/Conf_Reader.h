//
// Created by Kaede on 2025-05-13.
//

#ifndef CONF_READER_H
#define CONF_READER_H
#include <vector>
#include "Group.h"
#include "ini_configuration.h"

class Conf_Reader {
    public:
    Conf_Reader() =default;
    ~Conf_Reader() =default;

    /** Method for reading configuration and creating groups for further work.
     * Groups are CREATED IN MEMORY and MUST be deleted.
     *
     * @param conf  The configuration that will be read.
     * @param count number group for recording
     * @param existing_groups list of groups to check unique name
     * @return A pair where the first element is a vector of all groups that were found, and the second element is the error value, if it was detected.
     */
    static pair<std::vector<Group *>, int> read_conf(const ini::Configuration &conf, int &count, const std::vector<Group *> &existing_groups);
    static pair<std::vector<Group *>, int> read_conf(const ini::Configuration &conf, int &count);

    /** Extending the merge_conf(std::vector<Group *> &, std::vector<Group *> &, std::string) method to work starting from configuration files.
     *
     * @param first_file The name of the first configuration file.
     * @param second_file The name of the second configuration file.
     * @param new_file_name The name of the file where the new configuration will be written
     */
    static void merge_conf(const std::string &first_file, const std::string &second_file, const std::string &new_file_name);

    /** Extending the merge_conf(std::vector<Group *> &, std::vector<Group *> &, std::string) method to work starting from open configurations.
     *
     * @param first_conf
     * @param second_conf
     * @param new_file_name The name of the file where the new configuration will be written
     */
    static void merge_conf(const ini::Configuration &first_conf, const ini::Configuration &second_conf, const std::string &new_file_name);

    /**Method-function for saving the combined list of groups to the .ini file.
     * Does not check groups for repetition or overwriting each other.
     *
     * @param first_groups The first list of groups. It is considered the main one.
     * @param second_groups The second list of groups. It is considered additional.
     * @param new_file_name The name of the file where the new configuration will be written
     */
    static void merge_conf(const std::vector<Group *> &first_groups, const std::vector<Group *> &second_groups, const std::string &new_file_name);

    /** Method for saving configuration groups to a configuration file.
     *
     * @param groups Groups that should be saved
     * @param new_file_name Save file name
     */
    static void save_conf(const std::vector<Group *> &groups, const std::string &new_file_name);

    /** Method for saving a string as an .ini file.
     * Does not check the syntax of the string.
     *
     * @param data The line that will be written to the file.
     * @param filename The name of the file where the information will be written
     * @return True, if managed to write information to a file
     */
    static bool write_to_ini(const std::string &data, const std::string &filename);
};



#endif //CONF_READER_H
