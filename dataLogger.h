/*
 * dataLogger.h
 *
 *  Created on: 2 mai 2022
 *      Author: teamat
 */

#ifndef DATALOGGER_H_
#define DATALOGGER_H_

#include <stdint.h>
void datToCsv();

uint32_t binToCharDataConvert(const uint8_t* binData, char* charData, uint8_t* dataTypesTable, uint32_t dataTypeCount);

#endif /* DATALOGGER_H_ */
