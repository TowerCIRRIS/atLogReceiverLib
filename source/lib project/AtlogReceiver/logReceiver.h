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

	/**
	 * Function to open the serial communication port
	 *
	 * @param comPort	the port number
	 * @param baudRate	baudrate of the port
	 *
	 * @return	true on success
	 */
	bool openComm(int comPort, uint32_t baudRate);

	/**
	 * Close serial port
	 */
	void endComm();

	/**
	 * Send \r and read content of the communication buffer to make sur ther is no leftover data
	 *
	 */
	void reset();

	/**
	 * Request list of files on the device log memory
	 *
	 * @return the number of files
	 */
	int refreshFileList();

	/**
	 * Get the number of files <
	 * @note must have run refreshList() before
	 *
	 * @return the number of files
	 */
	int getFileListCount();

	/**
	 * After refreshing the fil list, you can get the name of the files
	 *
	 * @param fileSelect	The number of th file ( 0 to number of files-1)
	 * @param fileName		The buffer that will receive the name
	 * @return				The length of the string
	 */
	int getFileName(int fileSelect,char* fileName);

	/**
	 * Send the "start log" command to the device
	 */
	void startLog();

	/**
	 * Send the "stop log" command to the device
	 */
	void stopLog();

	/**
	 * Erase the log memory on the device. ( Warning all log data will be lost)
	 */
	void deleteAllLog();

	/**
	 * Download a file selected by its number
	 * @param dataNumber	number of the file to read
	 * @return	true on success
	 */
	bool downloadDataByNumber(int dataNumber);

	/**
	 * Download a file selected by its name
	 *
	 * @param filename the name of thje file to download
	 * @return	true on success
	 */
	bool downloadDataByName(const char* filename);

private:

	uint64_t getTimeMs();

	uint32_t mBaudRate;

	uint8_t *mInBuffer;
	uint8_t *mDataIn;
	uint32_t dataIndex = 0;
	uint32_t mMaxPacketLen = 0;
	uint32_t mMaxDataBuffer = 0;
	atComm *comm;

	HANDLE serialHandle;

	vector<string> mFileList;

};

#endif /* LOGRECEIVER_H_ */
