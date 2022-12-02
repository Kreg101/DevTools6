#include <functional>
#include <lib/ArgParser.h>

#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <iterator>

struct Options {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char* argv[]) {
    Options opt;
    std::vector<int> values;

    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << '\n';
    }

    ArgumentParser::ArgParser parser("Program");
    parser.AddIntArgument("N").MultiValue(1).Positional().StoreValues(values);
    parser.AddFlag("sum", "add args").StoreValue(opt.sum);
    parser.AddFlag("mult", "multiply args").StoreValue(opt.sum);
    parser.AddHelp('h', "help", "Program accumulate arguments");

    std::vector<std::string> arg;
    for (int i = 0; i < argc; i++) {
        arg.emplace_back(argv[i]);
    }

    if(!parser.Parse(arg)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if(parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    if(opt.sum) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 0) << std::endl;
    } else if(opt.mult) {
        std::cout << "Result: " << std::accumulate(values.begin(), values.end(), 1, std::multiplies<int>()) << std::endl;
    } else {
        std::cout << "No one options had chosen" << std::endl;
        std::cout << parser.HelpDescription();
        return 1;
    }

    return 0;

}
