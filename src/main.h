#ifndef __Main_H
#define __Main_H

namespace bin2cpp {

static const char* help =
    "\nbin2cpp create a c++ header file from a binary to include it in your source code\n"
    "\nCommand line options:\n"
    "  --help                     Show this help\n"
    "  --out-file <Out file name> Set output header file name\n"
    "  --out-dir  <Out dir name>  Set folder for output header files\n"
    "  --xor      <key>           Enable XOR for input file bytes\n"
    "\nUsage: \n"
    "bin2cpp file1.zip file2.png file3.exe --out-file files.h\n";
}

#endif /*__Main_H*/
