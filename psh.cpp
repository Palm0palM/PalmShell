#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

using std::vector;
using std::string;
using std::cin;
using std::cout;

// 主要功能函数
void eval(string);// 解析命令的主要函数
bool parseline(string&, vector<string>&);// 拆分buf中储存的命令，存入argv中
// ^ 这个函数返回是否是后台命令
bool builtin_command(vector<string>&);// 分析、处理内置命令
// ^ 这个函数返回是否是内置命令

int cpp_execve(vector<string>&);// 将vector<string>转为vector<char*>>后再调用execve，以兼容std::string与unix的C风格接口
void trim(string&);// 清除字符串前后的空格

pid_t Fork();// 包含报错代码的fork函数
void unix_error(string msg);// 报错函数

string prompt = ">";// 提示符

int main()
{
    string cmdline;// 存储用户输入

    while (true) {
        cout<<prompt<<" ";
        std::getline(cin, cmdline);
        if (cin.eof()){// 如果输入结束，则退出程序
            return 0;
        }

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
    if (!builtin_command(argv)) {
        //处理外部命令
        if ((pid = Fork()) == 0) {
            if (cpp_execve(argv) < 0) {//尝试执行外部命令
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
            cout << pid << " " << cmdline << endl;
        }
    }

    return;
}

bool builtin_command(vector<string>& argv)
{
    if (argv[0] == "quit"){
        cout << "Quiting Program..." << std::endl;
        exit(0);
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
    auto it_right = std::find_if(it_left, buf.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });

    while (it_right != buf.end()){// 只有右界限不到头，就不断循环寻找下一个词
        argv.push_back(string(it_left, it_right));
        // 更新左右界限
        it_left = it_right + 1;
        it_right = std::find_if(it_left, buf.end(), [](unsigned char ch) {
        return std::isspace(ch);
    });
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

void trim(string &s) {
    // 左
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    // 右
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

int cpp_execve(vector<string>& argv){
    std::vector<char*> c_argv;
    for (auto& arg : argv) {
        c_argv.push_back(arg.data());
    }
    c_argv.push_back(nullptr);// Unix要求参数列表最后一个指针应指向NULL

    return execve(c_argv[0], c_argv.data(), environ);
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