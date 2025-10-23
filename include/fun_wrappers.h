/* 这个文件包含了所有对Unix系统接口的包装以及某些C++库函数的重写
 * cpp_execve：
 * 接受C++标准库容器的execve，将vector<string>转为vector<char*>>后再调用execve，以兼容std::string与unix的C风格接口
 * 
 * unix_error：
 * 用起来更简单的报错函数
 * 
 * Fork:
 * 包含报错代码的fork函数
 * 
 * trim：
 * 清除字符串前后的空格
 * 
 * is_space/not_space：
 * 标准库函数std::is_space的包装
 * std::is_space只能接收int类型，此包装的功能是将unsigned char转为int
 * 
 * getline_with_arrowkey：
 * GNU Readline库的包装函数。接受一个字符串作为Prompt
 */
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <termios.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <readline/readline.h>
#include <readline/history.h>

using std::vector;
using std::string;
using std::cin;
using std::cout;

int cpp_execve(vector<string>&);
void trim(string&);

pid_t Fork();
void unix_error(string msg);

bool is_space(unsigned char);
bool not_space(unsigned char);

string getline_with_arrowkey(string prompt);