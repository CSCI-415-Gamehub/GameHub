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
	
	//** Check count
	if (db.numRows() < 1){
		cout << "ERROR" << DLM << "User does not exist." << endl;
		return 0;
	}

	//** Split input into vector
	tokenizeStr(postText, DLM, postData);

	cout << "Content-Type: text/plain\n\n";
	
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
	
	//** Fix lack of
	if (db.numRows() < 2){
		cout << "ERROR" << DLM << "User does not exist." << endl;
		return 0;
	}
	
	//** Handle commands
	if (postData[pd_COMMAND] == COMMAND_GAMELIST){
		//** Send list of games
	} else if (postData[pd_COMMAND] == COMMAND_SESSIONLIST){
		//** Send list of sessions for a game
	} else if (postData[pd_COMMAND] == COMMAND_STARTGAME){
		//** Initialize a game session
	} else if (postData[pd_COMMAND] == COMMAND_GAMEUPDATE){
		//** Send list of players in a game and ok message if all slots are taken
	} else if (postData[pd_COMMAND] == COMMAND_CHANGEUSER){
		//** Change slot of player in game
	} else {
		cout << "ERROR" << DLM << "Invalid command." << endl;
	}
	
	return 0;
}