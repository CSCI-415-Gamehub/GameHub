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
		  pd_COMMAND = 1;
const string COMMAND_MYPROFILE = "SHOWME",
			 COMMAND_OTHERPROFILE = "SHOWOTHER",
			 COMMAND_COLOR = "COLOR";
			 
//** Load profile input
const int pd_PROFID = 2;

int main(int argc, char* argv[])
{
	string postText = retrievePost(),
		   postStr,
		   queryStr,
		   sendStr = "",
		   tmpName,
		   tmpLevel,
		   tmpID,
		   userID,
		   userQuery = "SELECT Username, UserLevel FROM HubUsers WHERE UserID = ? LIMIT 1";
	int dbResult = 0;
	time_t currentTime = time(0);
	long int profileID = 0;
	sqltWrap db;
	vector <string> userData;
	
	cout << "Content-Type: text/html\n\n";
	
	//** Split input into vector
	tokenizeStr(postText, DLM, userData);
	
	//** Error checking (force user to send skey and command)
	if (userData.size() < 2){
		cout << "ERROR" << DLM << "Invalid command" << endl;
		return 0;
	}
	
	//** Connect to DB
	db.open(DIR_DB);
	
	//** Confirm logged in
	dbResult = checkSession(db, userData[pd_SKEY].c_str());
	if (dbResult != DB_SUCCESS){
		cout << "ERROR" << DLM << "Failed to check session id [" << dbResult << "]" << endl;
		return 0;
	}
	if (db.numRows() == 0){
		cout << "ERROR" << DLM << "Not logged in.";
		return 0;
	}
	userID = db[0][0];
	
	//** Get profile action
	if (userData[pd_COMMAND] == COMMAND_MYPROFILE || userData[pd_COMMAND] == COMMAND_OTHERPROFILE){
		//** Load profile of this user
		if (userData[pd_COMMAND] == COMMAND_OTHERPROFILE){
			profileID = strtol(userData[pd_PROFID].c_str(), NULL, 10);
		} else {
			profileID = strtol(userID.c_str(), NULL, 10);
		}
		
		db.prepare(userQuery);
		db.bind(1, profileID);
		
		//** Run command and send output
		if (db.runPrepared() != DB_SUCCESS) {
			cout << "ERROR" << DLM << "Invalid query \"" << userQuery << "\"" << endl;
			return 0;
		}
		
		//** Check count
		if (db.numRows() < 1){
			cout << "ERROR" << DLM << "User does not exist." << endl;
			return 0;
		}
		
		tmpName = db[0][0];
		tmpLevel = db[0][1];
		
		cout << "LOAD" << DLM << tmpName << DLM << tmpLevel << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_COLOR){
		//** Error checking (force user to send skey, command, and userid)
		if (userData.size() < 3){
			cout << "ERROR" << DLM << "Invalid command" << endl;
			return 0;
		}
	}
	
	//** Default to error
	cout << "ERROR" << DLM << "Invalid command" << endl;
}