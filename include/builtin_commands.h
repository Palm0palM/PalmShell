#pragma once
#include "fun_wrappers.h"


void echo(const vector<string>& argv);
void pwd();
void cd(const string&);
void cmd_pause();
void ls(const vector<string>&);
void cmd_system(const vector<string>&);
void clear();

extern map<string, char> ls_longarg_shortarg;
// ^ 这个Map存储ls长参数到短参数的映射