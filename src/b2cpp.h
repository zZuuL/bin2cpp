#ifndef __BIN_2_CPP_H
#define __BIN_2_CPP_H

#include <map>
#include <list>
#include <string>
#include <fstream>

class b2cpp {
public:
    typedef std::map
    <
        std::string,
        std::list<std::string>
    > FilesData;

    struct writen_file {
        size_t size;
        std::string name;
    };

public:
    b2cpp();
    void run();

private:
    bool __write_file_data(std::ofstream& stream, std::string const& file_name);
    void __add_file_struct(std::ofstream& stream);
    void __add_writen_files_data(std::ofstream& stream);

private:
    std::list<writen_file> __writen_file_list;
};

#endif /*__BIN_2_CPP_H*/
