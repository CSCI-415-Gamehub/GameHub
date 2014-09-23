CC = g++
DIR = ../public_html/cgi-bin/
TAGS = -lcrypto++ -lpthread

# Build All
all: common.o sqltdb.o randStrHash.o login.cgi logout.cgi register.cgi main.cgi chatsend.cgi chatupdate.cgi profile.cgi play.cgi

login.cgi: login.cpp sqltdb.o randStrHash.o common.o
	$(CC) -std=c++11 -o $(DIR)login.cgi login.cpp sqltdb.o randStrHash.o common.o -lsqlite3 $(TAGS)

logout.cgi: logout.cpp sqltdb.o common.o
	$(CC) -std=c++11 -o $(DIR)logout.cgi logout.cpp common.o sqltdb.o -lsqlite3
	
register.cgi: register.cpp sqltdb.o randStrHash.o common.o
	$(CC) -std=c++11 -o $(DIR)register.cgi register.cpp sqltdb.o randStrHash.o common.o -lsqlite3 $(TAGS)

main.cgi: main.cpp sqltdb.o common.o
	$(CC) -std=c++11 -o $(DIR)main.cgi main.cpp sqltdb.o common.o -lsqlite3
	
chatsend.cgi: chatsend.cpp sqltdb.o common.o
	$(CC) -std=c++11 -o $(DIR)chatsend.cgi chatsend.cpp sqltdb.o common.o -lsqlite3
	
chatupdate.cgi: chatupdate.cpp sqltdb.o common.o
	$(CC) -std=c++11 -o $(DIR)chatupdate.cgi chatupdate.cpp sqltdb.o common.o -lsqlite3
	
profile.cgi: profile.cpp sqltdb.o common.o
	$(CC) -std=c++11 -o $(DIR)profile.cgi profile.cpp sqltdb.o common.o -lsqlite3

play.cgi: play.cpp
	$(CC) -std=c++11 -o $(DIR)play.cgi play.cpp

# Clean
clean:
	rm -f *.o
	
# Base modules
common.o: libs/common.cpp
	$(CC) -c libs/common.cpp
	
sqltdb.o: libs/sqltdb.cpp
	$(CC) -c libs/sqltdb.cpp -lsqlite3

randStrHash.o: libs/randStrHash.cpp
	$(CC) -c libs/randStrHash.cpp -lcrypto++ -lpthread
	
dateTime.o: libs/dateTime.cpp
	$(CC) -c libs/dateTime.cpp