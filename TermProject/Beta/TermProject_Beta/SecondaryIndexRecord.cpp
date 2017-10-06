//
//  SecondaryIndexRecord.cpp
//  TermProject_Beta
//
//  Created by Sam Jentsch on 11/12/14.
//  Copyright (c) 2014 Sam Jentsch. All rights reserved.
//

#include "SecondaryIndexRecord.h"

SecondaryIndexRecord::SecondaryIndexRecord() {
    
}

SecondaryIndexRecord::SecondaryIndexRecord(int key) {
    setKey(key);
}

void SecondaryIndexRecord::addPrimaryKey(int primaryKey) {
    primaryKeys.push_back(primaryKey);
    std::sort(primaryKeys.begin(), primaryKeys.end());
}

void SecondaryIndexRecord::printPrimaryKeys() {
    for (int i = 0; i < primaryKeys.size(); i++) {
        std::cout << primaryKeys.at(i) << " ";
    }
}

#pragma mark - Delete
void SecondaryIndexRecord::removePrimaryKey(int primaryKey) {
    //Delete a primary key from the list associated with the secondary index.
    int listIndex = binarySearch(primaryKey, primaryKeys);
    std::vector<int>::iterator iterator = primaryKeys.begin();
    //std::cout << "REMOVING PRIMARY KEY AT INDEX: " << listIndex << std::endl;
    primaryKeys.erase(iterator + listIndex);
}


int SecondaryIndexRecord::binarySearch(int key, std::vector<int> searchList) {
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
        if (searchList.at(mid) == key) {
            return mid;
        } else if(searchList.at(mid) < key) {
            bottom_bound = mid + 1;
        } else {
            top_bound = mid - 1;
        }
    }
    
    return index;
}
