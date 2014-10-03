CREATE TABLE IF NOT EXISTS HubUsers
(
UserID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
FirstName VARCHAR (20),
LastName VARCHAR(20),
Username VARCHAR (20) COLLATE NOCASE,
Password VARCHAR (40),
PasswordSalt VARCHAR (20),
Email VARCHAR (30),
RegistrationCode VARCHAR(20),
RegistrationID INTEGER,
UserLevel INTEGER DEFAULT 1,
BanLevel INTEGER DEFAULT 0,
MuteLevel INTEGER DEFAULT 0,
Color VARCHAR (6),
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

CREATE TABLE IF NOT EXISTS HubMessages
(
Username VARCHAR (20),
Msg VARCHAR (200),
GameField VARCHAR (50),
Color VARCHAR (6),
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

CREATE TABLE IF NOT EXISTS Games
(
GameID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
GameName Varchar(30) COLLATE NOCASE,
PosterID Integer,
MinUsers Integer,
MaxUsers Integer,
URL Varchar(1042),
Description Varchar(1041),
TimesPlayed integer, 
Ratings integer,
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

CREATE TABLE IF NOT EXISTS Sessions
(
SessionID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
SessionKey VARCHAR(20),
UserID INTEGER,
UserLevel INTEGER,
UpdateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL,
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

Create Table IF NOT EXISTS GameSessions
(
SessionID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
GameID INTEGER,
UserID INTEGER,
Password VARCHAR (40),
PasswordSalt VARCHAR (20),
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

CREATE TABLE IF NOT EXISTS RecentlyPlayed
(
GameID INTEGER,
UserID INTEGER,
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

CREATE TABLE IF NOT EXISTS Settings
(
KeyID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
KeyName VARCHAR(20),
KeyValue VARCHAR(40)
);

CREATE TABLE IF NOT EXISTS AdminLog
(
UserID INTEGER,
LogText VARCHAR(300),
CreateTime DATETIME DEFAULT (strftime('%s', 'now')) NOT NULL
);

INSERT INTO HubUsers (Username, Password, PasswordSalt, UserLevel, RegistrationCode) VALUES ('Admin', 'n9/Ui6vcC+AgTwO/CEWuOlZTV3/GT9AGoOhXF1b+iJM=', 'BfTeolZ9EkjJfFafbdAQ', 3, 'hello');
INSERT INTO Settings (KeyName, KeyValue) VALUES ('Timeout', 3600);