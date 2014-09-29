CREATE TABLE HubUsers
(UserID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
FirstName VARCHAR (20),
LastName VARCHAR(20),
Username VARCHAR (20) COLLATE NOCASE,
Password VARCHAR (20),
PasswordSalt VARCHAR (20),
Email VARCHAR (30),
RegistrationCode INTEGER,
RegistrationID INTEGER,
InviteID VARCHAR (20),
UserLevel VARCHAR (20),
BanLevel INTEGER,
MuteLevel INTEGER,
Color VARCHAR (6),
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

CREATE TABLE HubMessages
(Username VARCHAR (20),
Msg VARCHAR (200),
GameField VARCHAR (50),
Color VARCHAR (6),
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

CREATE TABLE Games
(
GameID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
GameName Varchar(30) COLLATE NOCASE,
GameAddress Varchar(30),
PosterID Integer,
MinUsers Integer,
MaxUsers Integer,
URL Varchar(1042),
Description Varchar(1041),
TimesPlayed integer, 
Ratings integer,
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

Create Table Sessions 
(
SessionID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
GameID INTEGER,
UserID INTEGER,
UserLevel INTEGER,
UpdateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL,
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

Create Table GameSessions 
(
SessionID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
GameID INTEGER,
UserID INTEGER,
Password VARCHAR (20),
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

CREATE TABLE RecentlyPlayed
(
GameID INTEGER,
UserID INTEGER,
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);

CREATE TABLE Settings
(
KeyID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
KeyName VARCHAR(10),
KeyValue VARCHAR(10)
);

CREATE TABLE AdminLog
(
UserID INTEGER,
LogText VARCHAR(300),
CreateTime DATETIME DEFAULT strftime('%s', 'now') NOT NULL
);