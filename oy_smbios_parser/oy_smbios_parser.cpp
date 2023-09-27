// oy_smbios_parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "Structures.h"

//typedef unsigned char BYTE;
std::vector<SMBIOSStruct*> structure_table;
BYTE fileData[65535];

int dimm_cnt = 0;

void readFile(const char* filename, BYTE *Data)
{
	// open the file:
	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	// get its size:
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data:
	//std::vector<BYTE> Data(fileSize);
	file.read((char*)&Data[0], 65535);
	//return fileData;
}

SMBIOSStruct* getNextStruct(SMBIOSStruct* curStruct) {
	char* strings_begin = (char*)curStruct + curStruct->Length;
	char* next_strings = strings_begin + 1;
	while (*strings_begin != NULL || *next_strings != NULL) {
		++strings_begin;
		++next_strings;
	}
	return (SMBIOSStruct*)(next_strings + 1);
}

std::vector<std::string> getStrings(SMBIOSStruct* curStruct) {
	std::vector<std::string> strings;
	std::string res = "";
	strings.push_back(res);
	char* cur_char = (char*)curStruct + curStruct->Length;
	SMBIOSStruct* next_struct = getNextStruct(curStruct);

	while (cur_char < (char*)next_struct) {
		res.push_back(*cur_char);
		if (*cur_char == NULL) {
			strings.push_back(res);
			res = "";
		}
		++cur_char;
	}
	return strings;
}

void displayInformationCpu(SMBIOSStructType4* curStruct) {
	std::vector<std::string> strings = getStrings(curStruct);

	/*std::cout << "\tSocket Designation: " << strings[curStruct->SocketDesignation] << std::endl;
	std::cout << "\tProcessor Manufacturer: " << strings[curStruct->ProcessorManufacturer] << std::endl;
	std::cout << "\tProcessor Version: " << strings[curStruct->ProcessorVersion] << std::endl;

	std::cout << "\tSerial Number: " << strings[curStruct->SerialNumber] << std::endl;

	std::cout << "\tPart Number: " << strings[curStruct->PartNumber] << std::endl;*/

	std::cout << std::endl;
}

void displayInformationMem(SMBIOSStructType17* curStruct) {
	std::vector<std::string> strings = getStrings(curStruct);

	/*std::cout << "\tManufacturer: " << strings[curStruct->Manufacturer] << std::endl;

	std::cout << "\tSerial Number: " << strings[curStruct->SerialNumber] << std::endl;
	std::cout << "\tAsset Tag: " << strings[curStruct->AssetTag] << std::endl;
	std::cout << "\tPart Number: " << strings[curStruct->PartNumber] << std::endl;*/

	std::cout << std::endl;
}

void smbios_type4_parse(SMBIOSStruct* curStruct) {
	std::cout << "Processors" << std::endl;

	displayInformationCpu((SMBIOSStructType4*)curStruct);
}

void smbios_type17_parse(SMBIOSStruct* curStruct) {
	std::cout << "Memory Device" << std::endl;

	displayInformationMem((SMBIOSStructType17*)curStruct);
}

int main()
{
	readFile("smbios2", fileData);
	RawSMBIOSData* rawData = (RawSMBIOSData*)fileData;

	SMBIOSStruct* curStruct = (SMBIOSStruct*)rawData->SMBIOSTableData;
	//while ((char*)curStruct < (char*)rawData + rawData->Length) {
	while (dimm_cnt < 31) {
		if (curStruct->Type == 4) {
			smbios_type4_parse(curStruct);
		}
		if ( curStruct->Type == 17 ) {
			smbios_type17_parse(curStruct);
			dimm_cnt++;
		}
		curStruct = getNextStruct(curStruct);
	}
    std::cout << "SMBIOS had been parsed\n";
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
