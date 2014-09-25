#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "libs/common.hpp"
#include "libs/sqltdb.hpp"
#include "libs/query.hpp"
#include "libs/randStrHash.hpp"

using namespace std;

const int pd_SKEY = 0,
		  pd_COMMAND = 1;
const int pd_GAMENAME = 2,
		  pd_MIN_PLAYERS = 3,
		  pd_MAX_PLAYERS = 4,
		  pd_URL = 5,
		  pd_DESC = 6;
		  
const string COMMAND_MYPROFILE = "SHOWME",
			 COMMAND_OTHERPROFILE = "SHOWOTHER",
			 COMMAND_COLOR = "COLOR",
			 COMMAND_MYGAMES = "MYGAMES",
			 COMMAND_ADDGAME = "ADDGAME",
			 COMMAND_NEWREG = "NEWREG",
			 COMMAND_NEWPASS = "NEWPASS",
			 COMMAND_NEWSTATS = "NEWSTATS";
			 
//** Other fields
const int pd_PROFID = 2,
		  pc_COLOR = 2;

int profileCommandColor(sqltWrap &db, string &userID, vector<string> &userData){
	string queryStr = "UPDATE HubUsers SET Color = ? WHERE UserID = ?";
	int dbResult;
	
	//** Error checking (force user to send skey, command, and color)
	if (userData.size() < 3){
		cout << "ERROR" << DLM << "Invalid command" << endl;
		return 0;
	}
	
	//** Force color size to be exactly 6 characters
	if (userData[pc_COLOR].size() < 6){
		cout << "ERROR" << DLM << "Invalid command" << endl;
		return 0;
	}
	
	//** TODO Confirm color is 6 numbers
	
	//** Update user's chat color
	db.prepare(queryStr);
	db.bind(1, userData[pc_COLOR]);
	db.bind(2, userID);
	dbResult = db.runPrepared();
	
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR||Failed to run prepared query [" << dbResult << "]" << endl;
		return 0;
	}
	
	//** Return command signalling success
	cout << COMMAND_COLOR << DLM << userData[pc_COLOR] << endl;
	return 0;
}

int profileCommandMyGames(sqltWrap &db, string &userID, vector<string> &userData){
	string queryStr = "SELECT UserLevel FROM HubUsers WHERE UserID = ?";
	int dbResult,
		userLevel;
	
	//** Retrieve user level
	db.prepare(queryStr);
	db.bind(1, userID);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR||Failed to run prepared query [" << dbResult << "]" << endl;
		return 0;
	}
	
	//** Confirm user level is high enough to post games
	userLevel = atol(db[0][0].getData().c_str());
	if (userLevel < 3){
		cout << "ERROR||Invalid user level" << endl;
		return 0;
	}
	
	//** Get list of games
	queryStr = "SELECT GameID, GameName, URL, Description FROM Games WHERE PosterID = ?";
	db.prepare(queryStr);
	db.bind(1, userID);
	dbResult = db.runPrepared();
	
	//** Return command and data signalling success
	cout << COMMAND_MYGAMES << DLM << db.numRows();
	
	//** Output list of games
	for (int i=0;i<db.numRows();i++){
		cout << DLM << db[i][0]; //** GameID
		cout << DLM << db[i][1]; //** GameName
		cout << DLM << db[i][2]; //** URL
		cout << DLM << db[i][3]; //** Description
	}
	cout << endl;
	return 0;
}

int profileCommandAddGame(sqltWrap &db, string &userID, vector<string> &userData){
	string queryStr = "SELECT UserLevel FROM HubUsers WHERE UserID = ?";
	int dbResult,
		userLevel;
	
	//** Retrieve user level
	db.prepare(queryStr);
	db.bind(1, userID);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR||Failed to run prepared query [" << dbResult << "]" << endl;
		return 0;
	}
	
	//** Confirm user level is high enough to post games
	userLevel = atol(db[0][0].getData().c_str());
	if (userLevel < 3){
		cout << "ERROR||Invalid user level" << endl;
		return 0;
	}
	
	if (userData.size() < 7){
		cout << "ERROR" << DLM << "Invalid command" << endl;
		return 0;
	}
	
	//** Todo error checking on games
	queryStr = "INSERT INTO Games (PosterID, GameName, MinUsers, MaxUsers, URL, Description) VALUES (?, ?, ?, ?, ?, ?)";
	db.prepare(queryStr);
	db.bind(1, userID);
	db.bind(2, userData[pd_GAMENAME]);
	db.bind(3, userData[pd_MIN_PLAYERS]);
	db.bind(4, userData[pd_MAX_PLAYERS]);
	db.bind(5, userData[pd_URL]);
	db.bind(6, userData[pd_DESC]);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR" << DLM << "Failed to upload new game [" << dbResult << "] " << " \"" << queryStr << "\"";
		cout << "&nbsp;&nbsp;";
		cout << endl;
		return 0;
	}
	
	//** Return command and data signalling success
	cout << COMMAND_ADDGAME << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	string postText = retrievePost(),
		   postStr,
		   queryStr,
		   sendStr = "",
		   tmpName,
		   tmpLevel,
		   tmpBan,
		   tmpMute,
		   tmpID,
		   tmpRegCode,
		   tmpFname,
		   tmpLname,
		   userID,
		   skey;
	int dbResult = 0;
	time_t currentTime = time(0);
	long int profileID = 0,
			 userLevel = 0;
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
			//** Confirm client send id
			if (userData.size() < 3){
				cout << "ERROR" << DLM << "Invalid command" << endl;
				return 0;
			}
	
			profileID = strtol(userData[pd_PROFID].c_str(), NULL, 10);
		} else {
			profileID = strtol(userID.c_str(), NULL, 10);
		}
		
		queryStr = "SELECT Username, UserLevel, BanLevel, MuteLevel, RegistrationCode, FirstName, LastName FROM HubUsers WHERE UserID = ? LIMIT 1";
		
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
		tmpLevel = db[0][1];
		tmpBan = db[0][2];
		tmpMute = db[0][3];
		
		//** Only return a registration code if the user level is high enough
		if (tmpLevel != "1" && tmpLevel != "0"){
			tmpRegCode = db[0][4];
		} else {
			tmpRegCode = "";
		}
		
		tmpFname = db[0][5];
		tmpLname = db[0][6];
		
		//** Return command and data signalling success
		cout << userData[pd_COMMAND] << DLM << tmpName <<
										DLM << tmpLevel <<
										DLM << tmpBan <<
										DLM << tmpMute <<
										DLM << tmpRegCode << 
										DLM << tmpFname <<
										DLM << tmpLname << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_COLOR){
		//** Update user color
		profileCommandColor(db, userID, userData);
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_MYGAMES){
		//** Send user list of games attached to their id
		profileCommandMyGames(db, userID, userData);
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_NEWREG){
		//** Randomize used registration key
		randStr(skey, 20);
		if (setRegKey(db, userID.c_str(), skey.c_str())){
			//** Return command and data signalling success
			cout << COMMAND_NEWREG << DLM << skey << endl;
		}
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_ADDGAME){
		//** Add game to Games table under user's id
		profileCommandAddGame(db, userID, userData);
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_NEWPASS){
		//** Change user's password
		cout << COMMAND_NEWPASS << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_NEWSTATS){
		//** Update users basic info
		cout << COMMAND_NEWSTATS << endl;
		return 0;
	}
	
	//** Default to error
	cout << "ERROR" << DLM << "Invalid command" << endl;
}