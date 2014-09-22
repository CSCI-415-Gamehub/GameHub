/***********************************************************
verify.cpp

CGI script to handle a login
************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "randomStr.hpp"
#include "timeStamp.hpp"

using namespace std;

/***********************************************************
main
main()

Verify that user is still logged in.
************************************************************/

main()
{ string uid,
         receivedSkey,
         storedSkey,
         tStampStr,
         uFilePathAndName,
         pwd;
         
  TimeStamp oldTS,
            newTS;
            
  ifstream ifl;
  ofstream ofl;
  
  bool loginIsGood = true;
         
//*** OUTPUT CONTENT TYPE HEADER
cout << "Content-Type: text/plain\n\n";

//*** GET CURRENT TIME
newTS.setCurrentDateTimeStamp (1);

//*** GET  uid AND skeyFROM CLIENT
getline(cin, uid, '|');
getline(cin, receivedSkey, '|');



//*** CHECK FOR THE USER AND GET PASSWORD IF USER EXISTS
uFilePathAndName = "../../private/users/"; //*** the path part
uFilePathAndName += uid; //*** the path part with the name added
ifl.open(uFilePathAndName.c_str());
if (!ifl) //*** ERROR - RETURN ERROR AND MESSAGE
   {
   cout << "ERROR|UNKNOWN USER OR INCORRECT PASSWORD" << endl;
   exit(0);
   }

//*** GET THE STORED INFO & CLOSE FILE  
getline(ifl, pwd); 
getline(ifl, storedSkey);
getline(ifl, tStampStr);
oldTS.stringToStamp(tStampStr); //*** CONVERT THE STRING BACK TO TIMESTAMP
ifl.close();

//*** OPEN USER'S FILE FOR OUTPUT AND CHECK FOR SUCCESSFUL OPEN
ofl.open(uFilePathAndName.c_str()); 
if (!ofl) //*** THIS SHOULDN'T HAPPEN, BUT CHECK ANYWAY
   {
   cout << "ERROR|SOMETHING VERY STRANGE HAPPENED - PROBABLY NOT YOUR FAULT"
        << endl;
   exit(0);   
   }
   
//*** TEST THE SESSION KEY, IF BAD, SEND ERROR MESSAGE AND QUIT
if (receivedSkey != storedSkey)
   {
   cout << "ERROR|SESSION KEY IS INCORRECT" << endl;
   loginIsGood = false;
   }
else if ((oldTS + 60) < newTS)
   {
   
   //cout << "ERROR|SESSION HAS TIMED OUT" << endl;
   loginIsGood = false;
   
   }
else
   {
   //*** INDICATE OK AND RETURN skey to the user
   cout << "OK|" << storedSkey;
   }

//*** REWRITE THE USER'S FILE
ofl << pwd << endl;
if (loginIsGood) //*** ONLY ADD OTHER STUFF IF LOGIN STILL GOOD
   {
   ofl << storedSkey << endl;
   newTS.stringToStamp(tStampStr); //*** USE NEW TIME STAMP - MOST RECENT ACCESS
   ofl << tStampStr << endl;
   }
   
ofl.close();
}

