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

int createSession(sqltWrap &db, string &UserID, string &skey) {
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
	string query = "SELECT UserID FROM Sessions WHERE SessionKey = ?";
	
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	
	db.bind(1, skey);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }

	return dbResult;
}

int checkSession(sqltWrap &db, const char *skey, const char *extraVals){
	int dbResult,
		ret = -1;
	string eVals = extraVals,
		   query = "SELECT UserID" + eVals + " FROM Sessions WHERE SessionKey = ?";
	
	dbResult = db.prepare(query);
	if (dbResult != DB_SUCCESS){ return dbResult; }
	db.bind(1, skey);
	dbResult = db.runPrepared();
	if (dbResult != DB_SUCCESS){ return dbResult; }

	return dbResult;
}

#endif /* QUERY_HPP_ */