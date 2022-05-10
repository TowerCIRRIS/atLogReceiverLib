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


void datToCsv(const char* inputFile, const char* outputFile, bool showTypes)
{

	 uint32_t dataStructSize = 0;
	 uint32_t datacount = 0;
	 char inBuffer[1000];
	 char outBuffer[1000];
	 uint8_t dataTypesTable[50+1];
	 uint8_t testDataTable[50];

	ofstream csvfile;

	ifstream ifile;

	ifile.open(inputFile, std::ios::binary);

	if(ifile.is_open())
	{
		csvfile.open(outputFile);

		// Entête du fichier
			//Grosseur de la structure de données
			ifile.read(inBuffer, 4);
			dataStructSize = *(uint32_t*)inBuffer;

			// Nombre de données dans la structure
			ifile.read(inBuffer, 4);
			datacount = *(uint32_t*)inBuffer;

			// Table des types de données
			ifile.read((char*)dataTypesTable, datacount);

			// Extraction des titres de colonnes / nom des données
			char titleName[50];
			for(uint32_t ii = 0 ; ii < datacount; ii++)
			{
				if(ifile.getline(titleName, 50))
				{
					int readLen = ifile.gcount(); // strlen(titleName);

					// Ajout du type entre parenthèse à coté du titre (optionnel)
					if(showTypes)
					{
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
					else
					{
						sprintf(&titleName[readLen-1],",");
					}

				}

				csvfile.write(titleName,strlen(titleName));

			}

		titleName[0] = '\n';
		titleName[1] = '\0';

		csvfile.write(titleName,1);

		int lineCount = 0;
		bool datatoReadinFile = true;
		while(datatoReadinFile)
		{
			ifile.read(inBuffer, dataStructSize);
			int readChar = ifile.gcount();
			if(readChar > 0){

				int datalen = binToCharDataConvert((uint8_t*)inBuffer, outBuffer, dataTypesTable, datacount);
				outBuffer[datalen] = '\n';
				datalen++;
				outBuffer[datalen] = '\0';
				csvfile.write(outBuffer,datalen);
				lineCount++;

			}
			else
			{
				datatoReadinFile = false;
			}
		}

		cout << "Final line count:" << lineCount << std::endl;

		csvfile.close();
		ifile.close();

	}

}


uint32_t binToCharDataConvert(const uint8_t* binData, char* charData, uint8_t* dataTypesTable, uint32_t dataTypeCount)
{
    uint32_t dataPtr = 0;
	uint32_t targetPtr = 0;

    uint32_t uData;
    float  fData;
    int    intData;
    char cData;
    uint32_t nbCarWritten = 0;
    int64_t i64Data;
    uint64_t u64Data;

	for(uint32_t i = 0 ; i < dataTypeCount; i++)
	{
        switch(dataTypesTable[i])
        {
            case logDataType_float:
                fData = *(float*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%f,", fData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(float);
                break;

            case logDataType_int:
                intData = *(int*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%d,", intData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(int);
                break;

            case logDataType_uInt:
                uData = *(uint32_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%u,", uData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(uint32_t);
                break;

            case logDataType_char:
                cData = *(char*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%c,", cData);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(char);
                break;

            case logDataType_int64:
                i64Data = *(int64_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%lld,", i64Data);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(int64_t);
                break;
            case logDataType_uint64:
                u64Data = *(uint64_t*)&binData[dataPtr];
                nbCarWritten = sprintf(&charData[targetPtr],"%llu,", u64Data);
                targetPtr += nbCarWritten;
                dataPtr+= sizeof(uint64_t);
                break;


            default:
                break;
        }

	}

    return targetPtr;
}
