//
//  IndexListManager.h
//  TermProject_Alpha
//
//  Created by Sam Jentsch on 11/3/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#ifndef __TermProject_Alpha__IndexListManager__
#define __TermProject_Alpha__IndexListManager__

#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "IndexRecord.h"
#include "DataRecord.h"

#include "SecondaryIndexRecord.h"

class IndexListManager {
private:
    std::fstream &primaryIndexFile;
    std::fstream &secondaryIndexFile;

    std::vector<IndexRecord> keyList;
    std::vector<SecondaryIndexRecord> invertedList;
    
    template<typename T>
    int binarySearch(int key, std::vector<T> searchList);

public:
    IndexListManager(std::fstream &primaryIndexFile, std::fstream &secondaryIndexFile);
    
    void populateKeyListFromDataRecordFile(std::fstream &dataFile);
    
    void sortKeyList();
    void sortInvertedList();
    void printKeyList();
    void savePrimaryKeyListToFile();
    void saveSecondaryKeyListToFile();
    void populateSecondaryKeyListFromFile();
    void populateKeyListFromIndexRecordFile();
    
    //Search
    int getRecordOffsetForKey(int key);
    std::vector<int> getPrimaryKeysWithCode(int code);
    
    //Add
    void addDataRecordToKeyList(DataRecord newRecord, int RRN);
    void addDataRecordToInvertedList(DataRecord record);

    //Delete
    void deleteIndexRecordWithKey(int key);
    int deleteSecondaryIndexWithKey(int key);
    void removePrimaryIndexFromSecondaryKeyWithCode(int code, int primaryKey);
};

#endif /* defined(__TermProject_Alpha__IndexListManager__) */
