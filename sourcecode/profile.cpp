#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "libs/common.hpp"
#include "libs/sqltdb.hpp"
#include "libs/query.hpp"

using namespace std;

const int pd_SKEY = 0,
		  pd_ACTION = 1;
//** Load profile input
const int pd_PROFID = 2;

int main(int argc, char* argv[])
{
	string postText = retrievePost(),
		   postStr,
		   queryStr,
		   sendStr = "",
		   tmpName;
	int dbResult = 0;
	time_t currentTime = time(0);
	long int profileID = 0;
	sqltWrap db;
	vector <string> userData;
	
	cout << "Content-Type: text/html\n\n";
	
	//** Split input into vector
	tokenizeStr(postText, DLM, userData);
	
	//** Error checking
	if (userData.size() < 3){
		cout << "ERROR" << DLM << "Invalid command" << endl;
		return 0;
	}
	
	//** Get profile action
	if (userData[pd_ACTION] == "LOAD"){
		profileID = strtol(userData[pd_PROFID].c_str(), NULL, 10);
		
		queryStr = "SELECT Username FROM HubUsers WHERE UserID = ? LIMIT 1";
		db.prepare(queryStr);
		db.bind(1, profileID);
		
		//** Run command and send output
		if (db.runPrepared() != DB_SUCCESS) {
			cout << "ERROR" << DLM << "Invalid query \"" << queryStr << "\"" << endl;
			return 0;
		}
		
		//** Check count
		if (db.numRows() < 1){
			cout << "ERROR" << DLM << "User does not exist." << endl;
			return 0;
		}
		
		tmpName = db[0][0];
		
		cout << "LOAD" << DLM << tmpName << endl;
		return 0;
	}
	
	//** Default to error
	cout << "ERROR" << DLM << "Invalid command" << endl;
}