#include "ArgParser.h"

#include <vector>

Param::Param(const std::string &name_of_param, char short_name_of_param = ' ', const std::string& description = "") {
    init = Argument(name_of_param, short_name_of_param, description);
    is_multi_values = false;
    int_values = nullptr;
    real_count = 0;
    min_args_count = 0;
    is_positional = false;
}

Param::Param() {
    is_multi_values = false;
    int_values = nullptr;
    real_count = 0;
    min_args_count = 0;
    is_positional = false;
}

Flag::Flag(const std::string &name_of_flag, char short_name_of_flag = ' ', const std::string& description = "") {
    name = name_of_flag;
    short_name = short_name_of_flag;
    this->description = description;
    value = false;
    store = nullptr;
}

Flag::Flag(const std::string &name_of_flag, const std::string &description) {
    name = name_of_flag;
    this->description = description;
}

Argument::Argument() {
    need = false;
}

Argument::Argument(const std::string &name_of_flag, char short_name_of_flag, const std::string& description = "") {
    name = name_of_flag;
    short_name = short_name_of_flag;
    this->description = description;
    need = false;
}

using namespace ArgumentParser;

ArgParser::ArgParser(const std::string& parser_name) {
    name = parser_name;
    arguments = std::vector<Param>();
    flags = std::vector<Flag>();
    help = Argument();
}

ArgParser::ArgParser(const ArgParser &parser) {
    name = parser.name;
    arguments = parser.arguments;
    flags = parser.flags;
    help = parser.help;
}

bool ArgParser::Parse(const std::vector<std::string>& input) {
    std::string param;
    std::string value;
    char single_param;
    bool is_correct = false;
    bool need_help = false;


    for (auto argument: input) {
        if (argument[0] == '-') {
            is_correct = true;
            int i = 0;
            while (i < argument.length() && argument[i] != '=') {
                i++;
            }
            if (i != argument.length()) {
                if (argument[1] == '-') {
                    param = argument.substr(2, i - 2);
                } else {
                    single_param = argument[i-1];
                }
                value = argument.substr(i + 1);
                for (auto &params: arguments) {
                    if (params.init.name == param || params.init.short_name == single_param) {
                        params.init.values.emplace_back(value);
                        if (!params.store.empty()) {
                            *params.store[0] = value;
                        }
                        if (params.is_multi_values) {
                            params.int_values->emplace_back(atoi(value.c_str()));
                        }
                        params.real_count++;
                    }
                }
            } else if (argument[1] == '-') {
                if (argument == "--help") {
                    is_correct = true;
                    help.need = true;
                } else {
                    for (auto flag: flags) {
                        if (flag.name == argument.substr(2, argument.length() -2)) {
                            flag.value = true;
                            if (flag.store != nullptr) {
                                *flag.store = true;
                            }
                        }
                    }
                }
            } else  {
                for (auto flag : flags) {
                    if (argument.find(flag.short_name) != std::string::npos) {
                        is_correct = true;
                        flag.value = true;
                        if (flag.store != nullptr) {
                            *flag.store = true;
                        }
                    }
                }
            }
        } else {
            for (auto &each_argument: arguments) {
                if (each_argument.is_positional && argument != "app") {
                    is_correct = true;
                    each_argument.init.values.emplace_back(argument);
                    each_argument.real_count++;
                    if (each_argument.int_values != nullptr) {
                        each_argument.int_values->emplace_back(atoi(argument.c_str()));
                    }
                }
            }
        }

        param.clear();
        single_param = ' ';
        value.clear();

    }

    if (!is_correct) {
        if (arguments.empty()) {
            return true;
        } else {
            if (arguments[arguments.size() - 1].init.values.empty()) {
                return false;
            }
        }
    } else {
        if (help.need) {
            return true;
        }
        for (const auto& each_param: arguments) {
            if (each_param.is_multi_values && each_param.real_count < each_param.min_args_count) {
                return false;
            }
        }
    }
    return true;
}

ArgParser& ArgParser::AddStringArgument(const std::string& name_of_param) {
    Param param1(name_of_param);
    arguments.emplace_back(param1);
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char short_name_of_param, const std::string &name_of_param) {
    Param param1(name_of_param, short_name_of_param);
    arguments.emplace_back(param1);
    return *this;
}

std::string ArgParser::GetStringValue(const std::string& param) {
    for (auto params: arguments) {
        if (params.init.name == param) {
            return params.init.values[0];
        }
    }
}

void ArgParser::Default(char* value) {
    arguments[arguments.size() - 1].init.values.emplace_back(value);
    arguments[arguments.size() - 1].real_count++;
}

void ArgParser::StoreValue(std::string& value){
    arguments[arguments.size() - 1].store.emplace_back(&value);
}

ArgParser& ArgParser::AddIntArgument(const std::string &name_of_param) {
    Param param1(name_of_param);
    arguments.emplace_back(param1);
    return *this;
}

ArgParser &ArgParser::AddIntArgument(char short_name_of_param, const std::string &name_of_param) {
    Param param1(name_of_param, short_name_of_param);
    arguments.emplace_back(param1);
    return *this;
}

int ArgParser::GetIntValue(const std::string& param) {
    for (auto params: arguments) {
        if (params.init.name == param) {
            return atoi(params.init.values[0].c_str());
        }
    }
}

int ArgParser::GetIntValue(const std::string &param, int index) {
    for (auto params: arguments) {
        if (params.init.name == param) {
            return atoi(params.init.values[index].c_str());
        }
    }
}

ArgParser& ArgParser::MultiValue() {
    arguments[arguments.size() - 1].is_multi_values = true;
    return *this;
}

void ArgParser::StoreValues(std::vector<int>& int_values) {
    arguments[arguments.size() - 1].int_values = &int_values;
}

ArgParser &ArgParser::MultiValue(size_t min_args_count) {
    arguments[arguments.size() - 1].is_multi_values = true;
    arguments[arguments.size() - 1].min_args_count = min_args_count;
    return *this;
}

ArgParser& ArgParser::AddFlag(char short_name, const std::string& name, const std::string& description) {
    Flag flag(name, short_name, description);
    flags.emplace_back(flag);
    return *this;
}

bool ArgParser::GetFlag(const std::string &flag_name) {
    for (auto flag: flags) {
        if (flag.name == flag_name) {
            return true;
        }
    }
    return false;
}

void ArgParser::Default(bool value) {
    flags[flags.size() - 1].value = value;
}

void ArgParser::StoreValue(bool& value) {
    flags[flags.size() - 1].store = &value;
}

ArgParser &ArgParser::Positional() {
    arguments[arguments.size() - 1].is_positional = true;
    return *this;
}

void ArgParser::AddHelp(char shor_help, const std::string &full_help, const std::string &description) {
    help.short_name = shor_help;
    help.name = full_help;
    help.description = description;
}

bool ArgParser::Help() {
    return help.need;
}

ArgParser& ArgParser::AddStringArgument(char short_argument, const std::string &full_argument, const std::string &description) {
    Param param1(full_argument, short_argument, description);
    arguments.emplace_back(param1);
    return *this;
}

ArgParser &ArgParser::AddIntArgument(const std::string &number, const std::string &some_number) {
    return *this;
}

ArgParser &ArgParser::AddFlag(const std::string &name, const std::string &description) {
    Flag flag(name, description);
    flags.emplace_back(flag);
    return *this;
}

std::string &ArgParser::HelpDescription() {
    return help.description;
}
