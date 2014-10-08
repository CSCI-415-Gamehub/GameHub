/***********************************************************
infoUpdate.cpp
SUBMITTED BY: Martin
CGI script to handle the updating of username, password, font-color,
first name, and last name. These changes are
made from the profile page.
************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "libs/common.hpp"
#include "libs/query.hpp"
#include "libs/sqltdb.hpp"
#include "libs/randStrHash.hpp"

using namespace std;

/***********************************************************
main
main()

Update forusername, password, font-color,
first name, and last name.
These changes are made from the profile page.
************************************************************/

const int pd_USERNAME = 0,
	  pd_PASSWORD = 1,
	  pd_FIRSTNAME = 2,
	  pd_LASTNAME = 3,
	  pd_EMAIL = 4,
	  pd_REGCODE = 5;

main()
{ string uid, //current username
    	 requestedUid, //tmp storage to change to new username
	 combinedPassword, //current password before hash
	 requestedPassword, //tmp storage to change to new password
	 convertedPassword, //password after hash
	 pwSalt,
	 fontColor,
	 requestedFontColor,
         email,
         fName,
	 requestedFName,
         lName,
	 requestedLName,
         desiredTimeout,
	 postText = retrievePost(),
	 query,
	 skey,
	 inviteID;
  int unameSize = 0,
      dbResult = 0;
      sqltWrap db;
  vector<string> postData;
         
//*** OUTPUT CONTENT TYPE HEADER
cout << "Content-Type: text/plain\n\n";

//** No data sent
if (postText == "" || postText == DLM){
	cout << "ERROR" << DLM << "Invalid data";
	return 0;
}

//** Split input into vector
tokenizeStr(postText, DLM, postData);

//** Invalid data length
if (postData.size() < 6){
	cout << "ERROR" << DLM << "Invalid data";
	return 0;
}

//** Invalid data checking
if (!( is_alnum(postData[pd_USERNAME]) )){
	cout << "ERROR" << DLM << "Username must be alphanumeric.";
	return 0;
}
if (!( postData[pd_USERNAME].length() > 4 || postData[pd_USERNAME].length() < 20 )){
	cout << "ERROR" << DLM << "Username must be between 5 and 20 characters.";
	return 0;
}
if (postData[pd_USERNAME] == "ERROR"){
	cout << "ERROR" << DLM << "Invalid username.";
	return 0;
}
if (!( postData[pd_PASSWORD].length() > 6 || postData[pd_PASSWORD].length() < 40 )){
	cout << "ERROR" << DLM << "Password must be between 6 and 40.";
	return 0;
}
//** Convert username to uppercase for case insensitive matching
string strConvert = postData[pd_PASSWORD],
	   strResult = strConvert;

//** Connect to DB
db.open(DIR_DB);

//** Check if the name is taken
query = "SELECT Username FROM HubUsers WHERE Username = ?";
db.prepare(query);
db.bind(1, postData[pd_USERNAME].c_str());
dbResult = db.runPrepared();
if (dbResult != DB_SUCCESS) {
	cout << "ERROR" << DLM << "Failed to run prepared query [" << dbResult << "]" << strResult << " \"" << query << "\"" << endl;
	return 0;
}
if (db.numRows() > 0){
	cout << "ERROR" << DLM << "Username is already taken." << endl;
	return 0;
}


//** Password conversion
randStr(pwSalt, 20);
combinedPassword = postData[pd_PASSWORD] + pwSalt;
sha256Hash64(convertedPassword, combinedPassword);

//** Updates users changes to table
query = ("UPDATE HubUsers SET Username='%s',Password='%s',Color='%s' WHERE Username='%s';,requestedUid,requestedPassword,requestedColor,");
db.prepare(query);
db.bind(1, postData[pd_USERNAME]);
db.bind(2, convertedPassword);
db.bind(3, pwSalt);
dbResult = db.runPrepared();
if (dbResult != DB_SUCCESS) {
	cout << "ERROR" << DLM << "Failed to run prepared query [" << dbResult << "] " << " \"" << query << "\"" << endl;
	return 0;
}
SET ContactName='Alfred Schmidt', City='Hamburg'
WHERE CustomerName='Alfreds Futterkiste'; 

//** Randomize used registration key
randStr(skey, 20);
query = "UPDATE HubUsers SET RegistrationCode = ? WHERE RegistrationCode = ? LIMIT 1";
db.prepare(query);
	db.bind(1, skey);
db.bind(2, postData[pd_REGCODE].c_str());
dbResult = db.runPrepared();
if (dbResult != DB_SUCCESS) {
	cout << "ERROR" << DLM << "Failed to change registration key [" << dbResult << "] " << " \"" << query << "\"" << endl;
	return 0;
}

}

