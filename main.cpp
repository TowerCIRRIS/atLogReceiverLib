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

#define IN_BUFFER_SIZE 512
uint8_t inBuffer[IN_BUFFER_SIZE];
string dataOut[256];

#define maxPacketSize 1000

atComm comm(maxPacketSize);

int main(void)
{

	bool running = true;
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
	SSIZE_T dataCount;


	comm.resetBuffer();

	write_port(serialHandle, (uint8_t*)"\r", 1); // in case something was typed before
	Sleep(100);

	char outData[] = "downloaddata Data0002.dat\r";

	uint8_t dataIn[20000];
	uint32_t dataIndex = 0;

	flushPort(serialHandle);
	write_port(serialHandle, (uint8_t*)outData, sizeof(outData));

	bool fileOpened = false;
	ofstream outfile;




	while(running)
	{
		dataCount = read_port(serialHandle, inBuffer, IN_BUFFER_SIZE);
		//flushPort(serialHandle);
		if(dataCount > 0)
		{

			for(int i = 0; i < dataCount; i++)
			{

				comm.addReceivedBytes(&inBuffer[i], 1);

				if(comm.dataAvailable())
				{
					if(!fileOpened)
					{
						cout << "\n\rData receive, open file";
						fileOpened = true;
						outfile.open("outFile.dat",std::ios::binary);
					}
					dataInfo_t tempDataInfo;
					comm.getDataInfo(0, &tempDataInfo);

					int datalen = comm.getData(tempDataInfo, &dataIn[dataIndex], 20000-dataIndex);
					if(datalen >= 0)
					{
						dataIndex += datalen;

						if(comm.getLastPacketStatus())
						{
							cout << "\n\rLast packet received";
							running = false;
						}


					}
					else
					{
						cout << "\n\rData Length error";
					}

					comm.resetBuffer(); // reset for next iteration;
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

			datToCsv();

	}




	CloseHandle(serialHandle);


	return 0;
}


