#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include "b2cpp.h"
#include "data.h"

#ifdef __linux__
    #include <stdlib.h>
#elif _WIN32
    #include <fileapi.h> //GetFullPathNameA
#endif

/*
 * -----------------------------------------------------------------------]
 */

inline bool is_path_exists(std::string const& file_name) {
    std::ifstream ifs;
    ifs.open(file_name);
    if(ifs) {
        ifs.close();
        return true;
    }
    return false;
}

/*
 * -----------------------------------------------------------------------]
 */

inline std::string get_file_dir(const std::string& fname)
{
     size_t last_separator = fname.find_last_of("\\/");
     return (std::string::npos == last_separator)
         ? "."
         : fname.substr(0, last_separator);
}

/*
 * -----------------------------------------------------------------------]
 */

inline std::string file_name_short(std::string const& filename) {
    size_t last_separator = filename.find_last_of("\\/");
    return std::string::npos == last_separator ?
        filename :
        filename.substr(last_separator + 1, filename.size());
}

/*
 * -----------------------------------------------------------------------]
 */

inline  std::string file_name_without_ext(std::string const& filename) {
    std::string file_short_name = file_name_short(filename);
    size_t last_dot = file_short_name.find_last_of(".");
    return std::string::npos == last_dot ?
        file_short_name :
        file_short_name.substr(0, last_dot);
}

/*
 * -----------------------------------------------------------------------]
 */

inline std::string get_file_full_path (const std::string& file_name) {
    std::string result;
#ifdef __linux__
    result = std::string(realpath(file_name.c_str(), NULL));
#elif _WIN32
#endif
    return result;
}

/*
 * -----------------------------------------------------------------------]
 */

inline std::string get_separator() {
#ifdef __linux__
    return "/";
#elif _WIN32
    return "\\";
#endif
}

/*
 * -----------------------------------------------------------------------]
 */

inline b2cpp::FilesData init_files_data() {
    b2cpp::FilesData result;

    std::string save_path = bin2cpp::Config::instance().out_dir.empty() ?
        get_file_full_path(".") :
        get_file_full_path(bin2cpp::Config::instance().out_dir);

    for (auto const& file: bin2cpp::Config::instance().files) {
        if (is_path_exists(file)) {
            std::string out_file_name = bin2cpp::Config::instance().out_file.empty() ?
                file_name_without_ext(file) + ".h" :
                bin2cpp::Config::instance().out_file;

            result[save_path + get_separator() + out_file_name].push_back(file);
        }
    }

    return result;
}

/*
 * -----------------------------------------------------------------------]
 */

inline void write_data (std::ofstream& stream, std::string const& text) {
    stream.write(text.data(), text.size());
}

/*
 * -----------------------------------------------------------------------]
 */

b2cpp::b2cpp()
{}

/*
 * -----------------------------------------------------------------------]
 */

#define HEX(x) std::setw(8) << std::setfill('0') << std::hex << (int)( x )

bool b2cpp::__write_file_data(std::ofstream& stream, std::string const& file_name) {
    std::ifstream input_stream;
    writen_file wfile;

    input_stream.open(file_name, std::ios::in | std::ios::binary | std::ios::ate);
    if(input_stream) {
        wfile.size = input_stream.tellg();
        input_stream.seekg(0);

        char data;

        std::string row;
        size_t row_byte_index = 0;
        while (input_stream.good() && !input_stream.eof()) {
            input_stream.read(&data, sizeof(char));

            std::stringstream ss;
            ss << "0x" << HEX(data) << ", ";
            row.append(ss.str());

            if (row_byte_index == 20) {
                row_byte_index = 0;
                write_data(stream, "\t" + row + "\n");
                row.clear();
            }
            else {
                ++row_byte_index;
            }
        }

        if (!row.empty()) {
            std::string tmp(row.begin(), row.end() - 2);
            write_data(stream, "\t\t" + tmp + "\n");
        }
    }
    else {
        return false;
    }

    wfile.name = file_name_short(file_name);
    __writen_file_list.push_back(wfile);

    return true;
}

/*
 * -----------------------------------------------------------------------]
 */

void b2cpp::__add_writen_files_data(std::ofstream& stream) {
    write_data(stream, "static const __file __file_list[] = {\n");

    size_t writen_files_len = __writen_file_list.size() - 1;
    for (writen_file const i: __writen_file_list) {

        std::stringstream ss;
        ss << "\t{(char*)\"" << i.name << "\", "
           << i.size << ", (const unsigned int*)&" << file_name_without_ext(i.name) << "}";
        if (writen_files_len > 0) {
            ss << ",\n";
            --writen_files_len;
        }
        else {
            ss << "\n";
        }
        write_data(stream, ss.str());
    }
    write_data(stream, "};\n\n");
}

/*
 * -----------------------------------------------------------------------]
 */

void b2cpp::__add_file_struct(std::ofstream& stream) {
    write_data(stream, "struct __file {\n");
    write_data(stream, "\tchar* name;\n");
    write_data(stream, "\tsize_t size;\n");
    write_data(stream, "\tconst unsigned int* data;\n");
    write_data(stream, "};\n\n");
}

/*
 * -----------------------------------------------------------------------]
 */

void b2cpp::run() {

    b2cpp::FilesData data = init_files_data();

    for (auto i: data) {
        std::ofstream out_file (i.first, std::ios::out | std::ios::trunc);
        if (out_file.is_open()) {

            std::string file_header_H = "__" + file_name_without_ext(i.first) + "_H";

            write_data(out_file, "#ifndef " + file_header_H + "\n");
            write_data(out_file, "#define " + file_header_H + "\n\n");
            write_data(out_file, "namespace __" + file_name_without_ext(i.first) + "__ {\n\n");

            for (auto file: i.second) {
                write_data(out_file, "static const unsigned int " + file_name_without_ext(file) + "[] = {\n");
                if (!__write_file_data(out_file, file)) {
                    write_data(out_file, "\t/*Cannot read binary file*/\n");
                }
                write_data(out_file, "};\n\n");
            }

            if (!__writen_file_list.empty()) {
                __add_file_struct(out_file);
                __add_writen_files_data(out_file);
            }

            write_data(out_file, "}\n\n"); //close namespace
            write_data(out_file, "#endif /*" + file_header_H + "*/\n");
        }
        else {
            std::cerr << "Cannot create file: " << i.first << std::endl;
        }
    }
}
