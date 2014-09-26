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
		  
const string COMMAND_USERS = "USERS",					//Get list of users +(optional: name)
			COMMAND_TIMEOUT = "TIMEOUT",				//Set new global timeout +(new value)
			COMMAND_USERLEVEL = "USERLEVEL",			//Set user's level +(user id, new userlevel)
			COMMAND_REGLIST = "REGLIST",				//Get list of registration requests
			COMMAND_BANLIST = "BANLIST",				//Get list of banned or muted users
			COMMAND_BANUSER = "SETBAN",					//Set user ban level +(user id)
			COMMAND_MUTEUSER = "SETMUTE",				//Set user mute level +(user id)
			COMMAND_GAMEDELETE = "GAMEDELETE",			//Remove users game +(game id)
			COMMAND_USERDELETE = "USERDELETE";			//Remove user's account +(user id)

int main(int argc, char* argv[])
{
	string postText = retrievePost(),
		   postStr,
		   queryStr,
		   sendStr = "",
		   tmpName,
		   tmpLevel,
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
	if (userData[pd_COMMAND] == COMMAND_USERS){
		//** Get list of all users
		cout << COMMAND_USERS << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_TIMEOUT){
		//** Set global system timeout
		cout << COMMAND_TIMEOUT << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_USERLEVEL){
		//** Set users access level
		cout << COMMAND_USERLEVEL << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_REGLIST){
		//** Get registration request list
		cout << COMMAND_REGLIST << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_BANLIST){
		//** Get list of banned and muted users
		cout << COMMAND_BANLIST << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_BANUSER){
		//** Set users ban level
		cout << COMMAND_BANUSER << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_MUTEUSER){
		//** Set users mute level
		cout << COMMAND_MUTEUSER << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_GAMEDELETE){
		//** Delete a game
		cout << COMMAND_GAMEDELETE << endl;
		return 0;
	} else if (userData[pd_COMMAND] == COMMAND_USERDELETE){
		//** Delete a user
		cout << COMMAND_USERDELETE << endl;
		return 0;
	}
	
	//** Default to error
	cout << "ERROR" << DLM << "Invalid command" << endl;
}