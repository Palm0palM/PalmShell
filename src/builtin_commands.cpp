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

void pwd(){
    cout << fs::current_path() << std::endl;
}

void cd(const string& obj_path){
    fs::path new_path;
    if (obj_path == ".."){
        new_path = fs::current_path().parent_path();
    } 
    else if(obj_path == "."){
        new_path = fs::current_path();
    } 
    else if (obj_path[0] == '/'){
        new_path = obj_path;
    } 
    else if (obj_path[0] == '~'){
        new_path = home_path + obj_path.substr(1, obj_path.size() - 1);
    } 
    else if(obj_path[0] == '.' && obj_path[1] == '/'){
        new_path = fs::current_path().string() + obj_path.substr(1, obj_path.size() - 1);
    }
    else {
        new_path = fs::current_path().string() + '/' + obj_path;
    }
    try {
        if (!fs::exists(new_path)) {
            std::cerr << "cd: no such file or directory: " << obj_path << '\n';
            return;
        }

        if (!fs::is_directory(new_path)) {
            std::cerr << "cd: not a directory: " << obj_path << '\n';
            return;
        }

        fs::current_path(new_path);
        std::cout << "Changed directory to " << fs::current_path() << '\n';
    } 
    catch (const fs::filesystem_error& e) {
        std::cerr << "cd: error: " << e.what() << '\n';
    }
}