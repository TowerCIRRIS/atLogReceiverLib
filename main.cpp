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

	bool dataToReceive = true;
	cout << "\n\r ----------------STARTING------------------";

	serialHandle = open_serial_port(device, 460800);//CreateFile("COM6", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	    if(serialHandle == NULL)
	    {
	        std::cout << "Error opening port ";
	        return 1;
	    }
	    else
	    {
	        if (serialHandle == INVALID_HANDLE_VALUE) {
	            std::cout << "Error opening port ";
	            return 1;
	        }
	        else
	        {
	            std::cout << "Success:  port created\n\r\r ";
	        }
	    }

	    Sleep(2000);
	SSIZE_T readSize;


	comm.resetBuffer();

	// Envoyer la commande pour ¸recevoir le data
	flushPort(serialHandle);
	write_port(serialHandle, (uint8_t*)"\r", 1); // in case something was typed before
	Sleep(100);
	write_port(serialHandle, (uint8_t*)"\r", 1); // in case something was typed before
	Sleep(100);

	char outData[] = "downloaddata Data0003.dat\r";

#define DATA_BUFFER_SIZE	200000

	uint8_t dataIn[DATA_BUFFER_SIZE];
	uint32_t dataIndex = 0;

	flushPort(serialHandle);
	write_port(serialHandle, (uint8_t*)outData, sizeof(outData));


	// Réceptionn des données
	bool fileOpened = false;
	ofstream outfile;

	if(!fileOpened)
	{
		cout << "\n\rData receive, open file";
		fileOpened = true;
		outfile.open("outFile.dat",std::ios::binary);
	}

	//Réception des paquets
	comm.resetBuffer(); // reset pour le nouveau paquet;

#define MAX_RETRY 3
	uint32_t packetNumber = 0;
	uint32_t rxPacketNumber = 0;
	uint32_t retryCount = 0;

	uint64_t startTimeMs = getTimeMs();

	while(dataToReceive)
	{
		//Recevoir Data in indique longueur
		readSize = read_port(serialHandle, inBuffer, IN_BUFFER_SIZE);

		if(readSize > 0)
		{
			startTimeMs = getTimeMs();
			for(int i = 0; i < readSize; i++)
			{
				int status = comm.addReceivedBytes(&inBuffer[i], 1); //todo fix le cas ou les bytes débordent

				if (status < 0) {
					cout << "\n\rRx error getting bytes: " << status;
					comm.resetBuffer();
					retryCount++;

					if(retryCount >= MAX_RETRY)
					{
						cout << "\n\rMax retry reached, aborting.. Error: " << status;
						dataToReceive = false;
						outfile.close();
						fileOpened = false;
					}

					break; // exit for

				} else {
					if (comm.packetIsComplete()) {
						retryCount = 0; // Valid message received, clear retry counter for next errors
						int packetDataCount = comm.getDataCount();
						if (packetDataCount) {
							dataInfo_t tempDataInfo;

							bool sendAck = false;
							for (int packetSelect = 0; packetSelect < packetDataCount; packetSelect++) {

								// Ajoute le data au buffer.
								comm.getDataInfo(packetSelect, &tempDataInfo);

								if (tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_MESSAGE_COUNT) {
									int datalen = comm.getData(packetSelect, &rxPacketNumber, sizeof(rxPacketNumber));

									if (rxPacketNumber != packetNumber) {
										cout << "\n\rPacket missmatch, aborting.. packet number:" << packetNumber;
										dataToReceive = false;
										outfile.close();
										fileOpened = false;
									}
									sendAck = true;

								} else if (tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_BINARYLOGDATA) {
									int datalen = comm.getData(packetSelect, &dataIn[dataIndex],
									DATA_BUFFER_SIZE - dataIndex);

									if (datalen >= 0) {
										dataIndex += datalen;
										packetNumber++;


									} else {
										cout << "\n\rData Length error, aborting..";
										dataToReceive = false;
										outfile.close();
										fileOpened = false;
									}
								} else if (tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_EMPTY) {

								} else {
									cout << "\n\rData type error, aborting..";
									dataToReceive = false;
									outfile.close();
									fileOpened = false;
								}
							}

							if(sendAck)
							{
								write_port(serialHandle, (uint8_t*) "\r", 1); //ACK
							}


						} else {

						}

						// Regarder si c'est le dernier paquet du groupe de paquets à recevoir.
						if (comm.getLastPacketStatus()) {
							dataToReceive = false;
						}

						comm.resetBuffer(); // reset pour le prochain paquet au besoin;
					}
				}

			}
		}
		else
		{
//			if(getTimeMs() - startTimeMs > 3000)
//			{
//				cout << "\n\rData reception timeout, aborting..";
//				dataToReceive = false;
//				outfile.close();
//				fileOpened = false;
//			}
		}
	}

	if(fileOpened)
	{
			//cout << ((char*)dataIn)[i];
			cout << "\n\r Writing file";
			outfile.write((char*)dataIn,dataIndex);
			outfile.close();
			Sleep(100);

			datToCsv("outFile.dat","convertFile.csv",false);
	}


	CloseHandle(serialHandle);


	return 0;
}


