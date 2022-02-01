#include <iostream>

#include "main.h"
#include "b2cpp.h"
#include "commandlineparser.h"

int main(int argc, char **argv) {

    CommandLineParser parser (argc, argv);
    parser.parse();

    if (parser.is_help()) {
        std::cout << bin2cpp::help << std::endl;
        return EXIT_SUCCESS;
    }
    else {
        b2cpp app;
        app.run();
    }

    return EXIT_SUCCESS;
}
