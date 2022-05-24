//============================================================================
// Name        : AtlogReceiverLibTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * main.cpp
 *
 *  Created on: 28 avr. 2022
 *      Author: teamat
 */

#include <iostream>
using namespace std;

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <cstdlib>


#include "AtLogReceiver.h"



int main(void)
{

//	bool dataToReceive = true;
cout << "\n\r ----------------STARTING------------------" << std::endl;


openComm(5, 460800);

//vector<string> fileList;
int fileCount = refreshFileList();
cout << "File count: " << fileCount << std::endl;
if(fileCount)
{
	char filename[40];

	for(int i = 0; i < fileCount; i++)
	{
		getFileName(i, filename);
		cout << "\nFile#" << i << ": " << filename << std::endl;

		downloadDataByName(filename);
	}
}

startLog();

Sleep(3000);

stopLog();
Sleep(100);

fileCount = refreshFileList();
cout << "File count: " << fileCount << std::endl;
if(fileCount)
{
	char filename[40];

	for(int i = 0; i < fileCount; i++)
	{
		getFileName(i, filename);
		cout << "\nFile#" << i << ": " << filename << std::endl;

		downloadDataByNumber(i);
	}
}


//// test fichier inexistant
//downloadDataByName("Data0029.dat");

// Erae logs on sd card
//deleteAllLog();


endComm();

cout << "\nEnd of program" << std::endl;

	return 0;
}


