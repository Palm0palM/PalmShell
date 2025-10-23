#include "fun_wrappers.h"
#include "builtin_commands.h"

// 主要功能函数
void eval(string);// 解析命令的核心函数
bool parseline(string&, vector<string>&);// 拆分buf中储存的命令，存入argv中
// ^ 这个函数返回是否是后台命令
bool builtin_command(vector<string>&);// 分析、处理内置命令
// ^ 这个函数返回是否是内置命令
string path_display();
// ^ 这个函数用于提示符前显示当前路径，返回应显示的路径。目前主要的功能是把路径中存在的home_path替换为 ~ 符号

string prompt = "> ";// 命令提示符
string home_path = "";// 记录当前用户的home路径
bool is_home_tilde = true;// 这个变量控制在路径显示时，home文件夹是否被显示为 ~

int main()
{
    home_path = getenv("HOME");// 获取home路径
    string cmdline;// 存储用户输入
    
    while (true) {
        cmdline = getline_with_arrowkey(path_display());

        if (cmdline.empty() && cin.eof()){// 如果输入结束，则退出程序
            return 0;
        }

        //解析命令
        eval(cmdline);
    }

    return 0;
}


void eval(string cmdline)
{
    vector<string> argv;// 存储被拆分的命令
    bool is_bg;// 是否在后台运行？
    pid_t pid;// 子进程的pid

    // 拆分
    is_bg = parseline(cmdline, argv);

    if (argv.empty()){// 无视空行
        return;
    }

    // 处理命令
    bool is_builtin_command = builtin_command(argv);

    //处理外部命令
    if (!is_builtin_command) {
        argv[0] = fs::current_path().string() + '/' + argv[0];
        if ((pid = Fork()) == 0) {
            int exe_result = cpp_execve(argv);//尝试执行外部命令
            if (exe_result < 0) {
                cout << argv[0] << ": Command not found." << std::endl;
                exit(0);
            }
        }

        if (!is_bg) {// 对于非后台命令，等待命令执行完毕
            int status;
            if (waitpid(pid, &status, 0) < 0){
                unix_error("waitfg: waitpid error");
            }
        } else {// 对于后台命令，输出命令的pid
            cout << pid << " " << cmdline << std::endl;
        }
    }

    return;
}

bool builtin_command(vector<string>& argv)
{
    if (argv[0] == "quit"){
        cout << "Quiting..." << std::endl;
        exit(0);
        return 0;
    } 
    else if (argv[0] == "exit"){
        cout << "Exiting..." << std::endl;
        exit(0);
        return 0;
    }
    else if (argv[0] == "echo"){
        echo(argv);
        return true;
    }
    else if(argv[0] == "pwd"){
        pwd();
        return true;
    }
    else if(argv[0] == "cd"){
        if(argv.size() >= 2){// cd指令需要至少一个参数，多余的参数会被无视
            cd(argv[1]);
        }else{
            cout << "cd: Need a path directory to change into" << std::endl;
        }
        return true;
    }
    return false;
}


bool parseline(string& buf, vector<string>& argv)
{
    //清除空格
    trim(buf);

    //拆分
    //两个迭代器分别指向一个词的第一个字符、最后一个字符的下一个字符（左闭右开）
    auto it_left = buf.begin();
    auto it_right = std::find_if(it_left, buf.end(), is_space);

    while (it_right != buf.end()){// 只有右界限不到头，就不断循环寻找下一个词
        argv.push_back(string(it_left, it_right));
        // 更新左右界限
        it_left = it_right + 1;
        while((*it_left) == ' '){// 防止出现多个空格的情况
            it_left++;
        }

        it_right = std::find_if(it_left, buf.end(), is_space);
    }
    // 特殊情况：如果buf中只有一个词（没有空格）， it_right一开始就是end()，循环根本不会执行
    if (it_left != buf.end()) {
        argv.push_back(string(it_left, buf.end()));
    }
    

    if (argv.empty()){
        return false;// 空行显然不是内置命令
    }

    if(*(argv.end() - 1) == "&"){// 如果最后一个词是&，则为内置命令
        argv.pop_back();// 删除&
        return true;
    }
    
    return false;
}

string path_display(){
    auto unmodifiyed_str = fs::current_path().string();
    if (!is_home_tilde || unmodifiyed_str.size() < home_path.size()){// 如果home不应显示为tilde；或者当前路径长度小于homepath不可能需要替换，则直接返回
        return unmodifiyed_str + prompt;
    }

    // 这个循环用来检测路径是否存在home路径
    size_t i = 0;
    while (i < home_path.size()){
        if (unmodifiyed_str[i] == home_path[i]){
            ++i;
        } else {
            break;
        }
    }// 循环结束后，i指向源路径中包含的home路径的下一个字符

    if (i == home_path.size()){// 如果存在的话
        if (unmodifiyed_str.size() < home_path.size()){// 处理路径就是home本身的情况
            return "~";
        }
        return "~" + unmodifiyed_str.substr(i, unmodifiyed_str.size() - 1) + prompt;
    }
    return unmodifiyed_str + prompt;
}