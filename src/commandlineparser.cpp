#include "commandlineparser.h"

#include "data.h"

/*
 * -----------------------------------------------------------------------]
 */

inline bool contains (const std::string& str, const std::string& find_what) {
    return str.find(find_what) != std::string::npos;
}

/*
 * -----------------------------------------------------------------------]
 */

CommandLineParser::CommandLineParser(int argc, char **argv)
    : _argc(argc)
    , _argv(argv)
{}

/*
 * -----------------------------------------------------------------------]
 */

void CommandLineParser::parse() {

    bool is_out_file_item = false;
    bool is_out_dir_item = false;

    if (_argc > 1) {
        for (int i = 1; i < _argc; ++i) {
            std::string command_line_item (_argv[i]);
            if (contains(command_line_item, "--help")) {
                _help = true;
                return ;
            }
            else if (contains(command_line_item, "--out-file")) {
                is_out_file_item = !is_out_file_item;
            }
            else if (contains(command_line_item, "--out-dir")) {
                is_out_dir_item = !is_out_dir_item;
            }
            else {
                if (is_out_file_item) {
                    bin2cpp::Config::instance().out_file = command_line_item;
                    is_out_file_item = !is_out_file_item;
                }
                else if (is_out_dir_item) {
                    bin2cpp::Config::instance().out_dir = command_line_item;
                    is_out_dir_item = !is_out_dir_item;
                }
                else {
                    bin2cpp::Config::instance().files.push_back(command_line_item);
                }
            }
        }
    }
}
