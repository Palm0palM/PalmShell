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

void cd(const string& obj_path){
    //首先找到新路径
    fs::path new_path;
    if (obj_path == ".."){// 特殊情况-上一级路径
        new_path = fs::current_path().parent_path();
    } 
    else if(obj_path == "."){// 特殊情况-本路径
        new_path = fs::current_path();
    } 
    else if (obj_path[0] == '/'){// 绝对路径的情况
        new_path = obj_path;
    } 
    else if (obj_path[0] == '~'){// 从home目录开始的情况
        new_path = home_path + obj_path.substr(1, obj_path.size() - 1);
    } 
    else if(obj_path[0] == '.' && obj_path[1] == '/'){// 多此一举的相对路径
        new_path = fs::current_path().string() + obj_path.substr(1, obj_path.size() - 1);
    }
    else {// 其余情况-普通的相对路径
        new_path = fs::current_path().string() + '/' + obj_path;
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