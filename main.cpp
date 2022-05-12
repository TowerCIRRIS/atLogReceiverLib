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


logRx.openComm("\\\\.\\COM5", 460800);

vector<string> fileList;
int fileCount = logRx.refreshFileList();

char filename[40];

for(int i = 0; i < fileCount; i++)
{
	logRx.getFileName(i, filename);
	cout << "\nFile#" << i << ": " << filename << std::endl;

	logRx.downLoadData(filename);

	//logRx.reset();

}



logRx.endComm();

cout << "\nEnd of program" << std::endl;

	return 0;
}


