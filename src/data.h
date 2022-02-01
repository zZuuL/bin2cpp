#ifndef __Data_H
#define __Data_H


#include <list>
#include <string>

namespace bin2cpp {

struct Config {
    typedef std::list <std::string> Files;
    std::string out_file = "";
    std::string out_dir  = "";
    Files       files;

    static Config& instance() {
        static Config config;
        return config;
    }
};

}

#endif /*__Data_H*/
