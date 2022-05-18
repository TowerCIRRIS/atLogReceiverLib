/*
 * logReceiver.h
 *
 *  Created on: 9 mai 2022
 *      Author: teamat
 */

#ifndef LOGRECEIVER_H_
#define LOGRECEIVER_H_

#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <fstream>
#include <vector>
using namespace std;


#include "TeamAT_atComm/atComm.h"

#define MAX_RETRY 3


class logReceiver {
public:
	logReceiver(uint32_t maxPacketLenght, uint32_t maxDataBuffer = 200000);
	virtual ~logReceiver();

	bool openComm(int comPort, uint32_t baudRate);
	void endComm();

	bool reset();

	int refreshFileList();
	int getFileListCount();
	int getFileName(int fileSelect,char* fileName);

	bool downloadDataByNumber(int dataNumber);
	bool downloadDataByName(const char* filename);

private:

	vector<string> fileList;

	uint64_t getTimeMs();

	uint32_t mBaudRate;

	uint8_t *mInBuffer;
	uint8_t *mDataIn;
	uint32_t dataIndex = 0;
	uint32_t mMaxPacketLen = 0;
	uint32_t mMaxDataBuffer = 0;
	atComm *comm;

	bool mDirectFileWrite = false;

	HANDLE serialHandle;

	vector<string> mFileList;
	//int mFileCount = -1;

};

#endif /* LOGRECEIVER_H_ */
