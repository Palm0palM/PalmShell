#include "fun_wrappers.h"

int cpp_execve(vector<string>& argv){
    std::vector<char*> c_argv;
    for (auto& arg : argv) {
        c_argv.push_back(arg.data());
    }
    c_argv.push_back(nullptr);// Unix要求参数列表最后一个指针应指向NULL
    
    return execve(c_argv[0], c_argv.data(), environ);
}

void trim(string &s) {
    // 左
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
    // 右
    s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
}

void unix_error(string msg)
{
    fprintf(stderr, "%s: %s\n", msg.c_str(), strerror(errno));
    exit(0);
}

pid_t Fork() 
{
    pid_t pid;

    if ((pid = fork()) < 0)
	unix_error("Fork error");
    return pid;
}

bool is_space(unsigned char ch) {
    return std::isspace(static_cast<unsigned char>(ch));
}

bool not_space(unsigned char ch) {
    return !std::isspace(static_cast<unsigned char>(ch));
}

string getline_with_arrowkey(string prompt){
    char* input = readline(prompt.c_str());
    if (!input) exit(0);  // 处理EOF

    std::string result(input);
    free(input);

    if (!result.empty()){
        add_history(result.c_str());
    }
    return result;
}

bool is_cmd_arg(const string& arg, set<char>& arg_set){
    if (arg.size() < 2){// 长度太小的参数显然不是控制参数
        return false;
    }
    if (arg[0] == '-'){// 先考虑短参数
        if (arg[1] == '-'){// 再考虑长参数
            if (ls_longarg_shortarg.find(arg.substr(2)) != ls_longarg_shortarg.end()){
                arg_set.insert(ls_longarg_shortarg[arg.substr(2)]);// 长参数全部以短参数形式保存
                return true;
            }
        }
        for (size_t i = 1; i < arg.size(); ++i){// 逐个添加短参数
            arg_set.insert(arg[i]);
        }
        return true;
    }
    return false;
}