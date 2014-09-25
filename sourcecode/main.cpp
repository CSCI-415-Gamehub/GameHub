#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "libs/common.hpp"
#include "libs/sqltdb.hpp"
#include "libs/query.hpp"

const int pd_SKEY = 0,
		  pd_COMMAND = 1;
const string COMMAND_GAMELIST = "GAMES",
			 COMMAND_SESSIONLIST = "SESSIONS",
			 COMMAND_POSTGAME = "POST",
			 COMMAND_GAMEUPDATE = "UPDATE",
			 COMMAND_CHANGEUSER = "CHANGEUSER",
			 COMMAND_STARTGAME = "START",
			 COMMAND_JOIN = "JOIN";

using namespace std;

int mainSendGameList(sqltWrap &db, string gameID, string orderBy){
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
	queryStr += "LIMIT 10";
	
	//** Get list of games
	db.prepare(queryStr);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){
		cout << "ERROR" << DLM << "Failed to retrieve games [" << dbResult << "]" << endl;
		return 0;
	}
	
	//** Return command and data signalling success
	cout << COMMAND_GAMELIST << DLM << db.numRows();
	
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

int mainSendSessionList(sqltWrap &db, string gameID, string orderBy){
	string queryStr = "SELECT a.SessionID, b.Username FROM GameSessions a, HubUsers b WHERE UserID ORDER BY CreateTime ASC ";

	//** Build query
	if (orderBy != ""){
		//** Add order to query and pad with whitespace if necessary
		queryStr += "ORDER BY";
		if (orderBy.substr(0,1) != " ") queryStr += " ";
		queryStr += orderBy;
		if (orderBy.substr(orderBy.size()-1,1) != " ") queryStr += " ";
	}
	queryStr += "LIMIT 10";
	
	
}

int main(int argc, char* argv[])
{
	string postText = retrievePost();
	vector<string> postData;
	int dbResult = 0;
	sqltWrap db;
	
	cout << "Content-Type: text/plain\n\n";
	
	//** No data sent
	if (postText == "" || postText == DLM){
		cout << "ERROR" << DLM << "No message" << endl;
		return 0;
	}

	//** Split input into vector
	tokenizeStr(postText, DLM, postData);
	
	//** Connect to DB
	db.open(DIR_DB);
	
	//** Confirm logged in
	dbResult = checkSession(db, postData[pd_SKEY].c_str()) != DB_SUCCESS;
	if (dbResult != DB_SUCCESS){
		cout << "ERROR" << DLM << "Failed to check session id [" << dbResult << "]" << endl;
		return 0;
	}
	if (db.numRows() == 0){
		cout << "ERROR" << DLM << "Not logged in.";
		return 0;
	}
	
	//** Handle commands
	if (postData[pd_COMMAND] == COMMAND_GAMELIST){
		//** Send list of games
		mainSendGameList(db);
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_SESSIONLIST){ //**TODO: Finish this
		//** Send list of sessions for a game
		cout << COMMAND_SESSIONLIST << endl;
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_POSTGAME){
		//** Initialize a game session
		cout << COMMAND_POSTGAME << endl;
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_GAMEUPDATE){
		//** Send list of players in a game and ok message if all slots are taken
		cout << COMMAND_GAMEUPDATE << endl;
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_CHANGEUSER){
		//** Change slot of player in game
		cout << COMMAND_CHANGEUSER << endl;
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_STARTGAME){
		//** Initialize a game session
		cout << COMMAND_STARTGAME << endl;
		return 0;
	} else if (postData[pd_COMMAND] == COMMAND_JOIN){
		//** Initialize a game session
		cout << COMMAND_JOIN << endl;
		return 0;
	}
	
	cout << "ERROR" << DLM << "Invalid command." << endl;
	return 0;
}