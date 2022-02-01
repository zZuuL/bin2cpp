#ifndef __Comann_Line_Parser_H
#define __Comann_Line_Parser_H

class CommandLineParser {
public:
    CommandLineParser(int argc, char **argv);
    void parse();
    bool is_help() const {return _help;};

private:
    int _argc;
    char **_argv;
    bool _help = false;
};

#endif /*__Comann_Line_Parser_H*/
