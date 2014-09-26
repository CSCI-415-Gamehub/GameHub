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
const int pd_SORTBY = 2;
const int pd_GAMENAME = 2,
		  pd_MIN_PLAYERS = 3,
		  pd_MAX_PLAYERS = 4,
		  pd_URL = 5,
		  pd_DESC = 6;

const int DEFAULT_LIMIT = 15;
		  
const string COMMAND_MYGAMES = "MYGAMES",
			COMMAND_ADDGAME = "ADDGAME",
			COMMAND_GAMES = "GAMES",
			COMMAND_REMOVE = "REMOVE";
const string SORTBY_RATINGS = "RATING",
			SORTBY_TIME = "TIME",
			SORTBY_NAME = "NAME";

int gamesSendGameList(sqltWrap &db, string orderBy, string userID, int gLimit){
	string queryStr = "SELECT PosterID, GameID, GameName, URL, Description FROM Games ";
	int dbResult;

	//** Build query
	if (orderBy != ""){
		//** Add order to query and pad with whitespace if necessary
		queryStr += "ORDER BY";
		if (orderBy.substr(0,1) != " ") queryStr += " ";
		queryStr += orderBy;
		if (orderBy.substr(orderBy.size()-1,1) != " ") queryStr += " ";
	}
	queryStr += "LIMIT" + toString(gLimit);
	
	//** Get list of games
	db.prepare(queryStr);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){
		cout << "ERROR" << DLM << "Failed to retrieve games [" << dbResult << "]" << endl;
		return 0;
	}
	
	//** Return command and data signalling success
	cout << COMMAND_GAMES << DLM << db.numRows();
	
	//** Output list of games
	for (int i=0;i<db.numRows();i++){
		cout << DLM << db[i][0]; //** UserID
		cout << DLM << db[i][1]; //** GameID
		cout << DLM << db[i][2]; //** GameName
		cout << DLM << db[i][3]; //** URL
		cout << DLM << db[i][4]; //** Description
	}
	cout << endl;
}
		  
int gamesCommandMyGames(sqltWrap &db, string &userID, vector<string> &userData){
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

int gamesCommandAddGame(sqltWrap &db, string &userID, vector<string> &userData){
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
	
	//** Get games action
	if (userData[pd_COMMAND] == COMMAND_MYGAMES){
		//** Send user list of games attached to their id
		gamesCommandMyGames(db, userID, userData);
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_ADDGAME){
		//** Add game to Games table under user's id
		gamesCommandAddGame(db, userID, userData);
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_GAMES){
		//** Send user list of games
		if (userData.size() < 3){
			//** Default to time
			gamesSendGameList(db, "CreateTime", "", DEFAULT_LIMIT);
		} else {
			if (userData[pd_SORTBY] == SORTBY_RATINGS){
				gamesSendGameList(db, "Ratings", "", DEFAULT_LIMIT);
			} else if (userData[pd_SORTBY] == SORTBY_TIME){
				gamesSendGameList(db, "CreateTime", "", DEFAULT_LIMIT);
			} else if (userData[pd_SORTBY] == SORTBY_NAME){
				gamesSendGameList(db, "GameName", "", DEFAULT_LIMIT);
			} else {
				//** Unrecognised sort, Default to time
				gamesSendGameList(db, "CreateTime", "", DEFAULT_LIMIT);
			}
		}
		return 0;
	}
	
	//** Default to error
	cout << "ERROR" << DLM << "Invalid command" << endl;
}