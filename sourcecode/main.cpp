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
const string COMMAND_SESSIONLIST = "SESSIONS",
			 COMMAND_POSTGAME = "POST",
			 COMMAND_GAMEUPDATE = "UPDATE",
			 COMMAND_CHANGEUSER = "CHANGEUSER",
			 COMMAND_STARTGAME = "START",
			 COMMAND_JOIN = "JOIN",
			 COMMAND_REGREQUEST = "REGREQUEST";

using namespace std;

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

int mainRegRequest(sqltWrap &db, vector<string> &userData){
	string queryStr = "INSERT INTO -- () VALUES ()";
	int dbResult = 0;
	
	
	return 0;
}

int main(int argc, char* argv[])
{
	string postText = retrievePost();
	vector<string> userData;
	int dbResult = 0;
	sqltWrap db;
	
	cout << "Content-Type: text/plain\n\n";
	
	//** No data sent
	if (postText == "" || postText == DLM){
		cout << "ERROR" << DLM << "No message" << endl;
		return 0;
	}

	//** Split input into vector
	tokenizeStr(postText, DLM, userData);
	
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
	
	//** Handle commands
	if (userData[pd_COMMAND] == COMMAND_SESSIONLIST){ //**TODO: Finish this
		//** Send list of sessions for a game
		cout << COMMAND_SESSIONLIST << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_GAMEUPDATE){
		//** Send list of players in a game and ok message if all slots are taken
		cout << COMMAND_GAMEUPDATE << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_CHANGEUSER){
		//** Change slot of player in game
		cout << COMMAND_CHANGEUSER << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_STARTGAME){
		//** Initialize a game session
		cout << COMMAND_STARTGAME << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_JOIN){
		//** Initialize a game session
		cout << COMMAND_JOIN << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_REGREQUEST){
		//** Add a registration request
		cout << COMMAND_REGREQUEST << endl;
		return 0;
	}
	
	cout << "ERROR" << DLM << "Invalid command." << endl;
	return 0;
}