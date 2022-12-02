#pragma once
#include <string>
#include <vector>

struct Flag {

    std::string name;
    char short_name;
    std::string description;
    bool value;
    bool* store;

    Flag(const std::string& name_of_flag, char short_name_of_flag, const std::string& description);

    Flag(const std::string& name_of_flag, const std::string& description);

};

struct Argument {

    std::string name;
    char short_name;
    std::string description;
    bool need;
    std::vector<std::string> values;

    Argument();

    Argument(const std::string& name_of_flag, char short_name_of_flag, const std::string& description);
};

struct Param {
    Argument init;

    bool is_multi_values;

    std::vector<std::string*> store;
    std::vector<int>* int_values;

    int real_count;
    size_t min_args_count;
    bool is_positional;

    Param(const std::string& name_of_param, char short_name_of_param, const std::string& description);

    Param();
};

namespace ArgumentParser {

class ArgParser {

    std::string name;
    std::vector<Param> arguments;
    std::vector<Flag> flags;
    Argument help;

public:

    ArgParser(const std::string& parser_name);

    ArgParser(const ArgParser &parser);

    bool Parse(const std::vector<std::string>& arguments);

    ArgParser& AddStringArgument(const std::string& name_of_param);

    ArgParser& AddStringArgument(char short_name_of_param, const std::string& name_of_param);

    std::string GetStringValue(const std::string& param);

    void Default(char* value);

    void StoreValue(std::string& value);

    ArgParser& AddIntArgument(const std::string& name_of_param);

    ArgParser& AddIntArgument(char short_name_of_param, const std::string& name_of_param);

    int GetIntValue(const std::string& param);

    int GetIntValue(const std::string& param, int index);

    ArgParser& MultiValue();

    ArgParser& MultiValue(size_t min_args_count);

    void StoreValues(std::vector<int>& int_values);

    ArgParser& AddFlag(char short_name, const std::string& name, const std::string& description = "");

    ArgParser& AddFlag(const std::string& name, const std::string& description);

    bool GetFlag(const std::string& flag_name);

    void Default(bool value);

    void StoreValue(bool& value);

    ArgParser& Positional();

    void AddHelp(char shor_help, const std::string &full_help, const std::string &description);

    bool Help();

    ArgParser& AddStringArgument(char short_argument, const std::string& full_argument, const std::string& description);

    ArgParser& AddIntArgument(const std::string& number, const std::string& some_number);


    std::string &HelpDescription();

};

} // namespace ArgumentParser