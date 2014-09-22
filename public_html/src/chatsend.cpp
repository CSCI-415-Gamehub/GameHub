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
		  pd_MESSAGE = 1,
		  pd_TIMESTAMP = 2;

int main(int argc, char* argv[])
{
	string PostText = retrievePost(),
		   postName,
		   postMsg,
		   queryStr,
		   postStr,
		   sendStr,
		   tmpName,
		   tmpMessage,
		   tmpColor,
		   tmpMute,
		   UserID;
	int dbResult = 0;
	time_t currentTime;
	sqltWrap db;
	long int updateTime,
			muteLevel = 0;
	vector <string> userData;
	
	cout << "Content-Type: text/html\n\n";
	
	//** No data sent
	if (PostText == "" || PostText == DLM){
		cout << "ERROR" << DLM << "No message" << endl;
		return 0;
	}
	
	//** Split input into vector
	tokenizeStr(PostText, DLM, userData);
	
	//** Handle invalid user data length
	if (userData.size() < 3){
		cout << "ERROR" << DLM << "Invalid data recieved." << endl;
		return 0;
	}
	
	//** Connect to DB
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
	UserID = db[0][0];
	
	queryStr = "SELECT Username, Color, MuteLevel FROM HubUsers WHERE UserID = ?";
	dbResult = db.prepare(queryStr);
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR||Failed to prepare query. [" << dbResult << "]" << endl;
		return 0;
	}
	db.bind(1, UserID);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR" << DLM << "Failed to run prepared query [" << dbResult << "]" << endl;
		return 0;
	}
	
	tmpName = db[0][0];
	tmpColor = db[0][1];
	tmpMute = db[0][2];
	
	//** Check if muted
	muteLevel = strtol(tmpMute.c_str(), NULL, 10);
	if (muteLevel < 1){
	
		//** Send message to database
		queryStr = "INSERT INTO HubMessages (Username, Msg, Color) VALUES ( ?, ?, ? )";
		dbResult = db.prepare(queryStr);
		if (dbResult != DB_SUCCESS) {
			cout << "ERROR||Failed to prepare query. [" << dbResult << "]" << endl;
			return 0;
		}
		db.bind(1, tmpName);
		db.bind(2, userData[pd_MESSAGE].c_str());
		db.bind(3, tmpColor);
		
		dbResult = db.runPrepared();
		if (dbResult != DB_SUCCESS) {
			cout << "ERROR" << DLM << "Failed to run prepared query [" << dbResult << "]" << endl;
			return 0;
		}
	}
	
	//** Error checking
	if (!is_number(userData[2])){
		cout << "ERROR" << DLM << "Invalid update time '" << userData[2] << "'" << endl;
		return 0;
	}
	
	//** Convert time
	updateTime = strtol(userData[pd_TIMESTAMP].c_str(), NULL, 10);
	toString(postStr, updateTime);
	currentTime = time(0);
	
	//** Get messages from database
	queryStr = "SELECT Username, Msg, Color FROM HubMessages WHERE CreateTime >= ? ORDER BY CreateTime ASC";
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