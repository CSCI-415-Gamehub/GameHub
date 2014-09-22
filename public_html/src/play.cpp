#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include "libs/common.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Content-Type: text/html\n\n";
	cout << "<html><body>";
	cout << "You're now playing the game.";
	cout << "</body></html>" << endl;
	
	return 0;
}