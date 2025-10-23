#include "builtin_commands.h"

void echo(const vector<string>& argv){
    string delimiter = " ";
        for (size_t i = 1; i < (argv.size() - 1); ++i){
            if (argv[i] == "-d"){
                delimiter = argv[++i];
            } else {
                cout << argv[i] << delimiter;
            }
        }
    cout << *(argv.end() - 1) << std::endl;
}