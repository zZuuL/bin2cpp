#include <iostream>
#include <fstream>

#include "Binaries_Data_Header.h"

int main(int argc, char **argv) {

    size_t files_count = sizeof(__Binaries_Data_Header__::__file_list) /
                         sizeof(__Binaries_Data_Header__::__file);

    for (size_t i = 0; i != files_count; ++i) {
        const __Binaries_Data_Header__::__file& file = __Binaries_Data_Header__::__file_list[i];
        std::cout << file.name << std::endl;
        std::ofstream out_file (file.name, std::ios::out | std::ios::trunc | std::ios::binary);
        if (out_file){
            for (size_t i = 0; i != file.size; ++i) {
                char c (file.data[i]);
                out_file.write(&c, sizeof(char));
            }
            out_file.close();
        }
    }

    return 0;
}
