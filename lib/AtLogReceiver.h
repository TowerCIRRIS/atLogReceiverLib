/*
 * AtLogReceiver.h
 *
 *  Created on: 12 mai 2022
 *      Author: teamat
 */

#ifndef ATLOGRECEIVER_H_
#define ATLOGRECEIVER_H_

#include "stdbool.h"
#include "stdint.h"

//#ifdef ATLOGRECEIVER_EXPORTS

//#else
//  #define ATLOGRECEIVER_API __declspec(dllimport)
//#endif

#ifdef __cplusplus
extern "C"{
#endif


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
	 * Get the number of files
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
	 * Send the "stop log" command to the device
	 */
	void deleteAllLog();

	/**
	 * Download a file selected by its name
	 *
	 * @param filename the name of thje file to download
	 * @return	true on success
	 */
	bool downloadDataByName(const char* filename);

	/**
	 * Download a file selected by its number
	 * @param dataNumber	number of the file to read
	 * @return	true on success
	 */
	bool downloadDataByNumber(int dataNumber);

#ifdef __cplusplus
}
#endif

#endif /* ATLOGRECEIVER_H_ */
