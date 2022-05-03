/*
 * dataLogger.cpp
 *
 *  Created on: 2 mai 2022
 *      Author: teamat
 */

#include "dataLogger.h"
#include <iostream>


#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <string.h>
#include <cstdlib>

#include <fstream>
#include  <vector>

using namespace std;

#define logDataType_int         0
   #define logDataType_uInt        1
   #define logDataType_int64       2
   #define logDataType_uint64      3
   #define logDataType_float       4
   #define logDataType_char        5


void datToCsv()
{

	 uint32_t dataStructSize = 0;
	 uint32_t datacount = 0;
	 char inBuffer[5];
	 uint8_t dataTypesTable[50+1];
	 uint8_t testDataTable[50];


	ofstream csvfile;

	ifstream ifile;

	csvfile.open("convertFile.csv");


	ifile.open("outFile.dat", std::ios::binary);
	//std::ifstream bin_file("outFile.dat", std::ios::binary);

//	if (bin_file.good()) {
//		/*Read Binary data using streambuffer iterators.*/
//		std::vector<uint8_t> v_buf((std::istreambuf_iterator<char>(bin_file)), (std::istreambuf_iterator<char>()));
//		//vec_buf = v_buf;
//		bin_file.close();
//
//		v_buf.
//
//	else {
//		throw std::exception();
//	}

	if(ifile.is_open())
	{
		ifile.get(inBuffer, 4+1);//ifile.get((char*)&dataStructSize, 4);
			//            if(readCount >0 ) { // Read data structure size
		dataStructSize = *(uint32_t*)inBuffer;
		ifile.get(inBuffer, 4+1);//ifile.get((char*)&datacount, 4);
		datacount = *(uint32_t*)inBuffer;
	}


	//            	if(readCount) { // Read type data count
	ifile.get((char*)dataTypesTable, datacount+1); // load data types table

	                    for(uint32_t ii = 0 ; ii< datacount; ii++)
	                    {
	                        char titleName[50];
	                    	//string titleName[50];
	                        //int readLen = ifile.getline(titleName, 50);
	                        //int readLen = getline(ifile,titleName);
	                        //if(readLen)
	                        if(ifile.getline(titleName, 50))
	                        {
	                        	int readLen = strlen(titleName);

	                            switch(dataTypesTable[ii])        {
	                                case logDataType_float:
	                                    sprintf(&titleName[readLen-1],"(float),");
	                                    break;

	                                case logDataType_int:
	                                    sprintf(&titleName[readLen-1],"(int),");
	                                    break;

	                                case logDataType_uInt:
	                                    sprintf(&titleName[readLen-1],"(uint),");
	                                    break;

	                                case logDataType_char:
	                                    sprintf(&titleName[readLen-1],"(char),");
	                                    break;

	                                case logDataType_int64:
	                                    sprintf(&titleName[readLen-1],"(int64),");
	                                    break;
	                                case logDataType_uint64:
	                                    sprintf(&titleName[readLen-1],"(uint64),");
	                                    break;

	                                default:
	                                    sprintf(&titleName[readLen-1],"(?),");
	                                    break;
	                            }
	                        }

	                        csvfile.write(titleName,strlen(titleName));
	                        cout << "writing titles";
//	                        sdLogger_serialOut(titleName);
	                    }
//	                    char serialdata[200];
//	                    sprintf(serialdata,"\n\r\tStucture size:%lu, data types: %lu",dataStructSize, datacount);
//	                    debugPrint(serialdata);
//	                    debugPrint("\n\r");
	             //   }
	           // }


	        	csvfile.close();
	        	ifile.close();
}
