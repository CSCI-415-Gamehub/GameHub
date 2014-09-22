#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <vector>
#include <string>
#include <string.h>

using namespace std;

#ifndef LIB_COMMON_HPP_
#define LIB_COMMON_HPP_

//** Global constants
const string DIR_DB = "db/hub.sqlite",
			 DLM = "_|";

//** Methods
void toString (string &str, int num);

void toString (string &str, long num);

void toString (string &str, long long num);

string toUpper(string &str);

bool is_number(const string& s);

bool is_alnum(const string& s);

bool is_safe(const string& s);

bool is_hex(const string& s);

void replaceInStr (string &str, const string inStr, const string repStr);

string retrievePost();

//** Method adapted from http://www.cplusplus.com/faq/sequences/strings/split/
void tokenizeStr(string &str, const string &delim, vector<string> &dest);

//** Method adapted from http://www.cplusplus.com/faq/sequences/strings/split/
void tokenizeStr(string &str, const char* delim, vector<string> &dest);

template <class A, class B>
void Swap(A a, B b);

#endif /* LIB_COMMON_HPP_ */