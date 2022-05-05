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

#include "dataLogger.h"

HANDLE serialHandle;

const char * device = "\\\\.\\COM5";

#define IN_BUFFER_SIZE 10000
uint8_t inBuffer[IN_BUFFER_SIZE];
string dataOut[256];

#define maxPacketSize 10000

atComm comm(maxPacketSize);

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

	// Envoyer la commande pour �recevoir le data
	write_port(serialHandle, (uint8_t*)"\r", 1); // in case something was typed before
	Sleep(100);

	char outData[] = "downloaddata Data0003.dat\r";

#define DATA_BUFFER_SIZE	200000

	uint8_t dataIn[DATA_BUFFER_SIZE];
	uint32_t dataIndex = 0;

	flushPort(serialHandle);
	write_port(serialHandle, (uint8_t*)outData, sizeof(outData));


	// R�ceptionn des donn�es
	bool fileOpened = false;
	ofstream outfile;

	if(!fileOpened)
	{
		cout << "\n\rData receive, open file";
		fileOpened = true;
		outfile.open("outFile.dat",std::ios::binary);
	}

	//R�ception des paquets
	comm.resetBuffer(); // reset pour le nouveau paquet;

	uint32_t packetNumber = 0;
	uint32_t rxPacketNumber = 0;
	while(dataToReceive)
	{
		//Recevoir Data in indique longueur
		readSize = read_port(serialHandle, inBuffer, IN_BUFFER_SIZE);

		if(readSize > 0)
		{
			for(int i = 0; i < readSize; i++)
			{
				comm.addReceivedBytes(&inBuffer[i], 1); //todo fix le cas ou les bytes d�bordent

				if(comm.packetIsComplete())
				{
					int packetDataCount = comm.getDataCount();
					if(packetDataCount)
					{
						dataInfo_t tempDataInfo;


						for(int packetSelect = 0 ; packetSelect < packetDataCount; packetSelect++)
						{
							// Ajoute le data au buffer.
							comm.getDataInfo(packetSelect, &tempDataInfo);
							if(tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_MESSAGE_COUNT)
							{
								int datalen = comm.getData(packetSelect,&rxPacketNumber,sizeof(rxPacketNumber));

								if(rxPacketNumber != packetNumber)
								{
									cout << "\n\rPacket missing" << packetNumber;
									dataToReceive = false;
									outfile.close();
									fileOpened = false;
								}
							}
							else if (tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_BINARYLOGDATA)
							{
								int datalen = comm.getData(packetSelect, &dataIn[dataIndex], DATA_BUFFER_SIZE - dataIndex);

								if(datalen >= 0)
								{
									dataIndex += datalen;
									packetNumber++;
									write_port(serialHandle, (uint8_t*)"\r", 1); //ACK
									// Regarder si c'est le dernier paquet du groupe de paquets � recevoir.
//									if(comm.getLastPacketStatus())
//									{
//										dataToReceive = false;
//									}
								}
								else
								{
									cout << "\n\rData Length error";
									dataToReceive = false;
									outfile.close();
									fileOpened = false;
								}
							}
							else if (tempDataInfo.dataType == SDLOGGER_COMM_DATA_TYPE_EMPTY)
							{
//								if(comm.getLastPacketStatus())
//								{
//									dataToReceive = false;
//								}
							}
							else
							{
								cout << "\n\rData type error";
								dataToReceive = false;
								outfile.close();
								fileOpened = false;
							}

						}

//						if(comm.getLastPacketStatus())
//						{
//							dataToReceive = false;
//						}

					}
					else
					{
//						//Si le paquet est vide, regarde unituqment les status
//						if(comm.getLastPacketStatus())
//						{
//							dataToReceive = false;
//						}
					}

					// Regarder si c'est le dernier paquet du groupe de paquets � recevoir.
					if(comm.getLastPacketStatus())
					{
						dataToReceive = false;
					}

					comm.resetBuffer(); // reset pour le prochain paquet au besoin;
				}
			}
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


