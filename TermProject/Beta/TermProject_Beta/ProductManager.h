//
//  ProductManager.h
//  TermProject_Alpha
//
//  Created by Sam Jentsch on 11/3/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#ifndef __TermProject_Alpha__ProductManager__
#define __TermProject_Alpha__ProductManager__

#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "DataRecord.h"

class ProductManager {
private:
    std::fstream &binaryRecordFile;
    
    int headerNumber;
public:
    ProductManager(std::fstream &binaryRecordFile);
    DataRecord getRecordWithKey(int key);
    
    //***Data Record Binary File Reader***//
    void traverseFile();
    DataRecord getDataRecordAtOffset(int offset);
    int getNumberOfRecords();
    
    //***Data Record Binary File Writer***//
    void createBinaryRecordFile(std::ifstream &textFile);
    void writeDataRecordToBinaryFile(DataRecord newRecord, int line);
    int addDataRecordToBinaryFile(DataRecord record);
    void deleteRecordAtOffset(int offset);
    void updateHeader(int numberOfRecords);
    
    //***Search***//
    DataRecord searchFileForCost(double cost);
    DataRecord searchFileForDescription(std::string description);
    
    //***Conversion Methods***//
    DataRecord getDataRecordForString(std::string record);
    int convertStringToInt(std::string intString);
    double convertStringToDouble(std::string doubleString);
    std::vector<std::string> split(const std::string &s, char delim, std::vector<std::string> &elems);    
};

#endif /* defined(__TermProject_Alpha__ProductManager__) */
