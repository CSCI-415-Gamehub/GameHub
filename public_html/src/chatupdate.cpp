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
		  pd_TIMESTAMP = 1;

int main(int argc, char* argv[])
{
	string postText = retrievePost(),
		   postStr,
		   queryStr,
		   sendStr = "",
		   tmpName,
		   tmpMessage,
		   tmpColor;
	int dbResult = 0;
	time_t currentTime = time(0);
	long int updateTime = 0;
	sqltWrap db;
	vector <string> userData;
	
	cout << "Content-Type: text/html\n\n";
	
	//** Split input into vector
	tokenizeStr(postText, DLM, userData);
	
	//** Error checking
	if (!is_number(userData[pd_TIMESTAMP])){
		cout << "ERROR" << DLM << "Invalid update time" << endl;
		return 0;
	}
	
	//** Convert time
	updateTime = strtol(userData[pd_TIMESTAMP].c_str(), NULL, 10);
	toString(postStr, updateTime);
	
	//** Get messages from database
	db.open(DIR_DB);
	
	//** Confirm logged in
	dbResult = checkSession(db, userData[pd_SKEY].c_str()) != DB_SUCCESS;
	if (dbResult != DB_SUCCESS){
		cout << "ERROR" << DLM << "Failed to check session id [" << dbResult << "]" << endl;
		return 0;
	}
	if (db.numRows() == 0){
		cout << "ERROR" << DLM << "Not logged in.";
		return 0;
	}
	
	//** Get messages from database
	queryStr = "SELECT Username, Msg, Color FROM HubMessages WHERE CreateTime > ? ORDER BY CreateTime ASC";
	db.prepare(queryStr);
	db.bind(1, updateTime);
	
	//** Run command and send output
	if (db.runPrepared() == DB_SUCCESS) {
		sendStr = "";
		for (int i=0; i < db.numRows();i++){
			sendStr += DLM;
			tmpName = db[i][0];
			tmpMessage = db[i][1];
			tmpColor = db[i][2];
			sendStr += tmpName + DLM + tmpMessage + DLM + tmpColor;
		}
		cout << currentTime << sendStr << endl;
	} else {
		cout << "ERROR" << DLM << "Invalid query \"" << queryStr << "\"" << endl;
	}
}