#include "builtin_commands.h"

void echo(const vector<string>& argv){
    string delimiter = " ";
        for (size_t i = 1; i < (argv.size() - 1); ++i){
            if (argv[i] == "-d"){// -d参数用来修改分隔符
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

void cd(const string& obj_path_str){
    //首先找到新路径
    fs::path new_path = obj_path_str;

    if (new_path.is_absolute()){
        new_path = fs::canonical(new_path);
    } else {
        new_path = fs::current_path() / new_path;
    }

    try {
        if (!fs::exists(new_path)) {// 检测目标路径是否存在
            std::cerr << "cd: no such file or directory: " << obj_path << '\n';
            return;
        }

        if (!fs::is_directory(new_path)) {// 检测目标路径是文件夹吗
            std::cerr << "cd: not a directory: " << obj_path << '\n';
            return;
        }

        // 最后， 修改当前路径
        fs::current_path(new_path);
        std::cout << "Changed directory to " << fs::current_path() << '\n';
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << "cd: error: " << e.what() << '\n';
    }
}

void cmd_pause(){
    cout << "Press Enter to continue..." << std::endl;
    cin.get();
    return;
}