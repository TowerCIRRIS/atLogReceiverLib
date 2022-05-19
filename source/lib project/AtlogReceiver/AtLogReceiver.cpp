/*
 * AtLogReceiver.cpp
 *
 *  Created on: 12 mai 2022
 *      Author: teamat
 */


#include "AtLogReceiver.h"
#include "logReceiver.h"
#include "iostream"

#define DLLEXPORT  extern "C" __declspec(dllexport)

logReceiver logRx(1500);

DLLEXPORT bool openComm(int comPort, uint32_t baudRate)
{

	return logRx.openComm(comPort, baudRate);
}

DLLEXPORT void endComm()
{
	logRx.endComm();
}

DLLEXPORT void reset()
{
	logRx.reset();
}

DLLEXPORT int refreshFileList()
{
	return logRx.refreshFileList();
}

DLLEXPORT int getFileListCount()
{
	return logRx.getFileListCount();
}
DLLEXPORT int getFileName(int fileSelect,char* fileName)
{
	return logRx.getFileName(fileSelect, fileName);
}

DLLEXPORT bool downloadDataByName(const char* filename)
{

	return logRx.downloadDataByName(filename);
}

DLLEXPORT bool downloadDataByNumber(int dataNumber)
{

	return logRx.downloadDataByNumber(dataNumber);
}

DLLEXPORT void startLog()
{
	logRx.startLog();
}

DLLEXPORT void stopLog()
{
	return logRx.stopLog();
}

DLLEXPORT void deleteAllLog()
{
	return logRx.deleteAllLog();
}


