//
//  IndexListManager.cpp
//  TermProject_Alpha
//
//  Created by Sam Jentsch on 11/3/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#include "IndexListManager.h"

#pragma mark - Constructor
IndexListManager::IndexListManager(std::fstream &primaryIndexFile, std::fstream &secondaryIndexFile)
:primaryIndexFile(primaryIndexFile),
secondaryIndexFile(secondaryIndexFile)
{
    
}


#pragma mark - File Operations
void IndexListManager::populateKeyListFromDataRecordFile(std::fstream &dataFile) {
    //--------------------------------------------------------------------------------//
    //BRINGS KEYLIST INTO MEMORY.
    //Read strings from the file until it is empty. Convert these strings into records
    //for processing.
    //Preconditions: indexFile class attribute initialized through constructor.
    //Postconditions: records are created and added to the keyList for each string
    //in the file.
    //--------------------------------------------------------------------------------//
    std:: cout << "POPULATE\n";
    dataFile.seekg(0);
    
    int lines = 1;
    DataRecord header;
    dataFile.read(reinterpret_cast<char*>(&header), sizeof(DataRecord));
    //std::cout << "HEADER: " << header.getKey() << std::endl;
    
    DataRecord record;
    IndexRecord indexRecord;
    for (int i = 0; i < header.getKey(); i++) {
        dataFile.read(reinterpret_cast<char*>(&record), sizeof(DataRecord));
        //std::cout << "GOT RECORD: ";
        //record.printRecord();
        addDataRecordToKeyList(record, lines);
        addDataRecordToInvertedList(record);
        lines++;
    }
    
}

void IndexListManager::populateKeyListFromIndexRecordFile() {
    //--------------------------------------------------------------------------------//
    //BRINGS KEYLIST INTO MEMORY.
    //Read strings from the file until it is empty. Convert these strings into records
    //for processing.
    //Preconditions: indexFile class attribute initialized through constructor.
    //Postconditions: records are created and added to the keyList for each string
    //in the file.
    //--------------------------------------------------------------------------------//
    primaryIndexFile.seekg(0);
    
    IndexRecord header;
    primaryIndexFile.read(reinterpret_cast<char*>(&header), sizeof(IndexRecord));
    
    IndexRecord indexRecord;
    for (int i = 0; i < header.getKey(); i++) {
        primaryIndexFile.read(reinterpret_cast<char*>(&indexRecord), sizeof(IndexRecord));
        keyList.push_back(indexRecord);
    }
}

void IndexListManager::populateSecondaryKeyListFromFile() {
    /*
     HEADER — Number of Secondary key entries
     CODE
     NUMBER OF PRIMARY KEYS WITH CODE
     PRIMARY KEYS
     …
     
     CODE
     NUMBER OF PRIMARY KEYS WITH CODE
     PRIMARY KEYS
     …
     CODE
     NUMBER OF PRIMARY KEYS WITH CODE
     PRIMARY KEYS
     */
    
    secondaryIndexFile.seekg(0);
    SecondaryIndexRecord headerRecord;
    secondaryIndexFile.read(reinterpret_cast<char*>(&headerRecord), sizeof(SecondaryIndexRecord));
    
    int listSize;
    int code;
    int primaryKey;
    for (int i = 0; i < headerRecord.getKey(); i++) {
        secondaryIndexFile.read(reinterpret_cast<char*>(&code), sizeof(int));
        secondaryIndexFile.read(reinterpret_cast<char*>(&listSize), sizeof(int));
        SecondaryIndexRecord record(code);
        for (int j = 0; j < listSize; j++) {
            secondaryIndexFile.read(reinterpret_cast<char*>(&primaryKey), sizeof(int));
            record.addPrimaryKey(primaryKey);
        }
        invertedList.push_back(record);
    }
    sortInvertedList();
    
}

void IndexListManager::savePrimaryKeyListToFile() {
    //--------------------------------------------------------------------------------//
    //Save key list to file.
    //Preconditions: none
    //Postconditions: key list saved to file.
    //--------------------------------------------------------------------------------//
    primaryIndexFile.seekp(0);
    IndexRecord header((int)keyList.size(), 0);
    primaryIndexFile.write(reinterpret_cast<char*>(&header), sizeof(IndexRecord));
    //std::cout << "KEYLIST LENGTH: " << keyList.size() << std::endl;
    for (int i = 0; i < keyList.size(); i++) {
        //std::cout << "ADDING " << keyList.at(i).getKey() << " " << keyList.at(i).getRelativeOffset() << "\n";
        primaryIndexFile.write(reinterpret_cast<char*>(&keyList.at(i)), sizeof(IndexRecord));
        //std::cout << "SUCCESS\n";
    }
}

void IndexListManager::saveSecondaryKeyListToFile() {
    secondaryIndexFile.seekp(0);
    SecondaryIndexRecord headerRecord((int) invertedList.size());
    secondaryIndexFile.write(reinterpret_cast<char*>(&headerRecord), sizeof(SecondaryIndexRecord));
    
    for (int i = 0; i < (int)invertedList.size(); i++) {
        //Write out the code
        int code = invertedList.at(i).getKey();
        secondaryIndexFile.write(reinterpret_cast<char*>(&code), sizeof(int));
        
        //Now write out the number of primary keys following
        int listSize = (int)invertedList.at(i).primaryKeys.size();
        secondaryIndexFile.write(reinterpret_cast<char*>(&listSize), sizeof(int));
        
        //Now write out the primary keys
        for (int j = 0; j < (int)invertedList.at(i).primaryKeys.size(); j++) {
            int primaryKey = invertedList.at(i).primaryKeys.at(j);
            secondaryIndexFile.write(reinterpret_cast<char*>(&primaryKey), sizeof(int));
        }
    }
}

#pragma mark - Key List Handlers
void IndexListManager::sortKeyList() {
    //Such efficiency.
    //Much STL library usage.
    //Wow.
    
    std::sort(keyList.begin(), keyList.end());
}

void IndexListManager::sortInvertedList() {
    std::sort(invertedList.begin(), invertedList.end());
}

void IndexListManager::printKeyList() {
    //--------------------------------------------------------------------------------//
    //Display the key list to the console
    //Preconditions: none
    //Postconditions: key list is displayed to console.
    //--------------------------------------------------------------------------------//
    std::cout << "KEY LIST\n";
    for (int i = 0; i < keyList.size(); i++) {
        std::cout << keyList.at(i).getKey() << "     " << keyList.at(i).getRelativeOffset() << "\n";
    }
    
    std::cout << "SECONDARY KEY LIST\n";
    for (int i = 0; i < invertedList.size(); i++) {
        std::cout << invertedList.at(i).getKey() << " ";
        invertedList.at(i).printPrimaryKeys();
        std::cout << std::endl;
    }
}

#pragma mark - Search
int IndexListManager::getRecordOffsetForKey(int key) {
    //--------------------------------------------------------------------------------//
    //Read the record from the Data Record File that has a key matching the key passed.
    //Call binary search of the Key List to find the RRN for the record with the
    //matching key. Intialize a Data Record from the file and return to the caller.
    //Preconditions: key passed as integer to search for, initialize, and return.
    //Postconditions: A Data Record is initialized from the file (using the RRN from the
    //key list) and returned to the caller.
    //--------------------------------------------------------------------------------//
    int indexForRecord = -1;
    
    int indexRecordIndex = binarySearch(key, keyList);
    
    if (indexRecordIndex != -1) {
        indexForRecord = keyList.at(indexRecordIndex).getRelativeOffset();
    }
    
    return indexForRecord;

}

std::vector<int> IndexListManager::getPrimaryKeysWithCode(int code) {
    int listIndex = binarySearch(code, invertedList);
    std::vector<int> keys;
    
    if (listIndex != -1) {
        keys = invertedList.at(listIndex).primaryKeys;
    }
    
    return keys;
}

template<typename T>
int IndexListManager::binarySearch(int key, std::vector<T> searchList) {
    //--------------------------------------------------------------------------------//
    //Perform a binary search on the primary key list for the int key passed as parameter
    //Return the index of the record in the key list with a key matching the key passed.
    //Preconditions: key passed as integer to search for.
    //Postconditions: Index of record with key passed is returned to caller. -1 is
    //returned if the key was not found.
    //--------------------------------------------------------------------------------//
    int index = -1;
    
    int top_bound = (int)searchList.size() - 1;
    int bottom_bound = 0;
    
    int mid;
    
    while (top_bound >= bottom_bound) {
        mid = (top_bound+bottom_bound)/2;
        if (searchList.at(mid).getKey() == key) {
            return mid;
        } else if(searchList.at(mid).getKey() < key) {
            bottom_bound = mid + 1;
        } else {
            top_bound = mid - 1;
        }
    }
    
    return index;
}

#pragma mark - Add
void IndexListManager::addDataRecordToKeyList(DataRecord newRecord, int RRN) {
    IndexRecord indexRecord;
    
    indexRecord.setKey(newRecord.getKey());
    indexRecord.setRelativeOffset(RRN);
    keyList.push_back(indexRecord);
    
    sortKeyList();
}


void IndexListManager::addDataRecordToInvertedList(DataRecord record) {
    int code = record.getCode();
    
    int index = binarySearch(code, invertedList);
    if (index == -1) {
        //If search for code in existing list returns -1 add code to the list
        SecondaryIndexRecord newSecondaryIndex(code);
        newSecondaryIndex.addPrimaryKey(record.getKey());
        invertedList.push_back(newSecondaryIndex);
        sortInvertedList();
    } else {
        //Otherwise, add the primary key to the existing entry
        invertedList.at(index).addPrimaryKey(record.getKey());
    }
}

# pragma mark - Delete
void IndexListManager::deleteIndexRecordWithKey(int key) {
    int listIndex = binarySearch(key, keyList);
    std::vector<IndexRecord>::iterator iterator = keyList.begin();
    keyList.erase(iterator + listIndex);
}

int IndexListManager::deleteSecondaryIndexWithKey(int key) {
    int listIndex = binarySearch(key, invertedList);
    //std::cout << "DELETING AT INDEX: " << listIndex << std::endl;
    if (listIndex != -1) {
        std::vector<SecondaryIndexRecord>::iterator iterator = invertedList.begin();
        invertedList.erase(iterator + listIndex);
    }
    return listIndex;
}

void IndexListManager::removePrimaryIndexFromSecondaryKeyWithCode(int code, int primaryKey) {
    int listIndex = binarySearch(code, invertedList);
    //std::cout << "DELETING AT INDEX: " << listIndex << std::endl;
    SecondaryIndexRecord *deleteRecord = &invertedList.at(listIndex);
    
    deleteRecord->removePrimaryKey(primaryKey);
    if (deleteRecord->primaryKeys.size() == 0) {
        deleteSecondaryIndexWithKey(deleteRecord->getKey());
    }

}
