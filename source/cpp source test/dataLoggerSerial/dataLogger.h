/*
 * dataLogger.h
 *
 *  Created on: 2 mai 2022
 *      Author: teamat
 */

#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include <stdint.h>
#include <string>

using namespace std;

void datToCsv(const char* inputFile, const char* outputFile, bool showTypes);

uint32_t binToCharDataConvert(const uint8_t* binData, char* charData, uint8_t* dataTypesTable, uint32_t dataTypeCount);


#define SDLOGGER_COMM_BUFFER_SIZE				1042
#define SDLOGGER_COMM_DATA_TYPE_EMPTY			0x00
#define SDLOGGER_COMM_DATA_TYPE_BINARYLOGDATA	0x01
#define SDLOGGER_COMM_DATA_TYPE_MESSAGE_COUNT	0x02


#endif /* DATALOGGER_H_ */
