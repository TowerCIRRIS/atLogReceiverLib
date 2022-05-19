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

#include "TeamAT_atComm/atComm.h"
#include "serialComm.h"

#include <fstream>

#include <chrono>
#include <sys/time.h>
#include <ctime>

using namespace std::chrono;


#include "dataLogger.h"

#include "logReceiver.h"

logReceiver logRx(1500);

HANDLE serialHandle;

const char * device = "\\\\.\\COM5";

#define IN_BUFFER_SIZE 10000
uint8_t inBuffer[IN_BUFFER_SIZE];
string dataOut[256];

#define maxPacketSize 10000

atComm comm(maxPacketSize);

uint64_t getTimeMs()
{
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

int main(void)
{

//	bool dataToReceive = true;
cout << "\n\r ----------------STARTING------------------" << std::endl;


logRx.openComm(5, 460800);

vector<string> fileList;
int fileCount;

cout << "\n\r refreshing file list" << endl;
fileCount = logRx.refreshFileList();
cout << "File count: " << fileCount;

char filename[40];
uint64_t startTime = getTimeMs();

cout << "\nTest downloadDataByName:" << endl;
for(int i = 0; i < fileCount; i++)
{
	logRx.getFileName(i, filename);
	cout << "\nFile#" << i << ": " << filename << std::endl;

	logRx.downloadDataByName(filename);

}

cout << "\nTest downloadDataByNumber:" << endl;
for(int i = 0; i < fileCount; i++)
{
	logRx.downloadDataByNumber(i);
}


cout << "\nTest mauvais nom de fichier:" << endl;
logRx.downloadDataByName("mauvaisFichier.dat");

cout << "\nTest mauvais numérro de fichier:" << endl;
logRx.downloadDataByNumber(28);

cout << "\nTest delete log" << endl;
logRx.deleteAllLog();


Sleep(5000);
cout << "\nTest start log" << endl;
logRx.startLog();

Sleep(5000);

cout << "\nTest stop log" << endl;
logRx.stopLog();
Sleep(1000);

cout << "\nTest start log" << endl;
logRx.startLog();

Sleep(5000);

cout << "\nTest stop log" << endl;
logRx.stopLog();
Sleep(1000);

cout << "\nTest downloadDataByName post log :" << endl;

fileCount = logRx.refreshFileList();

cout << "File count: " << fileCount;

for(int i = 0; i < fileCount; i++)
{
	logRx.getFileName(i, filename);
	cout << "\nFile#" << i << ": " << filename << std::endl;

	logRx.downloadDataByName(filename);

}


logRx.endComm();
uint64_t endTime = getTimeMs();

uint64_t runTime = endTime - startTime;


cout << "\nEnd of program in " << runTime << "ms" << std::endl;

	return 0;
}


