#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "libs/common.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	string postText = retrievePost();
	vector<string> postData;
	int dbResult = 0;
	sqltWrap db;
	
	//** No data sent
	if (postText == "" || postText == DLM){
		cout << "ERROR" << DLM << "No message";
		return 0;
	}

	//** Split input into vector
	tokenizeStr(postText, DLM, postData);

	cout << "Content-Type: text/plain\n\n";
	
	return 0;
}