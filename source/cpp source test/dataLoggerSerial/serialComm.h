/*
 * serialComm.h
 *
 *  Created on: 28 avr. 2022
 *      Author: teamat
 */

#ifndef SERIALCOMM_H_
#define SERIALCOMM_H_

#include <fstream>
#include <iostream>

#include <windows.h>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

HANDLE open_serial_port(const char * device, uint32_t baud_rate);
int write_port(HANDLE port, uint8_t * buffer, size_t size);
SSIZE_T read_port(HANDLE port, uint8_t * buffer, size_t size);

int flushPort(HANDLE port);


#endif /* SERIALCOMM_H_ */
