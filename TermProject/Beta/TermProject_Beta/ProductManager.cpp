//
//  ProductManager.cpp
//  TermProject_Alpha
//
//  Created by Sam Jentsch on 11/3/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#include "ProductManager.h"

#pragma mark - Constructor
ProductManager::ProductManager(std::fstream &binaryRecordFile)
:binaryRecordFile(binaryRecordFile)
{
    
}


#pragma mark - Read From File
DataRecord ProductManager::getDataRecordAtOffset(int offset) {
    binaryRecordFile.seekg(0);
    binaryRecordFile.seekg(offset * sizeof(DataRecord));
    
    DataRecord record;
    binaryRecordFile.read(reinterpret_cast<char*>(&record), sizeof(DataRecord));
    
    return record;
}

void ProductManager::traverseFile() {
    binaryRecordFile.seekg(0);
    
    DataRecord header;
    std::cout<<"TRAVERSE FILE: " << "\n";
    int numberOfRecords = getNumberOfRecords();
    std::cout << "NUMBER OF RECORDS: " << numberOfRecords <<"\n";
    DataRecord record;
    for (int i = 0; i < numberOfRecords; i++) {
        //std::cout << "READING: ";
        binaryRecordFile.read(reinterpret_cast< char * >(&record), sizeof(DataRecord));
        record.printRecord();
    }
}

int ProductManager::getNumberOfRecords() {
    DataRecord header;
    
    binaryRecordFile.seekg(0);
    binaryRecordFile.read(reinterpret_cast< char * >(&header), sizeof(DataRecord));
    
    int numberOfRecords = header.getKey();
    
    headerNumber = numberOfRecords;
    //std::cout << "SETTING HEADER NUMBER TO: " <<headerNumber << std::endl;
    return numberOfRecords;
}

#pragma mark - Write To File

void ProductManager::createBinaryRecordFile(std::ifstream &textFile) {
    int lines = 0;
    std::string dataLine;
    
    binaryRecordFile.seekp(0);
    
    DataRecord newRecord;
    while (getline(textFile, dataLine)) {
        //Create duplicate binary file for seeking:
        newRecord = getDataRecordForString(dataLine);
        writeDataRecordToBinaryFile(newRecord, lines);
        lines++;
    }
}

void ProductManager::writeDataRecordToBinaryFile(DataRecord newRecord, int line) {
    //--------------------------------------------------------------------------------//
    //Duplicate the text record to a fixed field and write the record to the binary
    //file.
    //Preconditions: record string and line to write to passed as parameters.
    //Postconditions: record string converted to data record and written to file.
    //--------------------------------------------------------------------------------//
    //DataRecord newRecord = getDataRecordForString(record);
    std::cout << "INSERTING RECORD: ";
    newRecord.printRecord();
    
    int recordOffset = sizeof(DataRecord) * line;
    
    binaryRecordFile.seekp(recordOffset);
    binaryRecordFile.write(reinterpret_cast< char * >(&newRecord), sizeof(DataRecord));
}

void ProductManager::updateHeader(int numberOfRecords) {
    DataRecord header;
    header.setKey(++headerNumber);
    std::cout<<"Setting header to: " << header.getKey() << " Header Number is: " << headerNumber << "\n";
    binaryRecordFile.seekp(0);
    binaryRecordFile.write(reinterpret_cast< char * >(&header), sizeof(DataRecord));
}

int ProductManager::addDataRecordToBinaryFile(DataRecord record) {
    int RRN = getNumberOfRecords() + 1;
    writeDataRecordToBinaryFile(record, RRN);
    updateHeader(getNumberOfRecords()+1);
    return RRN;
}

void ProductManager::deleteRecordAtOffset(int offset) {
    binaryRecordFile.seekg(offset * sizeof(DataRecord));
    DataRecord deleteRecord;
    binaryRecordFile.read(reinterpret_cast< char * >(&deleteRecord), sizeof(DataRecord));
    binaryRecordFile.seekp(offset * sizeof(DataRecord));
    deleteRecord.setKey(-1);
    binaryRecordFile.write(reinterpret_cast< char * >(&deleteRecord), sizeof(DataRecord));
}

#pragma mark - Search

DataRecord ProductManager::searchFileForCost(double cost) {
    binaryRecordFile.seekg(0);
    
    bool found = false;
    DataRecord header;
    int numberOfRecords = getNumberOfRecords();
    DataRecord record;
    for (int i = 0; i < numberOfRecords && !found; i++) {
        //std::cout << "READING: ";
        binaryRecordFile.read(reinterpret_cast< char * >(&record), sizeof(DataRecord));
        if (record.getCost() == cost) {
            found = true;
        }
    }
    
    if (!found) {
        record.setKey(-1);
    }
    
    return record;
}

DataRecord ProductManager::searchFileForDescription(std::string description) {
    binaryRecordFile.seekg(0);
    
    bool found = false;
    DataRecord header;
    int numberOfRecords = getNumberOfRecords();
    DataRecord record;
    for (int i = 0; i < numberOfRecords && !found; i++) {
        //std::cout << "READING: ";
        binaryRecordFile.read(reinterpret_cast< char * >(&record), sizeof(DataRecord));
        //std::cout << "COMPARING: " << description << " & " << record.getName() << std::endl;
        if (description.compare(record.getName()) == 0) {
            //std::cout << "MATCH" << std::endl;
            found = true;
        }
    }
    
    if (!found) {
        record.setKey(-1);
    }
    
    return record;
}

#pragma mark - Conversion and Split Methods

DataRecord ProductManager::getDataRecordForString(std::string record) {
    //Get the record elements split by space.
    std::vector<std::string> splitRecord;
    splitRecord = split(record, ' ', splitRecord);
    
    //splitRecord[0] = key
    //splitRecord[1] = name
    //splitRecord[2] = code
    //splitRecord[3] = cost
    
    int key = convertStringToInt(splitRecord.at(0));
    const char *name = splitRecord.at(1).c_str();
    int code = convertStringToInt(splitRecord.at(2));
    double cost = convertStringToDouble(splitRecord.at(3));
    
    DataRecord newRecord(key, name, code, cost);
    
    return newRecord;
}

int ProductManager::convertStringToInt(std::string intString) {
    //--------------------------------------------------------------------------------//
    //Convert the string passed into an integer and return the integer value.
    //Preconditions: string representation of an integer passed as parameter.
    //Postconditions: the string passed is parsed and the integer value of the string
    //is returned to the caller.
    //--------------------------------------------------------------------------------//
    int convertedInteger;
    
    std::istringstream converter(intString);
    converter >> convertedInteger;
    
    return convertedInteger;
}

double ProductManager::convertStringToDouble(std::string doubleString) {
    //--------------------------------------------------------------------------------//
    //Convert the string passed into a double and return the double value.
    //Preconditions: string representation of a double passed as parameter.
    //Postconditions: the string passed is parsed and the double value of the string
    //is returned to the caller.
    //--------------------------------------------------------------------------------//
    double convertedDouble;
    
    std::istringstream converter(doubleString);
    converter >> convertedDouble;
    
    return convertedDouble;
}

std::vector<std::string> ProductManager::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    //--------------------------------------------------------------------------
    //Split a string using the delimeter specified. The vector that will contain
    //the split string is passed by reference
    //Precondition: a string to split, delimeter to split by, and vector to store
    //the split strings in passed as parameters.
    //Postcondition: The method splits the string by the parameter and stores it
    //in the elems vector. This vector is also returned to the caller.
    //--------------------------------------------------------------------------
    
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

