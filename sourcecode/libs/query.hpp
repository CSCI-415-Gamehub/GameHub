#include <stdlib.h>

#include "sqltdb.hpp"

using namespace std;

#ifndef QUERY_HPP_
#define QUERY_HPP_

int clearOldSessions(sqltWrap &db, string &UserID) {
	int dbResult;
	string query = "DELETE FROM Sessions WHERE UserID = ?";
	
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	
	db.bind(1, UserID);
	
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }
	return dbResult;
}

int createSession(sqltWrap &db, string &UserID, string &UserLevel, string &skey) {
	int dbResult;
	string query = "INSERT INTO Sessions (UserID, SessionKey) VALUES (?, ?)";
		   
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	
	db.bind(1, UserID);
	db.bind(2, skey);
	
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }
	return dbResult;
}

int checkSession(sqltWrap &db, const char *skey){
	int dbResult,
		ret = -1;
	string query = "SELECT UserID, UserLevel FROM Sessions WHERE SessionKey = ?";
	
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	
	db.bind(1, skey);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }

	return dbResult;
}

int checkSession(sqltWrap &db, const char *skey, const char *extraVals){
	int dbResult;
	string eVals = extraVals,
		   query = "SELECT UserID, UserLevel" + eVals + " FROM Sessions WHERE SessionKey = ?";
	
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	db.bind(1, skey);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }

	return dbResult;
}

bool setRegKey(sqltWrap &db, const char *skey, const char *userID){
	string queryStr = "UPDATE HubUsers SET RegistrationCode = ? WHERE UserID = ? AND UserLevel > 1 LIMIT 1";
	int dbResult;
	
	//** Change user registration key
	db.prepare(queryStr);
	db.bind(1, skey);
	db.bind(2, userID);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS) {
		cout << "ERROR" << DLM << "Failed to change registration key [" << dbResult << "] " << " \"" << queryStr << "\"" << endl;
		return false;
	}
	return true;
}

#endif /* QUERY_HPP_ */