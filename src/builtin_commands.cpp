#include "builtin_commands.h"
map<string, char> ls_longarg_shortarg = {
        {"recursive", 'R'},
        {"reverse", 'r'},
        {"human-readable", 'h'},
        {"help", 'H'},
        {"demiliter", 'd'}
};


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
            std::cerr << "cd: no such file or directory: " << obj_path_str << '\n';
            return;
        }

        if (!fs::is_directory(new_path)) {// 检测目标路径是文件夹吗
            std::cerr << "cd: not a directory: " << obj_path_str << '\n';
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

void ls(const vector<string>& argv){
    set<char> arg_set;// 参数列表
    fs::path obj_path;// 显示的目标路径
    string demiliter = "\n";// 表示分隔符

    if (argv.size() == 1){// 只有ls一个参数时，默认显示当前路径
        obj_path = fs::current_path();
    }
    else if (argv.size() == 2){// 只有两个参数时，默认第二个参数是目标路径
        obj_path = argv[1];
    }
    else if (argv.size() > 2){// 参数更多时，考虑是否控制参数
        bool is_patharg_exist = false;// 目标路径只能有一个，所以设置这一个变量，以将第一个非控制参数的参数当作目标路径
        for (size_t i = 1; i < argv.size(); ++i){
            if (!is_cmd_arg(argv[i], arg_set) && !is_patharg_exist){
                is_patharg_exist = true;
                obj_path = argv[i];
            }
        }
        if (!is_patharg_exist){// 没找到目标路径的情况下，默认显示当前路径
            obj_path = fs::current_path();
        }
    }

    try {
        if (!fs::exists(obj_path)) {// 检测目标路径是否存在
            std::cerr << "cd: no such file or directory: " << obj_path << '\n';
            return;
        }

        if (!fs::is_directory(obj_path)) {// 检测目标路径是文件夹吗
            std::cerr << "cd: not a directory: " << obj_path << '\n';
            return;
        }

        for (const auto& entry : fs::directory_iterator(obj_path)) {
            cout << entry.path().filename() << demiliter;
        }
    } catch (fs::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

}

void cmd_system(const vector<string>& argv){
    if (argv.size() <= 1){
        return;
    }

    string str;
    for (int i = 1; i < (argv.size() - 1); ++i){
        str += argv[i];
        str += " ";
    }
    
    str += argv[argv.size() - 1];
    system(str.c_str());
    return;
}

void clear(){
    system("clear");
    return;
}